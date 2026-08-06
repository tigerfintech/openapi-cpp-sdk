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
    ClientConfig cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(cfg);
    web::json::value result = client->get_quote_permission();
    ASSERT_FALSE(result.is_null());
    auto arr = extract_array(result);
    for (size_t i = 0; i < arr.size(); ++i) {
        const auto &entry = arr.at(i);
        ASSERT_TRUE(entry.is_object());
        if (entry.has_field(U("name"))) {
            EXPECT_FALSE(entry.at(U("name")).as_string().empty())
                << "QuotePermission[" << i << "].Name is empty";
        }
    }
}

TEST(QuoteClientInteg, GetAllSymbolNames_ReturnsValidStructure) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    ClientConfig cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(cfg);
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
    ClientConfig cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(cfg);
    web::json::value result = client->get_trading_calendar(U("US"), U("2024-01-01"), U("2024-12-31"));
    ASSERT_FALSE(result.is_null());
    auto arr = extract_array(result);
    ASSERT_GT(arr.size(), 0u) << "GetTradingCalendar returned empty for US 2024";
    const auto &entry = arr.at(0);
    ASSERT_TRUE(entry.is_object());
    if (entry.has_field(U("tradeDate"))) {
        EXPECT_FALSE(entry.at(U("tradeDate")).as_string().empty())
            << "TradingCalendar.tradeDate is empty";
    }
}

TEST(QuoteClientInteg, GetQuoteStockTrade_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    ClientConfig cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(cfg);
    web::json::value result = client->get_quote_stock_trade(make_symbols(U("AAPL")));
    ASSERT_FALSE(result.is_null());
    auto arr = extract_array(result);
    ASSERT_GT(arr.size(), 0u) << "GetQuoteStockTrade returned empty for AAPL";
    const auto &trade = arr.at(0);
    ASSERT_TRUE(trade.is_object());
    if (trade.has_field(U("symbol"))) {
        EXPECT_EQ(trade.at(U("symbol")).as_string(), U("AAPL"));
    }
}

TEST(QuoteClientInteg, GetStockDetail_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    ClientConfig cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(cfg);
    web::json::value result = client->get_stock_detail(make_symbols(U("AAPL")));
    ASSERT_FALSE(result.is_null());
    auto arr = extract_array(result);
    ASSERT_GT(arr.size(), 0u) << "GetStockDetail returned empty for AAPL";
    const auto &detail = arr.at(0);
    ASSERT_TRUE(detail.is_object());
    if (detail.has_field(U("symbol"))) {
        EXPECT_EQ(detail.at(U("symbol")).as_string(), U("AAPL"));
    }
}

TEST(QuoteClientInteg, GetHistoryTimeline_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    ClientConfig cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(cfg);
    web::json::value result = client->get_history_timeline(make_symbols(U("AAPL")), U("2024-06-03"));
    ASSERT_FALSE(result.is_null());
    auto arr = extract_array(result);
    ASSERT_GT(arr.size(), 0u) << "GetHistoryTimeline returned empty for AAPL";
    const auto &tl = arr.at(0);
    ASSERT_TRUE(tl.is_object());
    if (tl.has_field(U("symbol"))) {
        EXPECT_EQ(tl.at(U("symbol")).as_string(), U("AAPL"));
    }
}

TEST(QuoteClientInteg, GetTradeTick_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    ClientConfig cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(cfg);
    web::json::value result = client->get_trade_tick(make_symbols(U("AAPL")), TradingSession::Regular);
    ASSERT_FALSE(result.is_null());
    auto arr = extract_array(result);
    ASSERT_GT(arr.size(), 0u) << "GetTradeTick returned empty for AAPL";
    const auto &tick = arr.at(0);
    ASSERT_TRUE(tick.is_object());
    if (tick.has_field(U("symbol"))) {
        EXPECT_EQ(tick.at(U("symbol")).as_string(), U("AAPL"));
    }
}

TEST(QuoteClientInteg, GetQuoteRealTimeValue_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    ClientConfig cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(cfg);
    web::json::value result = client->get_quote_real_time_value(make_symbols(U("AAPL")));
    ASSERT_FALSE(result.is_null());
    auto arr = extract_array(result);
    ASSERT_GT(arr.size(), 0u) << "GetQuoteRealTimeValue returned empty for AAPL";
    const auto &rt = arr.at(0);
    ASSERT_TRUE(rt.is_object());
    if (rt.has_field(U("symbol"))) {
        EXPECT_EQ(rt.at(U("symbol")).as_string(), U("AAPL"));
    }
}

TEST(QuoteClientInteg, GetQuoteRealTime_ReturnsValidEntries) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    ClientConfig cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(cfg);
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
    ClientConfig cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(cfg);
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
    ClientConfig cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(cfg);
    web::json::value result = client->get_quote_shortable_stocks(make_symbols(U("AAPL")));
    ASSERT_FALSE(result.is_null());
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
    ClientConfig cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(cfg);
    web::json::value result = client->get_quote_depth(make_symbols(U("AAPL")));
    ASSERT_FALSE(result.is_null());
    auto arr = extract_array(result);
    ASSERT_GT(arr.size(), 0u) << "GetQuoteDepth returned empty for AAPL";
    const auto &depth = arr.at(0);
    ASSERT_TRUE(depth.is_object());
    if (depth.has_field(U("symbol"))) {
        EXPECT_EQ(depth.at(U("symbol")).as_string(), U("AAPL"));
    }
}

TEST(QuoteClientInteg, GetStockBroker_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    ClientConfig cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(cfg);
    web::json::value result = client->get_stock_broker(U("AAPL"));
    ASSERT_FALSE(result.is_null());
    // Stock broker may return an object or array depending on data availability.
    EXPECT_TRUE(result.is_array() || result.is_object());
}

TEST(QuoteClientInteg, GetCapitalDistribution_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    ClientConfig cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(cfg);
    web::json::value result = client->get_capital_distribution(U("AAPL"));
    ASSERT_FALSE(result.is_null());
    // Capital distribution may be empty outside market hours.
    EXPECT_TRUE(result.is_array() || result.is_object());
}

// ── Option quote: additional tests ───────────────────────────────────────────

TEST(QuoteClientInteg, GetOptionChain_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    ClientConfig cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(cfg);
    auto expiry = get_first_option_expiry(*client);
    if (expiry.empty()) {
        GTEST_SKIP() << "No option expiry available for AAPL";
    }
    web::json::value result = client->get_option_chain(U("AAPL"), expiry);
    ASSERT_FALSE(result.is_null());
    auto arr = extract_array(result);
    ASSERT_GT(arr.size(), 0u) << "GetOptionChain returned empty for AAPL@" << expiry;
    const auto &chain = arr.at(0);
    ASSERT_TRUE(chain.is_object());
    if (chain.has_field(U("symbol"))) {
        EXPECT_EQ(chain.at(U("symbol")).as_string(), U("AAPL"));
    }
    if (chain.has_field(U("items"))) {
        ASSERT_TRUE(chain.at(U("items")).is_array());
        ASSERT_GT(chain.at(U("items")).size(), 0u) << "OptionChain.items is empty";
        const auto &row = chain.at(U("items")).at(0);
        // At least one leg (call or put) should be present
        bool has_call = row.has_field(U("call")) && row.at(U("call")).is_object();
        bool has_put = row.has_field(U("put")) && row.at(U("put")).is_object();
        EXPECT_TRUE(has_call || has_put) << "OptionChainRow has neither call nor put";
        if (has_call) {
            const auto &call = row.at(U("call"));
            if (call.has_field(U("identifier"))) {
                EXPECT_FALSE(call.at(U("identifier")).as_string().empty());
            }
        }
        if (has_put) {
            const auto &put = row.at(U("put"));
            if (put.has_field(U("identifier"))) {
                EXPECT_FALSE(put.at(U("identifier")).as_string().empty());
            }
        }
    }
}

TEST(QuoteClientInteg, GetOptionBrief_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    ClientConfig cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(cfg);
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
    if (brief.has_field(U("identifier"))) {
        EXPECT_EQ(brief.at(U("identifier")).as_string(), identifier);
    }
}

TEST(QuoteClientInteg, GetOptionKlineValue_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    ClientConfig cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(cfg);
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
            if (item.has_field(U("close"))) {
                EXPECT_GT(item.at(U("close")).as_double(), 0.0);
            }
        }
    }
}

TEST(QuoteClientInteg, GetOptionTradeTick_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    ClientConfig cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(cfg);
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
    ClientConfig cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(cfg);
    web::json::value result = client->get_option_symbols(U("US"));
    ASSERT_FALSE(result.is_null());
    // Option symbols list may be large; just validate structure.
    EXPECT_TRUE(result.is_array() || result.is_object());
}

TEST(QuoteClientInteg, GetOptionDepth_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    ClientConfig cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(cfg);
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
    ClientConfig cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(cfg);
    auto identifier = get_first_option_identifier(*client);
    if (identifier.empty()) {
        GTEST_SKIP() << "No option identifier available for AAPL";
    }
    web::json::value result = client->get_option_timeline(make_identifiers(identifier), U("US"));
    ASSERT_FALSE(result.is_null());
    auto arr = extract_array(result);
    ASSERT_GT(arr.size(), 0u) << "GetOptionTimeline returned empty";
    const auto &tl = arr.at(0);
    ASSERT_TRUE(tl.is_object());
}

TEST(QuoteClientInteg, GetOptionAnalysis_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    ClientConfig cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(cfg);
    web::json::value result = client->get_option_analysis(make_symbols(U("AAPL")), U("US"));
    ASSERT_FALSE(result.is_null());
    auto arr = extract_array(result);
    ASSERT_GT(arr.size(), 0u) << "GetOptionAnalysis returned empty for AAPL";
    const auto &analysis = arr.at(0);
    ASSERT_TRUE(analysis.is_object());
    if (analysis.has_field(U("symbol"))) {
        EXPECT_EQ(analysis.at(U("symbol")).as_string(), U("AAPL"));
    }
}

TEST(QuoteClientInteg, GetWarrantFilter_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    ClientConfig cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(cfg);
    web::json::value result = client->get_warrant_filter(U("0700"));
    ASSERT_FALSE(result.is_null());
    // Warrant filter may return an object with items or an array.
    EXPECT_TRUE(result.is_array() || result.is_object());
}

TEST(QuoteClientInteg, GetWarrantRealTimeQuote_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    ClientConfig cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(cfg);
    auto warrant_symbol = get_first_warrant_symbol(*client, U("0700"));
    if (warrant_symbol.empty()) {
        GTEST_SKIP() << "No warrant symbol available for 0700";
    }
    web::json::value result = client->get_warrant_real_time_quote(warrant_symbol);
    ASSERT_FALSE(result.is_null());
    auto arr = extract_array(result);
    ASSERT_GT(arr.size(), 0u) << "GetWarrantRealTimeQuote returned empty";
    const auto &rt = arr.at(0);
    ASSERT_TRUE(rt.is_object());
}

// ── Future quote: additional tests ───────────────────────────────────────────

TEST(QuoteClientInteg, GetFutureExchange_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    ClientConfig cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(cfg);
    web::json::value result = client->get_future_exchange();
    ASSERT_FALSE(result.is_null());
    auto arr = extract_array(result);
    ASSERT_GT(arr.size(), 0u) << "GetFutureExchange returned empty";
    const auto &exchange = arr.at(0);
    ASSERT_TRUE(exchange.is_object());
}

TEST(QuoteClientInteg, GetFutureContracts_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    ClientConfig cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(cfg);
    web::json::value result = client->get_future_contracts(U("ES"));
    ASSERT_FALSE(result.is_null());
    // May return empty if ES futures are not available for the account.
    auto arr = extract_array(result);
    for (size_t i = 0; i < arr.size(); ++i) {
        ASSERT_TRUE(arr.at(i).is_object());
    }
}

TEST(QuoteClientInteg, GetFutureContinuousContracts_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    ClientConfig cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(cfg);
    web::json::value result = client->get_future_continuous_contracts(U("ES"));
    ASSERT_FALSE(result.is_null());
    auto arr = extract_array(result);
    for (size_t i = 0; i < arr.size(); ++i) {
        ASSERT_TRUE(arr.at(i).is_object());
    }
}

TEST(QuoteClientInteg, GetFutureCurrentContract_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    ClientConfig cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(cfg);
    web::json::value result = client->get_future_current_contract(U("ES"));
    ASSERT_FALSE(result.is_null());
    // May return object or array.
    EXPECT_TRUE(result.is_object() || result.is_array());
}

TEST(QuoteClientInteg, GetFutureContractByContractCode_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    ClientConfig cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(cfg);
    auto contract_code = get_first_future_contract_code(*client);
    if (contract_code.empty()) {
        GTEST_SKIP() << "No future contract code available";
    }
    web::json::value result = client->get_future_contract_by_contract_code(contract_code);
    ASSERT_FALSE(result.is_null());
    EXPECT_TRUE(result.is_object() || result.is_array());
}

TEST(QuoteClientInteg, GetFutureContractByExchangeCode_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    ClientConfig cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(cfg);
    auto exchange_code = get_first_future_exchange_code(*client);
    if (exchange_code.empty()) {
        GTEST_SKIP() << "No future exchange code available";
    }
    web::json::value result = client->get_future_contract_by_exchange_code(exchange_code);
    ASSERT_FALSE(result.is_null());
    auto arr = extract_array(result);
    for (size_t i = 0; i < arr.size(); ++i) {
        ASSERT_TRUE(arr.at(i).is_object());
    }
}

TEST(QuoteClientInteg, GetFutureKline_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    ClientConfig cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(cfg);
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
                if (item.has_field(U("close"))) {
                    EXPECT_GT(item.at(U("close")).as_double(), 0.0);
                }
            }
        }
    }
}

TEST(QuoteClientInteg, GetFutureRealTimeQuoteValue_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    ClientConfig cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(cfg);
    auto contract_code = get_first_future_contract_code(*client);
    if (contract_code.empty()) {
        GTEST_SKIP() << "No future contract code available";
    }
    web::json::value result = client->get_future_real_time_quote_value(make_symbols(contract_code));
    ASSERT_FALSE(result.is_null());
    auto arr = extract_array(result);
    for (size_t i = 0; i < arr.size(); ++i) {
        ASSERT_TRUE(arr.at(i).is_object());
    }
}

TEST(QuoteClientInteg, GetFutureRealTimeQuote_ReturnsValidEntries) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    ClientConfig cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(cfg);
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
    ClientConfig cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(cfg);
    auto contract_code = get_first_future_contract_code(*client);
    if (contract_code.empty()) {
        GTEST_SKIP() << "No future contract code available";
    }
    web::json::value result = client->get_future_tick(contract_code);
    ASSERT_FALSE(result.is_null());
    EXPECT_TRUE(result.is_array() || result.is_object());
}

TEST(QuoteClientInteg, GetFutureTradingDate_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    ClientConfig cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(cfg);
    auto contract_code = get_first_future_contract_code(*client);
    if (contract_code.empty()) {
        GTEST_SKIP() << "No future contract code available";
    }
    web::json::value result = client->get_future_trading_date(contract_code, U("2024-06-03"));
    ASSERT_FALSE(result.is_null());
    EXPECT_TRUE(result.is_array() || result.is_object());
}

TEST(QuoteClientInteg, GetFutureDepth_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    ClientConfig cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(cfg);
    auto contract_code = get_first_future_contract_code(*client);
    if (contract_code.empty()) {
        GTEST_SKIP() << "No future contract code available";
    }
    web::json::value result = client->get_future_depth(make_symbols(contract_code));
    ASSERT_FALSE(result.is_null());
    auto arr = extract_array(result);
    for (size_t i = 0; i < arr.size(); ++i) {
        ASSERT_TRUE(arr.at(i).is_object());
    }
}

TEST(QuoteClientInteg, GetFutureHistoryMainContract_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    ClientConfig cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(cfg);
    auto contract_code = get_first_future_contract_code(*client);
    if (contract_code.empty()) {
        GTEST_SKIP() << "No future contract code available";
    }
    web::json::value result = client->get_future_history_main_contract(make_symbols(contract_code));
    ASSERT_FALSE(result.is_null());
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
    ClientConfig cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(cfg);
    web::json::value result = client->get_fund_symbols();
    ASSERT_FALSE(result.is_null());
    auto arr = extract_array(result);
    for (size_t i = 0; i < arr.size(); ++i) {
        ASSERT_TRUE(arr.at(i).is_object());
    }
}

TEST(QuoteClientInteg, GetFundContracts_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    ClientConfig cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(cfg);
    auto fund_symbol = get_first_fund_symbol(*client);
    if (fund_symbol.empty()) {
        GTEST_SKIP() << "No fund symbol available";
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
    ClientConfig cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(cfg);
    auto fund_symbol = get_first_fund_symbol(*client);
    if (fund_symbol.empty()) {
        GTEST_SKIP() << "No fund symbol available";
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
    ClientConfig cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(cfg);
    auto fund_symbol = get_first_fund_symbol(*client);
    if (fund_symbol.empty()) {
        GTEST_SKIP() << "No fund symbol available";
    }
    web::json::value result = client->get_fund_history_quote(make_symbols(fund_symbol), -1, -1, 5);
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
    // get_financial_daily() is declared in quote_client.h but not yet
    // implemented in quote_client.cpp. Skip until the implementation lands.
    GTEST_SKIP() << "get_financial_daily() not yet implemented in SDK";
}

TEST(QuoteClientInteg, GetFinancialReport_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    // get_financial_report() is declared in quote_client.h but not yet
    // implemented in quote_client.cpp. Skip until the implementation lands.
    GTEST_SKIP() << "get_financial_report() not yet implemented in SDK";
}

TEST(QuoteClientInteg, GetCorporateSymbolChange_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    ClientConfig cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(cfg);
    web::json::value result = client->get_corporate_symbol_change(
        make_symbols(U("AAPL")), U("US"), U("2024-01-01"), U("2025-12-31"));
    ASSERT_FALSE(result.is_null());
    EXPECT_TRUE(result.is_array() || result.is_object());
}

TEST(QuoteClientInteg, GetCorporateDelisting_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    ClientConfig cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(cfg);
    web::json::value result = client->get_corporate_delisting(
        make_symbols(U("AAPL")), U("US"), U("2024-01-01"), U("2025-12-31"));
    ASSERT_FALSE(result.is_null());
    EXPECT_TRUE(result.is_array() || result.is_object());
}

TEST(QuoteClientInteg, GetCorporateIpo_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    ClientConfig cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(cfg);
    web::json::value result = client->get_corporate_ipo(
        make_symbols(U("AAPL")), U("US"), U("2024-01-01"), U("2025-12-31"));
    ASSERT_FALSE(result.is_null());
    EXPECT_TRUE(result.is_array() || result.is_object());
}

TEST(QuoteClientInteg, GetFinancialCurrency_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    ClientConfig cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(cfg);
    web::json::value result = client->get_financial_currency(make_symbols(U("AAPL")), U("US"));
    ASSERT_FALSE(result.is_null());
    EXPECT_TRUE(result.is_array() || result.is_object());
}

TEST(QuoteClientInteg, GetFinancialExchangeRate_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    ClientConfig cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(cfg);
    web::json::value currencies = web::json::value::array();
    currencies[0] = web::json::value::string(U("USD"));
    currencies[1] = web::json::value::string(U("HKD"));
    web::json::value result = client->get_financial_exchange_rate(
        currencies, U("2024-01-01"), U("2024-06-01"));
    ASSERT_FALSE(result.is_null());
    EXPECT_TRUE(result.is_array() || result.is_object());
}

// ── Industry: additional tests ────────────────────────────────────────────────

TEST(QuoteClientInteg, GetIndustryList_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    ClientConfig cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(cfg);
    web::json::value result = client->get_industry_list();
    ASSERT_FALSE(result.is_null());
    auto arr = extract_array(result);
    for (size_t i = 0; i < arr.size(); ++i) {
        ASSERT_TRUE(arr.at(i).is_object());
    }
}

TEST(QuoteClientInteg, GetIndustryStocks_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    ClientConfig cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(cfg);
    auto industry_id = get_first_industry_id(*client);
    if (industry_id.empty()) {
        GTEST_SKIP() << "No industry ID available";
    }
    web::json::value result = client->get_industry_stocks(industry_id, U("US"));
    ASSERT_FALSE(result.is_null());
    auto arr = extract_array(result);
    for (size_t i = 0; i < arr.size(); ++i) {
        ASSERT_TRUE(arr.at(i).is_object());
    }
}

TEST(QuoteClientInteg, GetStockIndustry_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    ClientConfig cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(cfg);
    web::json::value result = client->get_stock_industry(U("AAPL"));
    ASSERT_FALSE(result.is_null());
    EXPECT_TRUE(result.is_array() || result.is_object());
}

// ── Other quote: additional tests ────────────────────────────────────────────

TEST(QuoteClientInteg, GetStockFundamental_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    ClientConfig cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(cfg);
    web::json::value result = client->get_stock_fundamental(make_symbols(U("AAPL")), U("US"));
    ASSERT_FALSE(result.is_null());
    auto arr = extract_array(result);
    ASSERT_GT(arr.size(), 0u) << "GetStockFundamental returned empty for AAPL";
    const auto &fund = arr.at(0);
    ASSERT_TRUE(fund.is_object());
    if (fund.has_field(U("symbol"))) {
        EXPECT_EQ(fund.at(U("symbol")).as_string(), U("AAPL"));
    }
}

TEST(QuoteClientInteg, GetTradeRank_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    ClientConfig cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(cfg);
    web::json::value result = client->get_trade_rank(U("US"));
    ASSERT_FALSE(result.is_null());
    EXPECT_TRUE(result.is_array() || result.is_object());
}

TEST(QuoteClientInteg, GetQuoteOvernight_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    ClientConfig cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(cfg);
    web::json::value result = client->get_quote_overnight(make_symbols(U("AAPL")));
    ASSERT_FALSE(result.is_null());
    auto arr = extract_array(result);
    for (size_t i = 0; i < arr.size(); ++i) {
        ASSERT_TRUE(arr.at(i).is_object());
    }
}

TEST(QuoteClientInteg, GetBrokerHold_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    ClientConfig cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(cfg);
    web::json::value result = client->get_broker_hold(U("HK"));
    ASSERT_FALSE(result.is_null());
    EXPECT_TRUE(result.is_array() || result.is_object());
}

TEST(QuoteClientInteg, GetMarketScannerTags_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    ClientConfig cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(cfg);
    web::json::value result = client->get_market_scanner_tags(U("US"));
    ASSERT_FALSE(result.is_null());
    EXPECT_TRUE(result.is_array() || result.is_object());
}

TEST(QuoteClientInteg, GetMarketScanner_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    // get_market_scanner() is declared in quote_client.h but not yet
    // implemented in quote_client.cpp. Skip until the implementation lands.
    GTEST_SKIP() << "get_market_scanner() not yet implemented in SDK";
}

TEST(QuoteClientInteg, GetKlineQuota_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    ClientConfig cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(cfg);
    web::json::value result = client->get_kline_quota();
    ASSERT_FALSE(result.is_null());
    EXPECT_TRUE(result.is_array() || result.is_object());
}

TEST(QuoteClientInteg, GetAddonEntitlements_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    ClientConfig cfg = make_integ_config();
    auto client = std::make_shared<QuoteClient>(cfg);
    web::json::value result = client->get_addon_entitlements();
    ASSERT_FALSE(result.is_null());
    auto arr = extract_array(result);
    for (size_t i = 0; i < arr.size(); ++i) {
        ASSERT_TRUE(arr.at(i).is_object());
    }
}
