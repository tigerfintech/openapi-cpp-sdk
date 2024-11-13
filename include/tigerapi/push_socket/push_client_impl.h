#ifndef PUSH_CLIENT_IMPL_H
#define PUSH_CLIENT_IMPL_H

#include "boost/asio/io_service.hpp"
#include "tigerapi/push_client.h"
#include "tigerapi/client_config.h"
#include "openapi_pb/pb_source/Request.pb.h"
#include "openapi_pb/pb_source/Response.pb.h"

namespace TIGER_API
{
	class PushSocket;
	class PushClientImpl : public std::enable_shared_from_this<PushClientImpl>, public IPushClient
	{
	public:
		static std::shared_ptr<PushClientImpl> create_push_client_impl(const TIGER_API::ClientConfig& client_config);
		~PushClientImpl();
		PushClientImpl(const PushClientImpl&) = delete;
		PushClientImpl& operator=(const PushClientImpl&) = delete;
	private:
		PushClientImpl(const TIGER_API::ClientConfig& client_config);
	public:
		virtual void connect() override;
		virtual void disconnect() override;

		virtual void set_connected_callback(const std::function<void()>& cb) override;
		virtual void set_disconnected_callback(const std::function<void()>& cb) override;
		virtual void set_inner_error_callback(const std::function<void(std::string)>& cb) override;
		
		virtual void set_asset_changed_callback(const std::function<void(const tigeropen::push::pb::AssetData&)>& cb) override;
		virtual bool subscribe_asset(const std::string& account) override;
		virtual bool unsubscribe_asset(const std::string& account) override;
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

	std::shared_ptr<TIGER_API::IPushClient> IPushClient::create_push_client(const TIGER_API::ClientConfig& client_config)
	{
		return PushClientImpl::create_push_client_impl(client_config);
	}
}

#endif // PUSH_CLIENT_IMPL_H
