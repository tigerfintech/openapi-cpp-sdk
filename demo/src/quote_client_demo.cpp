#include "tigerapi/tiger_client.h"
#include "cpprest/http_client.h"
#include "cpprest/filestream.h"

using namespace std;
using namespace web;
using namespace web::json;


void test_grab_quote_permission(TigerClient tigerapi) {
    value obj = value::object(true);
    tigerapi.post(GRAB_QUOTE_PERMISSION, obj);
}

void test_get_market_status(TigerClient tigerapi) {
    value obj = value::object(true);
    obj[U("market")] = value::string("US");
    tigerapi.post(MARKET_STATE, obj);
}

void test_get_positions(TigerClient tigerapi) {
    value obj = value::object(true);
    obj[U("market")] = value::string("US");
    obj[P_ACCOUNT] = value::string("402901");
    tigerapi.post(POSITIONS, obj);
}

int main(int argc, char *args[]) {
    cout << "Tiger Api demo" << endl;
    TigerClient tigerapi;
    /************************** set config **********************/
    struct Config config;
    config.private_key = "-----BEGIN RSA PRIVATE KEY-----\n"
                         "";
    config.tiger_id = "2";
    config.server_url = "https://openapi-sandbox.tigerfintech.com/gateway";
    config.account = "";

    tigerapi.set_config(config);
//    test_grab_quote_permission(tigerapi);
//    test_get_market_status(tigerapi);
    test_get_positions(tigerapi);
    return 0;
}

