#include <iostream>
#include <cpprest/http_client.h>
#include "tigerapi.h"
#include "common/service_types.h"


using namespace std;
using namespace web;
using namespace web::json;


void test_grab_quote_permission(TIGERAPI tigerapi) {
    value obj = value::object(true);
    tigerapi.post(GRAB_QUOTE_PERMISSION, obj);
}

void test_get_market_status(TIGERAPI tigerapi) {
    value obj = value::object(true);
    obj[U("market")] = value::string("US");
    tigerapi.post(MARKET_STATE, obj);
}

void test_get_positions(TIGERAPI tigerapi) {
    value obj = value::object(true);
    obj[U("market")] = value::string("US");
    obj[P_ACCOUNT] = value::string("");
    tigerapi.post(POSITIONS, obj);
}

int main(int argc, char *args[]) {
    cout << "Tiger Api demo" << endl;
    TIGERAPI tigerapi;
    /************************** set config **********************/
    struct Config config;
    config.private_key = "-----BEGIN RSA PRIVATE KEY-----\n"
                         ""
                         "-----END RSA PRIVATE KEY-----";
    config.tiger_id = "2";
    config.server_url = "https://openapi-sandbox.tigerfintech.com/gateway";
    config.account = "";

    tigerapi.set_config(config);
//    test_grab_quote_permission(tigerapi);
//    test_get_market_status(tigerapi);
    test_get_positions(tigerapi);
    return 0;
}

