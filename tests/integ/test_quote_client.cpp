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
 *
 * Field-value assertions follow the Go SDK quote_client_integ_test.go patterns,
 * adapted for the C++ SDK which returns raw web::json::value (the unwrapped
 * "data" field from the API response envelope).
 */

namespace {

/// Build a JSON array of symbol strings, e.g. ["AAPL"].
web::json::value make_symbols(const utility::string_t &sym) {
    web::json::value arr = web::json::value::array();
    arr[0] = web::json::value::string(sym);
    return arr;
}

/// Extract an array from a result that may be either a raw JSON array
/// or a wrapper object with an "items" field.  Returns an empty array
/// if the result is null or empty.
web::json::value extract_array(const web::json::value &v) {
    if (v.is_null()) return web::json::value::array();
    if (v.is_array()) return v;
    if (v.is_object() && v.has_field(U("items"))) {
        const auto &items = v.at(U("items"));
        return items.is_null() ? web::json::value::array() : items;
    }
    return web::json::value::array();
}

} // anonymous namespace

// ── Existing test, enhanced with field-level assertions ──────────────────────

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
        // When entries exist, each should carry a symbol field.
        const auto &first = result.at(0);
        if (first.is_object()) {
            EXPECT_TRUE(first.has_field(U("symbol")));
            EXPECT_FALSE(first.at(U("symbol")).as_string().empty());
        }
    }
}

// ── Market state ─────────────────────────────────────────────────────────────

TEST(QuoteClientInteg, GetMarketState_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    ClientConfig cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(cfg);
    web::json::value result = client->get_market_state(U("US"));
    ASSERT_FALSE(result.is_null());

    auto arr = extract_array(result);
    ASSERT_GT(arr.size(), 0u) << "GetMarketState returned empty for US";

    const auto &entry = arr.at(0);
    ASSERT_TRUE(entry.is_object());
    ASSERT_TRUE(entry.has_field(U("market")));
    EXPECT_EQ(entry.at(U("market")).as_string(), U("US"));
    // At least one status field should be non-empty.
    bool has_status = false;
    if (entry.has_field(U("marketStatus")) && !entry.at(U("marketStatus")).as_string().empty()) {
        has_status = true;
    }
    if (entry.has_field(U("status")) && !entry.at(U("status")).as_string().empty()) {
        has_status = true;
    }
    EXPECT_TRUE(has_status) << "MarketState has no status (marketStatus/status both empty)";
}

// ── Brief ────────────────────────────────────────────────────────────────────

TEST(QuoteClientInteg, GetBrief_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    ClientConfig cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(cfg);
    web::json::value result = client->get_brief(make_symbols(U("AAPL")));
    ASSERT_FALSE(result.is_null());

    auto arr = extract_array(result);
    ASSERT_GT(arr.size(), 0u) << "GetBrief returned empty for AAPL";

    const auto &brief = arr.at(0);
    ASSERT_TRUE(brief.is_object());
    ASSERT_TRUE(brief.has_field(U("symbol")));
    EXPECT_EQ(brief.at(U("symbol")).as_string(), U("AAPL"));
    if (brief.has_field(U("latestPrice"))) {
        EXPECT_GT(brief.at(U("latestPrice")).as_double(), 0.0)
            << "latestPrice should be > 0 for AAPL";
    }
    if (brief.has_field(U("latestTime"))) {
        EXPECT_GT(brief.at(U("latestTime")).as_number().to_int64(), 0)
            << "latestTime should be non-zero";
    }
}

// ── K-line ───────────────────────────────────────────────────────────────────

TEST(QuoteClientInteg, GetKline_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    ClientConfig cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(cfg);
    web::json::value result = client->get_kline(make_symbols(U("AAPL")), BarPeriod::DAY,
                                                 /*begin_time=*/-1, /*end_time=*/-1,
                                                 QuoteRight::br, /*limit=*/5);
    ASSERT_FALSE(result.is_null());

    auto arr = extract_array(result);
    ASSERT_GT(arr.size(), 0u) << "GetKline returned empty for AAPL";

    const auto &kline = arr.at(0);
    ASSERT_TRUE(kline.is_object());
    ASSERT_TRUE(kline.has_field(U("symbol")));
    EXPECT_EQ(kline.at(U("symbol")).as_string(), U("AAPL"));
    ASSERT_TRUE(kline.has_field(U("items")));
    ASSERT_TRUE(kline.at(U("items")).is_array());
    ASSERT_GT(kline.at(U("items")).size(), 0u) << "Kline items is empty for AAPL";

    const auto &item = kline.at(U("items")).at(0);
    ASSERT_TRUE(item.is_object());
    if (item.has_field(U("time"))) {
        EXPECT_GT(item.at(U("time")).as_number().to_int64(), 0)
            << "KlineItem.time should be non-zero";
    }
    if (item.has_field(U("close"))) {
        EXPECT_GT(item.at(U("close")).as_double(), 0.0)
            << "KlineItem.close should be > 0";
    }
    // High should be >= low
    if (item.has_field(U("high")) && item.has_field(U("low"))) {
        EXPECT_GE(item.at(U("high")).as_double(), item.at(U("low")).as_double())
            << "KlineItem.high should be >= low";
    }
}

// ── Timeline ─────────────────────────────────────────────────────────────────

TEST(QuoteClientInteg, GetTimeline_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    ClientConfig cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(cfg);
    web::json::value result = client->get_timeline(make_symbols(U("AAPL")));
    ASSERT_FALSE(result.is_null());

    auto arr = extract_array(result);
    ASSERT_GT(arr.size(), 0u) << "GetTimeline returned empty for AAPL";

    const auto &tl = arr.at(0);
    ASSERT_TRUE(tl.is_object());
    ASSERT_TRUE(tl.has_field(U("symbol")));
    EXPECT_EQ(tl.at(U("symbol")).as_string(), U("AAPL"));

    // Timeline buckets may legitimately be empty outside trading hours.
    // When items exist, validate their fields.
    const utility::string_t bucket_names[] = {U("intraday"), U("preHours"), U("afterHours")};
    for (const auto &bucket_name : bucket_names) {
        if (!tl.has_field(bucket_name)) continue;
        const auto &bucket = tl.at(bucket_name);
        if (!bucket.is_object() || !bucket.has_field(U("items"))) continue;
        const auto &items = bucket.at(U("items"));
        if (!items.is_array() || items.size() == 0) continue;

        const auto &item = items.at(0);
        if (item.has_field(U("time"))) {
            EXPECT_GT(item.at(U("time")).as_number().to_int64(), 0)
                << "TimelineItem.time should be non-zero";
        }
        if (item.has_field(U("price"))) {
            EXPECT_GT(item.at(U("price")).as_double(), 0.0)
                << "TimelineItem.price should be > 0";
        }
        break; // only check first non-empty bucket
    }
}

// ── Option expiration ────────────────────────────────────────────────────────

TEST(QuoteClientInteg, GetOptionExpiration_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    ClientConfig cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(cfg);
    web::json::value result = client->get_option_expiration(make_symbols(U("AAPL")));
    ASSERT_FALSE(result.is_null());

    auto arr = extract_array(result);
    ASSERT_GT(arr.size(), 0u) << "GetOptionExpiration returned empty";

    const auto &exp = arr.at(0);
    ASSERT_TRUE(exp.is_object());
    ASSERT_TRUE(exp.has_field(U("symbol")));
    EXPECT_EQ(exp.at(U("symbol")).as_string(), U("AAPL"));
    ASSERT_TRUE(exp.has_field(U("dates")));
    ASSERT_TRUE(exp.at(U("dates")).is_array());
    EXPECT_GT(exp.at(U("dates")).size(), 0u) << "no expiry dates for AAPL";

    if (exp.has_field(U("optionSymbols"))) {
        EXPECT_GT(exp.at(U("optionSymbols")).size(), 0u)
            << "OptionExpiration.optionSymbols is empty, want non-empty";
    }
}

// ── Capital flow ─────────────────────────────────────────────────────────────

TEST(QuoteClientInteg, GetCapitalFlow_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    ClientConfig cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(cfg);
    web::json::value result = client->get_capital_flow(U("AAPL"), U("US"), U("day"));
    ASSERT_FALSE(result.is_null());
    ASSERT_TRUE(result.is_object());

    ASSERT_TRUE(result.has_field(U("symbol")));
    EXPECT_EQ(result.at(U("symbol")).as_string(), U("AAPL"));

    // Items may be empty outside market hours; when present, validate fields.
    if (result.has_field(U("items")) && result.at(U("items")).is_array()
        && result.at(U("items")).size() > 0) {
        const auto &item = result.at(U("items")).at(0);
        if (item.has_field(U("timestamp"))) {
            EXPECT_GT(item.at(U("timestamp")).as_number().to_int64(), 0)
                << "CapitalFlowItem.timestamp should be non-zero";
        }
    }
}

// ── Corporate action ─────────────────────────────────────────────────────────

TEST(QuoteClientInteg, GetCorporateAction_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    ClientConfig cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(cfg);
    web::json::value result = client->get_corporate_action(
        make_symbols(U("AAPL")), U("US"), U("DIVIDEND"), U("2024-01-01"), U("2025-12-31"));
    ASSERT_FALSE(result.is_null());

    // The server returns a map keyed by symbol: {"AAPL": [{...}, ...]}.
    // Flatten and validate the first action entry.
    ASSERT_TRUE(result.is_object());
    ASSERT_GT(result.size(), 0u) << "GetCorporateAction returned empty for AAPL DIVIDEND";

    // Iterate the map values and collect the first action entry.
    web::json::value first_action;
    bool found = false;
    for (const auto &kvp : result.as_object()) {
        if (kvp.second.is_array() && kvp.second.size() > 0) {
            first_action = kvp.second.at(0);
            found = true;
            break;
        }
    }
    ASSERT_TRUE(found) << "GetCorporateAction returned no action entries";
    ASSERT_TRUE(first_action.is_object());

    if (first_action.has_field(U("symbol"))) {
        EXPECT_EQ(first_action.at(U("symbol")).as_string(), U("AAPL"));
    }
    if (first_action.has_field(U("actionType"))) {
        EXPECT_FALSE(first_action.at(U("actionType")).as_string().empty())
            << "CorporateAction.actionType is empty, want non-empty";
    }
    if (first_action.has_field(U("executeDate"))) {
        EXPECT_FALSE(first_action.at(U("executeDate")).as_string().empty())
            << "CorporateAction.executeDate is empty, want non-empty date string";
    }
}

// ── Grab quote permission ────────────────────────────────────────────────────

TEST(QuoteClientInteg, GrabQuotePermission_ReturnsValidEntries) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    ClientConfig cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(cfg);
    web::json::value result = client->grab_quote_permission();
    ASSERT_FALSE(result.is_null());

    auto arr = extract_array(result);
    // Permission list may be empty if the account has no active entitlements.
    // When entries exist, validate identifier fields.
    for (size_t i = 0; i < arr.size(); ++i) {
        const auto &entry = arr.at(i);
        ASSERT_TRUE(entry.is_object());
        if (entry.has_field(U("name"))) {
            EXPECT_FALSE(entry.at(U("name")).as_string().empty())
                << "QuotePermission[" << i << "].Name is empty, want non-empty";
        }
    }
}
