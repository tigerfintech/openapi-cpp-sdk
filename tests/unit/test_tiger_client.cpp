#include <gtest/gtest.h>
#include "support/test_support.h"
#include "tigerapi/tiger_client.h"

using namespace TIGER_API;

// ── identifiers_to_options ────────────────────────────────────────────────────

// We test identifiers_to_options without making a network call.
// TigerClient's constructor only needs a valid ClientConfig (check() passes with
// non-empty tiger_id and private_key); the http_client is created lazily and
// no request is sent by identifiers_to_options.

namespace {
    // Create a TigerClient that won't make any network calls.
    // Using the fake offline config from test_support.h.
    std::unique_ptr<TigerClient> make_test_client() {
        auto cfg = test::make_offline_config();
        // No check_account needed for base TigerClient
        return std::make_unique<TigerClient>(*cfg);
    }
}

TEST(TigerClientTest, IdentifiersToOptions_SingleCallOption) {
    auto client = make_test_client();
    value identifiers = value::array();
    identifiers[0] = value::string(U("AAPL 220414C0017500"));

    value options = client->identifiers_to_options(identifiers);
    EXPECT_EQ(options.size(), 1u);
    EXPECT_TRUE(options[0].has_field(U("symbol")));
    EXPECT_EQ(options[0][U("symbol")].as_string(), U("AAPL"));
    EXPECT_TRUE(options[0].has_field(U("right")));
    EXPECT_EQ(options[0][U("right")].as_string(), U("CALL"));
    EXPECT_TRUE(options[0].has_field(U("strike")));
    EXPECT_TRUE(options[0].has_field(U("expiry")));
}

TEST(TigerClientTest, IdentifiersToOptions_SinglePutOption) {
    auto client = make_test_client();
    value identifiers = value::array();
    identifiers[0] = value::string(U("JD 211015P00045000"));

    value options = client->identifiers_to_options(identifiers);
    EXPECT_EQ(options.size(), 1u);
    EXPECT_EQ(options[0][U("symbol")].as_string(), U("JD"));
    EXPECT_EQ(options[0][U("right")].as_string(), U("PUT"));
}

TEST(TigerClientTest, IdentifiersToOptions_MultipleIdentifiers) {
    auto client = make_test_client();
    value identifiers = value::array();
    identifiers[0] = value::string(U("AAPL 220414C0017500"));
    identifiers[1] = value::string(U("AAPL 220414P0017500"));
    identifiers[2] = value::string(U("TSLA 220414C0070000"));

    value options = client->identifiers_to_options(identifiers);
    EXPECT_EQ(options.size(), 3u);
    EXPECT_EQ(options[0][U("symbol")].as_string(), U("AAPL"));
    EXPECT_EQ(options[1][U("right")].as_string(), U("PUT"));
    EXPECT_EQ(options[2][U("symbol")].as_string(), U("TSLA"));
}

TEST(TigerClientTest, IdentifiersToOptions_InvalidIdentifier_Skipped) {
    auto client = make_test_client();
    value identifiers = value::array();
    identifiers[0] = value::string(U("invalid_identifier"));
    identifiers[1] = value::string(U("AAPL 220414C0017500"));

    value options = client->identifiers_to_options(identifiers);
    // Only the valid one should be in the result
    EXPECT_EQ(options.size(), 1u);
    EXPECT_EQ(options[0][U("symbol")].as_string(), U("AAPL"));
}

TEST(TigerClientTest, IdentifiersToOptions_EmptyArray) {
    auto client = make_test_client();
    value identifiers = value::array();

    value options = client->identifiers_to_options(identifiers);
    EXPECT_EQ(options.size(), 0u);
}

TEST(TigerClientTest, IdentifiersToOptions_NonStringElement_Skipped) {
    auto client = make_test_client();
    value identifiers = value::array();
    identifiers[0] = value::number(12345);  // not a string
    identifiers[1] = value::string(U("AAPL 220414C0017500"));

    value options = client->identifiers_to_options(identifiers);
    EXPECT_EQ(options.size(), 1u);
}

// ── TigerClient construction ──────────────────────────────────────────────────

TEST(TigerClientTest, Constructor_DoesNotThrow) {
    EXPECT_NO_THROW(make_test_client());
}

TEST(TigerClientTest, Constructor_PreservesTigerId) {
    auto client = make_test_client();
    EXPECT_EQ(client->client_config.tiger_id, test::FAKE_TIGER_ID);
}

TEST(TigerClientTest, Constructor_PreservesAccount) {
    auto client = make_test_client();
    EXPECT_EQ(client->client_config.account, test::FAKE_ACCOUNT);
}

// ── Token refresh on TigerClient ──────────────────────────────────────────────

TEST(TigerClientTest, StopTokenRefresh_NotRunning_NoCrash) {
    auto client = make_test_client();
    EXPECT_NO_THROW(client->stop_token_refresh());
}

TEST(TigerClientTest, StartTokenRefresh_InvalidInterval_ReturnsFalse) {
    auto client = make_test_client();
    EXPECT_FALSE(client->start_token_refresh(0));
    EXPECT_FALSE(client->start_token_refresh(-1));
}

TEST(TigerClientTest, StartTokenRefresh_ValidInterval_ReturnsTrue) {
    auto client = make_test_client();
    // Use a large interval so query_token() is never called during the test
    EXPECT_TRUE(client->start_token_refresh(3600));
    client->stop_token_refresh();
}

TEST(TigerClientTest, StopTokenRefresh_AfterStart_NoCrash) {
    auto client = make_test_client();
    client->start_token_refresh(3600);
    EXPECT_NO_THROW(client->stop_token_refresh());
}
