#ifndef PUSH_CLIENT_IMPL_H
#define PUSH_CLIENT_IMPL_H

#include "../model.h"
#include "boost/asio/io_service.hpp"
#include "../include/tigerapi/push_client.h"

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
		ClientConfig client_config_;

	public:
		virtual void connect() override;
		virtual void disconnect() override;

		virtual void set_connected_callback(const std::function<void()>& cb) override;
		virtual void set_disconnected_callback(const std::function<void()>& cb) override;
		virtual void set_inner_error_callback(const std::function<void(std::string)>& cb) override;

		virtual void set_subscribe_callback(const std::function<void(const tigeropen::push::pb::Response&)>& cb) override;
		virtual void set_unsubscribe_callback(const std::function<void(const tigeropen::push::pb::Response&)>& cb) override;
		virtual void set_error_callback(const std::function<void(const tigeropen::push::pb::Response&)>& cb ) override;
		virtual void set_kickout_callback(const std::function<void(const tigeropen::push::pb::Response&)>& cb) override;

		virtual void set_asset_changed_callback(const std::function<void(const tigeropen::push::pb::AssetData&)>& cb) override;
		virtual bool subscribe_asset(const std::string& account) override;
		virtual bool unsubscribe_asset(const std::string& account) override;

		virtual void set_position_changed_callback(const std::function<void(const tigeropen::push::pb::PositionData&)>& cb) override;
		virtual bool subscribe_position(const std::string& account) override;
		virtual bool unsubscribe_position(const std::string& account) override;

		virtual void set_order_changed_callback(const std::function<void(const tigeropen::push::pb::OrderStatusData&)>& cb) override;
		virtual bool subscribe_order(const std::string& account) override;
		virtual bool unsubscribe_order(const std::string& account) override;

		virtual void set_transaction_changed_callback(const std::function<void(const tigeropen::push::pb::OrderTransactionData&)>& cb) override;
		virtual bool subscribe_transaction(const std::string& account) override;
		virtual bool unsubscribe_transaction(const std::string& account) override;
		
		virtual void set_query_subscribed_symbols_changed_callback(const std::function<void(const tigeropen::push::pb::Response& query_subscribed_symbols_response)>& cb) override;
		virtual void query_subscribed_symbols() override;
		virtual void set_quote_changed_callback(const std::function<void(const tigeropen::push::pb::QuoteBasicData&)>& cb) override;
		virtual void set_quote_bbo_changed_callback(const std::function<void(const tigeropen::push::pb::QuoteBBOData&)>& cb) override;
		virtual bool subscribe_quote(const std::vector<std::string>& symbols) override;
		virtual bool subscribe_future_quote(const std::vector<std::string>& symbols) override;
		virtual bool subscribe_option_quote(const std::vector<std::string>& symbols) override;
		virtual bool unsubscribe_quote(const std::vector<std::string>& symbols) override;

		virtual void set_quote_depth_changed_callback(const std::function<void(const tigeropen::push::pb::QuoteDepthData&)>& cb) override;
		virtual bool subscribe_quote_depth(const std::vector<std::string>& symbols) override;
		virtual bool unsubscribe_quote_depth(const std::vector<std::string>& symbols) override;

		virtual void set_kline_changed_callback(const std::function<void(const tigeropen::push::pb::KlineData&)>& cb) override;
		virtual bool subscribe_kline(const std::vector<std::string>& symbols) override;
		virtual bool unsubscribe_kline(const std::vector<std::string>& symbols) override;

		virtual void set_full_tick_changed_callback(const std::function<void(const tigeropen::push::pb::TickData&)>& cb) override;
		virtual void set_tick_changed_callback(const std::function<void(const TradeTick&)>& cb) override;
		virtual bool subscribe_tick(const std::vector<std::string>& symbols) override;
		virtual bool unsubscribe_tick(const std::vector<std::string>& symbols) override;

		virtual bool subscribe_market(const std::string& market) override;
		virtual bool unsubscribe_market(const std::string& market) override;

		virtual void set_stock_top_changed_callback(const std::function<void(const tigeropen::push::pb::StockTopData&)>& cb) override;
		virtual bool subscribe_stock_top(const std::string& market) override;
		virtual bool unsubscribe_stock_top(const std::string& market) override;
		
		virtual void set_option_top_changed_callback(const std::function<void(const tigeropen::push::pb::OptionTopData&)>& cb) override;
		virtual bool subscribe_option_top(const std::string& market) override;
		virtual bool unsubscribe_option_top(const std::string& market) override;

		virtual const ClientConfig& get_client_config() const override;

	private:
		bool send_frame(const tigeropen::push::pb::Request& request);
		void do_write(const std::string& frame);
		void do_disconnect();
		void on_message(const std::shared_ptr<tigeropen::push::pb::Response>& response_pb_object);
		bool send_trade_request(tigeropen::push::pb::SocketCommon_Command command, tigeropen::push::pb::SocketCommon_DataType datatype, const std::string& account);
		bool send_quote_request(tigeropen::push::pb::SocketCommon_Command command, tigeropen::push::pb::SocketCommon_DataType datatype, std::vector<std::string> symbols, const std::string& market);
		bool send_quote_request(tigeropen::push::pb::SocketCommon_Command command, tigeropen::push::pb::SocketCommon_DataType datatype, std::vector<std::string> symbols);
		std::shared_ptr<tigeropen::push::pb::QuoteBasicData> convert_to_basic_data(const tigeropen::push::pb::QuoteData& quote_data);
		std::shared_ptr<tigeropen::push::pb::QuoteBBOData> convert_to_bbo_data(const tigeropen::push::pb::QuoteData& quote_data);
		std::shared_ptr<TIGER_API::TradeTick> convert_tick(const tigeropen::push::pb::TradeTickData &data);
	private:
		std::function<void(const tigeropen::push::pb::Response& subscribe_response)> subscribe_callback_;
		std::function<void(const tigeropen::push::pb::Response& unsubscribe_response)> unsubscribe_callback_;
		std::function<void(const tigeropen::push::pb::Response& error_response)> error_callback_;
		std::function<void(const tigeropen::push::pb::Response& kickout_response)> kickout_callback_;
		std::function<void(const tigeropen::push::pb::AssetData& asset_data)> asset_changed_;
		std::function<void(const tigeropen::push::pb::PositionData& position_data)> position_changed_;
		std::function<void(const tigeropen::push::pb::OrderStatusData& order_status_data)> order_changed_;
		std::function<void(const tigeropen::push::pb::OrderTransactionData& transaction_data)> transaction_changed_;
		std::function<void(const tigeropen::push::pb::Response& query_subscribed_symbols_response)> query_subscribed_symbols_changed_;
		std::function<void(const tigeropen::push::pb::QuoteBasicData& quote_data)> quote_changed_;
		std::function<void(const tigeropen::push::pb::QuoteBBOData& quote_bbo_data)> quote_bbo_changed_;
		std::function<void(const tigeropen::push::pb::QuoteDepthData& quote_depth_data)> quote_depth_changed_;
		std::function<void(const tigeropen::push::pb::KlineData& kline_data)> kline_changed_;
		std::function<void(const tigeropen::push::pb::TickData& full_tick_data)> full_tick_changed_;
		std::function<void(const TradeTick& tick_data)> tick_changed_;
		std::function<void(const tigeropen::push::pb::StockTopData& stock_top_data)> stock_top_changed_;
		std::function<void(const tigeropen::push::pb::OptionTopData& option_top_data)> option_top_changed_;
	
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
