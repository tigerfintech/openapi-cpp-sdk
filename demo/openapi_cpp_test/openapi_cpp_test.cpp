// openapi_cpp_test.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
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

using namespace std;
using namespace web;
using namespace web::json;
using namespace TIGER_API;

/**
 * ���ý��׽ӿ�
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
 * ��������ӿ�
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
 * ֱ��ʹ�� TigerApi
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

int main()
{
    /************************** set config **********************/
    ClientConfig config = ClientConfig();
#if 1
	config.private_key = U("");
	config.tiger_id = U("");
	config.account = U("");
        
#else
	config.private_key = U("");
	config.tiger_id = U("");
	config.account = U("");
#endif


	auto push_client = IPushClient::create_push_client(config);
    push_client->set_position_changed_callback([](const tigeropen::push::pb::PositionData& data) {
        std::cout << "Position changed: " << data.DebugString() << std::endl;
    });
    push_client->set_order_changed_callback([](const tigeropen::push::pb::OrderData& data) {
        std::cout << "Order changed: " << data.DebugString() << std::endl;
    });
    push_client->set_asset_changed_callback([](const tigeropen::push::pb::AssetData& data) {
        std::cout << "Asset changed: " << data.DebugString() << std::endl;
    });

    push_client->subscribe_position();
    push_client->subscribe_order();
    push_client->subscribe_asset();
    
    push_client->connect();
	std::string input;
	while (true)
    {
		std::cout << "Enter command (type 'exit' to quit): ";
		std::getline(std::cin, input);

		if (input == "exit") {
			std::cout << "Exiting loop." << std::endl;
            push_client->disconnect();
			break;
		}
		// Process other commands or input here
		std::cout << "You entered: " << input << std::endl;
	}

    //config.lang = U("en_US");


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