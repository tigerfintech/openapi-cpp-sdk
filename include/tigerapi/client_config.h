//
// Created by sukai on 2022/12/29.
//

#ifndef TIGERAPI_CLIENT_CONFIG_H
#define TIGERAPI_CLIENT_CONFIG_H

#include <string>
#include <utility>
#include "constants.h"

using namespace std;

namespace TIGER_API {
    class ClientConfig {
    public:
        ClientConfig(bool sandbox_debug = false) : sandbox_debug(sandbox_debug) {
            if (sandbox_debug) {
                server_url = SANDBOX_TIGER_SERVER_URL;
                server_public_key = SANDBOX_TIGER_PUBLIC_KEY;
            }
        };

        ClientConfig(string tiger_id, string private_key, string account) : tiger_id(std::move(tiger_id)),
                                                                                                 private_key(std::move(
                                                                                                         private_key)),
                                                                                                 account(std::move(account)) {};

        ClientConfig(string tiger_id, string private_key, string account,
                     bool sandbox_debug = false) :
                tiger_id(std::move(tiger_id)), private_key(std::move(private_key)),
                account(std::move(account)), sandbox_debug(sandbox_debug) {
            if (sandbox_debug) {
                server_url = SANDBOX_TIGER_SERVER_URL;
                server_public_key = SANDBOX_TIGER_PUBLIC_KEY;
            }
        };

        string tiger_id;
        string private_key;
        string account;
        string charset = "UTF-8";
        string sign_type = "RSA";

        void set_server_url(const string &url) {
            this->server_url = url;
        }

        string get_server_url() {
            return this->server_url;
        }

    private:
        bool sandbox_debug = false;
        string server_url = TIGER_SERVER_URL;
        string server_public_key = TIGER_PUBLIC_KEY;
    };
}

#endif //TIGERAPI_CLIENT_CONFIG_H
