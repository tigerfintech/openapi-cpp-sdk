#include <iostream>
#include <cpprest/http_client.h>
#include "tigerapi.h"
#include <algorithm>
#include "utils.h"
#include <cpprest/json.h>
//#include <windows.h>

using namespace std;
using namespace web;
using namespace web::json;


int main(int argc, char *args[]) {
    cout << "Tiger Api demo" << endl;
//    TIGERAPI tigerapi;
    /************************** set config **********************/
    struct Config config;
    config.private_key = "";
    config.tiger_id = "";
    config.server_url = "https://openapi.tigerfintech.com/gateway";
    config.account = "";

    //tigerapi.set_config(config);
    ///************************** test examples **********************/
    value obj  = value::object(true);
//    obj[L"direction"] = value::string(L"long");
    obj[U("direction")] = value::string(U("long"));

    value obj2;
    obj2[U("afds")] = value::string(U("sa"));

    value abc = value::array();
    abc[0] = obj;
    abc[1] = obj2;
    cout << abc.serialize() << std::endl;
    return 0;
}