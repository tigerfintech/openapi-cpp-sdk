//
// Created by sukai on 2022/12/29.
//

#ifndef TIGERAPI_CLIENT_CONFIG_H
#define TIGERAPI_CLIENT_CONFIG_H

#include <string>
#include <utility>
#include <functional>
#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>
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

        // Copy constructor: copies configuration data only; refresh thread is NOT transferred.
        ClientConfig(const ClientConfig& other)
            : tiger_id(other.tiger_id), private_key(other.private_key),
              account(other.account), charset(other.charset), sign_type(other.sign_type),
              lang(other.lang), device_id(other.device_id), secret_key(other.secret_key),
              use_full_tick(other.use_full_tick), socket_ca_certs(other.socket_ca_certs),
              send_interval(other.send_interval), receive_interval(other.receive_interval),
              license(other.license), props_path(other.props_path),
              sandbox_debug(other.sandbox_debug), server_url(other.server_url),
              server_public_key(other.server_public_key), socket_url(other.socket_url),
              socket_port(other.socket_port)
        {
            std::lock_guard<std::mutex> lock(other.token_mutex_);
            token = other.token;
        }

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

        void check() const;
        void check_account() const;

        void set_server_url(const utility::string_t& url);

        void set_socket_url(const utility::string_t& url);

        void set_socket_port(const utility::string_t& port);

        void set_server_public_key(const utility::string_t& key);

        void set_token(const utility::string_t& token);

        const utility::string_t& get_server_url() const;

        const utility::string_t& get_server_pub_key() const;

        const utility::string_t& get_socket_url() const;

        const utility::string_t& get_socket_port() const;

        bool is_us();

        // Token auto-refresh.
        // fetch_fn: called every interval_seconds to obtain a new token string.
        // Returns false if a refresh thread is already running.
        bool start_token_refresh(int interval_seconds,
                                 std::function<utility::string_t()> fetch_fn);
        void stop_token_refresh();

        // Thread-safe token accessor for use inside send_request.
        utility::string_t get_token() const;

        // Persist a newly-obtained token to memory and file.
        void save_token(const utility::string_t& new_token);

        ~ClientConfig();

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

        // Token refresh background thread members.
        mutable std::mutex token_mutex_;
        std::atomic<bool> refresh_running_{false};
        std::thread refresh_thread_;
        std::condition_variable refresh_cv_;
        std::mutex refresh_cv_mutex_;
    };
}
#endif //TIGERAPI_CLIENT_CONFIG_H
