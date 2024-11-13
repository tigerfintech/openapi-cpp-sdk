#ifndef PUSH_CLIENT_H
#define PUSH_CLIENT_H
#include <memory>
#include <functional>
#include "tigerapi/win32.h"
#include "openapi_pb/pb_source/AssetData.pb.h"
#include "openapi_pb/pb_source/PositionData.pb.h"
#include "openapi_pb/pb_source/OrderData.pb.h"
#include "openapi_pb/pb_source/QuoteData.pb.h"
#include "openapi_pb/pb_source/KlineData.pb.h"
#include "openapi_pb/pb_source/TickData.pb.h"
#include "openapi_pb/pb_source/StockTopData.pb.h"
#include "openapi_pb/pb_source/OptionTopData.pb.h"
#include "openapi_pb/pb_source/QuoteDepthData.pb.h"


namespace TIGER_API
{
	class ClientConfig;
	class OPENAPI_EXPORT IPushClient
	{
	public:
		static std::shared_ptr<IPushClient> create_push_client(const TIGER_API::ClientConfig& client_config);
		virtual void connect() = 0;
		virtual void disconnect() = 0;

		virtual void set_connected_callback(const std::function<void()>& cb) = 0;
		virtual void set_disconnected_callback(const std::function<void()>& cb) = 0;
		virtual void set_inner_error_callback(const std::function<void(std::string)>& cb) = 0;
		
		virtual void set_asset_changed_callback(const std::function<void(const tigeropen::push::pb::AssetData&)>& cb) = 0;
		virtual bool subscribe_asset(const std::string& account) = 0;
		virtual bool unsubscribe_asset(const std::string& account) = 0;
		
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


		
		//TODO:other
	};
}

#endif // PUSH_CLIENT_H
