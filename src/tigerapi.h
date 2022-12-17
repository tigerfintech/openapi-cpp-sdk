
#ifndef CPPSDK_TIGERAPI_H
#define CPPSDK_TIGERAPI_H
#include <iostream>
#include <string>
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include "utils.h"
#include "constants.h"

using namespace std;
using namespace web;
using namespace web::http;
using namespace web::http::client;
using namespace web::json;

struct Config{
    string tiger_id;
    string private_key;
    string account;
    string server_url;
};

class TIGERAPI {
public:
    TIGERAPI() {};
    TIGERAPI(struct Config &cf) { set_config(cf);};
    ~TIGERAPI() {};
    void set_config(struct Config &cf);
    string send_request(const string &method, const string &request_path, const string &params= "");
    string get_sign(string timestamp, string method, string request_path, string body);

    void test_request();

private:
    struct Config client_config;
};


#endif //CPPSDK_TIGERAPI_H
