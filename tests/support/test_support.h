#pragma once
/**
 * test_support.h — TigerOpen C++ SDK test utilities
 *
 * Fake offline credentials for unit tests that must not make real network
 * calls, and helpers for integration tests that read live credentials from
 * environment variables.
 */
#include <cstdlib>
#include <stdexcept>
#include <string>
#include "tigerapi/client_config.h"

namespace TIGER_API {
namespace test {

// ── Fake offline credentials ─────────────────────────────────────────────────
// Intentionally invalid — allows ClientConfig construction and field-access
// tests without any real crypto or network I/O.
static const utility::string_t FAKE_TIGER_ID    = U("test_tiger_id_00001");
static const utility::string_t FAKE_ACCOUNT     = U("test_account_00001");
// A valid PEM-format placeholder (not a real key; signing will fail).
static const utility::string_t FAKE_PRIVATE_KEY =
    U("-----BEGIN RSA PRIVATE KEY-----\n")
    U("MIIEpAIBAAKCAQEA0000000000000000000000000000000000000000000000000\n")
    U("0000000000000000000000000000000000000000000000000000000000000000\n")
    U("-----END RSA PRIVATE KEY-----");

/**
 * Returns a sandbox ClientConfig for unit tests: valid construction but no
 * real credentials — do NOT call methods that sign or send requests.
 */
inline ClientConfig make_offline_config() {
    return ClientConfig(FAKE_TIGER_ID, FAKE_PRIVATE_KEY, FAKE_ACCOUNT,
                        /*sandbox_debug=*/true);
}

// ── Integration test helpers ─────────────────────────────────────────────────

/** Returns true when TIGER_RUN_INTEG=true is set in the environment. */
inline bool integ_enabled() {
    const char* v = std::getenv("TIGER_RUN_INTEG");
    return v != nullptr && std::string(v) == "true";
}

/** Live credentials sourced from environment variables. */
struct IntegCreds {
    utility::string_t tiger_id;
    utility::string_t private_key;
    utility::string_t account;
};

/**
 * Reads live credentials from:
 *   TIGER_TEST_ID, TIGER_TEST_PRIVATE_KEY, TIGER_TEST_ACCOUNT
 * Throws std::runtime_error if any variable is absent.
 */
inline IntegCreds read_integ_creds() {
    auto env = [](const char* name) -> utility::string_t {
        const char* v = std::getenv(name);
        if (!v || *v == '\0')
            throw std::runtime_error(std::string("Missing env var: ") + name);
#if defined(_WIN32)
        return utility::conversions::to_string_t(std::string(v));
#else
        return utility::string_t(v);
#endif
    };
    return {env("TIGER_TEST_ID"), env("TIGER_TEST_PRIVATE_KEY"), env("TIGER_TEST_ACCOUNT")};
}

/** Returns a sandbox ClientConfig populated from integration env vars. */
inline ClientConfig make_integ_config() {
    auto c = read_integ_creds();
    return ClientConfig(c.tiger_id, c.private_key, c.account, /*sandbox_debug=*/true);
}

} // namespace test
} // namespace TIGER_API
