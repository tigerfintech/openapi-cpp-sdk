#include "../include/tigerapi/tiger_client.h"
#include "../include/tigerapi/quote_client.h"
#include "../include/tigerapi/trade_client.h"
#include "../include/tigerapi/contract_util.h"
#include "../include/tigerapi/order_util.h"

using namespace std;
using namespace web;
using namespace web::json;
using namespace TIGER_API;

/**
 * 调用交易接口
 */
class TestTradeClient {
public:
    static void test_get_prime_asset(const std::shared_ptr<TradeClient>& trade_client) {
        value res = trade_client->get_prime_asset();
        cout << "asset: " << res << endl;
    }

    static void test_get_prime_portfolio(const std::shared_ptr<TradeClient>& trade_client) {
        PortfolioAccount res = trade_client->get_prime_portfolio();
        cout << "portfolio: " << res.to_string() << endl;
    }

    static void test_get_asset(const std::shared_ptr<TradeClient>& trade_client) {
        value res = trade_client->get_asset();
        cout << "asset: " << res << endl;
    }

    static void test_get_position(const std::shared_ptr<TradeClient>& trade_client) {
        value res = trade_client->get_positions();
        cout << "position: " << res << endl;
    }

    static void test_get_position_list(const std::shared_ptr<TradeClient>& trade_client) {
        vector<Position> res = trade_client->get_position_list();
        cout << "position size: " << res.size()  << " , first item: " << res[0].to_string() << endl;
    }

    static void test_get_orders(const std::shared_ptr<TradeClient>& trade_client) {
        value res = trade_client->get_orders();
        cout << "orders: " << res << endl;
    }

    static void test_get_active_orders(const std::shared_ptr<TradeClient>& trade_client) {
        value res = trade_client->get_active_orders();
        cout << "active orders: " << res << endl;
    }

    static void test_get_contract(const std::shared_ptr<TradeClient>& trade_client) {
        value res = trade_client->get_contract("AAPL");
        cout << "contract: " << res << endl;
    }

    static void test_place_order(const std::shared_ptr<TradeClient>& trade_client) {
        Contract contract = stock_contract("AAPL", "USD");
        Order order = limit_order(contract, "BUY", 1, 100.0);
        value res = trade_client->place_order(order);
        long id = res["id"].as_integer();
        cout << "order id: " << id << endl;
        cout << "place order result: " << res << endl;
    }

    static void test_get_order(const std::shared_ptr<TradeClient>& trade_client) {
//        Contract contract = stock_contract("AAPL", "USD");
//        Order order = limit_order(contract, "BUY", 1, 100.0);
//        trade_client->place_order(order);
        Order my_order = trade_client->get_order(29270263515317248);
        cout << "order : " << my_order.to_string() << endl;
    }

    static void test_cancel_order(const std::shared_ptr<TradeClient>& trade_client) {
        value res = trade_client->cancel_order(29270263515317248);
        cout << "cancel order : " << res << endl;
    }

    static void test_modify_order(const std::shared_ptr<TradeClient>& trade_client) {
        Contract contract = stock_contract("AAPL", "USD");
        Order order = limit_order(contract, "BUY", 1, 100.0);
        long id = (long) trade_client->place_order(order)["id"].as_number().to_uint64();
        value res = trade_client->modify_order(order, 105);
        cout << "modify order res: " << res << endl;
        Order mod_order = trade_client->get_order(id);
        cout << "modified order: " << mod_order.to_string() << endl;
    }


    static void test_trade(const std::shared_ptr<TradeClient>& trade_client) {
        TestTradeClient::test_get_position(trade_client);
    }
};


/**
 * 调用行情接口
 */

class TestQuoteClient {
public:
    static void test_grab_quote_permission(std::shared_ptr<QuoteClient> quote_client) {
        value perms = quote_client->grab_quote_permission();
        cout << "Quote perms: " << perms << endl;
    }

    static void test_get_symbols(std::shared_ptr<QuoteClient> quote_client) {
        value result = quote_client->get_symbols();
        cout << "result: " << result << endl;
    }

    static void test_get_symbols_names(const std::shared_ptr<QuoteClient> quote_client) {
//        value result = quote_client->get_all_symbol_names("HK");
        value result = quote_client->get_all_symbol_names(Market::HK);
        cout << "result: " << result << endl;
    }

    static void test_get_market_state(const std::shared_ptr<QuoteClient> quote_client) {
        value result = quote_client->get_market_state("HK");
        cout << "result: " << result << endl;
    }


    static void test_get_trading_calendar(const std::shared_ptr<QuoteClient> quote_client) {
        value result = quote_client->get_trading_calendar("US", "2023-01-01", "2023-03-01");
        cout << "result: " << result << endl;
    }

    static void test_get_timeline(const std::shared_ptr<QuoteClient> quote_client) {
        value symbols = value::array();
        symbols[0] = value::string("AAPL");
        symbols[1] = value::string("JD");
        value result = quote_client->get_timeline(symbols);
        cout << "result: " << result << endl;
    }

    static void test_get_history_timeline(const std::shared_ptr<QuoteClient> quote_client) {
        value symbols = value::array();
        symbols[0] = value::string("AAPL");
        symbols[1] = value::string("JD");
        value result = quote_client->get_history_timeline(symbols, "2023-01-10");
        cout << "result: " << result << endl;
    }


    static void test_get_quote_real_time(const std::shared_ptr<QuoteClient> quote_client) {
        value symbols = value::array();
        symbols[0] = value::string("AAPL");
        symbols[1] = value::string("JD");
        value result = quote_client->get_quote_real_time(symbols);
        cout << "result: " << result << endl;
    }

    static void test_get_quote_delay(const std::shared_ptr<QuoteClient> quote_client) {
        value symbols = value::array();
        symbols[0] = value::string("AAPL");
        symbols[1] = value::string("JD");
        value result = quote_client->get_quote_delay(symbols);
        cout << "result: " << result << endl;
    }

    static void test_get_quote_depth(const std::shared_ptr<QuoteClient> quote_client) {
        value symbols = value::array();
        symbols[0] = value::string("AAPL");
        symbols[1] = value::string("JD");
        value result = quote_client->get_quote_depth(symbols);
        cout << "result: " << result << endl;
    }

    static void test_get_shortable_stocks(const std::shared_ptr<QuoteClient> quote_client) {
        value symbols = value::array();
        symbols[0] = value::string("AAPL");
        symbols[1] = value::string("JD");
        value result = quote_client->get_quote_shortable_stocks(symbols);
        cout << "result: " << result << endl;
    }

    static void test_get_kline(std::shared_ptr<QuoteClient> quote_client) {
        value symbols = value::array();
        symbols[0] = value::string("AAPL");
        symbols[1] = value::string("JD");
        cout << "symbols " << symbols << endl;
        value result = quote_client->get_kline(symbols, "day", -1, -1, "br", 5);
        cout << "result: " << result << endl;
    }

    static void test_get_quote_stock_trade(std::shared_ptr<QuoteClient> quote_client) {
        value symbols = value::array();
        symbols[0] = value::string("AAPL");
        symbols[1] = value::string("JD");
        value result = quote_client->get_quote_stock_trade(symbols);
        cout << "Result: " << result << endl;
    }

    static void test_get_trade_tick(std::shared_ptr<QuoteClient> quote_client) {
        value symbols = value::array();
        symbols[0] = value::string("AAPL");
        symbols[1] = value::string("JD");
        value result = quote_client->get_trade_tick(symbols, 0, 100);
        cout << "Result: " << result << endl;
    }

    static void test_get_capital_flow(std::shared_ptr<QuoteClient> quote_client) {
        value result = quote_client->get_capital_flow("AAPL", "US", "intraday");
        cout << "Result: " << result << endl;
    }

    static void test_get_capital_distribution(std::shared_ptr<QuoteClient> quote_client) {
        value result = quote_client->get_capital_distribution("AAPL");
        cout << "Result: " << result << endl;
    }

    static void test_get_future_exchange(std::shared_ptr<QuoteClient> quote_client) {
        value result = quote_client->get_future_exchange();
        cout << "Result: " << result << endl;
    }

    static void test_get_future_contracts(std::shared_ptr<QuoteClient> quote_client) {
        value result = quote_client->get_future_contracts("CL");
        cout << "Result: " << result << endl;
    }

    static void test_get_future_current_contract(std::shared_ptr<QuoteClient> quote_client) {
        value result = quote_client->get_future_current_contract("CL");
        cout << "Result: " << result << endl;
    }

    static void test_get_future_continuous_contracts(std::shared_ptr<QuoteClient> quote_client) {
        value result = quote_client->get_future_continuous_contracts("CL");
        cout << "Result: " << result << endl;
    }

    static void test_get_future_contract_by_contract_code(std::shared_ptr<QuoteClient> quote_client) {
        value result = quote_client->get_future_contract_by_contract_code("CL2303");
        cout << "Result: " << result << endl;
    }

    static void test_get_future_trading_date(std::shared_ptr<QuoteClient> quote_client) {
        value result = quote_client->get_future_trading_date("CL2303", "2023-01-12");
        cout << "Result: " << result << endl;
    }

    static void test_get_future_kline(std::shared_ptr<QuoteClient> quote_client) {
        value symbols = value::array();
        symbols[0] = value::string("CL2303");
        cout << "symbols " << symbols << endl;
        value result = quote_client->get_future_kline(symbols);
        cout << "result: " << result << endl;
    }

    static void test_get_future_tick(std::shared_ptr<QuoteClient> quote_client) {
        value result = quote_client->get_future_tick("CL2312");
        cout << "result: " << result << endl;
    }

    static void test_get_future_real_time_quote(std::shared_ptr<QuoteClient> quote_client) {
        value symbols = value::array();
        symbols[0] = value::string("CL2303");
        cout << "symbols " << symbols << endl;
        value result = quote_client->get_future_real_time_quote(symbols);
        cout << "result: " << result << endl;
    }

    static void test_get_option_expiration(std::shared_ptr<QuoteClient> quote_client) {
        value symbols = value::array();
        symbols[0] = value::string("TSLA");
        cout << "symbols " << symbols << endl;
        value result = quote_client->get_option_expiration(symbols);
        cout << "result: " << result << endl;
    }

    static void test_get_option_chain(std::shared_ptr<QuoteClient> quote_client) {
        value result = quote_client->get_option_chain("TSLA", "2023-01-20");
        cout << "result: " << result << endl;
    }


    static void test_get_option_brief(std::shared_ptr<QuoteClient> quote_client) {
        value result = quote_client->get_option_brief("AAPL 220923C000155000");
        cout << "result: " << result << endl;
    }

    static void test_get_option_kline(std::shared_ptr<QuoteClient> quote_client) {
        value identifiers = value::array();
        identifiers[0] = value::string("AAPL 220923C000155000");
        value result = quote_client->get_option_kline(identifiers);
        cout << "result: " << result << endl;
    }

    static void test_get_option_trade_tick(std::shared_ptr<QuoteClient> quote_client) {
        value identifiers = value::array();
        identifiers[0] = value::string("AAPL 230923C000155000");
        value result = quote_client->get_option_trade_tick(identifiers);
        cout << "result: " << result << endl;
    }

    static void test_quote(const std::shared_ptr<QuoteClient> quote_client) {
        TestQuoteClient::test_get_option_trade_tick(quote_client);
    }
};


/**
 * 直接使用 TigerApi
 */
class TestTigerApi {
public:
    static void test_grab_quote_permission(std::shared_ptr<TigerClient> tigerapi) {
        value obj = value::object(true);
        value perms = tigerapi->post(GRAB_QUOTE_PERMISSION, obj);
        cout << "quote perms: " << perms << endl;
    }

    static void test_get_market_status(std::shared_ptr<TigerClient>  tigerapi) {
        value obj = value::object(true);
        obj[U("market")] = value::string("US");
        tigerapi->post(MARKET_STATE, obj);
    }

    static void test_get_positions(std::shared_ptr<TigerClient>  tigerapi) {
        value obj = value::object(true);
        obj[U("market")] = value::string("US");
        obj[P_ACCOUNT] = value::string("402901");
        tigerapi->post(POSITIONS, obj);
    }
};

int main(int argc, char *args[]) {
    LOGGER(info) << "Tiger API Demo";
    /************************** set config **********************/
    ClientConfig config = ClientConfig(true);

    config.private_key = "-----BEGIN RSA PRIVATE KEY-----\n"
                         "MIICXQIBAAKBgQC1amZa5YsGTklry7DAsUBOwXJCgrsZZtB21PImw/yLmrbqRfsS\n"
                         "3vawvMigLWcCwIDnHa+hpdpeze0eHIwbZzJzUDGvRALYK9t3D8pwPVxpwX1OF8Rf\n"
                         "HCM7YQvSOvPPnHHuVQvKaR7NNm1/WmvGXC9kVJdkYQ7kCmh52siFoy1MLQIDAQAB\n"
                         "AoGAVabcmIHTt7ByncBXvUJymDxhE+HhMEcImXJEueTCca8kOUu9FNXMJvmax3Vo\n"
                         "MzZsJbIwX+OMTEJxd0wHIlEA0gECjDwFK4Q42q+ptO4QABJQVSC6I+dOt2OIY28u\n"
                         "vT3rkenOO8KRIDt4F52PFd71ZdB1aaXixORORq1MdSLi8EkCQQDiviAB+L5R/HVx\n"
                         "wxvqZfJ530OtFd5IipZC9YZlY1CtXWCmu89LK7UUlEuNXyGsOxyz5jLqFuNRsie5\n"
                         "AC23tfEPAkEAzNMCa8axJWfPZIH4tGrbZ1F3I41BQdgp2zBmR7AyUMBDkli86Ozm\n"
                         "J7QUCJA/PJxK43/IYUWm4OU5Q+SvXCr3AwJBAJTBj1Y7zwES1CpSitn5EF+MbmX7\n"
                         "1t1YrsQ3OHkD80YJ4QMCbDkw75gUwox5QSoxjd8ow3Z4laJfc1gYGeZQ41kCQQCC\n"
                         "iQwm8cceBq3W6To+iUdw7itWngRz2Ta7uXnFwFYgvpeR4jnq3GfF7+9AkeWrVBQq\n"
                         "Ltrem0xCUfQP/+N+gudPAkBFLbt78/MpQGEDc7jyu/KE5Mp4wMMDQQwch9VLvsAZ\n"
                         "wWLysB6rZWpo3jIfp9zZ7c3zOYGNMWAZjtMmNkRJ8COH\n"
                         "-----END RSA PRIVATE KEY-----";
    config.tiger_id = "2";
    config.account = "402901";


    /**
     * 使用封装后的行情接口 QuoteClient
     */
    std::shared_ptr<QuoteClient> quote_client = std::make_shared<QuoteClient>(config);
//    quote_client->grab_quote_permission();
    TestQuoteClient::test_quote(quote_client);

    /**
     * 使用封装后的交易接口 TradeClient
     */
//    std::shared_ptr<TradeClient> trade_client = std::make_shared<TradeClient>(config);
//    TestTradeClient::test_trade(trade_client);

    /**
     * 直接使用未封装的 TigerApi
     */
//    std::shared_ptr<TigerClient> tigerapi = std::make_shared<TigerClient>(config);
//    TestTigerApi::test_grab_quote_permission(tigerapi);



    return 0;
}

