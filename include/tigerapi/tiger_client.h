
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
    class OPENAPI_EXPORT TigerClient {
    public:
        TigerClient() {};
        TigerClient(const ClientConfig &cf) {
            set_config(cf);
        }
        ~TigerClient() {};

        ClientConfig client_config;

        void set_config(const ClientConfig &cf);

        value post(const utility::string_t &api_method, value &params);

        value get(const utility::string_t &api_method, value &params);

        value send_request(const utility::string_t &http_method, const utility::string_t &api_method, value &body);

        value identifiers_to_options(value identifiers);

    private:

        utility::string_t build_sign_content(const value &obj);

        value build_common_params();
    };
}


#endif //TIGERAPI_TIGER_CLIENT_H
