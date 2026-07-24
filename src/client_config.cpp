#include "tigerapi/client_config.h"
#include "tigerapi/properties.h"
#include "tigerapi/enums.h"
#include <filesystem>


TIGER_API::ClientConfig::ClientConfig(bool sandbox_debug /*= false*/)
	: sandbox_debug(sandbox_debug) {
	if (sandbox_debug) {
		LOG(WARNING) << U("SANDBOX IS NOT SUPPORTED") << endl;
		// server_url = SANDBOX_TIGER_SERVER_URL;
		// server_public_key = SANDBOX_TIGER_PUBLIC_KEY;
	}
    if (is_us()) {
        server_url = US_TIGER_SERVER_URL;
        socket_url = US_TIGER_SOCKET_HOST;
        socket_port = US_TIGER_SOCKET_PORT;
    }
};

TIGER_API::ClientConfig::ClientConfig(utility::string_t tiger_id, utility::string_t private_key, utility::string_t account)
	: tiger_id(std::move(tiger_id)),
	private_key(std::move(private_key)),
	account(std::move(account)) {
    if (is_us()) {
        server_url = US_TIGER_SERVER_URL;
        socket_url = US_TIGER_SOCKET_HOST;
        socket_port = US_TIGER_SOCKET_PORT;
    }
};

TIGER_API::ClientConfig::ClientConfig(utility::string_t tiger_id, utility::string_t private_key, utility::string_t account, bool sandbox_debug /*= false*/, utility::string_t lang /*= U("en_US")*/)
	: tiger_id(std::move(tiger_id)),
	private_key(std::move(private_key)),
	account(std::move(account)),
	sandbox_debug(sandbox_debug),
	lang(lang) {
	if (sandbox_debug) {
		LOG(WARNING) << U("SANDBOX IS NOT SUPPORTED") << endl;
		// server_url = SANDBOX_TIGER_SERVER_URL;
		// server_public_key = SANDBOX_TIGER_PUBLIC_KEY;
		// socket_url = SANDBOX_TIGER_SOCKET_HOST;
		// socket_port = SANDBOX_TIGER_SOCKET_PORT;
	}
    if (is_us()) {
        server_url = US_TIGER_SERVER_URL;
        socket_url = US_TIGER_SOCKET_HOST;
        socket_port = US_TIGER_SOCKET_PORT;
    }
};

TIGER_API::ClientConfig::ClientConfig(bool sandbox_debug, const utility::string_t props_path)
	: sandbox_debug(sandbox_debug),
	props_path(props_path) {
    load_props();
	load_token();
    if (is_us()) {
        server_url = US_TIGER_SERVER_URL;
        socket_url = US_TIGER_SOCKET_HOST;
        socket_port = US_TIGER_SOCKET_PORT;
    }
    LOG(INFO) << U("ClientConfig initialized with props_path: ") << props_path << endl;
};

void TIGER_API::ClientConfig::check() const
{
	if (this->tiger_id.empty()) {
		LOG(ERROR) << U("Client Config error: tiger_id can't be empty") << endl;
		throw std::runtime_error("Client Config error: tiger_id can't be empty");
	}
	if (this->private_key.empty()) {
		LOG(ERROR) << U("Client Config error: private_key can't be empty") << endl;
		throw std::runtime_error("Client Config error: private_key can't be empty");
	}
}

void TIGER_API::ClientConfig::check_account() const
{
	if (this->account.empty()) {
		LOG(ERROR) << U("Client Config error: account can't be empty") << endl;
		throw std::runtime_error("Client Config error: account can't be empty");
	}
}

void TIGER_API::ClientConfig::set_server_url(const utility::string_t& url)
{
	this->server_url = url;
}

void TIGER_API::ClientConfig::set_socket_url(const utility::string_t& url)
{
	this->socket_url = url;
}

void TIGER_API::ClientConfig::set_socket_port(const utility::string_t& port)
{
	this->socket_port = port;
}

void TIGER_API::ClientConfig::set_server_public_key(const utility::string_t& key)
{
	this->server_public_key = key;
}

void TIGER_API::ClientConfig::set_token(const utility::string_t& token)
{
    std::lock_guard<std::mutex> lock(token_mutex_);
    this->token = token;
}

const utility::string_t& TIGER_API::ClientConfig::get_server_url() const
{
	return this->server_url;
}

const utility::string_t& TIGER_API::ClientConfig::get_server_pub_key() const
{
	return this->server_public_key;
}

const utility::string_t& TIGER_API::ClientConfig::get_socket_url() const
{
	return this->socket_url;
}

const utility::string_t& TIGER_API::ClientConfig::get_socket_port() const
{
	return this->socket_port;
}

void TIGER_API::ClientConfig::load_props()
{
	utility::string_t full_path = get_props_path(DEFAULT_PROPS_FILE);
	if (full_path.empty()) {
		return;
	}
	LOG(INFO) << U("Loading properties file from: ") << full_path << endl;

	try {
		std::filesystem::path fspath(full_path);
		std::ifstream file(fspath);
		if (!file.is_open()) {
			LOG(ERROR) << U("Failed to open properties file: ") << full_path << endl;
			return;
		}

		Properties props;
		props.load(file);

		if (tiger_id.empty()) {
			tiger_id = props.get_property(U("tiger_id"));
		}
		if (private_key.empty()) {
			// prefer pk8 (PKCS#8), fallback to pk1 (PKCS#1)
			utility::string_t pk8 = props.get_property(U("private_key_pk8"));
			if (!pk8.empty()) {
				private_key = pk8;
			} else {
				private_key = props.get_property(U("private_key_pk1"));
			}
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

		// Explicit server_url/server_public_key in the properties file take
		// precedence over any default set above, so callers can point the
		// client at a different gateway endpoint and signature-verification
		// key without changing code.
		utility::string_t custom_server_url = props.get_property(U("server_url"));
		if (!custom_server_url.empty()) {
			server_url = custom_server_url;
			LOG(INFO) << U("Overriding server_url from properties file: ") << server_url << endl;
		}
		utility::string_t custom_server_pub_key = props.get_property(U("server_public_key"));
		if (!custom_server_pub_key.empty()) {
			server_public_key = custom_server_pub_key;
			LOG(INFO) << U("Overriding server_public_key from properties file") << endl;
		}

	}
	catch (const std::exception& e) {
		LOG(ERROR) << U("Failed to load properties file: ") << Utils::str8to16(e.what()) << endl;
	}
	LOG(INFO) << U("Loaded properties file successfully, tiger_id: ") << tiger_id << " account: " << account
		<< endl;
}

utility::string_t TIGER_API::ClientConfig::get_props_path(const utility::string_t& filename) const
{
	if (props_path.empty()) {
		return U("");
	}
	// If props_path already points to a file (ends with .properties), use it directly
	// for the config file; for token file, derive from its directory.
	const utility::string_t suffix = U(".properties");
	const bool is_props_file = props_path.size() >= suffix.size() &&
	    props_path.substr(props_path.size() - suffix.size()) == suffix;
	if (is_props_file) {
		if (filename == DEFAULT_PROPS_FILE) {
			return props_path;
		}
		// Derive directory from the file path for token file
		auto sep = props_path.find_last_of(U("/\\"));
		if (sep != utility::string_t::npos) {
			return props_path.substr(0, sep + 1) + filename;
		}
		return filename;
	}
	// Otherwise treat as directory
	if (Utils::is_directory(props_path)) {
		return props_path + filename;
	}
	return U("");
}

utility::string_t TIGER_API::ClientConfig::get_token_path() const
{
	return get_props_path(DEFAULT_TOKEN_FILE);
}

void TIGER_API::ClientConfig::load_token()
{
	utility::string_t full_path = get_token_path();
	if (full_path.empty()) {
		return;
	}
	try {
		std::filesystem::path fspath(full_path);
		std::ifstream file(fspath);
		if (!file.is_open()) {
			LOG(ERROR) << U("Failed to open token file: ") << full_path << endl;
			return;
		}

		Properties props;
		props.load(file);

		// get token value
		utility::string_t loaded_token = props.get_property(U("token"));
		{
			std::lock_guard<std::mutex> lock(token_mutex_);
			token = loaded_token;
		}
		LOG(INFO) << U("Loaded token successfully, token: ") << token << endl;
	}
	catch (const std::exception& e) {
		LOG(ERROR) << U("Failed to load token file: ") << Utils::str8to16(e.what()) << endl;
	}
}

void TIGER_API::ClientConfig::save_token(const utility::string_t& new_token)
{
    utility::string_t full_path = get_token_path();

    if (full_path.empty()) {
        // No file path: just update memory.
        std::lock_guard<std::mutex> lock(token_mutex_);
        token = new_token;
        LOG(INFO) << U("No token file path configured; token updated in memory only") << endl;
        return;
    }

    // Write file first; only update memory on success.
    try {
        Properties props;
        props.set_property(U("token"), new_token);

        std::filesystem::path fspath(full_path);
        std::ofstream file(fspath);
        if (!file.is_open()) {
            LOG(ERROR) << U("Failed to open token file for writing: ") << full_path << endl;
            return;
        }

        props.store(file);
        LOG(INFO) << U("Saved token successfully, length=") << new_token.size() << endl;
    }
    catch (const std::exception& e) {
        LOG(ERROR) << U("Failed to save token file: ") << Utils::str8to16(e.what()) << endl;
        return;
    }

    // File write succeeded; now update in-memory token under lock.
    {
        std::lock_guard<std::mutex> lock(token_mutex_);
        token = new_token;
    }
}

bool TIGER_API::ClientConfig::is_us()
{
    return !license.empty() && license == enum_to_str(License::TBUS);
}

utility::string_t TIGER_API::ClientConfig::get_token() const
{
    std::lock_guard<std::mutex> lock(token_mutex_);
    return token;
}

bool TIGER_API::ClientConfig::start_token_refresh(int interval_seconds,
                                                    std::function<utility::string_t()> fetch_fn)
{
    // Guard against re-entry: take cv_mutex to serialize start/stop calls,
    // so stop_token_refresh() cannot race between flag-set and thread-create.
    std::lock_guard<std::mutex> start_lock(refresh_cv_mutex_);

    if (refresh_running_.load()) {
        LOG(WARNING) << U("Token refresh thread is already running") << endl;
        return false;
    }

    // Join any previously stopped thread before replacing it.
    if (refresh_thread_.joinable()) {
        refresh_thread_.join();
    }

    refresh_running_.store(true);
    refresh_thread_ = std::thread([this, interval_seconds, fetch_fn = std::move(fetch_fn)]() {
        LOG(INFO) << U("Token refresh thread started, interval=") << interval_seconds << U("s") << endl;
        while (refresh_running_) {
            std::unique_lock<std::mutex> cv_lock(refresh_cv_mutex_);
            refresh_cv_.wait_for(cv_lock, std::chrono::seconds(interval_seconds),
                                 [this] { return !refresh_running_.load(); });
            if (!refresh_running_) break;

            try {
                utility::string_t new_token = fetch_fn();
                if (!new_token.empty()) {
                    save_token(new_token);
                    LOG(INFO) << U("Token refreshed successfully") << endl;
                } else {
                    LOG(WARNING) << U("Token refresh returned empty token, keeping current") << endl;
                }
            } catch (const std::exception& e) {
                LOG(ERROR) << U("Token refresh failed: ") << Utils::str8to16(e.what()) << endl;
            } catch (...) {
                LOG(ERROR) << U("Token refresh failed with unknown exception") << endl;
            }
        }
        LOG(INFO) << U("Token refresh thread stopped") << endl;
    });

    return true;
}

void TIGER_API::ClientConfig::stop_token_refresh()
{
    {
        std::lock_guard<std::mutex> start_lock(refresh_cv_mutex_);
        if (!refresh_running_.load()) {
            return;
        }
        refresh_running_.store(false);
    }
    refresh_cv_.notify_all();
    if (refresh_thread_.joinable()) {
        refresh_thread_.join();
    }
}

TIGER_API::ClientConfig::~ClientConfig()
{
    stop_token_refresh();
}
