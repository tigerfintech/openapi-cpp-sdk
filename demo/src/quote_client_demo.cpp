#include "tigerapi/tiger_client.h"
#include "tigerapi/quote_client.h"
#include "tigerapi/trade_client.h"
#include "tigerapi/service_types.h"
#include "tigerapi/contract_util.h"
#include "tigerapi/order_util.h"
#include "tigerapi/client_config.h"
#include "cpprest/http_client.h"
#include "cpprest/filestream.h"

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
        ucout << "asset: " << res << endl;
    }

    static void test_get_prime_portfolio(const std::shared_ptr<TradeClient>& trade_client) {
        PortfolioAccount res = trade_client->get_prime_portfolio();
        ucout << "portfolio: " << res.to_string() << endl;
    }

    static void test_get_asset(const std::shared_ptr<TradeClient>& trade_client) {
        value res = trade_client->get_asset();
        ucout << "asset: " << res << endl;
    }

    static void test_get_position(const std::shared_ptr<TradeClient>& trade_client) {
        value res = trade_client->get_positions();
        ucout << "position: " << res << endl;
    }

    static void test_get_position_list(const std::shared_ptr<TradeClient>& trade_client) {
        vector<Position> res = trade_client->get_position_list();
        ucout << "position size: " << res.size()  << " , first item: " << res[0].to_string() << endl;
    }

    static void test_get_orders(const std::shared_ptr<TradeClient>& trade_client) {
        value res = trade_client->get_orders();
        ucout << "orders: " << res << endl;
    }

    static void test_get_active_orders(const std::shared_ptr<TradeClient>& trade_client) {
        value res = trade_client->get_active_orders();
        ucout << "active orders: " << res << endl;
    }

    static void test_get_contract(const std::shared_ptr<TradeClient>& trade_client) {
        value res = trade_client->get_contract("AAPL");
        ucout << "contract: " << res << endl;
    }

    static void test_place_order(const std::shared_ptr<TradeClient>& trade_client) {
        Contract contract = stock_contract("AAPL", "USD");
        Order order = limit_order(contract, "BUY", 1, 100.0);
        value res = trade_client->place_order(order);
        long id = res["id"].as_integer();
        ucout << "order id: " << id << endl;
        ucout << "place order result: " << res << endl;
    }

    static void test_get_order(const std::shared_ptr<TradeClient>& trade_client) {
//        Contract contract = stock_contract("AAPL", "USD");
//        Order order = limit_order(contract, "BUY", 1, 100.0);
//        trade_client->place_order(order);
        Order my_order = trade_client->get_order(29270263515317248);
        ucout << "order : " << my_order.to_string() << endl;
    }

    static void test_cancel_order(const std::shared_ptr<TradeClient>& trade_client) {
        value res = trade_client->cancel_order(29270263515317248);
        ucout << "cancel order : " << res << endl;
    }

    static void test_modify_order(const std::shared_ptr<TradeClient>& trade_client) {
        Contract contract = stock_contract("AAPL", "USD");
        Order order = limit_order(contract, "BUY", 1, 100.0);
        long id = (long) trade_client->place_order(order)["id"].as_number().to_uint64();
        value res = trade_client->modify_order(order, 105);
        ucout << "modify order res: " << res << endl;
        Order mod_order = trade_client->get_order(id);
        ucout << "modified order: " << mod_order.to_string() << endl;
    }


    static void test_trade(const std::shared_ptr<TradeClient>& trade_client) {
        TestTradeClient::test_get_orders(trade_client);
    }
};


/**
 * 调用行情接口
 */

class TestQuoteClient {
public:
    static void test_grab_quote_permission(std::shared_ptr<QuoteClient> quote_client) {
        value perms = quote_client->grab_quote_permission();
        ucout << "Quote perms: " << perms << endl;
    }

    static void test_get_symbols(std::shared_ptr<QuoteClient> quote_client) {
        value result = quote_client->get_symbols();
        ucout << "result: " << result << endl;
    }

    static void test_get_symbols_names(std::shared_ptr<QuoteClient> quote_client) {
//        value result = quote_client->get_all_symbol_names("HK");
        value result = quote_client->get_all_symbol_names(Market::HK);
        ucout << "result: " << result << endl;
    }

    static void test_get_kline(std::shared_ptr<QuoteClient> quote_client) {
        value symbols = value::array();
        symbols[0] = value::string("AAPL");
        symbols[1] = value::string("JD");
        ucout << "symbols " << symbols << endl;
        value result = quote_client->get_kline(symbols);
        ucout << "result: " << result << endl;
    }

    static void test_get_quote_stock_trade(std::shared_ptr<QuoteClient> quote_client) {
        value symbols = value::array();
        symbols[0] = value::string("AAPL");
        symbols[1] = value::string("JD");
        value result = quote_client->get_quote_stock_trade(symbols);
        ucout << "Result: " << result << endl;
    }

    static void test_quote(std::shared_ptr<QuoteClient> quote_client) {
        TestQuoteClient::test_get_symbols_names(quote_client);
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
        ucout << "quote perms: " << perms << endl;
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
    ucout << "Tiger Api main" << endl;
    /************************** set config **********************/
    ClientConfig config = ClientConfig(true);

    config.private_key = "-----BEGIN RSA PRIVATE KEY-----\n"
                         ""
                         "-----END RSA PRIVATE KEY-----";
    config.tiger_id = "";
    config.account = "";


    /**
     * 使用封装后的行情接口 QuoteClient
     */
    std::shared_ptr<QuoteClient> quote_client = std::make_shared<QuoteClient>(config);
    quote_client->grab_quote_permission();
    TestQuoteClient::test_quote(quote_client);

    /**
     * 使用封装后的交易接口 TradeClient
     */
    std::shared_ptr<TradeClient> trade_client = std::make_shared<TradeClient>(config);
    TestTradeClient::test_trade(trade_client);

    /**
     * 直接使用未封装的 TigerApi
     */
    std::shared_ptr<TigerClient> tigerapi = std::make_shared<TigerClient>(config);
    TestTigerApi::test_grab_quote_permission(tigerapi);



    return 0;
}