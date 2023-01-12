
#ifndef TIGERAPI_TIGER_CLIENT_H
#define TIGERAPI_TIGER_CLIENT_H
#include <iostream>
#include <string>
#include "cpprest/http_client.h"
#include "cpprest/filestream.h"
#include "utils.h"
#include "constants.h"
#include "client_config.h"

using namespace std;
using namespace web;
using namespace web::http;
using namespace web::http::client;
using namespace web::json;

namespace TIGER_API {
    class TigerClient {
    public:
        TigerClient() {};
        TigerClient(const ClientConfig &cf) {
            client_config = cf;
        }
        ~TigerClient() {};

        ClientConfig client_config;

        void set_config(const ClientConfig &cf);

        value post(const string &api_method, value &params);

        value get(const string &api_method, value &params);

        value send_request(const string &http_method, const string &api_method, value &body);

        value identifiers_to_options(value identifiers);

    private:

        string build_sign_content(const value &obj);

        value build_common_params();
    };
}


#endif //TIGERAPI_TIGER_CLIENT_H
