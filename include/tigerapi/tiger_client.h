
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
        explicit TigerClient(const ClientConfig &cf);
        ~TigerClient() = default;

        // client_config is read-only after construction; TigerClient is thread-safe
        // for concurrent send_request() calls once constructed.
        const ClientConfig client_config;

        value post(const utility::string_t &api_method, value &params);

        value get(const utility::string_t &api_method, value &params);

        value send_request(const utility::string_t &http_method, const utility::string_t &api_method, value &body);

        value identifiers_to_options(value identifiers);

    private:
        std::unique_ptr<http_client> http_client_;

        utility::string_t build_sign_content(const value &obj);

        value build_common_params();
    };
}


#endif //TIGERAPI_TIGER_CLIENT_H
