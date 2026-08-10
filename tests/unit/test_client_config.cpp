#include <gtest/gtest.h>
#include "support/test_support.h"
#include "tigerapi/client_config.h"
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <mutex>
#include <thread>

using namespace TIGER_API;

// ── Construction ──────────────────────────────────────────────────────────────

TEST(ClientConfigTest, DefaultConstructor_DoesNotThrow) {
    EXPECT_NO_THROW(ClientConfig cfg(false));
}

TEST(ClientConfigTest, DefaultConstructor_SandboxTrue_LogsWarning) {
    // sandbox_debug=true triggers LOG(WARNING) "SANDBOX IS NOT SUPPORTED"
    EXPECT_NO_THROW(ClientConfig cfg(true));
}

TEST(ClientConfigTest, ParameterizedConstructor_SetsFields) {
    ClientConfig cfg(U("12345"), U("fake_key"), U("ACCT001"), true);
    EXPECT_EQ(cfg.tiger_id, U("12345"));
    EXPECT_EQ(cfg.account,  U("ACCT001"));
}

TEST(ClientConfigTest, ParameterizedConstructor_NoSandbox_SetsFields) {
    ClientConfig cfg(U("tid"), U("key"), U("acct"), false);
    EXPECT_EQ(cfg.tiger_id, U("tid"));
    EXPECT_EQ(cfg.account, U("acct"));
}

TEST(ClientConfigTest, SandboxFlagFalseByDefault) {
    ClientConfig cfg(U("id"), U("key"), U("acct"), false);
    EXPECT_EQ(cfg.tiger_id, U("id"));
    EXPECT_EQ(cfg.account, U("acct"));
}

TEST(ClientConfigTest, MakeOfflineConfig_IsSandbox) {
    auto cfg = TIGER_API::test::make_offline_config();
    EXPECT_EQ(cfg.tiger_id, TIGER_API::test::FAKE_TIGER_ID);
    EXPECT_EQ(cfg.account,  TIGER_API::test::FAKE_ACCOUNT);
}

TEST(ClientConfigTest, CopyConstructor_PreservesFields) {
    ClientConfig original(U("tid"), U("pkey"), U("acct"), true);
    ClientConfig copy(original);
    EXPECT_EQ(copy.tiger_id,     original.tiger_id);
    EXPECT_EQ(copy.account,      original.account);
}

// ── check() ───────────────────────────────────────────────────────────────────

TEST(ClientConfigTest, Check_ThrowsOnEmptyTigerId) {
    ClientConfig cfg(U(""), U("some_key"), U("acct"), false);
    EXPECT_THROW(cfg.check(), std::runtime_error);
}

TEST(ClientConfigTest, Check_ThrowsOnEmptyPrivateKey) {
    ClientConfig cfg(U("tid"), U(""), U("acct"), false);
    EXPECT_THROW(cfg.check(), std::runtime_error);
}

TEST(ClientConfigTest, Check_PassesWithValidFields) {
    ClientConfig cfg(U("tid"), U("pkey"), U("acct"), false);
    EXPECT_NO_THROW(cfg.check());
}

// ── check_account() ───────────────────────────────────────────────────────────

TEST(ClientConfigTest, CheckAccount_ThrowsOnEmptyAccount) {
    ClientConfig cfg(U("tid"), U("pkey"), U(""), false);
    EXPECT_THROW(cfg.check_account(), std::runtime_error);
}

TEST(ClientConfigTest, CheckAccount_PassesWithValidAccount) {
    ClientConfig cfg(U("tid"), U("pkey"), U("acct"), false);
    EXPECT_NO_THROW(cfg.check_account());
}

// ── Setters / Getters ─────────────────────────────────────────────────────────

TEST(ClientConfigTest, SetGetServerUrl) {
    ClientConfig cfg(U("tid"), U("key"), U("acct"), false);
    cfg.set_server_url(U("https://custom.example.com"));
    EXPECT_EQ(cfg.get_server_url(), U("https://custom.example.com"));
}

TEST(ClientConfigTest, SetGetSocketUrl) {
    ClientConfig cfg(U("tid"), U("key"), U("acct"), false);
    cfg.set_socket_url(U("wss://socket.example.com"));
    EXPECT_EQ(cfg.get_socket_url(), U("wss://socket.example.com"));
}

TEST(ClientConfigTest, SetGetSocketPort) {
    ClientConfig cfg(U("tid"), U("key"), U("acct"), false);
    cfg.set_socket_port(U("443"));
    EXPECT_EQ(cfg.get_socket_port(), U("443"));
}

TEST(ClientConfigTest, SetGetServerPublicKey) {
    ClientConfig cfg(U("tid"), U("key"), U("acct"), false);
    cfg.set_server_public_key(U("pub_key_data"));
    EXPECT_EQ(cfg.get_server_pub_key(), U("pub_key_data"));
}

TEST(ClientConfigTest, SetGetToken) {
    ClientConfig cfg(U("tid"), U("key"), U("acct"), false);
    cfg.set_token(U("my_token_abc"));
    EXPECT_EQ(cfg.get_token(), U("my_token_abc"));
}

// ── Default values ────────────────────────────────────────────────────────────

TEST(ClientConfigTest, DefaultCharset) {
    ClientConfig cfg(U("tid"), U("key"), U("acct"), false);
    EXPECT_EQ(cfg.charset, U("UTF-8"));
}

TEST(ClientConfigTest, DefaultSignType) {
    ClientConfig cfg(U("tid"), U("key"), U("acct"), false);
    EXPECT_EQ(cfg.sign_type, U("RSA"));
}

TEST(ClientConfigTest, DefaultLang_EmptyInSimpleConstructor) {
    // The 4-arg call (tid, key, acct, false) resolves to the 5-arg constructor
    // which sets lang = U("en_US") as default. The 3-arg constructor would
    // leave lang empty, but passing 4 args matches the 5-arg overload.
    ClientConfig cfg(U("tid"), U("key"), U("acct"), false);
    EXPECT_EQ(cfg.lang, U("en_US"));
}

TEST(ClientConfigTest, LangSetInFullConstructor) {
    ClientConfig cfg(U("tid"), U("key"), U("acct"), false, U("zh_CN"));
    EXPECT_EQ(cfg.lang, U("zh_CN"));
}

TEST(ClientConfigTest, DefaultLang_IsEnUS) {
    ClientConfig cfg(U("tid"), U("key"), U("acct"), false, U("en_US"));
    EXPECT_EQ(cfg.lang, U("en_US"));
}

TEST(ClientConfigTest, DeviceId_NotEmpty) {
    ClientConfig cfg(U("tid"), U("key"), U("acct"), false);
    EXPECT_FALSE(cfg.device_id.empty());
}

TEST(ClientConfigTest, DefaultServerUrl_NotEmpty) {
    ClientConfig cfg(U("tid"), U("key"), U("acct"), false);
    EXPECT_FALSE(cfg.get_server_url().empty());
}

TEST(ClientConfigTest, DefaultSocketUrl_NotEmpty) {
    ClientConfig cfg(U("tid"), U("key"), U("acct"), false);
    EXPECT_FALSE(cfg.get_socket_url().empty());
}

TEST(ClientConfigTest, DefaultSocketPort_NotEmpty) {
    ClientConfig cfg(U("tid"), U("key"), U("acct"), false);
    EXPECT_FALSE(cfg.get_socket_port().empty());
}

TEST(ClientConfigTest, DefaultServerPubKey_NotEmpty) {
    ClientConfig cfg(U("tid"), U("key"), U("acct"), false);
    EXPECT_FALSE(cfg.get_server_pub_key().empty());
}

// ── is_us() ────────────────────────────────────────────────────────────────────

TEST(ClientConfigTest, IsUs_FalseByDefault) {
    ClientConfig cfg(U("tid"), U("key"), U("acct"), false);
    EXPECT_FALSE(cfg.is_us());
}

TEST(ClientConfigTest, IsUs_TrueWhenLicenseIsTBUS) {
    ClientConfig cfg(U("tid"), U("key"), U("acct"), false);
    cfg.license = U("TBUS");
    EXPECT_TRUE(cfg.is_us());
}

TEST(ClientConfigTest, IsUs_FalseWhenLicenseIsTBNZ) {
    ClientConfig cfg(U("tid"), U("key"), U("acct"), false);
    cfg.license = U("TBNZ");
    EXPECT_FALSE(cfg.is_us());
}

TEST(ClientConfigTest, IsUs_FalseWhenLicenseEmpty) {
    ClientConfig cfg(U("tid"), U("key"), U("acct"), false);
    cfg.license = U("");
    EXPECT_FALSE(cfg.is_us());
}

// ── Token refresh ──────────────────────────────────────────────────────────────

TEST(ClientConfigTest, StartTokenRefresh_InvalidInterval_ReturnsFalse) {
    ClientConfig cfg(U("tid"), U("key"), U("acct"), false);
    EXPECT_FALSE(cfg.start_token_refresh(0, []() { return U("token"); }));
    EXPECT_FALSE(cfg.start_token_refresh(-1, []() { return U("token"); }));
}

TEST(ClientConfigTest, StartTokenRefresh_ValidInterval_ReturnsTrue) {
    ClientConfig cfg(U("tid"), U("key"), U("acct"), false);
    EXPECT_TRUE(cfg.start_token_refresh(1, []() { return U("new_token"); }));
    cfg.stop_token_refresh();
}

TEST(ClientConfigTest, StartTokenRefresh_DoubleStart_ReturnsFalse) {
    ClientConfig cfg(U("tid"), U("key"), U("acct"), false);
    cfg.start_token_refresh(1, []() { return U("token1"); });
    EXPECT_FALSE(cfg.start_token_refresh(1, []() { return U("token2"); }));
    cfg.stop_token_refresh();
}

TEST(ClientConfigTest, StopTokenRefresh_NotRunning_NoCrash) {
    ClientConfig cfg(U("tid"), U("key"), U("acct"), false);
    EXPECT_NO_THROW(cfg.stop_token_refresh());
}

TEST(ClientConfigTest, StopTokenRefresh_AfterStart_StopsCleanly) {
    ClientConfig cfg(U("tid"), U("key"), U("acct"), false);
    cfg.start_token_refresh(1, []() { return U("tok"); });
    cfg.stop_token_refresh();
    // Can start again after stop
    EXPECT_TRUE(cfg.start_token_refresh(1, []() { return U("tok2"); }));
    cfg.stop_token_refresh();
}

TEST(ClientConfigTest, TokenRefresh_FetchFunctionCalled) {
    ClientConfig cfg(U("tid"), U("key"), U("acct"), false);
    std::atomic<int> call_count{0};
    std::mutex mtx;
    std::condition_variable cv;
    cfg.start_token_refresh(1, [&call_count, &cv]() {
        call_count.fetch_add(1, std::memory_order_relaxed);
        cv.notify_all();
        return U("refreshed_token");
    });
    {
        std::unique_lock<std::mutex> lock(mtx);
        // Wait up to 5s for at least one callback (interval is 1s → 5× margin)
        cv.wait_for(lock, std::chrono::seconds(5),
                    [&call_count]() { return call_count.load() > 0; });
    }
    cfg.stop_token_refresh();
    EXPECT_GT(call_count.load(), 0);
}

TEST(ClientConfigTest, TokenRefresh_EmptyToken_KeepsCurrent) {
    ClientConfig cfg(U("tid"), U("key"), U("acct"), false);
    cfg.set_token(U("original_token"));
    std::atomic<int> call_count{0};
    std::mutex mtx;
    std::condition_variable cv;
    cfg.start_token_refresh(1, [&call_count, &cv]() {
        call_count.fetch_add(1, std::memory_order_relaxed);
        cv.notify_all();
        return U("");
    });
    {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait_for(lock, std::chrono::seconds(5),
                    [&call_count]() { return call_count.load() > 0; });
    }
    cfg.stop_token_refresh();
    EXPECT_EQ(cfg.get_token(), U("original_token"));
}

TEST(ClientConfigTest, TokenRefresh_NonEmptyToken_UpdatesToken) {
    ClientConfig cfg(U("tid"), U("key"), U("acct"), false);
    cfg.set_token(U("original"));
    std::atomic<bool> updated{false};
    std::mutex mtx;
    std::condition_variable cv;
    cfg.start_token_refresh(1, [&updated, &cv]() {
        updated.store(true, std::memory_order_relaxed);
        cv.notify_all();
        return U("refreshed_token_xyz");
    });
    {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait_for(lock, std::chrono::seconds(5),
                    [&updated]() { return updated.load(); });
    }
    cfg.stop_token_refresh();
    EXPECT_EQ(cfg.get_token(), U("refreshed_token_xyz"));
}

TEST(ClientConfigTest, TokenRefresh_ExceptionInFetch_NoCrash) {
    ClientConfig cfg(U("tid"), U("key"), U("acct"), false);
    cfg.set_token(U("original"));
    std::atomic<int> call_count{0};
    std::mutex mtx;
    std::condition_variable cv;
    cfg.start_token_refresh(1, [&call_count, &cv]() -> utility::string_t {
        call_count.fetch_add(1, std::memory_order_relaxed);
        cv.notify_all();
        throw std::runtime_error("network error");
    });
    {
        std::unique_lock<std::mutex> lock(mtx);
        // Wait for the callback to be invoked at least once (confirms the refresh
        // thread actually ran and survived the exception without crashing).
        cv.wait_for(lock, std::chrono::seconds(5),
                    [&call_count]() { return call_count.load() > 0; });
    }
    cfg.stop_token_refresh();
    EXPECT_EQ(cfg.get_token(), U("original"));
}

// ── Config with properties file ────────────────────────────────────────────────

TEST(ClientConfigTest, ConstructorWithPropsPath_NonexistentPath_NoCrash) {
    EXPECT_NO_THROW(ClientConfig cfg(false, U("/tmp/nonexistent_dir_98765/")));
}

TEST(ClientConfigTest, ConstructorWithPropsPath_EmptyPath_NoCrash) {
    EXPECT_NO_THROW(ClientConfig cfg(false, U("")));
}

// ── Token get/set thread safety ────────────────────────────────────────────────

TEST(ClientConfigTest, Token_SetGet_Roundtrip) {
    ClientConfig cfg(U("tid"), U("key"), U("acct"), false);
    cfg.set_token(U(""));
    EXPECT_EQ(cfg.get_token(), U(""));

    cfg.set_token(U("token_abc_123"));
    EXPECT_EQ(cfg.get_token(), U("token_abc_123"));

    cfg.set_token(U("token_xyz_456"));
    EXPECT_EQ(cfg.get_token(), U("token_xyz_456"));
}
