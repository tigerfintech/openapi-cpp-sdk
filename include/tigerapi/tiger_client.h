
#ifndef TIGERAPI_TIGER_CLIENT_H
#define TIGERAPI_TIGER_CLIENT_H
#include <iostream>
#include <string>
#include "cpprest/http_client.h"
#include "cpprest/filestream.h"
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

namespace TIGER_API {
    class TigerClient {
    public:
        TigerClient() {};

        TigerClient(struct Config &cf) { set_config(cf); };

        ~TigerClient() {};

        struct Config client_config;

        void set_config(struct Config &cf);

        value post(const string &api_method, value &params);

        value get(const string &api_method, value &params);

        value send_request(const string &http_method, const string &api_method, value &body);

    private:

        string build_sign_content(const value &obj);

        value build_common_params();
    };
}


#endif //TIGERAPI_TIGER_CLIENT_H
