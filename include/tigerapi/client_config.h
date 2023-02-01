//
// Created by sukai on 2022/12/29.
//

#ifndef TIGERAPI_CLIENT_CONFIG_H
#define TIGERAPI_CLIENT_CONFIG_H

#include <string>
#include <utility>
#include "constants.h"
#include "log.h"

using namespace std;

namespace TIGER_API {
    class ClientConfig {
    public:
        ClientConfig(bool sandbox_debug = false) : sandbox_debug(sandbox_debug) {
            if (sandbox_debug) {
                server_url = SANDBOX_TIGER_SERVER_URL;
                server_public_key = SANDBOX_TIGER_PUBLIC_KEY;
            }
            init_log();
        };

        ClientConfig(utility::string_t tiger_id, utility::string_t private_key, utility::string_t account) : tiger_id(std::move(tiger_id)),
                                                                                                 private_key(std::move(
                                                                                                         private_key)),
                                                                                                 account(std::move(account)) {};

        ClientConfig(utility::string_t tiger_id, utility::string_t private_key, utility::string_t account,
                     bool sandbox_debug = false) :
                tiger_id(std::move(tiger_id)), private_key(std::move(private_key)),
                account(std::move(account)), sandbox_debug(sandbox_debug) {
            if (sandbox_debug) {
                server_url = SANDBOX_TIGER_SERVER_URL;
                server_public_key = SANDBOX_TIGER_PUBLIC_KEY;
            }
            init_log();
        };

        utility::string_t tiger_id;
        utility::string_t private_key;
        utility::string_t account;
        utility::string_t charset = U("UTF-8");
        utility::string_t sign_type = U("RSA");

        void set_server_url(const utility::string_t &url) {
            this->server_url = url;
        }

        utility::string_t get_server_url() {
            return this->server_url;
        }

    private:
        bool sandbox_debug = false;
        utility::string_t server_url = TIGER_SERVER_URL;
        utility::string_t server_public_key = TIGER_PUBLIC_KEY;

        void init_log()
        {
//            logging::add_file_log(
//                    keywords::file_name = U("tigerapi_%N.log"),
//                    keywords::rotation_size = 10 * 1024 * 1024,
//                    keywords::time_based_rotation = sinks::file::rotation_at_time_point(0, 0, 0),
//                    keywords::format = U("[%TimeStamp%]: %Message%")
//                    );
            //logging::core::get() returns a pointer to the core singleton
            //logging::core::get()->set_filter(logging::trivial::severity >= logging::trivial::debug);
        }
    };
}

#endif //TIGERAPI_CLIENT_CONFIG_H
