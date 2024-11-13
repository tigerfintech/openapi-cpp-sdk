#include "tigerapi/push_socket/push_socket.h"
#include "tigerapi/version.h"
#include <bitset>
#include <regex>
#include "google/protobuf/util/json_util.h"
#include "cpprest/json.h"

static const int MEMORY_POOL_PAGE_SIZE = 1024;
static const int MEMORY_POOL_BLOCK_NUM = 1024;
static const std::string CONNECTED_HEART_BEAT_CFG_KEY = "heart-beat";

std::shared_ptr<TIGER_API::PushSocket> TIGER_API::PushSocket::create_push_socket(boost::asio::io_service* io_service,
	const TIGER_API::ClientConfig& client_config)
{
	return std::shared_ptr<TIGER_API::PushSocket>(new TIGER_API::PushSocket(io_service, client_config));
}

TIGER_API::PushSocket::~PushSocket()
{

}

TIGER_API::PushSocket::PushSocket(boost::asio::io_service* io_service,
	const TIGER_API::ClientConfig& client_config)
{
	io_service_ = io_service;
	client_config_ = client_config;
	send_interval_ = client_config_.send_interval;
	recv_interval_ = client_config_.receive_interval;
	
	//初始化内存池，防止接收数据频繁的内存分配和释放引起的性能问题和内存碎片问题
	recv_buff_pool_.reset(new boost::pool<>(MEMORY_POOL_PAGE_SIZE, MEMORY_POOL_BLOCK_NUM));
	
	//创建一个保活定时器
	keep_alive_timer_ = std::make_shared<boost::asio::deadline_timer>(*io_service);
	
	//创建重连定时器
	reconnect_timer_ = std::make_shared<boost::asio::deadline_timer>(*io_service);
}

void TIGER_API::PushSocket::set_connected_callback(const std::function<void()>& cb)
{
	connected_callback_ = cb;
}

void TIGER_API::PushSocket::set_disconnected_callback(const std::function<void()>& cb)
{
	disconnected_callback_ = cb;
}

void TIGER_API::PushSocket::set_on_message_callback(const std::function<void(const std::shared_ptr<tigeropen::push::pb::Response>& response_pb_object)>& cb)
{
	on_message_callback_ = cb;
}

void TIGER_API::PushSocket::set_inner_error_callback(const std::function<void(std::string)>& cb)
{
	on_inner_error_callback_ = cb;
}

void TIGER_API::PushSocket::connect()
{
	try
	{
		boost::asio::ip::tcp::resolver resolver(*io_service_);
		boost::asio::ip::tcp::resolver::query query(utility::conversions::to_utf8string(client_config_.socket_url),
			utility::conversions::to_utf8string(client_config_.socket_port));
		boost::asio::ip::tcp::resolver::iterator rit = resolver.resolve(query);

		//打印dns解析之后的IP地址
		std::string str_target_server_ip = rit->endpoint().address().to_string();
		LOG(INFO) << "resolved ip: " << str_target_server_ip;

		init_socket();

		socket_->lowest_layer().async_connect(*rit,
			boost::bind(&PushSocket::handle_connect, this, boost::asio::placeholders::error, ++rit));

		socket_state_ = SocketState::CONNECTING;
	}
	catch (const boost::system::system_error& e)
	{
		LOG(ERROR) << e.what();
	}
}

void TIGER_API::PushSocket::disconnect()
{
	if (keep_alive_timer_)
	{
		keep_alive_timer_->cancel();
	}
	close_session();
}

bool TIGER_API::PushSocket::send_message(const std::string& msg)
{
	if (msg.empty() || socket_state_ != SocketState::CONNECTED || !socket_->lowest_layer().is_open())
	{
		return false;
	}

	//协议封包
	TIGER_API::PushFrameEncoder encoder;
	std::vector<unsigned char> data = encoder.encode_frame(msg);

	//异步发送
	boost::asio::async_write(*socket_,
		boost::asio::buffer(data, data.size()),
		boost::bind(&PushSocket::handle_write, this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred, data.size()));
	return true;
}

void TIGER_API::PushSocket::init_socket()
{
	//创建ssl上下文，指定ssl版本
	boost::asio::ssl::context ssl_content(boost::asio::ssl::context::sslv23);
#if 1
	ssl_content.set_options(boost::asio::ssl::context::default_workarounds | boost::asio::ssl::context::single_dh_use);
#else
	ssl_content.set_options(boost::asio::ssl::context::default_workarounds);
#endif
	if (client_config_.socket_ca_certs.empty())
	{
		//禁用证书验证
		ssl_content.set_verify_mode(boost::asio::ssl::verify_none);
	}
	else
	{
		//验证证书
		ssl_content.set_verify_mode(boost::asio::ssl::verify_peer);
		ssl_content.set_verify_mode(boost::asio::ssl::context::verify_peer
			| boost::asio::ssl::context::verify_fail_if_no_peer_cert);
		ssl_content.load_verify_file(utility::conversions::to_utf8string(client_config_.socket_ca_certs));
	}

	socket_.emplace(*io_service_, ssl_content);

	socket_->set_verify_callback(boost::bind(&PushSocket::verify_certificate, this, _1, _2));
}

bool TIGER_API::PushSocket::verify_certificate(bool preverified, boost::asio::ssl::verify_context& ctx)
{
	// The verify callback can be used to check whether the certificate that is
	// being presented is valid for the peer. For example, RFC 2818 describes
	// the steps involved in doing this for HTTPS. Consult the OpenSSL
	// documentation for more details. Note that the callback is called once
	// for each certificate in the certificate chain, starting from the root
	// certificate authority.

	char subject_name[256];
	X509* cert = X509_STORE_CTX_get_current_cert(ctx.native_handle());
	X509_NAME_oneline(X509_get_subject_name(cert), subject_name, 256);

	LOG(INFO)<< "[cert subject]:" << subject_name;

	return preverified;
}

unsigned int TIGER_API::PushSocket::get_next_id()
{
	return ++id_counter_;
}

void TIGER_API::PushSocket::close_session()
{
	if (reconnect_timer_)
	{
		reconnect_timer_->cancel();
	}
	
	LOG(INFO) << "close socket";
	socket_state_ = SocketState::DISCONNECTED;

	try
	{
		bool open = socket_->lowest_layer().is_open();
		if (open)
		{
			boost::system::error_code ec;
			socket_->lowest_layer().shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
			if (ec)
			{
				dispatch_inner_error_callback(ec.message());
			}

			socket_->lowest_layer().close(ec);
			if (ec)
			{
				dispatch_inner_error_callback(ec.message());
			}
		}
	}
	catch (const boost::system::system_error& e)
	{
		LOG(ERROR) << e.what();
		dispatch_inner_error_callback(e.what());
	}

	dispatch_disconnected_callback();
}

void TIGER_API::PushSocket::send_authentication()
{
	//对tiger_id根据private_key进行签名
	utility::string_t sign = Utils::get_sign(client_config_.private_key, client_config_.tiger_id);

	// 创建一个 Request 对象
	tigeropen::push::pb::Request request;
	request.set_command(tigeropen::push::pb::SocketCommon_Command_CONNECT);
	request.set_id(get_next_id());

	// 创建一个 Request_Connect 对象
	tigeropen::push::pb::Request_Connect* connect_request = request.mutable_connect();
	connect_request->set_tigerid(utility::conversions::to_utf8string(client_config_.tiger_id));
	connect_request->set_sign(utility::conversions::to_utf8string(sign));
	connect_request->set_sdkversion(utility::conversions::to_utf8string(P_SDK_VERSION_PREFIX + U(PROJECT_VERSION)));
	connect_request->set_acceptversion("3");
	connect_request->set_sendinterval(client_config_.send_interval);
	connect_request->set_receiveinterval(client_config_.receive_interval);
	connect_request->set_usefulltick(client_config_.user_full_tick);
	
	//序列化pb对象到字符串
	std::string packed_frame = request.SerializeAsString();
	if (!packed_frame.empty())
	{
		std::string packed_frame_json;
		google::protobuf::util::JsonPrintOptions options;
		MessageToJsonString(request, &packed_frame_json, options).ok();

		LOG(INFO) << "[authenticate message]: " << packed_frame_json;

		send_message(packed_frame);
	}
}

void TIGER_API::PushSocket::start_keep_alive()
{
	keep_alive_timer_->expires_from_now(boost::posix_time::milliseconds(500));
	keep_alive_timer_->async_wait(boost::bind(&PushSocket::handle_timer, this, boost::asio::placeholders::error));
}

void TIGER_API::PushSocket::send_heart_beat()
{
	last_send_heart_beat_time_ = time(0);

	tigeropen::push::pb::Request request;
	request.set_command(tigeropen::push::pb::SocketCommon_Command_HEARTBEAT);
	request.set_id(get_next_id());

	send_message(request.SerializeAsString());
}

void TIGER_API::PushSocket::auto_reconnect()
{
	LOG(INFO) << "try reconnecting after " << reconnect_interval_ / 1000 << " seconds...";
	socket_state_  = SocketState::CONNECTING;
	reconnect_timer_->expires_from_now(boost::posix_time::milliseconds(reconnect_interval_));
	reconnect_timer_->async_wait([this](const boost::system::error_code& error) 
	{
		if (!error) 
		{
			LOG(INFO) << "start automatic reconnection";
			try 
			{
				boost::asio::ip::tcp::resolver resolver(*io_service_);
				boost::asio::ip::tcp::resolver::query query(utility::conversions::to_utf8string(client_config_.socket_url),
					utility::conversions::to_utf8string(client_config_.socket_port));
				boost::asio::ip::tcp::resolver::iterator iterator = resolver.resolve(query);

				init_socket();

				socket_->lowest_layer().async_connect(*iterator,
					boost::bind(&PushSocket::handle_connect, this,
						boost::asio::placeholders::error, ++iterator));

				socket_state_ = SocketState::CONNECTING;
			}
			catch (const boost::system::system_error& e)
			{
				LOG(ERROR) << e.what();
				socket_state_ = SocketState::DISCONNECTED;
			}
		}
		else 
		{
			LOG(ERROR) << "reconnection timer error: " << error.message();
			socket_state_ = SocketState::DISCONNECTED;
		}
	});
}

void TIGER_API::PushSocket::handle_connect(const boost::system::error_code& error, boost::asio::ip::tcp::resolver::iterator endpoint_iterator)
{
	try
	{
		if (!error)
		{
			LOG(INFO) << "connect success";
			//ssl连接成功之后需要async_handshake()，成功之后才能发起异步读写。
			socket_->async_handshake(boost::asio::ssl::stream_base::client,
				boost::bind(&PushSocket::handle_handshake, this,
					boost::asio::placeholders::error));
		}
		else if (endpoint_iterator != boost::asio::ip::tcp::resolver::iterator()) 
		{
			socket_->lowest_layer().close();
			boost::asio::ip::tcp::endpoint endpoint = *endpoint_iterator;
			socket_->lowest_layer().async_connect(endpoint,
				boost::bind(&PushSocket::handle_connect, this,
					boost::asio::placeholders::error, ++endpoint_iterator));
		}
		else
		{
			LOG(ERROR) << "[connect failed]: " << error;
			dispatch_inner_error_callback(error.message());

			//连接失败关闭会话
			close_session();
		}
	}
	catch (const boost::system::system_error& e)
	{
		LOG(ERROR) << "[connect failed]: " << e.what();
		dispatch_inner_error_callback(e.what());
		close_session();
	}
}

void TIGER_API::PushSocket::handle_handshake(const boost::system::error_code& error)
{
	try
	{
		if (!error)
		{
			LOG(INFO) << "handshake success";

			//握手成功之后，设置状态为CONNECTED
			socket_state_ = SocketState::CONNECTED;

			//设置socket选项
			socket_->lowest_layer().set_option(boost::asio::ip::tcp::acceptor::linger(true, 0));
			socket_->lowest_layer().set_option(boost::asio::socket_base::keep_alive(true));

			//启动保活监测定时任务
			start_keep_alive();

			//启动异步读任务，保持IO循环的运行
			read_head();

			//身份认证
			send_authentication();

			//连接回调，只有建立连接成功之后才会回调
			dispatch_connected_callback();
		}
		else
		{
			LOG(ERROR) << "Handshake Failed: " << error;
			//握手失败关闭会话
			dispatch_inner_error_callback(error.message());
			close_session();
		}
	}
	catch (const boost::system::system_error& e)
	{
		LOG(ERROR) << "[handshake failed]: " << e.what();
		dispatch_inner_error_callback(e.what());
		close_session();
	}
}

void TIGER_API::PushSocket::handle_write(const boost::system::error_code& error,
	size_t bytes_transferred,
	size_t frame_len)
{
	if (error || frame_len != bytes_transferred)
	{
		LOG(ERROR) << "[send failed]: " << error;
		dispatch_inner_error_callback(error.message());
		close_session();
	}
}

void TIGER_API::PushSocket::handle_read_head(const boost::system::error_code& error, size_t bytes_transferred)
{
	if (error || bytes_transferred == 0)
	{
		LOG(ERROR) << "[read head failed]: " << error;
		dispatch_inner_error_callback(error.message());
		close_session();
	}
	else
	{
		
#if	1
		// 循环打印每个字节的二进制值
		for (size_t i = 0; i < bytes_transferred; ++i) 
		{
			std::bitset<8> binary(head_buff_[i]);
			LOG(DEBUG) << "[header tag] " << i + 1 << ": " << binary;
		}
#endif
		last_io_time_ = time(nullptr);
		if (frame_decoder_.push_byte(head_buff_[0]))
		{
			//头部已经独取完毕，开始读取body
			auto frame_len = frame_decoder_.get_frame_size();
			read_body(frame_len);
		}
		else
		{
			//继续读取头部
			read_head();
		}
	}
}

void TIGER_API::PushSocket::handle_read_body(const boost::system::error_code& error,
	size_t bytes_transferred,
	char* recv_buff,
	int page_num,
	size_t frame_len)
{
	if (error || 0 == bytes_transferred || bytes_transferred != frame_len)
	{
		LOG(ERROR) << "[read body failed]: " << error;
		dispatch_inner_error_callback(error.message());
		close_session();
		return;
	}

	last_io_time_ = time(nullptr);

	std::shared_ptr<tigeropen::push::pb::Response> response_pb_object = std::make_shared<tigeropen::push::pb::Response>();
	if (!response_pb_object->ParseFromArray(recv_buff, frame_len))
	{
		close_session();
		return;
	}
#if 1
	std::string packed_frame_json;
	google::protobuf::util::JsonPrintOptions options;
	MessageToJsonString(*response_pb_object, &packed_frame_json, options).ok();
	LOG(DEBUG) << "[recv message]: " << packed_frame_json;
#endif

	message_filter(response_pb_object);

	if (on_message_callback_)
	{
		//下发数据
		on_message_callback_(response_pb_object);
	}

	//缓存还回内存池
	if (recv_buff)
	{
		recv_buff_pool_->ordered_free(recv_buff, page_num);
	}

	//继续读取下一包数据
	read_head();
}

void TIGER_API::PushSocket::handle_timer(const boost::system::error_code& error)
{
	auto now_time = time(0);
	if (!error) 
	{
		if (socket_state_ == SocketState::CONNECTED)
		{
			// 检查是否需要发送心跳
			if (now_time - last_send_heart_beat_time_ > send_interval_ / 1000)
			{
				send_heart_beat();
			}
			//检查空闲状态是否多长时间没有收到数据，断开重连
			if (now_time - last_io_time_ > recv_interval_ / 1000)
			{
				LOG(ERROR) << "heart beat timeout";
				close_session();
				auto_reconnect();
			}
		}
		else if (socket_state_ == SocketState::DISCONNECTED)
		{
			auto_reconnect();
		}

		// 再次启动定时器
		start_keep_alive(); 
	}
	else 
	{
		dispatch_inner_error_callback(error.message());
	}
}

void TIGER_API::PushSocket::read_head()
{
	boost::asio::async_read(*socket_,
		boost::asio::buffer(head_buff_, 1),
		boost::bind(&PushSocket::handle_read_head, this,boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

void TIGER_API::PushSocket::read_body(size_t frame_len)
{
	size_t page_num = frame_len / MEMORY_POOL_PAGE_SIZE + 1;

	char* recv_buff = (char*)recv_buff_pool_->ordered_malloc(page_num);
	if (!recv_buff)
	{
		close_session();
		return;
	}
	boost::asio::async_read(*socket_, boost::asio::buffer(recv_buff, frame_len),
		boost::bind(&PushSocket::handle_read_body, shared_from_this(),
			boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred, recv_buff, page_num, frame_len));
}

void TIGER_API::PushSocket::dispatch_connected_callback()
{
	if (connected_callback_)
	{
		connected_callback_();
	}
}

void TIGER_API::PushSocket::dispatch_disconnected_callback()
{
	if (disconnected_callback_)
	{
		disconnected_callback_();
	}
}

void TIGER_API::PushSocket::dispatch_inner_error_callback(const std::string& error)
{
	if (on_inner_error_callback_)
	{
		on_inner_error_callback_(error);
	}
}

void TIGER_API::PushSocket::message_filter(const std::shared_ptr<tigeropen::push::pb::Response>& response_pb_object)
{
	if (response_pb_object->command() == tigeropen::push::pb::SocketCommon_Command_CONNECTED)
	{
		//连接成功消息
		const std::string& str_msg = response_pb_object->msg();
		if (str_msg.find(CONNECTED_HEART_BEAT_CFG_KEY) != std::wstring::npos)
		{
			LOG(INFO) << str_msg;
			web::json::value json_value = web::json::value::parse(str_msg);
			auto utf16_key = utility::conversions::to_utf16string(CONNECTED_HEART_BEAT_CFG_KEY);
			if (json_value.has_field(utf16_key) && json_value[utf16_key].is_string()) 
			{
				auto heart_beart_mag = json_value[utf16_key].as_string();

				std::vector<std::wstring> tokens;
				std::wregex regex(L",");
				std::wsregex_token_iterator it(heart_beart_mag.begin(), heart_beart_mag.end(), regex, -1);
				std::wsregex_token_iterator end;

				while (it != end)
				{
					tokens.push_back(*it++);
				}

				if (tokens.size() == 2)
				{
					send_interval_ = std::stoi(tokens[0]);
					recv_interval_ = std::stoi(tokens[1]);
				}
			}
		}
	}
}
