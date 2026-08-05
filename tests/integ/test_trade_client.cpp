#include <gtest/gtest.h>
#include "support/test_support.h"
#include "tigerapi/trade_client.h"
#include "tigerapi/client_config.h"
#include <memory>

using namespace TIGER_API;
using namespace TIGER_API::test;

/**
 * Integration tests for TradeClient.
 * All tests call GTEST_SKIP() unless TIGER_RUN_INTEG=true.
 *
 * Required env vars when TIGER_RUN_INTEG=true:
 *   TIGER_TEST_ID, TIGER_TEST_PRIVATE_KEY, TIGER_TEST_ACCOUNT
 */

TEST(TradeClientInteg, GetAsset_DoesNotThrow) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    ClientConfig cfg = make_integ_config();
    auto client = std::make_shared<TradeClient>(cfg);
    EXPECT_NO_THROW({
        web::json::value result = client->get_asset();
        EXPECT_FALSE(result.is_null());
    });
}
