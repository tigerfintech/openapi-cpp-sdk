// openapi_cpp_test.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
#include <functional>
#include "tigerapi/push_client.h"
#include "tigerapi/tiger_client.h"
#include "tigerapi/quote_client.h"
#include "tigerapi/trade_client.h"
#include "tigerapi/contract_util.h"
#include "tigerapi/model.h"
#include "tigerapi/order_util.h"
#include "tigerapi/utils.h"
#include "cpprest/details/basic_types.h"
#include "tigerapi/price_util.h"
#include "tigerapi/easylogging++.h"
#include <chrono>
#include <thread>


using namespace std;
using namespace web;
using namespace web::json;
using namespace TIGER_API;

/**
 * Test Trade Client
 */
class TestTradeClient {
public:
    static void test_get_prime_asset(const std::shared_ptr<TradeClient>& trade_client) {
        value res = trade_client->get_prime_asset();
        ucout << U("asset: ") << res << endl;
    }

    static void test_get_prime_portfolio(const std::shared_ptr<TradeClient>& trade_client) {
        PortfolioAccount res = trade_client->get_prime_portfolio();
        ucout << U("portfolio: ") << res.to_string() << endl;
    }

    static void test_get_asset(const std::shared_ptr<TradeClient>& trade_client) {
        value res = trade_client->get_asset();
        ucout << U("asset: ") << res << endl;
    }

    static void test_get_position(const std::shared_ptr<TradeClient>& trade_client) {
        value res = trade_client->get_positions();
        ucout << U("position: ") << res << endl;
    }

    static void test_get_position_list(const std::shared_ptr<TradeClient>& trade_client) {
        vector<Position> res = trade_client->get_position_list();
        ucout << U("position size: ") << res.size() << U(" , first item: ") << res[0].to_string() << endl;
    }

    static void test_get_orders(const std::shared_ptr<TradeClient>& trade_client) {
        value res = trade_client->get_orders();
        ucout << U("orders: ") << res << endl;
    }

    static void test_get_active_orders(const std::shared_ptr<TradeClient>& trade_client) {
        value res = trade_client->get_active_orders();
        ucout << U("active orders: ") << res << endl;
    }

    static void test_get_transactions(const std::shared_ptr<TradeClient>& trade_client) {
        value res = trade_client->get_transactions(trade_client->client_config.account, U("AAPL"));
        ucout << U("transactions: ") << res << endl;
    }

    static void test_get_contract(const std::shared_ptr<TradeClient>& trade_client) {
        value res = trade_client->get_contract(U("AAPL"));
        ucout << U("contract: ") << res << endl;
    }

    static void test_get_contracts(const std::shared_ptr<TradeClient>& trade_client) {
        value symbols = value::array();
        symbols[0] = value::string(U("AAPL"));
        symbols[1] = value::string(U("JD"));
        value res = trade_client->get_contracts(symbols, U("STK"));
        ucout << U("contracts: ") << res << endl;
    }

    static void test_get_quote_contract(const std::shared_ptr<TradeClient>& trade_client) {
        value res = trade_client->get_quote_contract(U("00700"), U("IOPT"), U("20230728"));
        ucout << U("quote contract: ") << res << endl;
    }

    static void test_get_estimate_tradable_quantity(const std::shared_ptr<TradeClient>& trade_client) {
        Contract contract = ContractUtil::stock_contract(U("AAPL"), U("USD"));
        Order order = OrderUtil::limit_order(contract, U("BUY"), 1, 100.0);
        value res = trade_client->get_estimate_tradable_quantity(order);
        ucout << U("estimate tradable quantity: ") << res << endl;
    }

    static void test_place_order(const std::shared_ptr<TradeClient>& trade_client) {
        Contract contract = ContractUtil::stock_contract(U("AAPL"), U("USD"));
        Order order = OrderUtil::limit_order(contract, U("BUY"), 1, 100.0);
        value res = trade_client->place_order(order);
        unsigned long long id = res[U("id")].as_number().to_uint64();
        ucout << U("return id: ") << id << endl;
        ucout << U("order id: ") << order.id << endl;
        ucout << U("place order result: ") << res << endl;
    }

    static void test_place_future_order(const std::shared_ptr<TradeClient>& trade_client) {
        Contract contract = ContractUtil::future_contract(U("NG2308"), U("USD"));
        Order order = OrderUtil::limit_order(contract, U("BUY"), 1, 1.5);
        value res = trade_client->place_order(order);
        unsigned long long id = res[U("id")].as_number().to_uint64();
        ucout << U("return id: ") << id << endl;
        ucout << U("order id: ") << order.id << endl;
        ucout << U("place order result: ") << res << endl;
    }

    static void test_place_option_order(const std::shared_ptr<TradeClient>& trade_client) {
        Contract contract = ContractUtil::option_contract(U("TQQQ"), U("20230915"), U("43.0"), U("CALL"), U("USD"));
        //Contract contract = ContractUtil::option_contract(U("AAPL"), U("20230721"), U("185.0"), U("PUT"), U("USD"));
        //Contract contract = ContractUtil::option_contract(U("AAPL 230721C00185000"));
        //Contract contract = ContractUtil::option_contract(U("AAPL 230721P00185000"));
        Order order = OrderUtil::limit_order(contract, U("BUY"), 3, U("0.18"));
       // order.adjust_limit = 0.01;
        value res = trade_client->place_order(order);                          
        //unsigned long long id = res[U("id")].as_number().to_uint64();
        ucout << U("order id: ") << order.id << endl;
        ucout << U("place order result: ") << res << endl;
    }


    static void test_get_order(const std::shared_ptr<TradeClient>& trade_client) {
        //        Contract contract = stock_contract(U("AAPL"), U("USD"));
        //        Order order = OrderUtil::limit_order(contract, U("BUY"), 1, 100.0);
        //        trade_client->place_order(order);
        Order my_order = trade_client->get_order(31318009878020096);
        ucout << U("order id ") << my_order.id << endl;
        ucout << U("order : ") << my_order.to_string() << endl;
    }

    static void test_cancel_order(const std::shared_ptr<TradeClient>& trade_client) {
        value res = trade_client->cancel_order(31319396151853056);
        ucout << U("cancel order : ") << res << endl;
    }

    static void test_modify_order(const std::shared_ptr<TradeClient>& trade_client) {
        Contract contract = ContractUtil::stock_contract(U("AAPL"), U("USD"));
        Order order = OrderUtil::limit_order(contract, U("BUY"), 1, 100.0);
        long id = (long)trade_client->place_order(order)[U("id")].as_number().to_uint64();
        value res = trade_client->modify_order(order, 105);
        ucout << U("modify order res: ") << res << endl;
        Order mod_order = trade_client->get_order(id);
        ucout << U("modified order: ") << mod_order.to_string() << endl;
    }

    static void test_get_analytics_asset(const std::shared_ptr<TradeClient>& trade_client) {
        value res = trade_client->get_analytics_asset(trade_client->client_config.account, U("2023-11-01"),
                                                      U("2023-12-31"));
        ucout << U("analytics asset: ") << res << endl;
    }

    static void test_get_segment_fund_history(const std::shared_ptr<TradeClient>& trade_client) {
        value res = trade_client->get_segment_fund_history();
        ucout << U("segment fund history: ") << res << endl;
    }

    static void test_get_segment_fund_available(const std::shared_ptr<TradeClient>& trade_client) {
        value res = trade_client->get_segment_fund_available(U("SEC"));
        ucout << U("segment fund available: ") << res << endl;
    }

    static void test_transfer_segment_fund(const std::shared_ptr<TradeClient>& trade_client) {
        value res = trade_client->transfer_segment_fund(U("SEC"), U("FUT"), 10.0);
        ucout << U("transfer segment fund: ") << res << endl;
    }

    static void test_place_forex_order(const std::shared_ptr<TradeClient>& trade_client) {
        value res = trade_client->place_forex_order(U("SEC"), U("USD"), U("HKD"), 1.0);
        ucout << U("place forex order: ") << res << endl;
    }

    static void test_price_util(const std::shared_ptr<TradeClient>& trade_client) {
        value contract = trade_client->get_contract(U("AAPL"));
        value tick_sizes = contract.at( U("tickSizes") );
        double res = PriceUtil::fix_price_by_tick_size(1.111, tick_sizes);
        ucout << U("fix price by tick size: ") << res << endl;
    }

    static void test_trade(const std::shared_ptr<TradeClient>& trade_client) {
        TestTradeClient::test_price_util(trade_client);
    }

};


/**
 * Test Quote Client
 */

class TestQuoteClient {
public:
    static void test_grab_quote_permission(std::shared_ptr<QuoteClient> quote_client) {
        value perms = quote_client->grab_quote_permission();
        ucout << U("Quote perms: ") << perms << endl;
    }

    static void test_get_symbols(std::shared_ptr<QuoteClient> quote_client) {
        value result = quote_client->get_symbols();
        ucout << U("result: ") << result << endl;
    }

    static void test_get_symbols_names(const std::shared_ptr<QuoteClient> quote_client) {
        //        value result = quote_client->get_all_symbol_names(U("HK"));
        value result = quote_client->get_all_symbol_names(Market::HK, false);
        ucout << U("result: ") << result << endl;
    }

    static void test_get_market_state(const std::shared_ptr<QuoteClient> quote_client) {
        value result = quote_client->get_market_state(U("HK"));
        ucout << U("result: ") << result << endl;
    }


    static void test_get_trading_calendar(const std::shared_ptr<QuoteClient> quote_client) {
        value result = quote_client->get_trading_calendar(U("US"), U("2023-01-01"), U("2023-03-01"));
        ucout << U("result: ") << result << endl;
    }

    static void test_get_timeline(const std::shared_ptr<QuoteClient> quote_client) {
        value symbols = value::array();
        symbols[0] = value::string(U("AAPL"));
        symbols[1] = value::string(U("JD"));
        value result = quote_client->get_timeline(symbols, false, 1675167178931);
        ucout << U("result: ") << result << endl;
    }

    static void test_get_history_timeline(const std::shared_ptr<QuoteClient> quote_client) {
        value symbols = value::array();
        symbols[0] = value::string(U("AAPL"));
        symbols[1] = value::string(U("JD"));
        value result = quote_client->get_history_timeline(symbols, U("2023-01-10"));
        ucout << U("result: ") << result << endl;
    }


    static void test_get_quote_real_time(const std::shared_ptr<QuoteClient> quote_client) {
        value symbols = value::array();
        symbols[0] = value::string(U("AAPL"));
        symbols[1] = value::string(U("JD"));
        auto result = quote_client->get_quote_real_time_value(symbols);
        vector<RealtimeQuote> result1 = quote_client->get_quote_real_time(symbols);
        ucout << U("result: ") << result.at(0).serialize() << endl;
    }

    static void test_get_quote_delay(const std::shared_ptr<QuoteClient> quote_client) {
        value symbols = value::array();
        symbols[0] = value::string(U("AAPL"));
        symbols[1] = value::string(U("JD"));
        value result = quote_client->get_quote_delay(symbols);
        ucout << U("result: ") << result << endl;
    }

    static void test_get_quote_depth(const std::shared_ptr<QuoteClient> quote_client) {
        value symbols = value::array();
        symbols[0] = value::string(U("AAPL"));
        symbols[1] = value::string(U("JD"));
        value result = quote_client->get_quote_depth(symbols);
        ucout << U("result: ") << result << endl;
    }

    static void test_get_shortable_stocks(const std::shared_ptr<QuoteClient> quote_client) {
        value symbols = value::array();
        symbols[0] = value::string(U("AAPL"));
        symbols[1] = value::string(U("JD"));
        value result = quote_client->get_quote_shortable_stocks(symbols);
        ucout << U("result: ") << result << endl;
    }

    static void test_get_kline(std::shared_ptr<QuoteClient> quote_client) {
        value symbols = value::array();
        symbols[0] = value::string(U("AAPL"));
        symbols[1] = value::string(U("JD"));
        //ucout << U("symbols ") << symbols << endl;
//        value result = quote_client->get_kline(symbols, U("day"), -1, -1, U("br"), 5);
        vector<Kline> result = quote_client->get_kline(symbols, U("day"), -1, -1, 5);
        ucout << result.at(0).to_string() << endl;
    }

    static void test_get_quote_stock_trade(std::shared_ptr<QuoteClient> quote_client) {
        value symbols = value::array();
        symbols[0] = value::string(U("AAPL"));
        symbols[1] = value::string(U("JD"));
        value result = quote_client->get_quote_stock_trade(symbols);
        ucout << U("Result: ") << result << endl;
    }

    static void test_get_trade_tick(std::shared_ptr<QuoteClient> quote_client) {
        value symbols = value::array();
        symbols[0] = value::string(U("AAPL"));
        symbols[1] = value::string(U("JD"));
        value result = quote_client->get_trade_tick(symbols, 0, 100000000);
        ucout << U("Result: ") << result << endl;
    }

    static void test_get_capital_flow(std::shared_ptr<QuoteClient> quote_client) {
        value result = quote_client->get_capital_flow(U("AAPL"), U("US"), U("intraday"));
        ucout << U("Result: ") << result << endl;
    }

    static void test_get_capital_distribution(std::shared_ptr<QuoteClient> quote_client) {
        value result = quote_client->get_capital_distribution(U("AAPL"));
        ucout << U("Result: ") << result << endl;
    }

    static void test_get_future_exchange(std::shared_ptr<QuoteClient> quote_client) {
        value result = quote_client->get_future_exchange();
        ucout << U("Result: ") << result << endl;
    }

    static void test_get_future_contracts(std::shared_ptr<QuoteClient> quote_client) {
        value result = quote_client->get_future_contracts(U("CL"));
        ucout << U("Result: ") << result << endl;
    }

    static void test_get_future_current_contract(std::shared_ptr<QuoteClient> quote_client) {
        value result = quote_client->get_future_current_contract(U("CL"));
        ucout << U("Result: ") << result << endl;
    }

    static void test_get_future_continuous_contracts(std::shared_ptr<QuoteClient> quote_client) {
        value result = quote_client->get_future_continuous_contracts(U("CL"));
        ucout << U("Result: ") << result << endl;
    }

    static void test_get_future_contract_by_contract_code(std::shared_ptr<QuoteClient> quote_client) {
        value result = quote_client->get_future_contract_by_contract_code(U("CL2303"));
        ucout << U("Result: ") << result << endl;
    }

    static void test_get_future_trading_date(std::shared_ptr<QuoteClient> quote_client) {
        value result = quote_client->get_future_trading_date(U("CL2303"), U("2023-01-12"));
        ucout << U("Result: ") << result << endl;
    }

    static void test_get_future_kline(std::shared_ptr<QuoteClient> quote_client) {
        value symbols = value::array();
        symbols[0] = value::string(U("CL2303"));
        ucout << U("symbols ") << symbols << endl;
//        value result = quote_client->get_future_kline(symbols);
        vector<Kline> result = quote_client->get_future_kline(symbols, BarPeriod::DAY);
        ucout << U("result: ") << result.at(0).to_string() << endl;
    }

    static void test_get_future_tick(std::shared_ptr<QuoteClient> quote_client) {
        value result = quote_client->get_future_tick(U("CL2312"));
        ucout << U("result: ") << result << endl;
    }

    static void test_get_future_real_time_quote(std::shared_ptr<QuoteClient> quote_client) {
        value symbols = value::array();
        symbols[0] = value::string(U("CL2303"));
        ucout << U("symbols ") << symbols << endl;
//        value result = quote_client->get_future_real_time_quote(symbols);
        auto result = quote_client->get_future_real_time_quote(symbols);
        ucout << U("result: ") << result.at(0).to_string() << endl;
    }

    static void test_get_option_expiration(std::shared_ptr<QuoteClient> quote_client) {
        value symbols = value::array();
        symbols[0] = value::string(U("TSLA"));
        ucout << U("symbols ") << symbols << endl;
        value result = quote_client->get_option_expiration(symbols);
        ucout << U("result: ") << result << endl;
    }

    static void test_get_option_chain(std::shared_ptr<QuoteClient> quote_client) {
        value result = quote_client->get_option_chain(U("TSLA"), U("2023-01-20"));
        ucout << U("result: ") << result << endl;
    }


    static void test_get_option_brief(std::shared_ptr<QuoteClient> quote_client) {
        value result = quote_client->get_option_brief(U("AAPL 230224C000150000"));
        ucout << U("result: ") << result << endl;
    }

    static void test_get_option_kline(std::shared_ptr<QuoteClient> quote_client) {
        value identifiers = value::array();
        identifiers[0] = value::string(U("AAPL 230224C000150000"));
//        value result = quote_client->get_option_kline(identifiers, 1639026000000, 1649026000000);
        vector<Kline> result = quote_client->get_option_kline(identifiers, 1639026000000, 1649026000000);
        ucout << U("result: ") << result.at(0).to_string() << endl;
    }

    static void test_get_option_trade_tick(std::shared_ptr<QuoteClient> quote_client) {
        value identifiers = value::array();
        identifiers[0] = value::string(U("AAPL 230224C000150000"));
        value result = quote_client->get_option_trade_tick(identifiers);
        ucout << U("result: ") << result << endl;
    }

    static void test_get_warrant_real_time_quote(std::shared_ptr<QuoteClient> quote_client) {
        value result = quote_client->get_warrant_real_time_quote(U("15792"));
        ucout << U("result: ") << result << endl;
    }

    static void test_get_warrant_filter(std::shared_ptr<QuoteClient> quote_client) {
        value result = quote_client->get_warrant_filter(U("00700"));
        ucout << U("result: ") << result << endl;
    }

    static void test_get_kline_quota(std::shared_ptr<QuoteClient> quote_client) {
        value result = quote_client->get_kline_quota();
        ucout << U("result: ") << result << endl;
    }


    static void test_quote(const std::shared_ptr<QuoteClient> quote_client) {
        TestQuoteClient::test_get_kline_quota(quote_client);
    }
};


/**
 * Directly use TigerApi
 */
class TestTigerApi {
public:
    static void test_grab_quote_permission(std::shared_ptr<TigerClient> tigerapi) {
        value obj = value::object(true);
        value perms = tigerapi->post(GRAB_QUOTE_PERMISSION, obj);
        ucout << U("quote perms: ") << perms << endl;
    }

    static void test_get_market_status(std::shared_ptr<TigerClient>  tigerapi) {
        value obj = value::object(true);
        obj[U("market")] = value::string(U("US"));
        tigerapi->post(MARKET_STATE, obj);
    }

    static void test_get_positions(std::shared_ptr<TigerClient>  tigerapi) {
        value obj = value::object(true);
        obj[U("market")] = value::string(U("US"));
        obj[P_ACCOUNT] = value::string(U("1111"));
        tigerapi->post(POSITIONS, obj);
    }
};


std::atomic<bool> keep_running(true);

void signal_handler(int signal)
{
    if (signal == SIGINT || signal == SIGTERM)
    {
        keep_running = false;
    }
}


class TestPushClient {
private:
    std::shared_ptr<IPushClient> push_client;
    std::vector<std::string> symbols;

public:
    TestPushClient(std::shared_ptr<IPushClient> client) : push_client(client) {
        std::vector<std::string> hk_option_symbols = {"TCH.HK 20241230 410.00 CALL"};
        std::vector<std::string> future_symbols = {"CL2412"};
        symbols = future_symbols;
    }

    void connected_callback() {
        ucout << "Connected to push server" << std::endl;
        push_client->subscribe_position(push_client->get_client_config().account);
        push_client->subscribe_order(push_client->get_client_config().account);
        push_client->subscribe_asset(push_client->get_client_config().account);
        // push_client->query_subscribed_symbols();
        push_client->subscribe_quote(symbols);
        // push_client->subscribe_kline(symbols);
        // push_client->subscribe_quote_depth(symbols);
        push_client->subscribe_tick(symbols);
    }

    void position_changed_callback(const tigeropen::push::pb::PositionData& data) {
        ucout << "Position changed:" << std::endl;
        ucout << "- symbol: " << data.symbol() << std::endl;
        ucout << "- positionqty: " << data.positionqty() << std::endl;
    }

    void order_changed_callback(const tigeropen::push::pb::OrderStatusData& data) {
        // example: {"dataType":"OrderStatus","orderStatusData":{"id":"37129891133115200","account":"123123123","symbol":"PDD","identifier":"PDD","multiplier":1,
        // "action":"BUY","market":"US","currency":"USD","segType":"S","secType":"STK","orderType":"LMT","isLong":true,"totalQuantity":"1","limitPrice":50,
        // "status":"PendingSubmit","replaceStatus":"NONE","cancelStatus":"NONE","outsideRth":true,"canModify":true,"canCancel":true,"name":"PDD Holdings","source":"openapi","openTime":"1732177851000","timestamp":"1732177851874"}}
        ucout << "Order changed:" << std::endl;
        ucout << "- id: " << data.id() << std::endl;
        ucout << "- status: " << data.status() << std::endl;
        ucout << "- avgfillprice: " << data.avgfillprice() << std::endl;
    }

    void asset_changed_callback(const tigeropen::push::pb::AssetData& data) {
        // example: {"dataType":"Asset","assetData":{"account":"111111111111","segType":"S","availableFunds":797832.55572773679,
        // "excessLiquidity":826227.79308567208,"netLiquidation":944515.55984458746,"equityWithLoan":944494.85984458751,"buyingPower":3191330.2229109472,
        // "cashBalance":656046.6059349241,"grossPositionValue":288448.25390966347,"initMarginReq":146662.30411685078,"maintMarginReq":118287.76675891539,"timestamp":"1732177851891"}}
        ucout << "Asset changed:" << std::endl;
        ucout << "- cashbalance: " << data.cashbalance() << std::endl;
        ucout << "- netliquidation: " << data.netliquidation() << std::endl;
    }

    void tick_changed_callback(const TradeTick& data) {
        ucout << "TradeTick changed: " << std::endl;
        ucout << "- data: " << data.to_string() << std::endl;
    }

    void full_tick_changed_callback(const tigeropen::push::pb::TickData& data) {
        ucout << "Full TickData changed: " << std::endl;
        ucout << "- symbol: " << data.symbol() << std::endl;
        ucout << "- tick size: " << data.ticks_size() << std::endl;
    }

    void query_subscribed_symbols_changed_callback(const tigeropen::push::pb::Response& data) {
        ucout << "QuerySubscribedSymbols changed: " << std::endl;
        ucout << "- data: " << data.msg() << std::endl;
    }

    void quote_changed_callback(const tigeropen::push::pb::QuoteBasicData& data) {
        ucout << "BasicQuote changed: " << std::endl;
        ucout << "- symbol: " << data.symbol() << std::endl;
        ucout << "- latestPrice: " << data.latestprice() << std::endl;
        ucout << "- volume: " << data.volume() << std::endl;
    }

    void quote_bbo_changed_callback(const tigeropen::push::pb::QuoteBBOData& data) {
        ucout << "BBOQuote changed: " << std::endl;
        ucout << "- symbol: " << data.symbol() << std::endl;
        ucout << "- bidPrice: " << data.bidprice() << std::endl;
        ucout << "- askPrice: " << data.askprice() << std::endl;
    }

    void quote_depth_changed_callback(const tigeropen::push::pb::QuoteDepthData& data) {
        ucout << "QuoteDepth changed: " << std::endl;
        ucout << "- symbol: " << data.symbol() << std::endl;
        ucout << "- ask price size: " << data.ask().price_size() << std::endl;
        ucout << "- bid price size: " << data.bid().price_size() << std::endl;
    }

    void kline_changed_callback(const tigeropen::push::pb::KlineData& data) {
        ucout << "Kline changed: " << std::endl;
        ucout << "- symbol: " << data.symbol() << std::endl;
        ucout << "- open: " << data.open() << std::endl;
        ucout << "- high: " << data.high() << std::endl;
        ucout << "- low: " << data.low() << std::endl;
        ucout << "- close: " << data.close() << std::endl;
    }

    void start_test(ClientConfig config) {
        push_client->set_connected_callback(std::bind(&TestPushClient::connected_callback, this));
        push_client->set_position_changed_callback(std::bind(&TestPushClient::position_changed_callback, this, std::placeholders::_1));
        push_client->set_order_changed_callback(std::bind(&TestPushClient::order_changed_callback, this, std::placeholders::_1));
        push_client->set_asset_changed_callback(std::bind(&TestPushClient::asset_changed_callback, this, std::placeholders::_1));
        push_client->set_tick_changed_callback(std::bind(&TestPushClient::tick_changed_callback, this, std::placeholders::_1));
        push_client->set_full_tick_changed_callback(std::bind(&TestPushClient::full_tick_changed_callback, this, std::placeholders::_1));
        push_client->set_query_subscribed_symbols_changed_callback(std::bind(&TestPushClient::query_subscribed_symbols_changed_callback, this, std::placeholders::_1));
        push_client->set_quote_changed_callback(std::bind(&TestPushClient::quote_changed_callback, this, std::placeholders::_1));
        push_client->set_quote_bbo_changed_callback(std::bind(&TestPushClient::quote_bbo_changed_callback, this, std::placeholders::_1));
        push_client->set_quote_depth_changed_callback(std::bind(&TestPushClient::quote_depth_changed_callback, this, std::placeholders::_1));
        push_client->set_kline_changed_callback(std::bind(&TestPushClient::kline_changed_callback, this, std::placeholders::_1));


        push_client->connect();

        std::signal(SIGINT, signal_handler);  //Ctrl+C
        std::signal(SIGTERM, signal_handler); //kill
        while (keep_running)
        {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

        push_client->unsubscribe_quote(symbols);
        push_client->unsubscribe_kline(symbols);
        push_client->unsubscribe_quote_depth(symbols);
        push_client->unsubscribe_tick(symbols);
        push_client->unsubscribe_asset(config.account);
        push_client->unsubscribe_position(config.account);
        push_client->unsubscribe_order(config.account);
        push_client->disconnect();
    }

    static void test_push_client(std::shared_ptr<IPushClient> push_client, ClientConfig config) {
        TestPushClient test(push_client);
        test.start_test(config);
    }
};


int main()
{
    /************************** set config **********************/
    ClientConfig config = ClientConfig();
#if 1
	// config.private_key = U("");
	// config.tiger_id = U("");
	// config.account = U("");
    // config.private_key = U("MIIEowIBAAKCAQEAmkjAHDZS5DkiS+e73DfAa0jTboHPb7ucuaxpVsRE+d9n6DxR36yVZb0AWMfycJQbbooH/O1YYa8yu7AYoesgG3RVmKKoFaK58nzP821Q/4mRQxOC9MY/URbvMQxwRjSax3kNj4uQVzjkwTObRU/VvdhMhWuusnvht0YkhDmYhrG0ZmKsY+1DW9sHWdPwB3W7n2rAkQwaq6+W0uc1P9qUEU2+UpKYLkIXSaJjj3D110XNl4Z2NuORoIjxcg+7pVxxaX5avkouUKeoKBza1oixiAnlwFJ9jtAAjQyTBfzxlKmSxdP+xhuDhZ+LRsuR9+EHiaFfkfUAFG/NE+90U6aizQIDAQABAoIBADXkK5avMw0dF+nDrE8j+kZ/TbtQBJFWy0vf+XuhWyac7sKkL0UeF6FN/40DaqAAXASkml2y2ihk9iexPylv4+szG6MRTf0gtisJhenVyXE5MFBpxFiT1xIQHMTZP26GeXorwpzddzHAYcdKq3XuiQ6jOzdvgbo+o31NNvwo0ipZ45ZA+fztWPjMr+uOpzDYCCTKd6fnOpD5RHGNCm4cN8PCXStjZeolXua6MZYU78Fvn1KXnwxL8a3a4jbZo3cMgIIOHlAzMWlbw2dEOHGB2gqjxO0MneHoAHSE/rIbsq8U2AmOWdLh2m48aPLRfeXZKjaxTeC4URA7F+guXNKZcNECgYEAqGfmiQ/8UE7bqiS6DuAyPJ49yexZErevMNQbV8j2HulO21YiA5nrrWk8K/0U/Uq+lVOc3D+yie7HJUCF9JsLEbgpozNoYPOKV6Om0/zQRFjXslbWTKMXJsQ0piwqqt6hq0AySmJfEyqeqWzG+sxWdu8F18uYig3Z8Hdr6OeW6a8CgYEA6oh55hGchPEcboQRksn2O7gCjX9FdI9Edt9VU3nzBOyoUf2kJD2cUVFVtSF5Bqhbu2TkQo+Dt49DNLZZN7ZEaBLckDKFGZSdD1qkGo7QUX1pZ5raDOIsUmAnpWEnKxSZYcq3Ls6DtE1UF6wvb4AYRkLk1uH7Gs++Evf3B+ZgpkMCgYB//kf3EFFCcEz/6d4HhFpqPBLWL24bjcP3Jp6Jf9Gf+M7YYYSEnC3FpN/4xud3A6IKOHxV4Yu+zd5J79pf6J7nHsN2qJHCbhdNOLKJF/Lqn2OAxgMmSCZGboVdbuAW8hKJ58bRRibgmFnyhxxMbZWh21TjiHgtJQ3pd7pqT+ohPQKBgQC596mg0oonqAABadt9Z4NF6yyrAiYSvJ9vGrvPyzz1xKnbCaIj0m6m1DV/wKD0tH64YtHULXQmEwUZQRYRZQds9rfJ5yU/GWZIjHjfTirE0wrHSjRoTtcSIoiNTDIAqP7OWi1SghJQ35g0hSW/5fRZkiVediXkJqHy7IgGzZgJfQKBgFuiAlQmqf/DCCSZ9g7qzSw28T+d7mOCAdI8QdY+A8MZ1Vn6HinUY/5exG6kiAAU+kqtDyDY6Uw1I7p4yesOXBKx4mUdgqD3Oyxp66QaVzkr90ZBFgMXVEp+G0EbjvsWlrZRX6nLxf2i0VUMiFTi6GLmkXypMHK0ADNliCHMPQ4k");
    // config.tiger_id = U("20150899");
    // config.account = U("572386");
    config.private_key = U("MIICWwIBAAKBgQCSW+K1OtE7BBvKa6SwTLa87wfvVuJNTjZnqXD+MLfUnVx4f7pGFbGstCHzwcozGvJfDL7lghiLzL3uRRHJTcBkTlTpE4yOStPubJE2jz8IU0U6vBg2rRKVzFKf9wH8iqn8tCcvjqwEEhE5IKfoMFPunDhviltg5jxsb/A/FNPAJQIDAQABAoGAMIIFxJ0m5ZF6CH/eyJPUaRAsKBY60PZgL8FH8ueAthYlpMZonXvpNF1Ld3e+JWwD8p7N0lFpA0SrmoIH4BlF7zh2VyPfG5NSvsTUjQpWPzM6SSQ+7L6N76yYexWvI/4AcxDnH/WW2pu6onZc4iueDMQ2ODTuTz+O8HmcJbTDRg0CQQDFHRPN5+2Py+vUcjS9MLyWgTVZuFRzkELv9viBuusSp8TYnpq2fpGhOaQf/ZpYIA3lCwdx9/s3YlHWtLWd9ciXAkEAvhUs98hwkZChYL9d234vJrDmaqjBbkovYRCL+8BTHTqf1vtghMuEKOkL3998vAUnfIH3Czeoanrr0GsTyKw4owJABSuRFGqL8aSgp+7i8T9xAuWat6K9HFbhWpf4xrZXaiLaZ/ApELZbLYZshqZJQRx+IM7cQCIIBdjfWqlcxirnOwJAEHB8ZyGquqwriNaEI4uSQGJkSy01LXi1CEwKD0fe6FQ9prYu7s07LN6a7E3D1+8KLzZ+SzYRFsRyyJ+6IkziyQJAI9t5akK9P2ps1/NXXMLeJRMe1GtLiG3d5h21x3ehezzG00QDEiOsEHwK4BxyjaybVBqwSRsy1TphvuGcBOaKpA==");
    config.tiger_id = U("20150194");
    config.account = U("20190313161436217");
    config.use_full_tick = true;

#else
	config.private_key = U("");
	config.tiger_id = U("");
	config.account = U("");
#endif

    //config.lang = U("en_US");

    auto push_client = IPushClient::create_push_client(config);
    TestPushClient::test_push_client(push_client, config);
    /**
     *  QuoteClient
     */
    //std::shared_ptr<QuoteClient> quote_client = std::make_shared<QuoteClient>(config);
    //TestQuoteClient::test_quote(quote_client);

    /**
     * TradeClient
     */
     //std::shared_ptr<TradeClient> trade_client = std::make_shared<TradeClient>(config);
     //TestTradeClient::test_trade(trade_client);

     /**
      *  TigerApi
      */
      //    std::shared_ptr<TigerClient> tigerapi = std::make_shared<TigerClient>(config);
      //    TestTigerApi::test_grab_quote_permission(tigerapi);
	


    return 0;
}