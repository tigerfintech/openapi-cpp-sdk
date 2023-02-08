//
// Created by sukai on 2022/12/29.
//

#ifndef TIGERAPI_CLIENT_CONFIG_H
#define TIGERAPI_CLIENT_CONFIG_H

#include <string>
#include <utility>
#include "constants.h"
#include "easylogging++.h"
#include "utils.h"
#include "win32.h"

using namespace std;

namespace TIGER_API {
    class OPENAPI_EXPORT ClientConfig {
    public:
        ClientConfig(bool sandbox_debug = false) : sandbox_debug(sandbox_debug) {
            if (sandbox_debug) {
                server_url = SANDBOX_TIGER_SERVER_URL;
                server_public_key = SANDBOX_TIGER_PUBLIC_KEY;
            }
        };

        ClientConfig(utility::string_t tiger_id, utility::string_t private_key, utility::string_t account) : tiger_id(std::move(tiger_id)),
                                                                                                 private_key(std::move(
                                                                                                         private_key)),
                                                                                                 account(std::move(account)) {};

        ClientConfig(utility::string_t tiger_id, utility::string_t private_key, utility::string_t account,
                     bool sandbox_debug = false, utility::string_t lang = U("en_US")) :
                tiger_id(std::move(tiger_id)), private_key(std::move(private_key)),
                account(std::move(account)), sandbox_debug(sandbox_debug), lang(lang) {
            if (sandbox_debug) {
                server_url = SANDBOX_TIGER_SERVER_URL;
                server_public_key = SANDBOX_TIGER_PUBLIC_KEY;
            }
        };

        utility::string_t tiger_id;
        utility::string_t private_key;
        utility::string_t account;
        utility::string_t charset = U("UTF-8");
        utility::string_t sign_type = U("RSA");
        utility::string_t lang;
        utility::string_t device_id = Utils::get_device_id();

        void set_server_url(const utility::string_t &url) {
            this->server_url = url;
        }

        utility::string_t get_server_url() {
            return this->server_url;
        }

        utility::string_t get_server_pub_key() {
            return this->server_public_key;
        }

    private:
        bool sandbox_debug = false;
        utility::string_t server_url = TIGER_SERVER_URL;
        utility::string_t server_public_key = TIGER_PUBLIC_KEY;
    };
}

#endif //TIGERAPI_CLIENT_CONFIG_H
