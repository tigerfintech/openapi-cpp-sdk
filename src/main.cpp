#include "../include/tigerapi/tiger_client.h"
#include "../include/tigerapi/quote_client.h"
#include "../include/tigerapi/service_types.h"


using namespace std;
using namespace web;
using namespace web::json;
using namespace TIGERAPI;

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

    static void test_get_kline(std::shared_ptr<QuoteClient> quote_client) {
        value symbols = value::array();
        symbols[0] = value::string("AAPL");
        symbols[1] = value::string("JD");
        value result = quote_client->get_kline(symbols);
        cout << "result: " << result << endl;
    }

    static void test_get_quote_stock_trade(std::shared_ptr<QuoteClient> quote_client) {
        value symbols = value::array();
        symbols[0] = value::string("AAPL");
        symbols[1] = value::string("JD");
        value result = quote_client->get_quote_stock_trade(symbols);
        cout << "Result: " << result << endl;
    }

    static void test_quote(std::shared_ptr<QuoteClient> quote_client) {
        TestQuoteClient::test_get_symbols(quote_client);
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
    cout << "Tiger Api main" << endl;
    /************************** set config **********************/
    struct Config config;
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
    config.server_url = "https://openapi-sandbox.tigerfintech.com/gateway";
    config.account = "";

    /**
     * 直接 使用 TigerApi
     */
//    std::shared_ptr<TigerClient> tigerapi = std::make_shared<TigerClient>();
//    tigerapi->set_config(config);
//    TestTigerApi::test_grab_quote_permission(tigerapi);


    /**
     * 使用封装后的行情接口 QuoteClient
     */
    std::shared_ptr<QuoteClient> quote_client = std::make_shared<QuoteClient>();
    quote_client->set_config(config);
//    quote_client->grab_quote_permission();
    TestQuoteClient::test_quote(quote_client);
    return 0;
}

