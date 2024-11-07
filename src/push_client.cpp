#include "tigerapi/push_client.h"
#include "tigerapi/client_config.h"
#include "tigerapi/push_socket/push_socket.h"
#include "openapi_pb\pb_source\PushData.pb.h"
#include <iostream>
#include "google/protobuf/util/json_util.h"

std::shared_ptr<TIGER_API::PushClient> TIGER_API::PushClient::create_push_client(const TIGER_API::ClientConfig& client_config)
{
	return std::shared_ptr<TIGER_API::PushClient>(new TIGER_API::PushClient());
}

TIGER_API::PushClient::~PushClient()
{
	io_service_.stop();

	if (worker_thread_)
	{
		worker_thread_->join();
	}
}

TIGER_API::PushClient::PushClient()
{

}

void TIGER_API::PushClient::connect(const TIGER_API::ClientConfig& client_config)
{
	LOG(INFO) << "create a worker thread to perform asynchronous network connections";
	//启动工作线程
	worker_thread_ = std::shared_ptr<std::thread>(new std::thread([this, client_config]
	{
		socket_ = PushSocket::create_push_socket(&io_service_, client_config);
		socket_->connect();

		LOG(INFO) << "io_service run on work thread";
		io_service_.run();
	}));
}

void TIGER_API::PushClient::disconnect()
{
	//跨线程调用，需要异步投递任务
	io_service_.post(boost::bind(&PushClient::do_disconnect, this));
}

void TIGER_API::PushClient::set_connected_callback(const std::function<void()>& cb)
{
	if (socket_)
	{
		socket_->set_connected_callback(cb);
	}
}

void TIGER_API::PushClient::set_disconnected_callback(const std::function<void()>& cb)
{
	if (socket_)
	{
		socket_->set_disconnected_callback(cb);
	}
}

void TIGER_API::PushClient::set_inner_error_callback(const std::function<void(std::string)>& cb)
{
	if (socket_)
	{
		socket_->set_inner_error_callback(cb);
	}
}

void TIGER_API::PushClient::set_asset_changed_callback(const std::function<void(const tigeropen::push::pb::AssetData&)>& cb)
{
	asset_changed_ = cb;
}

bool TIGER_API::PushClient::subscribe_asset(const std::string& account)
{
	if (!socket_)
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

bool TIGER_API::PushClient::unsubscribe_asset(const std::string& account)
{
	if (!socket_)
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

bool TIGER_API::PushClient::send_frame(const tigeropen::push::pb::Request& request)
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
	io_service_.post(boost::bind(&PushClient::do_write, this, packed_frame));

	return true;
}

void TIGER_API::PushClient::do_write(const std::string& frame)
{
	if (socket_)
	{
		socket_->send_message(frame);
	}
}

void TIGER_API::PushClient::do_disconnect()
{
	if (socket_)
	{
		socket_->disconnect();
	}
}

void TIGER_API::PushClient::on_message(const std::shared_ptr<tigeropen::push::pb::Response>& response_pb_object)
{
	if (response_pb_object->body().datatype() == tigeropen::push::pb::SocketCommon_DataType_Asset && asset_changed_)
	{
		asset_changed_(response_pb_object->body().assetdata());
	}
	//TODO:other message type
}
