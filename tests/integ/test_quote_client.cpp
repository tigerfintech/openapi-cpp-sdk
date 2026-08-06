#include <gtest/gtest.h>
#include "support/test_support.h"
#include "tigerapi/quote_client.h"
#include <memory>

using namespace TIGER_API;
using namespace TIGER_API::test;

/**
 * Integration tests for QuoteClient.
 * All tests call GTEST_SKIP() unless TIGER_RUN_INTEG=true, so the unit-only
 * CI stage never requires real credentials.
 *
 * Required env vars when TIGER_RUN_INTEG=true:
 *   TIGER_TEST_ID, TIGER_TEST_PRIVATE_KEY, TIGER_TEST_ACCOUNT
 */

TEST(QuoteClientInteg, GetSymbols_ReturnsValidStructure) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    ClientConfig cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(cfg);
    web::json::value result = client->get_symbols();
    EXPECT_FALSE(result.is_null());
    EXPECT_TRUE(result.is_array() || result.is_object());
    // Verify the result is actually non-empty, matching the test's intent
    if (result.is_array()) {
        EXPECT_GT(result.size(), 0u);
    }
}
