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
#include <fstream>
#include "properties.h"

using namespace std;

namespace TIGER_API {
    class OPENAPI_EXPORT ClientConfig {
    public:
        ClientConfig(bool sandbox_debug = false) : sandbox_debug(sandbox_debug) {
            if (sandbox_debug) {
                LOG(WARNING) << U("SANDBOX IS NOT SUPPORTED") << endl;
                // server_url = SANDBOX_TIGER_SERVER_URL;
                // server_public_key = SANDBOX_TIGER_PUBLIC_KEY;
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

        ClientConfig(utility::string_t props_path) : props_path(props_path) {
            load_props();
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
        utility::string_t license;
        utility::string_t token;
        utility::string_t props_path;

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

        void load_props() {
            utility::string_t full_path = get_props_path(DEFAULT_PROPS_FILE);
            if (full_path.empty()) {
                return;
            }

            try {
                std::ifstream file(Utils::str16to8(full_path));
                if (!file.is_open()) {
                    LOG(ERROR) << U("Failed to open properties file: ") << full_path << endl;
                    return;
                }

                Properties props;
                props.load(file);

                // 只在值为空时从配置文件加载
                if (tiger_id.empty()) {
                    tiger_id = props.get_property(U("tiger_id"));
                }
                if (private_key.empty()) {
                    private_key = props.get_property(U("private_key_pk1"));
                }
                if (account.empty()) {
                    account = props.get_property(U("account"));
                }
                if (license.empty()) {
                    license = props.get_property(U("license"));
                }

                utility::string_t env = props.get_property(U("env"));
                std::transform(env.begin(), env.end(), env.begin(), ::toupper);
                if (env == U("SANDBOX")) {
                    sandbox_debug = true;
                    server_url = SANDBOX_TIGER_SERVER_URL;
                    server_public_key = SANDBOX_TIGER_PUBLIC_KEY;
                    socket_url = SANDBOX_TIGER_SOCKET_HOST;
                    socket_port = SANDBOX_TIGER_SOCKET_PORT;
                }
                
            } catch (const std::exception& e) {
                LOG(ERROR) << U("Failed to load properties file: ") << Utils::str8to16(e.what()) << endl;
            }
        }

        utility::string_t get_props_path(const utility::string_t& filename) const {
            if (!props_path.empty()) {
                if (Utils::is_directory(props_path)) {
                    return Utils::path_join(props_path, filename);
                } else {
                    utility::string_t dirname = Utils::path_dirname(props_path);
                    return Utils::path_join(dirname, filename);
                }
            }
            return utility::string_t();
        }

        utility::string_t get_token_path() const {
            return get_props_path(DEFAULT_TOKEN_FILE);
        }

        void load_token() {
            utility::string_t full_path = get_token_path();
            if (!full_path.empty()) {
                try {
                    std::ifstream file(Utils::str16to8(full_path));
                    if (!file.is_open()) {
                        LOG(ERROR) << U("Failed to open token file: ") << full_path << endl;
                        return;
                    }

                    Properties props;
                    props.load(file);
                    
                    // 获取token值
                    token = props.get_property(U("token"));
                    
                } catch (const std::exception& e) {
                    LOG(ERROR) << U("Failed to load token file: ") << Utils::str8to16(e.what()) << endl;
                }
            }
        }

        void save_token(const utility::string_t& new_token) {
            utility::string_t full_path = get_token_path();
            if (!full_path.empty()) {
                try {
                    Properties props;
                    props.set_property(U("token"), new_token);
                    
                    std::ofstream file(Utils::str16to8(full_path));
                    if (!file.is_open()) {
                        LOG(ERROR) << U("Failed to open token file for writing: ") << full_path << endl;
                        return;
                    }
                    
                    props.store(file);
                    token = new_token;
                    
                } catch (const std::exception& e) {
                    LOG(ERROR) << U("Failed to save token file: ") << Utils::str8to16(e.what()) << endl;
                }
            }
        }
    };
}

#endif //TIGERAPI_CLIENT_CONFIG_H
