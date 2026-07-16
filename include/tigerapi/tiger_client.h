
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

        // Prevent silent copy/move that would lose the refresh thread state.
        TigerClient(const TigerClient&) = delete;
        TigerClient& operator=(const TigerClient&) = delete;
        TigerClient(TigerClient&&) = delete;
        TigerClient& operator=(TigerClient&&) = delete;

        // client_config holds connection params and token state; mutable for token refresh.
        ClientConfig client_config;

        value post(const utility::string_t &api_method, value &params);

        value get(const utility::string_t &api_method, value &params);

        value send_request(const utility::string_t &http_method, const utility::string_t &api_method, value &body);

        value identifiers_to_options(value identifiers);

        // Fetch a new token from the server via user_token_refresh.
        // Returns empty string on failure.
        utility::string_t query_token();

        // Convenience: start background token auto-refresh.
        // interval_seconds: how often to refresh (e.g. 3600 = 1 hour).
        // Returns false if a refresh thread is already running.
        bool start_token_refresh(int interval_seconds);
        void stop_token_refresh();

    private:
        std::unique_ptr<http_client> http_client_;

        utility::string_t build_sign_content(const value &obj);

        value build_common_params();
    };
}


#endif //TIGERAPI_TIGER_CLIENT_H
