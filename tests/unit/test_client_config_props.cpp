#include <gtest/gtest.h>
#include "tigerapi/client_config.h"
#include "tigerapi/properties.h"
#include <fstream>
#include <string>
#include <chrono>
#include <thread>
#include <filesystem>

using namespace TIGER_API;

// ── load_props via properties file ─────────────────────────────────────────────

TEST(ClientConfigPropsTest, LoadProps_BasicConfig) {
    // Write a temp properties file
    const char* tmp_path = "/tmp/test_cfg_basic.properties";
    {
        std::ofstream out(tmp_path);
        out << "tiger_id=props_tid_001\n";
        out << "account=props_acct_001\n";
        out << "private_key_pk1=-----BEGIN " "RSA PRIVATE KEY-----\ndGVzdA==\n-----END RSA PRIVATE KEY-----\n";
        out << "license=TBUS\n";
    }

    ClientConfig cfg(false, utility::string_t(tmp_path));
    EXPECT_EQ(cfg.tiger_id, U("props_tid_001"));
    EXPECT_EQ(cfg.account, U("props_acct_001"));
    EXPECT_FALSE(cfg.private_key.empty());
    EXPECT_EQ(cfg.license, U("TBUS"));
}

TEST(ClientConfigPropsTest, LoadProps_Pk8Preferred) {
    const char* tmp_path = "/tmp/test_cfg_pk8.properties";
    {
        std::ofstream out(tmp_path);
        out << "tiger_id=tid_pk8\n";
        out << "private_key_pk8=pk8_key_data\n";
        out << "private_key_pk1=pk1_key_data\n";
    }

    ClientConfig cfg(false, utility::string_t(tmp_path));
    EXPECT_EQ(cfg.private_key, U("pk8_key_data"));
}

TEST(ClientConfigPropsTest, LoadProps_Pk1Fallback) {
    const char* tmp_path = "/tmp/test_cfg_pk1.properties";
    {
        std::ofstream out(tmp_path);
        out << "tiger_id=tid_pk1\n";
        out << "private_key_pk1=pk1_key_data\n";
    }

    ClientConfig cfg(false, utility::string_t(tmp_path));
    EXPECT_EQ(cfg.private_key, U("pk1_key_data"));
}

TEST(ClientConfigPropsTest, LoadProps_SandboxEnv) {
    const char* tmp_path = "/tmp/test_cfg_sandbox.properties";
    {
        std::ofstream out(tmp_path);
        out << "env=SANDBOX\n";
    }

    ClientConfig cfg(false, utility::string_t(tmp_path));
    // sandbox_debug should be true when env=SANDBOX
    // Also server_url should be sandbox URL
    EXPECT_EQ(cfg.get_server_url(), SANDBOX_TIGER_SERVER_URL);
}

TEST(ClientConfigPropsTest, LoadProps_CustomServerUrl) {
    const char* tmp_path = "/tmp/test_cfg_custom_url.properties";
    {
        std::ofstream out(tmp_path);
        out << "tiger_id=tid_custom\n";
        out << "server_url=https://custom.server.com\n";
        out << "server_public_key=custom_pub_key\n";
    }

    ClientConfig cfg(false, utility::string_t(tmp_path));
    EXPECT_EQ(cfg.get_server_url(), U("https://custom.server.com"));
    EXPECT_EQ(cfg.get_server_pub_key(), U("custom_pub_key"));
}

TEST(ClientConfigPropsTest, LoadProps_EmptyFile) {
    const char* tmp_path = "/tmp/test_cfg_empty.properties";
    {
        std::ofstream out(tmp_path);
        // empty file
    }

    ClientConfig cfg(false, utility::string_t(tmp_path));
    EXPECT_TRUE(cfg.tiger_id.empty());
    EXPECT_TRUE(cfg.account.empty());
}

TEST(ClientConfigPropsTest, LoadProps_DirectoryPath) {
    // When props_path is a directory (not ending in .properties),
    // it looks for tiger_openapi_config.properties in that directory.
    // Directory path must have a trailing slash for the path concatenation.
    const char* dir_path = "/tmp/test_cfg_dir/";
    std::filesystem::create_directories(dir_path);
    const char* file_path = "/tmp/test_cfg_dir/tiger_openapi_config.properties";
    {
        std::ofstream out(file_path);
        out << "tiger_id=dir_tid\n";
        out << "account=dir_acct\n";
    }

    ClientConfig cfg(false, utility::string_t(dir_path));
    EXPECT_EQ(cfg.tiger_id, U("dir_tid"));
    EXPECT_EQ(cfg.account, U("dir_acct"));
}

TEST(ClientConfigPropsTest, LoadProps_NonexistentDirectory) {
    // Nonexistent directory → load_props returns early
    EXPECT_NO_THROW(ClientConfig cfg(false, U("/tmp/nonexistent_dir_99999/")));
}

TEST(ClientConfigPropsTest, LoadProps_NonexistentFile) {
    // When props_path points to a nonexistent .properties file,
    // load_props tries to open it and logs error but doesn't crash.
    EXPECT_NO_THROW(ClientConfig cfg(false, U("/tmp/nonexistent_file_99999.properties")));
}

// ── load_token via properties file ─────────────────────────────────────────────

TEST(ClientConfigPropsTest, LoadToken_FromFile) {
    // Create a directory with both config and token files
    const char* dir_path = "/tmp/test_cfg_token_dir/";
    std::filesystem::create_directories(dir_path);

    // Create config file
    {
        std::ofstream out("/tmp/test_cfg_token_dir/tiger_openapi_config.properties");
        out << "tiger_id=tid_token\n";
        out << "account=acct_token\n";
    }
    // Create token file
    {
        std::ofstream out("/tmp/test_cfg_token_dir/tiger_openapi_token.properties");
        out << "token=loaded_token_abc123\n";
    }

    ClientConfig cfg(false, utility::string_t(dir_path));
    EXPECT_EQ(cfg.get_token(), U("loaded_token_abc123"));
}

TEST(ClientConfigPropsTest, LoadToken_NoTokenFile) {
    // Create a directory with only config file (no token file)
    const char* dir_path = "/tmp/test_cfg_no_token_dir";
    std::filesystem::create_directories(dir_path);
    {
        std::ofstream out("/tmp/test_cfg_no_token_dir/tiger_openapi_config.properties");
        out << "tiger_id=tid_no_token\n";
    }

    ClientConfig cfg(false, utility::string_t(dir_path));
    EXPECT_TRUE(cfg.get_token().empty());
}

// ── get_props_path logic (indirectly tested) ───────────────────────────────────

TEST(ClientConfigPropsTest, PropsPath_IsPropertiesFile) {
    // When props_path ends with .properties, it's used directly for config
    const char* tmp_path = "/tmp/test_cfg_isfile.properties";
    {
        std::ofstream out(tmp_path);
        out << "tiger_id=file_tid\n";
    }

    ClientConfig cfg(false, utility::string_t(tmp_path));
    EXPECT_EQ(cfg.tiger_id, U("file_tid"));
}

// ── Config with TBUS license → US server ──────────────────────────────────────

TEST(ClientConfigPropsTest, LicenseTBUS_SetsUSServer) {
    const char* tmp_path = "/tmp/test_cfg_tbus.properties";
    {
        std::ofstream out(tmp_path);
        out << "tiger_id=tid_tbus\n";
        out << "license=TBUS\n";
    }

    ClientConfig cfg(false, utility::string_t(tmp_path));
    EXPECT_TRUE(cfg.is_us());
    EXPECT_EQ(cfg.get_server_url(), US_TIGER_SERVER_URL);
    EXPECT_EQ(cfg.get_socket_url(), US_TIGER_SOCKET_HOST);
    EXPECT_EQ(cfg.get_socket_port(), US_TIGER_SOCKET_PORT);
}

TEST(ClientConfigPropsTest, LicenseTBSG_NotUS) {
    const char* tmp_path = "/tmp/test_cfg_tbsg.properties";
    {
        std::ofstream out(tmp_path);
        out << "tiger_id=tid_tbsg\n";
        out << "license=TBSG\n";
    }

    ClientConfig cfg(false, utility::string_t(tmp_path));
    EXPECT_FALSE(cfg.is_us());
}

// ── save_token to file via start_token_refresh ─────────────────────────────────

TEST(ClientConfigPropsTest, TokenRefresh_SavesTokenToFile) {
    // Set up a temp directory with config and token files
    const char* dir_path = "/tmp/test_cfg_refresh_dir/";
    std::filesystem::create_directories(dir_path);

    {
        std::ofstream out("/tmp/test_cfg_refresh_dir/tiger_openapi_config.properties");
        out << "tiger_id=tid_refresh\n";
        out << "account=acct_refresh\n";
    }

    // Create a ClientConfig with a directory path so save_token writes to file
    ClientConfig cfg(false, utility::string_t(dir_path));

    // Start token refresh with a short interval and a fetch function that returns a token
    cfg.start_token_refresh(1, []() { return U("refreshed_file_token_456"); });

    // Wait for at least one refresh cycle
    std::this_thread::sleep_for(std::chrono::milliseconds(1500));
    cfg.stop_token_refresh();

    // Token should be in memory
    EXPECT_EQ(cfg.get_token(), U("refreshed_file_token_456"));

    // Token should also be persisted to file
    std::ifstream token_file("/tmp/test_cfg_refresh_dir/tiger_openapi_token.properties");
    ASSERT_TRUE(token_file.is_open());
    Properties props;
    props.load(token_file);
    token_file.close();
    EXPECT_EQ(props.get_property(U("token")), U("refreshed_file_token_456"));
}
