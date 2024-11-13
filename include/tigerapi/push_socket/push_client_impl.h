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

		virtual void set_position_changed_callback(const std::function<void(const tigeropen::push::pb::PositionData&)>& cb) = 0;
		virtual bool subscribe_position(const std::string& account) = 0;
		virtual bool unsubscribe_position(const std::string& account) = 0;

		virtual void set_order_changed_callback(const std::function<void(const tigeropen::push::pb::OrderData&)>& cb) = 0;
		virtual bool subscribe_order(const std::string& account) = 0;
		virtual bool unsubscribe_order(const std::string& account) = 0;

		
		virtual void set_query_subscribed_symbols_changed_callback(const std::function<void(const std::vector<std::string>& symbols)>& cb) = 0;
		virtual void query_subscribed_symbols() = 0;
		virtual void set_quote_changed_callback(const std::function<void(const tigeropen::push::pb::QuoteData&)>& cb) = 0;
		virtual bool subscribe_quote(const std::vector<std::string>& symbols) = 0;
		virtual bool subscribe_future_quote(const std::vector<std::string>& symbols) = 0;
		virtual bool subscribe_option_quote(const std::vector<std::string>& symbols) = 0;
		virtual bool unsubscribe_quote(const std::vector<std::string>& symbols) = 0;

		virtual void set_quote_bbo_changed_callback(const std::function<void(const tigeropen::push::pb::QuoteData&)>& cb) = 0;
		virtual bool subscribe_quote_bbo(const std::vector<std::string>& symbols) = 0;
		virtual bool unsubscribe_quote_bbo(const std::vector<std::string>& symbols) = 0;

		virtual void set_quote_depth_changed_callback(const std::function<void(const tigeropen::push::pb::QuoteDepthData&)>& cb) = 0;
		virtual bool subscribe_quote_depth(const std::vector<std::string>& symbols) = 0;
		virtual bool unsubscribe_quote_depth(const std::vector<std::string>& symbols) = 0;

		virtual void set_kline_changed_callback(const std::function<void(const tigeropen::push::pb::KlineData&)>& cb) = 0;
		virtual bool subscribe_kline(const std::vector<std::string>& symbols) = 0;
		virtual bool unsubscribe_kline(const std::vector<std::string>& symbols) = 0;

		virtual void set_full_tick_changed_callback(const std::function<void(const tigeropen::push::pb::TickData&)>& cb) = 0;
		virtual bool subscribe_full_tick(const std::vector<std::string>& symbols) = 0;
		virtual bool unsubscribe_full_tick(const std::vector<std::string>& symbols) = 0;

		virtual void set_tick_changed_callback(const std::function<void(const tigeropen::push::pb::TickData&)>& cb) = 0;
		virtual bool subscribe_tick(const std::vector<std::string>& symbols) = 0;
		virtual bool unsubscribe_tick(const std::vector<std::string>& symbols) = 0;

		virtual void subscribe_market(const std::string& market) = 0;
		virtual void unsubscribe_market(const std::string& market) = 0;

		virtual void set_stock_top_changed_callback(const std::function<void(const tigeropen::push::pb::StockTopData&)>& cb) = 0;
		virtual bool subscribe_stock_top(const std::string& market) = 0;
		virtual bool unsubscribe_stock_top(const std::string& market) = 0;
		
		virtual void set_option_top_changed_callback(const std::function<void(const tigeropen::push::pb::OptionTopData&)>& cb) = 0;
		virtual bool subscribe_option_top(const std::string& market) = 0;
		virtual bool unsubscribe_option_top(const std::string& market) = 0;

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
