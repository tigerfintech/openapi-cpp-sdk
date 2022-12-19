#include <iostream>
#include <cpprest/http_client.h>
#include "tigerapi.h"
#include <algorithm>
#include "utils.h"
#include "common/service_types.h"
#include <cpprest/json.h>
//#include <windows.h>

using namespace std;
using namespace web;
using namespace web::json;


void test_grab_quote_permission(TIGERAPI tigerapi) {
    value obj = value::object(true);
    obj[U("tiger_id")] = value(1);
    obj[U("method")] = value::string(GRAB_QUOTE_PERMISSION);

    tigerapi.send_request("POST", obj);
}


int main(int argc, char *args[]) {
    cout << "Tiger Api demo" << endl;
    TIGERAPI tigerapi;
    /************************** set config **********************/
    struct Config config;
    config.private_key = "";
    config.tiger_id = "1";
    config.server_url = "https://openapi-sandbox.tigerfintech.com/gateway";
    config.account = "";

    tigerapi.set_config(config);
    test_grab_quote_permission(tigerapi);
    ///************************** test examples **********************/
    value obj  = value::object(true);
    obj[U("direction")] = value::string(U("long"));

    value obj2;
    obj2[U("afds")] = value::string(U("sa"));

    value abc = value::array();
    abc[0] = obj;
    abc[1] = obj2;
    cout << abc.serialize() << std::endl;
    return 0;
}

