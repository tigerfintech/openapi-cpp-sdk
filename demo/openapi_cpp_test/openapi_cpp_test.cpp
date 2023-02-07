// openapi_cpp_test.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
#include "tigerapi/tiger_client.h"
#include "tigerapi/quote_client.h"
#include "tigerapi/trade_client.h"
#include "tigerapi/contract_util.h"
#include "tigerapi/order_util.h"
#include <cpprest/details/basic_types.h>
//#include "common/easylogging++.h"

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

    static void test_get_contract(const std::shared_ptr<TradeClient>& trade_client) {
        value res = trade_client->get_contract(U("AAPL"));
        ucout << U("contract: ") << res << endl;
    }

    static void test_place_order(const std::shared_ptr<TradeClient>& trade_client) {
        Contract contract = stock_contract(U("AAPL"), U("USD"));
        Order order = limit_order(contract, U("BUY"), 1, 100.0);
        value res = trade_client->place_order(order);
        long id = res[U("id")].as_integer();
        ucout << U("order id: ") << id << endl;
        ucout << U("place order result: ") << res << endl;
    }

    static void test_get_order(const std::shared_ptr<TradeClient>& trade_client) {
        //        Contract contract = stock_contract(U("AAPL"), U("USD"));
        //        Order order = limit_order(contract, U("BUY"), 1, 100.0);
        //        trade_client->place_order(order);
        Order my_order = trade_client->get_order(29270263515317248);
        ucout << U("order : ") << my_order.to_string() << endl;
    }

    static void test_cancel_order(const std::shared_ptr<TradeClient>& trade_client) {
        value res = trade_client->cancel_order(29270263515317248);
        ucout << U("cancel order : ") << res << endl;
    }

    static void test_modify_order(const std::shared_ptr<TradeClient>& trade_client) {
        Contract contract = stock_contract(U("AAPL"), U("USD"));
        Order order = limit_order(contract, U("BUY"), 1, 100.0);
        long id = (long)trade_client->place_order(order)[U("id")].as_number().to_uint64();
        value res = trade_client->modify_order(order, 105);
        ucout << U("modify order res: ") << res << endl;
        Order mod_order = trade_client->get_order(id);
        ucout << U("modified order: ") << mod_order.to_string() << endl;
    }


    static void test_trade(const std::shared_ptr<TradeClient>& trade_client) {
        TestTradeClient::test_place_order(trade_client);
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
        value result = quote_client->get_all_symbol_names(Market::HK);
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
        value result = quote_client->get_quote_real_time(symbols);
        ucout << U("result: ") << result << endl;
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
        value result = quote_client->get_kline(symbols, U("day"), -1, -1, U("br"), 5);
        //        ucout << U("result: ") << result << endl;
        //LOG(INFO) << U("result: ") << result.serialize() << endl;
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
        value result = quote_client->get_trade_tick(symbols, 0, 100);
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
        value result = quote_client->get_future_kline(symbols);
        ucout << U("result: ") << result << endl;
    }

    static void test_get_future_tick(std::shared_ptr<QuoteClient> quote_client) {
        value result = quote_client->get_future_tick(U("CL2312"));
        ucout << U("result: ") << result << endl;
    }

    static void test_get_future_real_time_quote(std::shared_ptr<QuoteClient> quote_client) {
        value symbols = value::array();
        symbols[0] = value::string(U("CL2303"));
        ucout << U("symbols ") << symbols << endl;
        value result = quote_client->get_future_real_time_quote(symbols);
        ucout << U("result: ") << result << endl;
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
        value result = quote_client->get_option_brief(U("AAPL 230317C000135000"));
        ucout << U("result: ") << result << endl;
    }

    static void test_get_option_kline(std::shared_ptr<QuoteClient> quote_client) {
        value identifiers = value::array();
        identifiers[0] = value::string(U("AMD 220819C000165000"));
        value result = quote_client->get_option_kline(identifiers, 1639026000000, 1649026000000);
        ucout << U("result: ") << result << endl;
    }

    static void test_get_option_trade_tick(std::shared_ptr<QuoteClient> quote_client) {
        value identifiers = value::array();
        identifiers[0] = value::string(U("AAPL 230317C000135000"));
        value result = quote_client->get_option_trade_tick(identifiers);
        ucout << U("result: ") << result << endl;
    }

    static void test_quote(const std::shared_ptr<QuoteClient> quote_client) {
        TestQuoteClient::test_get_timeline(quote_client);
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
        obj[P_ACCOUNT] = value::string(U("402901"));
        tigerapi->post(POSITIONS, obj);
    }
};

int main()
{
    /************************** set config **********************/
    ClientConfig config = ClientConfig(true);

    config.private_key = U("-----BEGIN RSA PRIVATE KEY-----\n")
        U("MIICXQIBAAKBgQC1amZa5YsGTklry7DAsUBOwXJCgrsZZtB21PImw/yLmrbqRfsS\n")
        U("3vawvMigLWcCwIDnHa+hpdpeze0eHIwbZzJzUDGvRALYK9t3D8pwPVxpwX1OF8Rf\n")
        U("HCM7YQvSOvPPnHHuVQvKaR7NNm1/WmvGXC9kVJdkYQ7kCmh52siFoy1MLQIDAQAB\n")
        U("AoGAVabcmIHTt7ByncBXvUJymDxhE+HhMEcImXJEueTCca8kOUu9FNXMJvmax3Vo\n")
        U("MzZsJbIwX+OMTEJxd0wHIlEA0gECjDwFK4Q42q+ptO4QABJQVSC6I+dOt2OIY28u\n")
        U("vT3rkenOO8KRIDt4F52PFd71ZdB1aaXixORORq1MdSLi8EkCQQDiviAB+L5R/HVx\n")
        U("wxvqZfJ530OtFd5IipZC9YZlY1CtXWCmu89LK7UUlEuNXyGsOxyz5jLqFuNRsie5\n")
        U("AC23tfEPAkEAzNMCa8axJWfPZIH4tGrbZ1F3I41BQdgp2zBmR7AyUMBDkli86Ozm\n")
        U("J7QUCJA/PJxK43/IYUWm4OU5Q+SvXCr3AwJBAJTBj1Y7zwES1CpSitn5EF+MbmX7\n")
        U("1t1YrsQ3OHkD80YJ4QMCbDkw75gUwox5QSoxjd8ow3Z4laJfc1gYGeZQ41kCQQCC\n")
        U("iQwm8cceBq3W6To+iUdw7itWngRz2Ta7uXnFwFYgvpeR4jnq3GfF7+9AkeWrVBQq\n")
        U("Ltrem0xCUfQP/+N+gudPAkBFLbt78/MpQGEDc7jyu/KE5Mp4wMMDQQwch9VLvsAZ\n")
        U("wWLysB6rZWpo3jIfp9zZ7c3zOYGNMWAZjtMmNkRJ8COH\n")
        U("-----END RSA PRIVATE KEY-----");
    config.tiger_id = U("2");
    config.account = U("402901");
//    config.lang = U("en_US");


    /**
     * ʹ�÷�װ�������ӿ� QuoteClient
     */
    std::shared_ptr<QuoteClient> quote_client = std::make_shared<QuoteClient>(config);
    //    quote_client->grab_quote_permission();
    TestQuoteClient::test_quote(quote_client);

    /**
     * ʹ�÷�װ��Ľ��׽ӿ� TradeClient
     */
//         std::shared_ptr<TradeClient> trade_client = std::make_shared<TradeClient>(config);
//         TestTradeClient::test_trade(trade_client);

         /**
          * ֱ��ʹ��δ��װ�� TigerApi
          */
          //    std::shared_ptr<TigerClient> tigerapi = std::make_shared<TigerClient>(config);
          //    TestTigerApi::test_grab_quote_permission(tigerapi);



    return 0;
}