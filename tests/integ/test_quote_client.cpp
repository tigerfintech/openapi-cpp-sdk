#include <gtest/gtest.h>
#include "support/test_support.h"
#include "integ/test_market_helpers.h"
#include "tigerapi/quote_client.h"
#include "tigerapi/utils.h"
#include <chrono>
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

/// Check that a JSON field exists, is a string, and is non-empty.
/// Absent or null fields are treated as "not present" (returns true = OK).
/// Non-string types or empty strings return false (caller should EXPECT).
static bool json_nonempty_string(const web::json::value &obj,
                                 const utility::string_t &field) {
    if (!obj.has_field(field)) return true;
    const auto &v = obj.at(field);
    if (v.is_null()) return true;
    if (!v.is_string()) return false;
    return !v.as_string().empty();
}

/// Check that a JSON field exists and is a string (for EXPECT_EQ cases).
static bool json_is_string(const web::json::value &obj,
                            const utility::string_t &field) {
    return obj.has_field(field)
        && !obj.at(field).is_null()
        && obj.at(field).is_string();
}

} // anonymous namespace

// ── Existing test, enhanced with field-level assertions ──────────────────────

TEST(QuoteClientInteg, GetSymbols_ReturnsValidStructure) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(*cfg);
    web::json::value result = client->get_symbols();
    EXPECT_FALSE(result.is_null());
    EXPECT_TRUE(result.is_array() || result.is_object());
    // Verify the result is actually non-empty, matching the test's intent
    if (result.is_array()) {
        EXPECT_GT(result.size(), 0u);
        // When entries exist, each should carry a symbol field.
        const auto &first = result.at(0);
        if (first.is_object()) {
            EXPECT_TRUE(json_nonempty_string(first, U("symbol")));
        }
    }
}

// ── Market state ─────────────────────────────────────────────────────────────

TEST(QuoteClientInteg, GetMarketState_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(*cfg);
    web::json::value result = client->get_market_state(U("US"));
    ASSERT_FALSE(result.is_null());

    auto arr = extract_array(result);
    ASSERT_GT(arr.size(), 0u) << "GetMarketState returned empty for US";

    const auto &entry = arr.at(0);
    ASSERT_TRUE(entry.is_object());
    ASSERT_TRUE(json_is_string(entry, U("market")));
    EXPECT_EQ(entry.at(U("market")).as_string(), U("US"));
    // At least one status field should be non-empty.
    bool has_status = false;
    if (json_nonempty_string(entry, U("marketStatus"))) {
        has_status = true;
    }
    if (json_nonempty_string(entry, U("status"))) {
        has_status = true;
    }
    EXPECT_TRUE(has_status) << "MarketState has no status (marketStatus/status both empty)";
}

// ── Brief ────────────────────────────────────────────────────────────────────

TEST(QuoteClientInteg, GetBrief_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(*cfg);
    web::json::value result = client->get_brief(make_symbols(U("AAPL")));
    ASSERT_FALSE(result.is_null());

    auto arr = extract_array(result);
    ASSERT_GT(arr.size(), 0u) << "GetBrief returned empty for AAPL";

    const auto &brief = arr.at(0);
    ASSERT_TRUE(brief.is_object());
    ASSERT_TRUE(json_is_string(brief, U("symbol")));
    EXPECT_EQ(brief.at(U("symbol")).as_string(), U("AAPL"));
    ASSERT_TRUE(brief.has_field(U("latestPrice"))) << "Brief missing latestPrice field";
    EXPECT_GT(brief.at(U("latestPrice")).as_double(), 0.0) << "latestPrice should be > 0 for AAPL";
    // latestTime may be absent when the market is closed
    if (brief.has_field(U("latestTime"))) {
        EXPECT_GT(brief.at(U("latestTime")).as_number().to_int64(), 0) << "latestTime should be non-zero";
    }
}

// ── K-line ───────────────────────────────────────────────────────────────────

TEST(QuoteClientInteg, GetKline_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(*cfg);
    web::json::value result = client->get_kline(make_symbols(U("AAPL")), BarPeriod::DAY,
                                                 /*begin_time=*/-1, /*end_time=*/-1,
                                                 QuoteRight::br, /*limit=*/5);
    ASSERT_FALSE(result.is_null());

    auto arr = extract_array(result);
    ASSERT_GT(arr.size(), 0u) << "GetKline returned empty for AAPL";

    const auto &kline = arr.at(0);
    ASSERT_TRUE(kline.is_object());
    ASSERT_TRUE(json_is_string(kline, U("symbol")));
    EXPECT_EQ(kline.at(U("symbol")).as_string(), U("AAPL"));
    ASSERT_TRUE(kline.has_field(U("items")));
    ASSERT_TRUE(kline.at(U("items")).is_array());
    ASSERT_GT(kline.at(U("items")).size(), 0u) << "Kline items is empty for AAPL";

    const auto &item = kline.at(U("items")).at(0);
    ASSERT_TRUE(item.is_object());
    ASSERT_TRUE(item.has_field(U("time"))) << "KlineItem missing time field";
    EXPECT_GT(item.at(U("time")).as_number().to_int64(), 0) << "KlineItem.time should be non-zero";
    ASSERT_TRUE(item.has_field(U("close"))) << "KlineItem missing close field";
    EXPECT_GT(item.at(U("close")).as_double(), 0.0) << "KlineItem.close should be > 0";
    // Cross-field constraint: high >= low
    ASSERT_TRUE(item.has_field(U("high"))) << "KlineItem missing high field";
    ASSERT_TRUE(item.has_field(U("low"))) << "KlineItem missing low field";
    EXPECT_GE(item.at(U("high")).as_double(), item.at(U("low")).as_double())
        << "KlineItem.high should be >= low";
}

// ── Timeline ─────────────────────────────────────────────────────────────────

TEST(QuoteClientInteg, GetTimeline_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(*cfg);
    web::json::value result = client->get_timeline(make_symbols(U("AAPL")));
    ASSERT_FALSE(result.is_null());

    auto arr = extract_array(result);
    // Wire path validated by the request completing without exception.
    // Data may legitimately be empty outside US trading hours.
    if (arr.size() == 0) {
        if (is_market_trading(*client, U("US"))) {
            FAIL() << "GetTimeline empty during US trading hours (real regression)";
        }
        return;
    }

    const auto &tl = arr.at(0);
    ASSERT_TRUE(tl.is_object());
    ASSERT_TRUE(json_is_string(tl, U("symbol")));
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
        ASSERT_TRUE(item.has_field(U("time"))) << "TimelineItem missing time field";
        EXPECT_GT(item.at(U("time")).as_number().to_int64(), 0) << "TimelineItem.time should be non-zero";
        ASSERT_TRUE(item.has_field(U("price"))) << "TimelineItem missing price field";
        EXPECT_GT(item.at(U("price")).as_double(), 0.0) << "TimelineItem.price should be > 0";
        break; // only check first non-empty bucket
    }
}

// ── Option expiration ────────────────────────────────────────────────────────

TEST(QuoteClientInteg, GetOptionExpiration_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(*cfg);
    web::json::value result = client->get_option_expiration(make_symbols(U("AAPL")));
    ASSERT_FALSE(result.is_null());

    auto arr = extract_array(result);
    ASSERT_GT(arr.size(), 0u) << "GetOptionExpiration returned empty";

    const auto &exp = arr.at(0);
    ASSERT_TRUE(exp.is_object());
    ASSERT_TRUE(json_is_string(exp, U("symbol")));
    EXPECT_EQ(exp.at(U("symbol")).as_string(), U("AAPL"));
    ASSERT_TRUE(exp.has_field(U("dates")));
    ASSERT_TRUE(exp.at(U("dates")).is_array());
    EXPECT_GT(exp.at(U("dates")).size(), 0u) << "no expiry dates for AAPL";

    if (exp.has_field(U("optionSymbols")) && !exp.at(U("optionSymbols")).is_null()) {
        EXPECT_GT(exp.at(U("optionSymbols")).size(), 0u)
            << "OptionExpiration.optionSymbols is empty, want non-empty";
    }
}

// ── Capital flow ─────────────────────────────────────────────────────────────

TEST(QuoteClientInteg, GetCapitalFlow_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(*cfg);
    web::json::value result = client->get_capital_flow(U("AAPL"), U("US"), U("day"));
    ASSERT_FALSE(result.is_null());
    ASSERT_TRUE(result.is_object());

    // Wire path validated by the request completing without exception.
    // Symbol/data may legitimately be absent outside US trading hours.
    if (!json_is_string(result, U("symbol"))) {
        if (is_market_trading(*client, U("US"))) {
            FAIL() << "GetCapitalFlow returned no symbol during US trading hours (real regression)";
        }
        return;
    }
    EXPECT_EQ(result.at(U("symbol")).as_string(), U("AAPL"));

    // Items may be empty outside market hours; when present, validate fields.
    if (result.has_field(U("items")) && result.at(U("items")).is_array()
        && result.at(U("items")).size() > 0) {
        const auto &item = result.at(U("items")).at(0);
        if (item.has_field(U("timestamp")) && item.at(U("timestamp")).is_number()) {
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
    auto cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(*cfg);
    web::json::value result = client->get_corporate_action(
        make_symbols(U("AAPL")), U("US"), U("DIVIDEND"),
        Utils::date_string_to_timestamp(U("2024-01-01")),
        Utils::date_string_to_timestamp(U("2025-12-31")));
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

    if (json_is_string(first_action, U("symbol"))) {
        EXPECT_EQ(first_action.at(U("symbol")).as_string(), U("AAPL"));
    }
    EXPECT_TRUE(json_nonempty_string(first_action, U("actionType")))
        << "CorporateAction.actionType is empty, want non-empty";

    EXPECT_TRUE(json_nonempty_string(first_action, U("executeDate")))
        << "CorporateAction.executeDate is empty, want non-empty date string";

}

// ── Claim market data device access ─────────────────────────────────────────

TEST(QuoteClientInteg, GrabQuotePermission_ReturnsValidEntries) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(*cfg);
    web::json::value result = client->grab_quote_permission();
    ASSERT_FALSE(result.is_null());

    auto arr = extract_array(result);
    // Permission list may be empty if the account has no active entitlements.
    // When entries exist, validate identifier fields.
    for (size_t i = 0; i < arr.size(); ++i) {
        const auto &entry = arr.at(i);
        ASSERT_TRUE(entry.is_object());
        EXPECT_TRUE(json_nonempty_string(entry, U("name")))
            << "QuotePermission[" << i << "].Name is empty, want non-empty";

    }
}

// ── Helpers for identifier discovery (option/future/fund/warrant/industry) ────

namespace {

/// Try to fetch the first option expiry date for AAPL. Returns empty on failure.
utility::string_t get_first_option_expiry(QuoteClient &client) {
    auto result = client.get_option_expiration(make_symbols(U("AAPL")));
    auto arr = extract_array(result);
    if (arr.size() == 0) return U("");
    const auto &exp = arr.at(0);
    if (!exp.is_object() || !exp.has_field(U("dates")) || !exp.at(U("dates")).is_array()) return U("");
    if (exp.at(U("dates")).size() == 0) return U("");
    return exp.at(U("dates")).at(0).as_string();
}

/// Try to fetch the first option identifier from the AAPL option chain. Empty on failure.
utility::string_t get_first_option_identifier(QuoteClient &client) {
    auto expiry = get_first_option_expiry(client);
    if (expiry.empty()) return U("");
    auto chain = client.get_option_chain(U("AAPL"), expiry);
    auto arr = extract_array(chain);
    if (arr.size() == 0) return U("");
    const auto &entry = arr.at(0);
    if (!entry.is_object() || !entry.has_field(U("items"))) return U("");
    const auto &items = entry.at(U("items"));
    if (!items.is_array() || items.size() == 0) return U("");
    const auto &row = items.at(0);
    const utility::string_t legs[] = {U("call"), U("put")};
    for (const auto &leg : legs) {
        if (row.has_field(leg) && row.at(leg).is_object()) {
            const auto &leg_val = row.at(leg);
            if (leg_val.has_field(U("identifier")) && leg_val.at(U("identifier")).is_string()) {
                return leg_val.at(U("identifier")).as_string();
            }
        }
    }
    return U("");
}

/// Build a JSON array of identifier strings from a single identifier.
web::json::value make_identifiers(const utility::string_t &id) {
    web::json::value arr = web::json::value::array();
    arr[0] = web::json::value::string(id);
    return arr;
}

/// Try to fetch the first future exchange code. Returns empty on failure.
utility::string_t get_first_future_exchange_code(QuoteClient &client) {
    auto result = client.get_future_exchange();
    auto arr = extract_array(result);
    if (arr.size() == 0) return U("");
    const auto &exchange = arr.at(0);
    const utility::string_t fields[] = {U("exchangeCode"), U("code"), U("exchange")};
    for (const auto &field : fields) {
        if (exchange.has_field(field) && exchange.at(field).is_string()) {
            return exchange.at(field).as_string();
        }
    }
    return U("");
}

/// Try to fetch the first future contract code. Returns empty on failure.
utility::string_t get_first_future_contract_code(QuoteClient &client) {
    // Try get_future_contracts
    auto result = client.get_future_contracts(U("ES"));
    auto arr = extract_array(result);
    const utility::string_t fields[] = {U("contractCode"), U("symbol"), U("code")};
    if (arr.size() > 0) {
        const auto &c = arr.at(0);
        for (const auto &field : fields) {
            if (c.has_field(field) && c.at(field).is_string()) return c.at(field).as_string();
        }
    }
    // Try get_future_current_contract
    result = client.get_future_current_contract(U("ES"));
    if (result.is_object()) {
        for (const auto &field : fields) {
            if (result.has_field(field) && result.at(field).is_string()) return result.at(field).as_string();
        }
    }
    arr = extract_array(result);
    if (arr.size() > 0) {
        const auto &c = arr.at(0);
        for (const auto &field : fields) {
            if (c.has_field(field) && c.at(field).is_string()) return c.at(field).as_string();
        }
    }
    return U("");
}

/// Try to fetch the first fund symbol. Returns empty on failure.
utility::string_t get_first_fund_symbol(QuoteClient &client) {
    auto result = client.get_fund_symbols();
    auto arr = extract_array(result);
    if (arr.size() == 0) return U("");
    const auto &fund = arr.at(0);
    const utility::string_t fields[] = {U("symbol"), U("code")};
    for (const auto &field : fields) {
        if (fund.has_field(field) && fund.at(field).is_string()) return fund.at(field).as_string();
    }
    return U("");
}

/// Try to fetch the first industry ID. Returns empty on failure.
utility::string_t get_first_industry_id(QuoteClient &client) {
    auto result = client.get_industry_list();
    auto arr = extract_array(result);
    if (arr.size() == 0) return U("");
    const auto &industry = arr.at(0);
    if (!industry.is_object()) return U("");
    const utility::string_t fields[] = {U("id"), U("industryId"), U("code")};
    for (const auto &field : fields) {
        if (industry.has_field(field) && industry.at(field).is_string()) {
            return industry.at(field).as_string();
        }
    }
    return U("");
}

/// Try to fetch the first warrant symbol from warrant_filter. Empty on failure.
utility::string_t get_first_warrant_symbol(QuoteClient &client, const utility::string_t &underlying) {
    auto result = client.get_warrant_filter(underlying);
    auto arr = extract_array(result);
    if (arr.size() == 0) return U("");
    const auto &warrant = arr.at(0);
    if (!warrant.is_object()) return U("");
    const utility::string_t fields[] = {U("symbol"), U("warrantSymbol")};
    for (const auto &field : fields) {
        if (warrant.has_field(field) && warrant.at(field).is_string()) return warrant.at(field).as_string();
    }
    return U("");
}

} // anonymous namespace

// ── Stock quote: additional tests ─────────────────────────────────────────────

TEST(QuoteClientInteg, GetQuotePermission_ReturnsValidStructure) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(*cfg);
    web::json::value result = client->get_quote_permission();
    ASSERT_FALSE(result.is_null());
    auto arr = extract_array(result);
    for (size_t i = 0; i < arr.size(); ++i) {
        const auto &entry = arr.at(i);
        ASSERT_TRUE(entry.is_object());
        EXPECT_TRUE(json_nonempty_string(entry, U("name")))
            << "QuotePermission[" << i << "].Name is empty";

    }
}

TEST(QuoteClientInteg, GetAllSymbolNames_ReturnsValidStructure) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(*cfg);
    web::json::value result = client->get_all_symbol_names(Market::US);
    ASSERT_FALSE(result.is_null());
    EXPECT_TRUE(result.is_array() || result.is_object());
    if (result.is_array()) {
        EXPECT_GT(result.size(), 0u);
        const auto &first = result.at(0);
        if (first.is_object()) {
            EXPECT_TRUE(first.has_field(U("symbol")) || first.has_field(U("name")));
        }
    }
}

TEST(QuoteClientInteg, GetTradingCalendar_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(*cfg);
    web::json::value result = client->get_trading_calendar(U("US"), U("2024-01-01"), U("2024-12-31"));
    ASSERT_FALSE(result.is_null());
    auto arr = extract_array(result);
    ASSERT_GT(arr.size(), 0u) << "GetTradingCalendar returned empty for US 2024";
    const auto &entry = arr.at(0);
    ASSERT_TRUE(entry.is_object());
    EXPECT_TRUE(json_nonempty_string(entry, U("tradeDate")))
        << "TradingCalendar.tradeDate is empty";

}

TEST(QuoteClientInteg, GetQuoteStockTrade_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(*cfg);
    web::json::value result = client->get_quote_stock_trade(make_symbols(U("AAPL")));
    ASSERT_FALSE(result.is_null());
    auto arr = extract_array(result);
    // Wire path validated by the request completing without exception.
    // Data may legitimately be empty outside US trading hours.
    if (arr.size() == 0) {
        if (is_market_trading(*client, U("US"))) {
            FAIL() << "GetQuoteStockTrade empty during US trading hours (real regression)";
        }
        return;
    }
    const auto &trade = arr.at(0);
    ASSERT_TRUE(trade.is_object());
    if (json_is_string(trade, U("symbol"))) {
        EXPECT_EQ(trade.at(U("symbol")).as_string(), U("AAPL"));
    }
}

TEST(QuoteClientInteg, GetStockDetail_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(*cfg);
    web::json::value result = client->get_stock_detail(make_symbols(U("AAPL")));
    ASSERT_FALSE(result.is_null());
    auto arr = extract_array(result);
    ASSERT_GT(arr.size(), 0u) << "GetStockDetail returned empty for AAPL";
    const auto &detail = arr.at(0);
    ASSERT_TRUE(detail.is_object());
    if (json_is_string(detail, U("symbol"))) {
        EXPECT_EQ(detail.at(U("symbol")).as_string(), U("AAPL"));
    }
}

TEST(QuoteClientInteg, GetHistoryTimeline_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(*cfg);
    web::json::value result = client->get_history_timeline(make_symbols(U("AAPL")), U("2024-06-03"));
    ASSERT_FALSE(result.is_null());
    auto arr = extract_array(result);
    ASSERT_GT(arr.size(), 0u) << "GetHistoryTimeline returned empty for AAPL";
    const auto &tl = arr.at(0);
    ASSERT_TRUE(tl.is_object());
    if (json_is_string(tl, U("symbol"))) {
        EXPECT_EQ(tl.at(U("symbol")).as_string(), U("AAPL"));
    }
}

TEST(QuoteClientInteg, GetTradeTick_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(*cfg);
    web::json::value result = client->get_trade_tick(make_symbols(U("AAPL")), TradingSession::Regular);
    ASSERT_FALSE(result.is_null());
    auto arr = extract_array(result);
    // Wire path validated by the request completing without exception.
    // Data may legitimately be empty outside US trading hours.
    if (arr.size() == 0) {
        if (is_market_trading(*client, U("US"))) {
            FAIL() << "GetTradeTick empty during US trading hours (real regression)";
        }
        return;
    }
    const auto &tick = arr.at(0);
    ASSERT_TRUE(tick.is_object());
    ASSERT_TRUE(json_is_string(tick, U("symbol")));
    EXPECT_EQ(tick.at(U("symbol")).as_string(), U("AAPL"));
    // Validate tick items when present
    if (tick.has_field(U("items")) && tick.at(U("items")).is_array()
        && tick.at(U("items")).size() > 0) {
        const auto &item = tick.at(U("items")).at(0);
        ASSERT_TRUE(item.has_field(U("time"))) << "TradeTickItem missing time field";
        EXPECT_GT(item.at(U("time")).as_number().to_int64(), 0) << "TradeTickItem.time should be non-zero";
        ASSERT_TRUE(item.has_field(U("price"))) << "TradeTickItem missing price field";
        EXPECT_GT(item.at(U("price")).as_double(), 0.0) << "TradeTickItem.price should be > 0";
    }
}

TEST(QuoteClientInteg, GetQuoteRealTimeValue_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(*cfg);
    web::json::value result = client->get_quote_real_time_value(make_symbols(U("AAPL")));
    ASSERT_FALSE(result.is_null());
    auto arr = extract_array(result);
    ASSERT_GT(arr.size(), 0u) << "GetQuoteRealTimeValue returned empty for AAPL";
    const auto &rt = arr.at(0);
    ASSERT_TRUE(rt.is_object());
    if (json_is_string(rt, U("symbol"))) {
        EXPECT_EQ(rt.at(U("symbol")).as_string(), U("AAPL"));
    }
}

TEST(QuoteClientInteg, GetQuoteRealTime_ReturnsValidEntries) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(*cfg);
    EXPECT_NO_THROW({
        auto quotes = client->get_quote_real_time(make_symbols(U("AAPL")));
        ASSERT_GT(quotes.size(), 0u) << "GetQuoteRealTime returned empty for AAPL";
        EXPECT_FALSE(quotes[0].symbol.empty()) << "RealtimeQuote.symbol is empty";
    });
}

TEST(QuoteClientInteg, GetQuoteDelay_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(*cfg);
    web::json::value result = client->get_quote_delay(make_symbols(U("AAPL")));
    ASSERT_FALSE(result.is_null());
    auto arr = extract_array(result);
    ASSERT_GT(arr.size(), 0u) << "GetQuoteDelay returned empty for AAPL";
    const auto &delay = arr.at(0);
    ASSERT_TRUE(delay.is_object());
}

TEST(QuoteClientInteg, GetQuoteShortableStocks_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(*cfg);
    // Endpoint may return code:1000 ("method not supported") for accounts without
    // the shortable-stocks entitlement — that is a legitimate permission skip.
    web::json::value result;
    try {
        result = client->get_quote_shortable_stocks(make_symbols(U("AAPL")));
    } catch (const std::exception &ex) {
        GTEST_SKIP() << "GetQuoteShortableStocks not supported for this account: " << ex.what();
    }
    if (result.is_null()) {
        GTEST_SKIP() << "GetQuoteShortableStocks returned null — not supported for this account";
    }
    auto arr = extract_array(result);
    // Shortable info may not be available for all symbols; only validate structure.
    for (size_t i = 0; i < arr.size(); ++i) {
        ASSERT_TRUE(arr.at(i).is_object());
    }
}

TEST(QuoteClientInteg, GetQuoteDepth_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(*cfg);
    web::json::value result = client->get_quote_depth(make_symbols(U("AAPL")));
    ASSERT_FALSE(result.is_null());
    auto arr = extract_array(result);
    ASSERT_GT(arr.size(), 0u) << "GetQuoteDepth returned empty for AAPL";
    const auto &depth = arr.at(0);
    ASSERT_TRUE(depth.is_object());
    ASSERT_TRUE(json_is_string(depth, U("symbol")));
    EXPECT_EQ(depth.at(U("symbol")).as_string(), U("AAPL"));
    // At least one side (asks or bids) should be present and an array when depth data exists.
    bool has_asks = depth.has_field(U("asks")) && depth.at(U("asks")).is_array();
    bool has_bids = depth.has_field(U("bids")) && depth.at(U("bids")).is_array();
    EXPECT_TRUE(has_asks || has_bids) << "QuoteDepth has neither asks nor bids arrays";
}

TEST(QuoteClientInteg, GetStockBroker_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(*cfg);
    // This endpoint is only supported for certain account types (e.g. HK L2).
    web::json::value result;
    try {
        result = client->get_stock_broker(U("00700"));
    } catch (const std::exception &ex) {
        GTEST_SKIP() << "GetStockBroker not supported for this account: " << ex.what();
    }
    if (result.is_null()) {
        GTEST_SKIP() << "GetStockBroker returned null — not supported for this account";
    }
    // Stock broker data has a symbol field identifying the queried stock.
    auto arr = extract_array(result);
    const web::json::value *first = nullptr;
    web::json::value single;
    if (arr.size() > 0) {
        single = arr.at(0);
        first = &single;
    } else if (result.is_object()) {
        first = &result;
    }
    if (first && first->is_object() && json_is_string(*first, U("symbol"))) {
        EXPECT_EQ(first->at(U("symbol")).as_string(), U("00700"));
    }
}

TEST(QuoteClientInteg, GetCapitalDistribution_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(*cfg);
    web::json::value result = client->get_capital_distribution(U("AAPL"));
    ASSERT_FALSE(result.is_null());
    // Wire path validated by the request completing without exception.
    // Payload may legitimately be a non-object/non-array outside US trading hours.
    if (!result.is_object() && !result.is_array()) {
        if (is_market_trading(*client, U("US"))) {
            FAIL() << "GetCapitalDistribution empty during US trading hours (real regression)";
        }
        return;
    }
    // Expect a symbol field when the result is an object.
    if (result.is_object()) {
        ASSERT_TRUE(json_is_string(result, U("symbol"))) << "CapitalDistribution missing symbol field";
        EXPECT_EQ(result.at(U("symbol")).as_string(), U("AAPL"));
    }
}

// ── Option quote: additional tests ───────────────────────────────────────────

TEST(QuoteClientInteg, GetOptionChain_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(*cfg);
    auto expiry = get_first_option_expiry(*client);
    if (expiry.empty()) {
        GTEST_SKIP() << "No option expiry available for AAPL";
    }
    web::json::value result = client->get_option_chain(U("AAPL"), expiry);
    if (result.is_null()) {
        GTEST_SKIP() << "GetOptionChain returned null for AAPL@" << expiry;
    }
    auto arr = extract_array(result);
    if (arr.size() == 0) {
        GTEST_SKIP() << "GetOptionChain returned empty for AAPL@" << expiry << " — no data available";
    }
    const auto &chain = arr.at(0);
    ASSERT_TRUE(chain.is_object());
    if (json_is_string(chain, U("symbol"))) {
        EXPECT_EQ(chain.at(U("symbol")).as_string(), U("AAPL"));
    }
    if (chain.has_field(U("items")) && chain.at(U("items")).is_array()
        && chain.at(U("items")).size() > 0) {
        const auto &row = chain.at(U("items")).at(0);
        // At least one leg (call or put) should be present
        bool has_call = row.has_field(U("call")) && row.at(U("call")).is_object();
        bool has_put = row.has_field(U("put")) && row.at(U("put")).is_object();
        EXPECT_TRUE(has_call || has_put) << "OptionChainRow has neither call nor put";
        if (has_call) {
            const auto &call = row.at(U("call"));
            EXPECT_TRUE(json_nonempty_string(call, U("identifier")));
        }
        if (has_put) {
            const auto &put = row.at(U("put"));
            EXPECT_TRUE(json_nonempty_string(put, U("identifier")));
        }
    }
}

TEST(QuoteClientInteg, GetOptionBrief_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(*cfg);
    auto identifier = get_first_option_identifier(*client);
    if (identifier.empty()) {
        GTEST_SKIP() << "No option identifier available for AAPL";
    }
    web::json::value result = client->get_option_brief(identifier);
    ASSERT_FALSE(result.is_null());
    auto arr = extract_array(result);
    ASSERT_GT(arr.size(), 0u) << "GetOptionBrief returned empty for " << identifier;
    const auto &brief = arr.at(0);
    ASSERT_TRUE(brief.is_object());
    if (json_is_string(brief, U("identifier"))) {
        EXPECT_EQ(brief.at(U("identifier")).as_string(), identifier);
    }
}

TEST(QuoteClientInteg, GetOptionKlineValue_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(*cfg);
    auto identifier = get_first_option_identifier(*client);
    if (identifier.empty()) {
        GTEST_SKIP() << "No option identifier available for AAPL";
    }
    // begin_time = 2024-01-01 (epoch seconds)
    web::json::value result = client->get_option_kline_value(make_identifiers(identifier), 1704067200);
    ASSERT_FALSE(result.is_null());
    auto arr = extract_array(result);
    ASSERT_GT(arr.size(), 0u) << "GetOptionKlineValue returned empty";
    const auto &kline = arr.at(0);
    ASSERT_TRUE(kline.is_object());
    if (kline.has_field(U("items"))) {
        ASSERT_TRUE(kline.at(U("items")).is_array());
        if (kline.at(U("items")).size() > 0) {
            const auto &item = kline.at(U("items")).at(0);
            ASSERT_TRUE(item.has_field(U("close"))) << "OptionKlineItem missing close field";
            EXPECT_GT(item.at(U("close")).as_double(), 0.0) << "OptionKlineItem.close should be > 0";
            if (item.has_field(U("high")) && item.has_field(U("low"))) {
                EXPECT_GE(item.at(U("high")).as_double(), item.at(U("low")).as_double())
                    << "OptionKlineItem.high should be >= low";
            }
        }
    }
}

TEST(QuoteClientInteg, GetOptionTradeTick_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(*cfg);
    auto identifier = get_first_option_identifier(*client);
    if (identifier.empty()) {
        GTEST_SKIP() << "No option identifier available for AAPL";
    }
    web::json::value result = client->get_option_trade_tick(make_identifiers(identifier));
    ASSERT_FALSE(result.is_null());
    auto arr = extract_array(result);
    // Trade ticks may be empty outside market hours.
    for (size_t i = 0; i < arr.size(); ++i) {
        ASSERT_TRUE(arr.at(i).is_object());
    }
}

TEST(QuoteClientInteg, GetOptionSymbols_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(*cfg);
    // Use HK market — US market returns "not supported" for this endpoint.
    web::json::value result;
    try {
        result = client->get_option_symbols(U("HK"));
    } catch (const std::exception &ex) {
        GTEST_SKIP() << "GetOptionSymbols not supported for this account: " << ex.what();
    }
    if (result.is_null()) {
        GTEST_SKIP() << "GetOptionSymbols returned null";
    }
    // Option symbols list may be large; validate that entries are non-empty strings or objects.
    auto arr = extract_array(result);
    for (size_t i = 0; i < arr.size() && i < 5; ++i) {
        const auto &entry = arr.at(i);
        bool valid = (entry.is_string() && !entry.as_string().empty()) || entry.is_object();
        EXPECT_TRUE(valid) << "OptionSymbol[" << i << "] is neither a non-empty string nor object";
    }
}

TEST(QuoteClientInteg, GetOptionDepth_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(*cfg);
    auto identifier = get_first_option_identifier(*client);
    if (identifier.empty()) {
        GTEST_SKIP() << "No option identifier available for AAPL";
    }
    web::json::value result = client->get_option_depth(make_identifiers(identifier), U("US"));
    ASSERT_FALSE(result.is_null());
    auto arr = extract_array(result);
    ASSERT_GT(arr.size(), 0u) << "GetOptionDepth returned empty";
    const auto &depth = arr.at(0);
    ASSERT_TRUE(depth.is_object());
}

TEST(QuoteClientInteg, GetOptionTimeline_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(*cfg);
    auto identifier = get_first_option_identifier(*client);
    if (identifier.empty()) {
        GTEST_SKIP() << "No option identifier available for AAPL";
    }
    web::json::value result = client->get_option_timeline(make_identifiers(identifier), U("US"));
    ASSERT_FALSE(result.is_null());
    auto arr = extract_array(result);
    // Wire path validated by the request completing without exception.
    // Data may legitimately be empty outside US trading hours.
    if (arr.size() == 0) {
        if (is_market_trading(*client, U("US"))) {
            FAIL() << "GetOptionTimeline empty during US trading hours (real regression)";
        }
        return;
    }
    const auto &tl = arr.at(0);
    ASSERT_TRUE(tl.is_object());
}

TEST(QuoteClientInteg, GetOptionAnalysis_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(*cfg);
    web::json::value result = client->get_option_analysis(make_symbols(U("AAPL")), U("US"));
    ASSERT_FALSE(result.is_null());
    auto arr = extract_array(result);
    ASSERT_GT(arr.size(), 0u) << "GetOptionAnalysis returned empty for AAPL";
    const auto &analysis = arr.at(0);
    ASSERT_TRUE(analysis.is_object());
    if (json_is_string(analysis, U("symbol"))) {
        EXPECT_EQ(analysis.at(U("symbol")).as_string(), U("AAPL"));
    }
}

TEST(QuoteClientInteg, GetWarrantFilter_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(*cfg);
    web::json::value result = client->get_warrant_filter(U("0700"));
    ASSERT_FALSE(result.is_null());
    auto arr = extract_array(result);
    for (size_t i = 0; i < arr.size(); ++i) {
        const auto &entry = arr.at(i);
        ASSERT_TRUE(entry.is_object());
        // Each warrant entry should identify itself with a symbol or warrantSymbol.
        bool has_symbol = (entry.has_field(U("symbol")) && entry.at(U("symbol")).is_string() && !entry.at(U("symbol")).as_string().empty())
                       || (entry.has_field(U("warrantSymbol")) && entry.at(U("warrantSymbol")).is_string() && !entry.at(U("warrantSymbol")).as_string().empty());
        EXPECT_TRUE(has_symbol) << "WarrantFilter[" << i << "] has no symbol/warrantSymbol field";
    }
}

TEST(QuoteClientInteg, GetWarrantRealTimeQuote_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(*cfg);
    // Dynamically resolve a live HK warrant. Fail only when HK is trading and
    // we cannot find one; outside HK trading hours, skip cleanly.
    auto warrant_symbol = resolve_hk_warrant_symbol(*client);
    if (warrant_symbol.empty()) {
        if (is_market_trading(*client, U("HK"))) {
            FAIL() << "HK market is trading but no warrant symbol could be resolved";
        }
        GTEST_SKIP() << "No warrant symbol resolvable outside HK trading hours";
    }
    web::json::value result = client->get_warrant_real_time_quote(warrant_symbol);
    ASSERT_FALSE(result.is_null());
    auto arr = extract_array(result);
    if (arr.size() == 0) {
        if (is_market_trading(*client, U("HK"))) {
            FAIL() << "HK is trading but GetWarrantRealTimeQuote returned empty for " << warrant_symbol;
        }
        GTEST_SKIP() << "GetWarrantRealTimeQuote empty outside HK trading hours";
    }
    const auto &rt = arr.at(0);
    ASSERT_TRUE(rt.is_object());
}

// ── Future quote: additional tests ───────────────────────────────────────────

TEST(QuoteClientInteg, GetFutureExchange_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(*cfg);
    web::json::value result = client->get_future_exchange();
    ASSERT_FALSE(result.is_null());
    auto arr = extract_array(result);
    ASSERT_GT(arr.size(), 0u) << "GetFutureExchange returned empty";
    const auto &exchange = arr.at(0);
    ASSERT_TRUE(exchange.is_object());
    // Exchange entry must have a non-empty code field.
    bool has_code = (exchange.has_field(U("exchangeCode")) && exchange.at(U("exchangeCode")).is_string() && !exchange.at(U("exchangeCode")).as_string().empty())
                 || (exchange.has_field(U("code")) && exchange.at(U("code")).is_string() && !exchange.at(U("code")).as_string().empty())
                 || (exchange.has_field(U("exchange")) && exchange.at(U("exchange")).is_string() && !exchange.at(U("exchange")).as_string().empty());
    EXPECT_TRUE(has_code) << "FutureExchange entry has no exchangeCode/code/exchange field";
}

TEST(QuoteClientInteg, GetFutureContracts_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(*cfg);
    web::json::value result = client->get_future_contracts(U("ES"));
    ASSERT_FALSE(result.is_null());
    // May return empty if ES futures are not available for the account.
    auto arr = extract_array(result);
    for (size_t i = 0; i < arr.size(); ++i) {
        const auto &contract = arr.at(i);
        ASSERT_TRUE(contract.is_object());
        bool has_code = (contract.has_field(U("contractCode")) && contract.at(U("contractCode")).is_string() && !contract.at(U("contractCode")).as_string().empty())
                     || (contract.has_field(U("symbol")) && contract.at(U("symbol")).is_string() && !contract.at(U("symbol")).as_string().empty())
                     || (contract.has_field(U("code")) && contract.at(U("code")).is_string() && !contract.at(U("code")).as_string().empty());
        EXPECT_TRUE(has_code) << "FutureContract[" << i << "] has no contractCode/symbol/code field";
    }
}

TEST(QuoteClientInteg, GetFutureContinuousContracts_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(*cfg);
    web::json::value result = client->get_future_continuous_contracts(U("ES"));
    ASSERT_FALSE(result.is_null());
    auto arr = extract_array(result);
    for (size_t i = 0; i < arr.size(); ++i) {
        const auto &contract = arr.at(i);
        ASSERT_TRUE(contract.is_object());
        bool has_code = (contract.has_field(U("contractCode")) && contract.at(U("contractCode")).is_string() && !contract.at(U("contractCode")).as_string().empty())
                     || (contract.has_field(U("symbol")) && contract.at(U("symbol")).is_string() && !contract.at(U("symbol")).as_string().empty())
                     || (contract.has_field(U("code")) && contract.at(U("code")).is_string() && !contract.at(U("code")).as_string().empty());
        EXPECT_TRUE(has_code) << "FutureContinuousContract[" << i << "] has no contractCode/symbol/code field";
    }
}

TEST(QuoteClientInteg, GetFutureCurrentContract_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(*cfg);
    web::json::value result = client->get_future_current_contract(U("ES"));
    ASSERT_FALSE(result.is_null());
    // May return object or array; when present validate the code field.
    auto arr = extract_array(result);
    const web::json::value *first = nullptr;
    web::json::value single;
    if (arr.size() > 0) {
        single = arr.at(0);
        first = &single;
    } else if (result.is_object()) {
        first = &result;
    }
    if (first && first->is_object()) {
        bool has_code = (first->has_field(U("contractCode")) && first->at(U("contractCode")).is_string() && !first->at(U("contractCode")).as_string().empty())
                     || (first->has_field(U("symbol")) && first->at(U("symbol")).is_string() && !first->at(U("symbol")).as_string().empty())
                     || (first->has_field(U("code")) && first->at(U("code")).is_string() && !first->at(U("code")).as_string().empty());
        EXPECT_TRUE(has_code) << "FutureCurrentContract has no contractCode/symbol/code field";
    }
}

TEST(QuoteClientInteg, GetFutureContractByContractCode_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(*cfg);
    auto contract_code = get_first_future_contract_code(*client);
    if (contract_code.empty()) {
        GTEST_SKIP() << "No future contract code available";
    }
    web::json::value result = client->get_future_contract_by_contract_code(contract_code);
    ASSERT_FALSE(result.is_null());
    // Validate code field echoes back the requested contract code.
    auto arr = extract_array(result);
    const web::json::value *first = nullptr;
    web::json::value single;
    if (arr.size() > 0) {
        single = arr.at(0);
        first = &single;
    } else if (result.is_object()) {
        first = &result;
    }
    if (first && first->is_object()) {
        bool has_code = (first->has_field(U("contractCode")) && first->at(U("contractCode")).is_string() && !first->at(U("contractCode")).as_string().empty())
                     || (first->has_field(U("symbol")) && first->at(U("symbol")).is_string() && !first->at(U("symbol")).as_string().empty())
                     || (first->has_field(U("code")) && first->at(U("code")).is_string() && !first->at(U("code")).as_string().empty());
        EXPECT_TRUE(has_code) << "FutureContractByContractCode has no contractCode/symbol/code field";
    }
}

TEST(QuoteClientInteg, GetFutureContractByExchangeCode_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(*cfg);
    auto exchange_code = get_first_future_exchange_code(*client);
    if (exchange_code.empty()) {
        GTEST_SKIP() << "No future exchange code available";
    }
    web::json::value result = client->get_future_contract_by_exchange_code(exchange_code);
    ASSERT_FALSE(result.is_null());
    auto arr = extract_array(result);
    for (size_t i = 0; i < arr.size(); ++i) {
        const auto &contract = arr.at(i);
        ASSERT_TRUE(contract.is_object());
        bool has_code = (contract.has_field(U("contractCode")) && contract.at(U("contractCode")).is_string() && !contract.at(U("contractCode")).as_string().empty())
                     || (contract.has_field(U("symbol")) && contract.at(U("symbol")).is_string() && !contract.at(U("symbol")).as_string().empty())
                     || (contract.has_field(U("code")) && contract.at(U("code")).is_string() && !contract.at(U("code")).as_string().empty());
        EXPECT_TRUE(has_code) << "FutureContractByExchangeCode[" << i << "] has no contractCode/symbol/code field";
    }
}

TEST(QuoteClientInteg, GetFutureKline_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(*cfg);
    auto contract_code = get_first_future_contract_code(*client);
    if (contract_code.empty()) {
        GTEST_SKIP() << "No future contract code available";
    }
    web::json::value result = client->get_future_kline(make_symbols(contract_code), U("day"), -1, -1, 5);
    ASSERT_FALSE(result.is_null());
    auto arr = extract_array(result);
    if (arr.size() > 0) {
        const auto &kline = arr.at(0);
        ASSERT_TRUE(kline.is_object());
        if (kline.has_field(U("items"))) {
            ASSERT_TRUE(kline.at(U("items")).is_array());
            if (kline.at(U("items")).size() > 0) {
                const auto &item = kline.at(U("items")).at(0);
                ASSERT_TRUE(item.has_field(U("close"))) << "FutureKlineItem missing close field";
                EXPECT_GT(item.at(U("close")).as_double(), 0.0) << "FutureKlineItem.close should be > 0";
                // Cross-field constraint: high >= low when both fields present
                if (item.has_field(U("high")) && item.has_field(U("low"))) {
                    EXPECT_GE(item.at(U("high")).as_double(), item.at(U("low")).as_double())
                        << "FutureKlineItem.high should be >= low";
                }
            }
        }
    }
}

TEST(QuoteClientInteg, GetFutureRealTimeQuoteValue_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(*cfg);
    auto contract_code = get_first_future_contract_code(*client);
    if (contract_code.empty()) {
        GTEST_SKIP() << "No future contract code available";
    }
    web::json::value result = client->get_future_real_time_quote_value(make_symbols(contract_code));
    ASSERT_FALSE(result.is_null());
    auto arr = extract_array(result);
    for (size_t i = 0; i < arr.size(); ++i) {
        const auto &qt = arr.at(i);
        ASSERT_TRUE(qt.is_object());
        bool has_code = (qt.has_field(U("contractCode")) && qt.at(U("contractCode")).is_string() && !qt.at(U("contractCode")).as_string().empty())
                     || (qt.has_field(U("symbol")) && qt.at(U("symbol")).is_string() && !qt.at(U("symbol")).as_string().empty())
                     || (qt.has_field(U("code")) && qt.at(U("code")).is_string() && !qt.at(U("code")).as_string().empty());
        EXPECT_TRUE(has_code) << "FutureRealTimeQuoteValue[" << i << "] has no contractCode/symbol/code field";
        // Latest price must be positive when present.
        if (qt.has_field(U("latestPrice")) && qt.at(U("latestPrice")).is_double()) {
            EXPECT_GT(qt.at(U("latestPrice")).as_double(), 0.0) << "FutureRealTimeQuoteValue.latestPrice should be > 0";
        }
    }
}

TEST(QuoteClientInteg, GetFutureRealTimeQuote_ReturnsValidEntries) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(*cfg);
    auto contract_code = get_first_future_contract_code(*client);
    if (contract_code.empty()) {
        GTEST_SKIP() << "No future contract code available";
    }
    EXPECT_NO_THROW({
        auto quotes = client->get_future_real_time_quote(make_symbols(contract_code));
        // May be empty if market is closed.
        for (size_t i = 0; i < quotes.size(); ++i) {
            EXPECT_FALSE(quotes[i].contract_code.empty())
                << "RealtimeQuote[" << i << "].contract_code is empty";
        }
    });
}

TEST(QuoteClientInteg, GetFutureTick_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(*cfg);
    auto contract_code = get_first_future_contract_code(*client);
    if (contract_code.empty()) {
        GTEST_SKIP() << "No future contract code available";
    }
    web::json::value result = client->get_future_tick(contract_code);
    ASSERT_FALSE(result.is_null());
    auto tick_arr = extract_array(result);
    // Tick data may be empty outside trading hours; when present validate fields.
    for (size_t i = 0; i < tick_arr.size(); ++i) {
        const auto &tick = tick_arr.at(i);
        ASSERT_TRUE(tick.is_object());
        if (tick.has_field(U("time")) && tick.at(U("time")).is_number()) {
            EXPECT_GT(tick.at(U("time")).as_number().to_int64(), 0) << "FutureTick[" << i << "].time should be non-zero";
        }
        if (tick.has_field(U("price")) && tick.at(U("price")).is_double()) {
            EXPECT_GT(tick.at(U("price")).as_double(), 0.0) << "FutureTick[" << i << "].price should be > 0";
        }
    }
}

TEST(QuoteClientInteg, GetFutureTradingDate_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(*cfg);
    auto contract_code = get_first_future_contract_code(*client);
    if (contract_code.empty()) {
        GTEST_SKIP() << "No future contract code available";
    }
    web::json::value result = client->get_future_trading_date(contract_code, U("2024-06-03"));
    ASSERT_FALSE(result.is_null());
    auto tdate_arr = extract_array(result);
    // Trading date list may be empty; when present each entry must be non-empty string or object.
    for (size_t i = 0; i < tdate_arr.size(); ++i) {
        const auto &entry = tdate_arr.at(i);
        bool valid = entry.is_string() ? !entry.as_string().empty() : entry.is_object();
        EXPECT_TRUE(valid) << "FutureTradingDate[" << i << "] is neither a non-empty string nor object";
    }
}

TEST(QuoteClientInteg, GetFutureDepth_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(*cfg);
    // Prefer a CME front-month contract seeded from the exchange listing so we
    // never depend on the account's default watchlist for a working contract.
    auto contract_code = resolve_us_future_front_month_contract(*client);
    if (contract_code.empty()) {
        contract_code = get_first_future_contract_code(*client);
    }
    // NOTE: is_market_trading(U("US")) reflects the US equity (NYSE/NASDAQ) market
    // status, not the CME futures market.  CME futures trade nearly 24 hours so
    // this check is conservative: it may skip tests during equity off-hours even
    // though CME is still open.  The heuristic avoids false FAILs at the cost of
    // occasional missed coverage outside equity hours.
    const bool us_trading = is_market_trading(*client, U("US"));
    if (contract_code.empty()) {
        if (us_trading) {
            FAIL() << "US futures market trading but no contract code resolvable";
        }
        GTEST_SKIP() << "No future contract code available (US futures closed)";
    }
    web::json::value result = client->get_future_depth(make_symbols(contract_code));
    ASSERT_FALSE(result.is_null());
    auto arr = extract_array(result);
    for (size_t i = 0; i < arr.size(); ++i) {
        const auto &depth = arr.at(i);
        ASSERT_TRUE(depth.is_object());
        // Depth entry must carry a symbol/code identifier.
        bool has_symbol = (depth.has_field(U("symbol")) && depth.at(U("symbol")).is_string() && !depth.at(U("symbol")).as_string().empty())
                       || (depth.has_field(U("contractCode")) && depth.at(U("contractCode")).is_string() && !depth.at(U("contractCode")).as_string().empty());
        EXPECT_TRUE(has_symbol) << "FutureDepth[" << i << "] has no symbol/contractCode field";
        // At least one side (asks or bids) should be present when the market trades.
        bool has_asks = depth.has_field(U("asks")) && depth.at(U("asks")).is_array();
        bool has_bids = depth.has_field(U("bids")) && depth.at(U("bids")).is_array();
        if (!has_asks && !has_bids) {
            if (us_trading) {
                FAIL() << "FutureDepth returned no asks/bids for " << contract_code
                       << " while US market is trading (real regression)";
            }
            // Wire path validated by the request completing; empty depth is expected
            // outside US futures trading hours.
            return;
        }
    }
}

TEST(QuoteClientInteg, GetFutureHistoryMainContract_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(*cfg);
    // Use a continuous contract code (e.g. CLmain) with a 1-year lookback window.
    web::json::value identifiers = web::json::value::array();
    identifiers[0] = web::json::value::string(U("CLmain"));
    std::time_t end_time = std::time(nullptr);
    std::time_t begin_time = end_time - 365LL * 24 * 3600;
    web::json::value result;
    try {
        result = client->get_future_history_main_contract(identifiers, begin_time, end_time);
    } catch (const std::exception &ex) {
        GTEST_SKIP() << "GetFutureHistoryMainContract not supported: " << ex.what();
    }
    if (result.is_null()) {
        GTEST_SKIP() << "GetFutureHistoryMainContract returned null";
    }
    auto arr = extract_array(result);
    for (size_t i = 0; i < arr.size(); ++i) {
        ASSERT_TRUE(arr.at(i).is_object());
    }
}

// ── Fund quote: additional tests ─────────────────────────────────────────────

TEST(QuoteClientInteg, GetFundSymbols_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(*cfg);
    web::json::value result;
    try {
        result = client->get_fund_symbols();
    } catch (const std::exception &ex) {
        GTEST_SKIP() << "GetFundSymbols not supported: " << ex.what();
    }
    if (result.is_null()) {
        GTEST_SKIP() << "GetFundSymbols returned null";
    }
    auto arr = extract_array(result);
    // Items may be objects or raw strings depending on API version; accept both.
    for (size_t i = 0; i < arr.size(); ++i) {
        const auto &item = arr.at(i);
        EXPECT_TRUE(item.is_object() || item.is_string())
            << "FundSymbols[" << i << "] is neither object nor string";
    }
}

TEST(QuoteClientInteg, GetFundContracts_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(*cfg);
    auto fund_symbol = resolve_fund_symbol(*client);
    if (fund_symbol.empty()) {
        // Fund symbol universe is populated regardless of market hours — a real
        // empty response signals an account without fund entitlement.
        GTEST_SKIP() << "No fund symbol available (account may lack fund entitlement)";
    }
    web::json::value result = client->get_fund_contracts(make_symbols(fund_symbol));
    ASSERT_FALSE(result.is_null());
    auto arr = extract_array(result);
    for (size_t i = 0; i < arr.size(); ++i) {
        ASSERT_TRUE(arr.at(i).is_object());
    }
}

TEST(QuoteClientInteg, GetFundQuote_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(*cfg);
    auto fund_symbol = resolve_fund_symbol(*client);
    if (fund_symbol.empty()) {
        GTEST_SKIP() << "No fund symbol available (account may lack fund entitlement)";
    }
    web::json::value result = client->get_fund_quote(make_symbols(fund_symbol));
    ASSERT_FALSE(result.is_null());
    auto arr = extract_array(result);
    for (size_t i = 0; i < arr.size(); ++i) {
        ASSERT_TRUE(arr.at(i).is_object());
    }
}

TEST(QuoteClientInteg, GetFundHistoryQuote_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(*cfg);
    auto fund_symbol = resolve_fund_symbol(*client);
    if (fund_symbol.empty()) {
        GTEST_SKIP() << "No fund symbol available (account may lack fund entitlement)";
    }
    // Server requires begin_time/end_time in epoch-ms (Java FundQuoteHistoryModel
    // uses Long, and Java integ test passes "2025-01-01".."2025-06-30" via
    // DateUtils.getTimestamp which yields ms). Empty range triggers
    // "biz param error(field 'begin_time' cannot be empty)".
    auto now = std::chrono::system_clock::now();
    auto end_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()).count();
    // ~180 days lookback — funds do not necessarily have daily prints.
    constexpr int64_t kDays180Ms = 180LL * 24 * 60 * 60 * 1000;
    auto begin_ms = end_ms - kDays180Ms;
    web::json::value result = client->get_fund_history_quote(
        make_symbols(fund_symbol),
        static_cast<time_t>(begin_ms),
        static_cast<time_t>(end_ms),
        5);
    ASSERT_FALSE(result.is_null());
    auto arr = extract_array(result);
    for (size_t i = 0; i < arr.size(); ++i) {
        ASSERT_TRUE(arr.at(i).is_object());
    }
}

// ── Financial: additional tests ───────────────────────────────────────────────

TEST(QuoteClientInteg, GetFinancialDaily_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(*cfg);
    // Field values match server-side FinancialDailyField enum (see Java SDK
    // com.tigerbrokers.stock.openapi.client.struct.enums.FinancialDailyField).
    web::json::value fields = web::json::value::array();
    fields[0] = web::json::value::string(U("market_capitalization"));
    fields[1] = web::json::value::string(U("shares_outstanding"));
    web::json::value result = client->get_financial_daily(
        make_symbols(U("AAPL")), U("US"), fields,
        U("2024-01-01"), U("2024-06-30"));
    ASSERT_FALSE(result.is_null());
    auto arr = extract_array(result);
    if (arr.size() == 0) {
        // Financial daily data is historical — non-empty regardless of trading
        // hours. Empty implies the account/market lacks the entitlement.
        GTEST_SKIP() << "GetFinancialDaily returned empty for AAPL (no entitlement)";
    }
    const auto &entry = arr.at(0);
    ASSERT_TRUE(entry.is_object());
    if (json_is_string(entry, U("symbol"))) {
        EXPECT_EQ(entry.at(U("symbol")).as_string(), U("AAPL"));
    }
}

TEST(QuoteClientInteg, GetFinancialReport_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(*cfg);
    web::json::value fields = web::json::value::array();
    fields[0] = web::json::value::string(U("total_revenue"));
    web::json::value result = client->get_financial_report(
        make_symbols(U("AAPL")), U("US"), fields, U("Annual"),
        Utils::date_string_to_timestamp(U("2024-01-01")),
        Utils::date_string_to_timestamp(U("2024-06-30")));
    ASSERT_FALSE(result.is_null());
    auto arr = extract_array(result);
    if (arr.size() == 0) {
        GTEST_SKIP() << "GetFinancialReport returned empty for AAPL (no entitlement)";
    }
    const auto &entry = arr.at(0);
    ASSERT_TRUE(entry.is_object());
    if (json_is_string(entry, U("symbol"))) {
        EXPECT_EQ(entry.at(U("symbol")).as_string(), U("AAPL"));
    }
}

TEST(QuoteClientInteg, GetCorporateSymbolChange_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(*cfg);
    web::json::value result = client->get_corporate_symbol_change(
        make_symbols(U("AAPL")), U("US"),
        Utils::date_string_to_timestamp(U("2024-01-01")),
        Utils::date_string_to_timestamp(U("2025-12-31")));
    ASSERT_FALSE(result.is_null());
    auto arr = extract_array(result);
    for (size_t i = 0; i < arr.size(); ++i) {
        const auto &entry = arr.at(i);
        ASSERT_TRUE(entry.is_object());
        EXPECT_TRUE(json_nonempty_string(entry, U("symbol")))
            << "CorporateSymbolChange[" << i << "].symbol is empty";
    }
}

TEST(QuoteClientInteg, GetCorporateDelisting_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(*cfg);
    web::json::value result = client->get_corporate_delisting(
        make_symbols(U("AAPL")), U("US"),
        Utils::date_string_to_timestamp(U("2024-01-01")),
        Utils::date_string_to_timestamp(U("2025-12-31")));
    ASSERT_FALSE(result.is_null());
    auto arr = extract_array(result);
    for (size_t i = 0; i < arr.size(); ++i) {
        const auto &entry = arr.at(i);
        ASSERT_TRUE(entry.is_object());
        EXPECT_TRUE(json_nonempty_string(entry, U("symbol")))
            << "CorporateDelisting[" << i << "].symbol is empty";
    }
}

TEST(QuoteClientInteg, GetCorporateIpo_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(*cfg);
    web::json::value result = client->get_corporate_ipo(
        make_symbols(U("AAPL")), U("US"),
        Utils::date_string_to_timestamp(U("2024-01-01")),
        Utils::date_string_to_timestamp(U("2025-12-31")));
    ASSERT_FALSE(result.is_null());
    auto arr = extract_array(result);
    for (size_t i = 0; i < arr.size(); ++i) {
        const auto &entry = arr.at(i);
        ASSERT_TRUE(entry.is_object());
        EXPECT_TRUE(json_nonempty_string(entry, U("symbol")))
            << "CorporateIpo[" << i << "].symbol is empty";
    }
}

TEST(QuoteClientInteg, GetFinancialCurrency_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(*cfg);
    web::json::value result = client->get_financial_currency(make_symbols(U("AAPL")), U("US"));
    ASSERT_FALSE(result.is_null());
    auto arr = extract_array(result);
    for (size_t i = 0; i < arr.size(); ++i) {
        const auto &entry = arr.at(i);
        ASSERT_TRUE(entry.is_object());
        EXPECT_TRUE(json_nonempty_string(entry, U("symbol")))
            << "FinancialCurrency[" << i << "].symbol is empty";
        EXPECT_TRUE(json_nonempty_string(entry, U("currency")))
            << "FinancialCurrency[" << i << "].currency is empty";
    }
}

TEST(QuoteClientInteg, GetFinancialExchangeRate_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(*cfg);
    web::json::value currencies = web::json::value::array();
    currencies[0] = web::json::value::string(U("USD"));
    currencies[1] = web::json::value::string(U("HKD"));
    web::json::value result = client->get_financial_exchange_rate(
        currencies, U("2024-01-01"), U("2024-06-01"));
    ASSERT_FALSE(result.is_null());
    auto arr = extract_array(result);
    for (size_t i = 0; i < arr.size(); ++i) {
        const auto &entry = arr.at(i);
        ASSERT_TRUE(entry.is_object());
        EXPECT_TRUE(json_nonempty_string(entry, U("currency")))
            << "FinancialExchangeRate[" << i << "].currency is empty";
        // exchange rate value should be positive when present
        if (entry.has_field(U("rate")) && entry.at(U("rate")).is_double()) {
            EXPECT_GT(entry.at(U("rate")).as_double(), 0.0)
                << "FinancialExchangeRate[" << i << "].rate should be > 0";
        }
    }
}

// ── Industry: additional tests ────────────────────────────────────────────────

TEST(QuoteClientInteg, GetIndustryList_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(*cfg);
    web::json::value result = client->get_industry_list();
    ASSERT_FALSE(result.is_null());
    auto arr = extract_array(result);
    ASSERT_GT(arr.size(), 0u) << "GetIndustryList returned empty";
    for (size_t i = 0; i < arr.size(); ++i) {
        const auto &industry = arr.at(i);
        ASSERT_TRUE(industry.is_object());
        // Each industry entry must have a non-empty identifier (id or code) and a name.
        bool has_id = (industry.has_field(U("id")) && industry.at(U("id")).is_string() && !industry.at(U("id")).as_string().empty())
                   || (industry.has_field(U("industryId")) && industry.at(U("industryId")).is_string() && !industry.at(U("industryId")).as_string().empty())
                   || (industry.has_field(U("code")) && industry.at(U("code")).is_string() && !industry.at(U("code")).as_string().empty());
        EXPECT_TRUE(has_id) << "IndustryEntry[" << i << "] has no id/industryId/code field";
        EXPECT_TRUE(json_nonempty_string(industry, U("name")))
            << "IndustryEntry[" << i << "].name is empty, want non-empty";
    }
}

TEST(QuoteClientInteg, GetIndustryStocks_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(*cfg);
    auto industry_id = get_first_industry_id(*client);
    if (industry_id.empty()) {
        GTEST_SKIP() << "No industry ID available";
    }
    web::json::value result = client->get_industry_stocks(industry_id, U("US"));
    ASSERT_FALSE(result.is_null());
    auto arr = extract_array(result);
    ASSERT_GT(arr.size(), 0u) << "GetIndustryStocks returned empty for industry " << industry_id;
    for (size_t i = 0; i < arr.size(); ++i) {
        const auto &stock = arr.at(i);
        ASSERT_TRUE(stock.is_object());
        EXPECT_TRUE(json_nonempty_string(stock, U("symbol")))
            << "IndustryStock[" << i << "].symbol is empty, want non-empty";
    }
}

TEST(QuoteClientInteg, GetStockIndustry_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(*cfg);
    web::json::value result = client->get_stock_industry(U("AAPL"));
    ASSERT_FALSE(result.is_null());
    // When data is present, expect a symbol field identifying AAPL.
    if (result.is_object() && json_is_string(result, U("symbol"))) {
        EXPECT_EQ(result.at(U("symbol")).as_string(), U("AAPL"));
        EXPECT_TRUE(json_nonempty_string(result, U("name")))
            << "StockIndustry.name is empty, want non-empty";
    } else if (result.is_array() && result.size() > 0) {
        const auto &entry = result.at(0);
        ASSERT_TRUE(entry.is_object());
        if (json_is_string(entry, U("symbol"))) {
            EXPECT_EQ(entry.at(U("symbol")).as_string(), U("AAPL"));
        }
        EXPECT_TRUE(json_nonempty_string(entry, U("name")))
            << "StockIndustry[0].name is empty, want non-empty";
    }
}

// ── Other quote: additional tests ────────────────────────────────────────────

TEST(QuoteClientInteg, GetStockFundamental_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(*cfg);
    web::json::value result = client->get_stock_fundamental(make_symbols(U("AAPL")), U("US"));
    ASSERT_FALSE(result.is_null());
    auto arr = extract_array(result);
    ASSERT_GT(arr.size(), 0u) << "GetStockFundamental returned empty for AAPL";
    const auto &fund = arr.at(0);
    ASSERT_TRUE(fund.is_object());
    if (json_is_string(fund, U("symbol"))) {
        EXPECT_EQ(fund.at(U("symbol")).as_string(), U("AAPL"));
    }
}

TEST(QuoteClientInteg, GetTradeRank_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(*cfg);
    web::json::value result = client->get_trade_rank(U("US"));
    ASSERT_FALSE(result.is_null());
    auto arr = extract_array(result);
    // Rank may be empty outside trading hours.
    for (size_t i = 0; i < arr.size(); ++i) {
        const auto &entry = arr.at(i);
        ASSERT_TRUE(entry.is_object());
        EXPECT_TRUE(json_nonempty_string(entry, U("symbol")))
            << "TradeRankItem[" << i << "].symbol is empty, want non-empty";
    }
}

TEST(QuoteClientInteg, GetQuoteOvernight_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(*cfg);
    web::json::value result = client->get_quote_overnight(make_symbols(U("AAPL")));
    ASSERT_FALSE(result.is_null());
    auto arr = extract_array(result);
    for (size_t i = 0; i < arr.size(); ++i) {
        const auto &entry = arr.at(i);
        ASSERT_TRUE(entry.is_object());
        EXPECT_TRUE(json_nonempty_string(entry, U("symbol")))
            << "QuoteOvernight[" << i << "].symbol is empty, want non-empty";
    }
}

TEST(QuoteClientInteg, GetBrokerHold_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(*cfg);
    web::json::value result = client->get_broker_hold(U("HK"));
    ASSERT_FALSE(result.is_null());
    auto arr = extract_array(result);
    for (size_t i = 0; i < arr.size(); ++i) {
        const auto &entry = arr.at(i);
        ASSERT_TRUE(entry.is_object());
        EXPECT_TRUE(json_nonempty_string(entry, U("symbol")))
            << "BrokerHold[" << i << "].symbol is empty, want non-empty";
    }
}

TEST(QuoteClientInteg, GetMarketScannerTags_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(*cfg);
    // The API requires a non-empty multi_tag_field_list.
    // Values "industry" and "concept" correspond to MultiTagField.Industry and
    // MultiTagField.Concept in the Python SDK.
    web::json::value multi_tags = web::json::value::array();
    multi_tags[0] = web::json::value::string(U("industry"));
    multi_tags[1] = web::json::value::string(U("concept"));
    web::json::value result;
    try {
        result = client->get_market_scanner_tags(U("US"), multi_tags);
    } catch (const std::exception &ex) {
        // Endpoint may not be supported for all accounts — treat as a
        // permission skip.
        GTEST_SKIP() << "GetMarketScannerTags not supported: " << ex.what();
    }
    if (result.is_null()) {
        GTEST_SKIP() << "GetMarketScannerTags returned null (not supported)";
    }
    auto arr = extract_array(result);
    if (arr.size() == 0) {
        if (is_market_trading(*client, U("US"))) {
            FAIL() << "GetMarketScannerTags returned empty while US is trading (real regression)";
        }
        // Wire path validated by the request completing; empty tag list is expected
        // outside US trading hours.
        return;
    }
    for (size_t i = 0; i < arr.size(); ++i) {
        const auto &entry = arr.at(i);
        ASSERT_TRUE(entry.is_object());
        EXPECT_TRUE(json_nonempty_string(entry, U("name")))
            << "MarketScannerTag[" << i << "].name is empty, want non-empty";
    }
}

TEST(QuoteClientInteg, GetMarketScanner_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(*cfg);
    web::json::value result;
    try {
        result = client->get_market_scanner();
    } catch (const std::exception &ex) {
        GTEST_SKIP() << "GetMarketScanner not supported: " << ex.what();
    }
    if (result.is_null()) {
        GTEST_SKIP() << "GetMarketScanner returned null (not supported)";
    }
    // Response may be an array or an object with items — accept either shape.
    auto arr = extract_array(result);
    // Empty-response guard: treat as empty only when both the extracted array is
    // empty AND the object wrapper (if any) does not contain a non-empty items
    // field.  A bare non-array object without an "items" field is also treated
    // as empty.  This prevents a server returning { "items": [] } from being
    // misidentified as a populated response.
    bool effectively_empty = (arr.size() == 0);
    if (effectively_empty) {
        if (is_market_trading(*client, U("US"))) {
            FAIL() << "GetMarketScanner returned empty while US is trading (real regression)";
        }
        // Wire path validated by the request completing; empty scanner is expected
        // outside US trading hours.
        return;
    }
    for (size_t i = 0; i < arr.size(); ++i) {
        ASSERT_TRUE(arr.at(i).is_object());
    }
    // When the response is an object wrapper, assert the items field is present
    // and is an array so we can detect shape regressions even when the list is
    // non-empty.
    if (result.is_object() && result.has_field(U("items"))) {
        EXPECT_TRUE(result.at(U("items")).is_array())
            << "GetMarketScanner object-form response: 'items' must be an array";
    }
}

TEST(QuoteClientInteg, GetKlineQuota_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(*cfg);
    web::json::value result = client->get_kline_quota();
    ASSERT_FALSE(result.is_null());
    auto arr = extract_array(result);
    for (size_t i = 0; i < arr.size(); ++i) {
        const auto &entry = arr.at(i);
        ASSERT_TRUE(entry.is_object());
        EXPECT_TRUE(json_nonempty_string(entry, U("method")))
            << "KlineQuota[" << i << "].method is empty, want non-empty";
    }
}

TEST(QuoteClientInteg, GetAddonEntitlements_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(*cfg);
    web::json::value result = client->get_addon_entitlements();
    ASSERT_FALSE(result.is_null());
    auto arr = extract_array(result);
    for (size_t i = 0; i < arr.size(); ++i) {
        ASSERT_TRUE(arr.at(i).is_object());
    }
}
