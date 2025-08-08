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
                socket_url = SANDBOX_TIGER_SOCKET_HOST;
                socket_port = SANDBOX_TIGER_SOCKET_PORT;
            }
        };

        utility::string_t tiger_id;
        utility::string_t private_key;
        utility::string_t account;
        utility::string_t charset = U("UTF-8");
        utility::string_t sign_type = U("RSA");
        utility::string_t lang;
        utility::string_t device_id = Utils::get_device_id();
        utility::string_t secret_key;
        bool use_full_tick = false;
		utility::string_t socket_ca_certs;
        unsigned int send_interval = 10 * 1000;
        unsigned int receive_interval = 10 * 1000;

        void check() {
            if (this->tiger_id.empty()) {
                LOG(ERROR) << U("Client Config error: tiger_id can't be empty") << endl;
                throw std::runtime_error("Client Config error: tiger_id can't be empty");
            }
            if (this->private_key.empty()) {
                LOG(ERROR) << U("Client Config error: private_key can't be empty") << endl;
                throw std::runtime_error("Client Config error: private_key can't be empty");
            }
        }

        void check_account() {
            if (this->account.empty()) {
                LOG(ERROR) << U("Client Config error: account can't be empty") << endl;
                throw std::runtime_error("Client Config error: account can't be empty");
            }
        }

        void set_server_url(const utility::string_t &url) {
            this->server_url = url;
        }

        void set_socket_url(const utility::string_t &url) {
            this->socket_url = url;
        }

        void set_socket_port(const utility::string_t &port) {
            this->socket_port = port;
        }

        const utility::string_t& get_server_url() {
            return this->server_url;
        }

        const utility::string_t& get_server_pub_key() {
            return this->server_public_key;
        }

        const utility::string_t& get_socket_url() {
            return this->socket_url;
        }

        const utility::string_t& get_socket_port() {
            return this->socket_port;
        }

    private:
        bool sandbox_debug = false;
        utility::string_t server_url = TIGER_SERVER_URL;
        utility::string_t server_public_key = TIGER_PUBLIC_KEY;
        utility::string_t socket_url = TIGER_SOCKET_HOST;
        utility::string_t socket_port = TIGER_SOCKET_PORT;
    };
}

#endif //TIGERAPI_CLIENT_CONFIG_H
