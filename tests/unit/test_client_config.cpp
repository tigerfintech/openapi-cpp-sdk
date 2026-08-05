#include <gtest/gtest.h>
#include "support/test_support.h"
#include "tigerapi/client_config.h"

using namespace TIGER_API;

// ── Construction ──────────────────────────────────────────────────────────────

TEST(ClientConfigTest, DefaultConstructor_DoesNotThrow) {
    EXPECT_NO_THROW(ClientConfig cfg(false));
}

TEST(ClientConfigTest, ParameterizedConstructor_SetsFields) {
    ClientConfig cfg(U("12345"), U("fake_key"), U("ACCT001"),
                     /*sandbox_debug=*/true);
    EXPECT_EQ(cfg.tiger_id, U("12345"));
    EXPECT_EQ(cfg.account,  U("ACCT001"));
    EXPECT_TRUE(cfg.sandbox_debug);
}

TEST(ClientConfigTest, SandboxFlagFalseByDefault) {
    ClientConfig cfg(U("id"), U("key"), U("acct"));
    EXPECT_FALSE(cfg.sandbox_debug);
}

TEST(ClientConfigTest, MakeOfflineConfig_IsSandbox) {
    ClientConfig cfg = TIGER_API::test::make_offline_config();
    EXPECT_TRUE(cfg.sandbox_debug);
    EXPECT_EQ(cfg.tiger_id, TIGER_API::test::FAKE_TIGER_ID);
    EXPECT_EQ(cfg.account,  TIGER_API::test::FAKE_ACCOUNT);
}

TEST(ClientConfigTest, CopyConstructor_PreservesFields) {
    ClientConfig original(U("tid"), U("pkey"), U("acct"), true);
    ClientConfig copy(original);
    EXPECT_EQ(copy.tiger_id,     original.tiger_id);
    EXPECT_EQ(copy.account,      original.account);
    EXPECT_EQ(copy.sandbox_debug, original.sandbox_debug);
}
