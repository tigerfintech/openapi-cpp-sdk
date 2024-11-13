#include "tigerapi/push_socket/push_client_impl.h"
#include "tigerapi/client_config.h"
#include "tigerapi/push_socket/push_socket.h"
#include "openapi_pb\pb_source\PushData.pb.h"
#include "google/protobuf/util/json_util.h"

std::shared_ptr<TIGER_API::PushClientImpl> TIGER_API::PushClientImpl::create_push_client_impl(const TIGER_API::ClientConfig& client_config)
{
	return std::shared_ptr<TIGER_API::PushClientImpl>(new TIGER_API::PushClientImpl(client_config));
}

TIGER_API::PushClientImpl::~PushClientImpl()
{
	io_service_.stop();

	if (worker_thread_)
	{
		worker_thread_->join();
	}
}

TIGER_API::PushClientImpl::PushClientImpl(const TIGER_API::ClientConfig& client_config)
{
	socket_ = PushSocket::create_push_socket(&io_service_, client_config);
}

void TIGER_API::PushClientImpl::connect()
{
	LOG(INFO) << "create a worker thread to perform asynchronous network connections";
	//启动工作线程
	worker_thread_ = std::shared_ptr<std::thread>(new std::thread([this]
	{
		
		socket_->connect();

		LOG(INFO) << "io_service run on work thread";
		io_service_.run();
	}));
}

void TIGER_API::PushClientImpl::disconnect()
{
	//跨线程调用，需要异步投递任务
	io_service_.post(boost::bind(&PushClientImpl::do_disconnect, this));
}

void TIGER_API::PushClientImpl::set_connected_callback(const std::function<void()>& cb)
{
	socket_->set_connected_callback(cb);
}

void TIGER_API::PushClientImpl::set_disconnected_callback(const std::function<void()>& cb)
{
	socket_->set_disconnected_callback(cb);
}

void TIGER_API::PushClientImpl::set_inner_error_callback(const std::function<void(std::string)>& cb)
{
	socket_->set_inner_error_callback(cb);
}

void TIGER_API::PushClientImpl::set_asset_changed_callback(const std::function<void(const tigeropen::push::pb::AssetData&)>& cb)
{
	asset_changed_ = cb;
}

bool TIGER_API::PushClientImpl::subscribe_asset(const std::string& account)
{
	if (account.empty())
	{
		return false;
	}

	tigeropen::push::pb::Request request;
	request.set_command(tigeropen::push::pb::SocketCommon_Command_SUBSCRIBE);
	request.set_id(socket_->get_next_id());

	tigeropen::push::pb::Request_Subscribe* subscribe = request.mutable_subscribe();
	subscribe->set_datatype(tigeropen::push::pb::SocketCommon_DataType_Asset);
	if (!account.empty())
	{
		subscribe->set_account(account.c_str());
	}

	send_frame(request);

	return true;
}

bool TIGER_API::PushClientImpl::unsubscribe_asset(const std::string& account)
{
	if (account.empty())
	{
		return false;
	}

	tigeropen::push::pb::Request request;
	request.set_command(tigeropen::push::pb::SocketCommon_Command_UNSUBSCRIBE);
	request.set_id(socket_->get_next_id());

	tigeropen::push::pb::Request_Subscribe* subscribe = request.mutable_subscribe();
	subscribe->set_datatype(tigeropen::push::pb::SocketCommon_DataType_Asset);
	if (!account.empty())
	{
		subscribe->set_account(account.c_str());
	}

	send_frame(request);

	return true;
}

bool TIGER_API::PushClientImpl::send_frame(const tigeropen::push::pb::Request& request)
{
	//序列化pb对象到字符串
	std::string packed_frame = request.SerializeAsString();
	if (packed_frame.empty())
	{
		return false;
	}

	std::string packed_frame_json;
	google::protobuf::util::JsonPrintOptions options;
	MessageToJsonString(request, &packed_frame_json, options).ok();

	LOG(DEBUG) << "send frame:" << packed_frame_json;

	//跨线程，异步投递任务
	io_service_.post(boost::bind(&PushClientImpl::do_write, this, packed_frame));

	return true;
}

void TIGER_API::PushClientImpl::do_write(const std::string& frame)
{
	socket_->send_message(frame);
}

void TIGER_API::PushClientImpl::do_disconnect()
{
	socket_->disconnect();
}

void TIGER_API::PushClientImpl::on_message(const std::shared_ptr<tigeropen::push::pb::Response>& response_pb_object)
{
	if (response_pb_object->body().datatype() == tigeropen::push::pb::SocketCommon_DataType_Asset && asset_changed_)
	{
		asset_changed_(response_pb_object->body().assetdata());
	}
	//TODO:other message type
}
