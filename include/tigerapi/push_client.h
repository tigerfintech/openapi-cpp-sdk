#ifndef PUSH_CLIENT_H
#define PUSH_CLIENT_H
#include <memory>
#include <functional>
#include <thread>
#include "boost/asio/io_service.hpp"
#include "tigerapi/win32.h"
#include "openapi_pb/pb_source/Request.pb.h"
#include "openapi_pb/pb_source/Response.pb.h"
#include "openapi_pb/pb_source/AssetData.pb.h"

namespace TIGER_API
{
	class PushSocket;
	class ClientConfig;

	class OPENAPI_EXPORT PushClient : public std::enable_shared_from_this<PushClient>
	{
	public:
		static std::shared_ptr<PushClient> create_push_client(const TIGER_API::ClientConfig& client_config);
		~PushClient();
		PushClient(const PushClient&) = delete;
		PushClient& operator=(const PushClient&) = delete;
	private:
		PushClient();
	public:
		void connect(const TIGER_API::ClientConfig& client_config);
		void disconnect();

		void set_connected_callback(const std::function<void()>& cb);
		void set_disconnected_callback(const std::function<void()>& cb);
		void set_inner_error_callback(const std::function<void(std::string)>& cb);
		
		void set_asset_changed_callback(const std::function<void(const tigeropen::push::pb::AssetData&)>& cb);
		bool subscribe_asset(const std::string& account);
		bool unsubscribe_asset(const std::string& account);
		
		//TODO:other

	private:
		bool send_frame(const tigeropen::push::pb::Request& request);
		void do_write(const std::string& frame);
		void do_disconnect();
		void on_message(const std::shared_ptr<tigeropen::push::pb::Response>& response_pb_object);
	private:
		std::function<void(const tigeropen::push::pb::AssetData&)> asset_changed_;
	private:
		boost::asio::io_service io_service_;
		std::shared_ptr<TIGER_API::PushSocket> socket_;
		std::shared_ptr<std::thread> worker_thread_;
	};
}

#endif // PUSH_CLIENT_H
