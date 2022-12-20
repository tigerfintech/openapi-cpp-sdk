
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
    string charset = "UTF-8";
    string sign_type = "RSA";
};

class TIGERAPI {
public:
    TIGERAPI() {};
    TIGERAPI(struct Config &cf) { set_config(cf);};
    ~TIGERAPI() {};
    void set_config(struct Config &cf);
    string post(const string &api_method, value &params);
    string get(const string &api_method, value &params);
    string send_request(const string &http_method, const string &api_method, value &body);
private:
    struct Config client_config;
    string build_sign_content(const value& obj);
    value build_common_params();
};


#endif //CPPSDK_TIGERAPI_H
