//
// Created by sukai on 2022/12/29.
//

#ifndef TIGERAPI_CLIENT_CONFIG_H
#define TIGERAPI_CLIENT_CONFIG_H

#include <string>
#include <utility>
#include "constants.h"
#include "logger.h"
#include "utils.h"
#include "win32.h"

using namespace std;

namespace TIGER_API {
    class OPENAPI_EXPORT ClientConfig {
    public:
        ClientConfig(bool sandbox_debug = false);
        ClientConfig(utility::string_t tiger_id, utility::string_t private_key, utility::string_t account);
        ClientConfig(utility::string_t tiger_id, utility::string_t private_key, utility::string_t account,
            bool sandbox_debug = false, utility::string_t lang = U("en_US"));
        ClientConfig(bool sandbox_debug, const utility::string_t props_path);

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
        utility::string_t license;
        utility::string_t token;
        utility::string_t props_path;

        void check();
        void check_account();

        void set_server_url(const utility::string_t& url);

        void set_socket_url(const utility::string_t& url);

        void set_socket_port(const utility::string_t& port);

        void set_server_public_key(const utility::string_t& key);

        void set_token(const utility::string_t& token);

        const utility::string_t& get_server_url();

        const utility::string_t& get_server_pub_key();

        const utility::string_t& get_socket_url();

        const utility::string_t& get_socket_port();

    private:
        bool sandbox_debug = false;
        utility::string_t server_url = TIGER_SERVER_URL;
        utility::string_t server_public_key = TIGER_PUBLIC_KEY;
        utility::string_t socket_url = TIGER_SOCKET_HOST;
        utility::string_t socket_port = TIGER_SOCKET_PORT;

        void load_props();

        utility::string_t get_props_path(const utility::string_t& filename) const;

        utility::string_t get_token_path() const;

        void load_token();

        void save_token(const utility::string_t& new_token);
    };
}
#endif //TIGERAPI_CLIENT_CONFIG_H
