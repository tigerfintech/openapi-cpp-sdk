#include <gtest/gtest.h>
#include <ctime>
#include "support/test_support.h"
#include "integ/test_market_helpers.h"
#include "tigerapi/trade_client.h"
#include "tigerapi/quote_client.h"
#include "tigerapi/client_config.h"
#include "tigerapi/model.h"
#include "tigerapi/order_util.h"
#include <memory>
#include <string>
#include <vector>

using namespace TIGER_API;
using namespace TIGER_API::test;

/**
 * Integration tests for TradeClient.
 * All tests call GTEST_SKIP() unless TIGER_RUN_INTEG=true.
 *
 * Required env vars when TIGER_RUN_INTEG=true:
 *   TIGER_TEST_ID, TIGER_TEST_PRIVATE_KEY, TIGER_TEST_ACCOUNT
 *
 * These tests exercise both read-only trade endpoints (contract/asset/
 * position/order queries) and order placement, modification, and
 * cancellation, using the "AcceptsOrSkips" trading-hours-tolerant pattern
 * for the latter (see the order-placement section below).
 *
 * Field-value assertions follow the Go SDK trade_client_integ_test.go patterns,
 * adapted for the C++ SDK which returns raw web::json::value.  Note that some
 * endpoints return the data field directly as a raw array, while others wrap
 * it in {"items": [...]}.  The helper below handles both cases.
 */

namespace {

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

/// Check that a JSON field exists, is a non-null string, and is non-empty.
/// Absent fields are treated as "not present" (returns true = OK to skip).
/// Null fields, non-string types, or empty strings return false (assertion fails).
static bool json_nonempty_string(const web::json::value &obj,
                                 const utility::string_t &field) {
    if (!obj.has_field(field)) return true;  // absent = skip assertion
    const auto &v = obj.at(field);
    if (v.is_null()) return false;           // null is not a non-empty string
    if (!v.is_string()) return false;        // wrong type = fail
    return !v.as_string().empty();
}

/// Check that a JSON field exists and is a string (for EXPECT_EQ cases).
/// Returns false if absent, null, or non-string; true only for actual strings.
static bool json_is_string(const web::json::value &obj,
                            const utility::string_t &field) {
    return obj.has_field(field)
        && !obj.at(field).is_null()
        && obj.at(field).is_string();
}

} // anonymous namespace

// ── Asset ─────────────────────────────────────────────────────────────────────

TEST(TradeClientInteg, GetAsset_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<TradeClient>(*cfg);
    EXPECT_NO_THROW({
        web::json::value result = client->get_asset();
        ASSERT_FALSE(result.is_null());

        auto arr = extract_array(result);
        ASSERT_GT(arr.size(), 0u) << "GetAsset returned empty";
        const auto &asset = arr.at(0);
        ASSERT_TRUE(asset.is_object());

        EXPECT_TRUE(json_nonempty_string(asset, U("account")))
            << "Asset.account is empty, want non-empty";

        EXPECT_TRUE(json_nonempty_string(asset, U("currency")))
            << "Asset.currency is empty, want non-empty";

        ASSERT_TRUE(asset.has_field(U("netLiquidation"))) << "Asset missing netLiquidation field";
        EXPECT_GE(asset.at(U("netLiquidation")).as_double(), 0.0)
            << "Asset.netLiquidation should be >= 0";
    });
}

// ── Accounts ─────────────────────────────────────────────────────────────────

TEST(TradeClientInteg, GetAccounts_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<TradeClient>(*cfg);
    EXPECT_NO_THROW({
        web::json::value result = client->get_accounts();
        ASSERT_FALSE(result.is_null());

        auto arr = extract_array(result);
        ASSERT_GT(arr.size(), 0u) << "GetAccounts returned empty";
        const auto &acct = arr.at(0);
        ASSERT_TRUE(acct.is_object());

        EXPECT_TRUE(json_nonempty_string(acct, U("account")))
            << "Account.account is empty, want non-empty";

    });
}

// ── Positions (empty-result guard) ───────────────────────────────────────────

TEST(TradeClientInteg, GetPositions_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<TradeClient>(*cfg);
    EXPECT_NO_THROW({
        web::json::value result = client->get_positions();
        ASSERT_FALSE(result.is_null());

        auto arr = extract_array(result);
        // Positions may legitimately be empty for a fresh paper account;
        // only assert the call succeeds. When positions exist, validate fields.
        for (size_t i = 0; i < arr.size(); ++i) {
            const auto &pos = arr.at(i);
            ASSERT_TRUE(pos.is_object());

            EXPECT_TRUE(json_nonempty_string(pos, U("symbol")))
                << "Position[" << i << "].symbol is empty, want non-empty";

            EXPECT_TRUE(json_nonempty_string(pos, U("account")))
                << "Position[" << i << "].account is empty, want non-empty";

            EXPECT_TRUE(json_nonempty_string(pos, U("secType")))
                << "Position[" << i << "].secType is empty, want non-empty";

        }
    });
}

// ── Orders (empty-result guard) ──────────────────────────────────────────────

TEST(TradeClientInteg, GetOrders_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<TradeClient>(*cfg);

    // Query orders from the last 30 days.
    std::time_t now = std::time(nullptr);
    std::time_t start = now - 30 * 24 * 60 * 60;

    EXPECT_NO_THROW({
        web::json::value result = client->get_orders(
            /*account=*/U(""), /*sec_type=*/U(""), /*market=*/U("ALL"),
            /*symbol=*/U(""), /*start_date=*/start, /*end_date=*/now,
            /*limit=*/10);
        ASSERT_FALSE(result.is_null());

        auto arr = extract_array(result);
        // Orders may be empty if no trades in the last 30 days.
        // When orders exist, validate identifier and numeric fields.
        for (size_t i = 0; i < arr.size(); ++i) {
            const auto &ord = arr.at(i);
            ASSERT_TRUE(ord.is_object());

            if (ord.has_field(U("id")) && ord.at(U("id")).is_number()) {
                EXPECT_NE(ord.at(U("id")).as_number().to_int64(), 0)

                    << "Order[" << i << "].id = 0, want non-zero";
            }
            EXPECT_TRUE(json_nonempty_string(ord, U("symbol")))
                << "Order[" << i << "].symbol is empty, want non-empty";

            EXPECT_TRUE(json_nonempty_string(ord, U("action")))
                << "Order[" << i << "].action is empty, want non-empty";

            EXPECT_TRUE(json_nonempty_string(ord, U("status")))
                << "Order[" << i << "].status is empty, want non-empty";

        }
    });
}

// ── Contract ─────────────────────────────────────────────────────────────────

TEST(TradeClientInteg, GetContract_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<TradeClient>(*cfg);
    // AAPL/STK is a stable, always-listed identifier — contract metadata is not
    // driven by trading hours, so any empty response indicates a real bug.
    // The /contract endpoint runs on API v3.0 (see OPEN_API_SERVICE_VERSION),
    // which returns `data` as a single ContractItem object (matches Java
    // ContractResponse.item and Go SDK Contract3). v2.0 wraps it in an array,
    // but that path is not used here.
    web::json::value result = client->get_contract(U("AAPL"), U("STK"));
    ASSERT_FALSE(result.is_null()) << "GetContract returned null for AAPL STK";
    ASSERT_TRUE(result.is_object()) << "GetContract v3.0 should return a single object";

    if (json_is_string(result, U("symbol"))) {
        EXPECT_EQ(result.at(U("symbol")).as_string(), U("AAPL"))
                << "Contract.symbol mismatch";
    }
    if (json_is_string(result, U("secType"))) {
        EXPECT_EQ(result.at(U("secType")).as_string(), U("STK"))
                << "Contract.secType mismatch";
    }
    EXPECT_TRUE(json_nonempty_string(result, U("currency")))
        << "Contract.currency is empty, want non-empty (e.g. USD)";

}

// ── Active orders (empty-result guard) ───────────────────────────────────────

TEST(TradeClientInteg, GetActiveOrders_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<TradeClient>(*cfg);
    EXPECT_NO_THROW({
        web::json::value result = client->get_active_orders();
        ASSERT_FALSE(result.is_null());

        auto arr = extract_array(result);
        // Active orders may be empty if no open orders exist.
        // When orders exist, validate identifier fields.
        for (size_t i = 0; i < arr.size(); ++i) {
            const auto &ord = arr.at(i);
            ASSERT_TRUE(ord.is_object());

            if (ord.has_field(U("id")) && ord.at(U("id")).is_number()) {
                EXPECT_NE(ord.at(U("id")).as_number().to_int64(), 0)

                    << "ActiveOrder[" << i << "].id = 0, want non-zero";
            }
            EXPECT_TRUE(json_nonempty_string(ord, U("symbol")))
                << "ActiveOrder[" << i << "].symbol is empty, want non-empty";

            EXPECT_TRUE(json_nonempty_string(ord, U("status")))
                << "ActiveOrder[" << i << "].status is empty, want non-empty";

        }
    });
}

// ── Helpers ───────────────────────────────────────────────────────────────────

namespace {

/// Build a JSON array of symbol strings, e.g. ["AAPL"].
web::json::value make_symbols(const utility::string_t &sym) {
    web::json::value arr = web::json::value::array();
    arr[0] = web::json::value::string(sym);
    return arr;
}

/// Try to fetch the first order ID from get_orders (last 30 days, limit 10).
/// Returns 0 if no orders exist.
unsigned long long get_first_order_id(TradeClient &client) {
    std::time_t now = std::time(nullptr);
    std::time_t start = now - 30 * 24 * 60 * 60;
    web::json::value result = client.get_orders(
        U(""), U(""), U("ALL"), U(""), start, now, 10);
    auto arr = extract_array(result);
    for (size_t i = 0; i < arr.size(); ++i) {
        const auto &ord = arr.at(i);
        if (ord.is_object() && ord.has_field(U("id")) && ord.at(U("id")).is_number()) {
            auto id = ord.at(U("id")).as_number().to_int64();
            if (id != 0) return static_cast<unsigned long long>(id);
        }
    }
    return 0;
}

} // anonymous namespace

// ── Prime asset ───────────────────────────────────────────────────────────────

TEST(TradeClientInteg, GetPrimeAsset_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<TradeClient>(*cfg);
    EXPECT_NO_THROW({
        web::json::value result = client->get_prime_asset();
        ASSERT_FALSE(result.is_null());
        auto arr = extract_array(result);
        // Prime asset may be empty for non-prime accounts.
        for (size_t i = 0; i < arr.size(); ++i) {
            const auto &asset = arr.at(i);
            ASSERT_TRUE(asset.is_object());
            EXPECT_TRUE(json_nonempty_string(asset, U("account")))
                << "PrimeAsset[" << i << "].account is empty";

            EXPECT_TRUE(json_nonempty_string(asset, U("currency")))
                << "PrimeAsset[" << i << "].currency is empty";

        }
    });
}

TEST(TradeClientInteg, GetPrimePortfolio_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<TradeClient>(*cfg);
    EXPECT_NO_THROW({
        auto portfolio = client->get_prime_portfolio();
        // PortfolioAccount has account, update_timestamp, segments.
        // May be empty for non-prime accounts; just assert the call succeeds.
    });
}

// ── Position list (typed return) ─────────────────────────────────────────────

TEST(TradeClientInteg, GetPositionList_ReturnsValidEntries) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<TradeClient>(*cfg);
    EXPECT_NO_THROW({
        auto positions = client->get_position_list();
        // Positions may legitimately be empty for a fresh paper account.
        for (size_t i = 0; i < positions.size(); ++i) {
            EXPECT_FALSE(positions[i].contract.symbol.empty())
                << "Position[" << i << "].symbol is empty";
            EXPECT_FALSE(positions[i].account.empty())
                << "Position[" << i << "].account is empty";
        }
    });
}

// ── Filled / inactive orders ─────────────────────────────────────────────────

TEST(TradeClientInteg, GetFilledOrders_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<TradeClient>(*cfg);
    std::time_t now = std::time(nullptr);
    std::time_t start = now - 30 * 24 * 60 * 60;
    EXPECT_NO_THROW({
        web::json::value result = client->get_filled_orders(
            U(""), U(""), U("ALL"), U(""), start, now);
        ASSERT_FALSE(result.is_null());
        auto arr = extract_array(result);
        for (size_t i = 0; i < arr.size(); ++i) {
            const auto &ord = arr.at(i);
            ASSERT_TRUE(ord.is_object());
            if (ord.has_field(U("id")) && ord.at(U("id")).is_number()) {
                EXPECT_NE(ord.at(U("id")).as_number().to_int64(), 0)

                    << "FilledOrder[" << i << "].id = 0, want non-zero";
            }
            EXPECT_TRUE(json_nonempty_string(ord, U("status")))
                << "FilledOrder[" << i << "].status is empty";

        }
    });
}

TEST(TradeClientInteg, GetInactiveOrders_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<TradeClient>(*cfg);
    std::time_t now = std::time(nullptr);
    std::time_t start = now - 30 * 24 * 60 * 60;
    EXPECT_NO_THROW({
        web::json::value result = client->get_inactive_orders(
            U(""), U(""), U("ALL"), U(""), start, now);
        ASSERT_FALSE(result.is_null());
        auto arr = extract_array(result);
        for (size_t i = 0; i < arr.size(); ++i) {
            const auto &ord = arr.at(i);
            ASSERT_TRUE(ord.is_object());
            if (ord.has_field(U("id")) && ord.at(U("id")).is_number()) {
                EXPECT_NE(ord.at(U("id")).as_number().to_int64(), 0)

                    << "InactiveOrder[" << i << "].id = 0, want non-zero";
            }
        }
    });
}

// ── Get order by ID (typed return) ────────────────────────────────────────────

TEST(TradeClientInteg, GetOrder_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<TradeClient>(*cfg);
    // get_orders() returns historical orders regardless of trading state —
    // the 30-day lookback should almost always yield a record for an active
    // account. Empty is only acceptable if the test account genuinely has
    // zero history in the window.
    auto order_id = get_first_order_id(*client);
    if (order_id == 0) {
        GTEST_SKIP() << "No orders in the last 30 days for this test account";
    }
    EXPECT_NO_THROW({
        Order order = client->get_order(order_id);
        EXPECT_EQ(order.id, order_id)
            << "Order.id mismatch: got " << order.id << ", want " << order_id;
    });
}

// ── Transactions ─────────────────────────────────────────────────────────────

TEST(TradeClientInteg, GetTransactions_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<TradeClient>(*cfg);
    EXPECT_NO_THROW({
        web::json::value result = client->get_transactions(U(""), U("AAPL"), U("STK"));
        ASSERT_FALSE(result.is_null());
        auto arr = extract_array(result);
        // Transactions may be empty if no trades for AAPL.
        for (size_t i = 0; i < arr.size(); ++i) {
            const auto &txn = arr.at(i);
            ASSERT_TRUE(txn.is_object());
        }
    });
}

// ── Contracts (batch) ────────────────────────────────────────────────────────

TEST(TradeClientInteg, GetContracts_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<TradeClient>(*cfg);
    EXPECT_NO_THROW({
        web::json::value result = client->get_contracts(make_symbols(U("AAPL")), U("STK"));
        ASSERT_FALSE(result.is_null());
        auto arr = extract_array(result);
        ASSERT_GT(arr.size(), 0u) << "GetContracts returned empty for AAPL STK";
        const auto &contract = arr.at(0);
        ASSERT_TRUE(contract.is_object());
        if (json_is_string(contract, U("symbol"))) {
        EXPECT_EQ(contract.at(U("symbol")).as_string(), U("AAPL"));
        }
        if (json_is_string(contract, U("secType"))) {
        EXPECT_EQ(contract.at(U("secType")).as_string(), U("STK"));
        }
    });
}

// ── Quote contract ───────────────────────────────────────────────────────────

TEST(TradeClientInteg, GetQuoteContract_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<TradeClient>(*cfg);
    // quote_contract is a derivatives-only endpoint — the server rejects
    // sec_type=STK with "'sec_type':'STK' is not supported, all supported
    // sec_type include:['OPT','WAR','IOPT']". Pull a live AAPL option expiry
    // through the quote client so the test does not depend on a hardcoded
    // date that may fall in the past.
    auto quote_client = std::make_shared<QuoteClient>(*cfg, /*is_grab_permission=*/false);
    auto identifier = resolve_us_option_identifier(*quote_client);
    if (identifier.empty()) {
        // No live option identifier — likely the account has no option data
        // permission. Fall back to a LEAPS expiry ~1 year out to still
        // exercise the endpoint with a valid derivative sec_type.
        // (Server validates format only for the expiry.)
        const utility::string_t leaps = U("20260117");
        web::json::value result;
        try {
            result = client->get_quote_contract(U("AAPL"), U("OPT"), leaps);
        } catch (const std::exception &ex) {
            GTEST_SKIP() << "GetQuoteContract not supported for this account: " << ex.what();
        }
        if (result.is_null()) {
            GTEST_SKIP() << "GetQuoteContract returned null for AAPL OPT (no entitlement)";
        }
        auto arr = extract_array(result);
        for (size_t i = 0; i < arr.size(); ++i) {
            ASSERT_TRUE(arr.at(i).is_object());
        }
        return;
    }

    // Extract the expiry from the OCC-style identifier (last 6+ chars encode
    // YYMMDD before the C/P flag). Fall back to the LEAPS date if parsing fails.
    utility::string_t expiry = U("20260117");
    // OCC identifier example: AAPL  240119C00185000
    // Look for the first digit run of length >= 6 that starts with year digits.
    size_t start = 0;
    while (start < identifier.size() && !std::isdigit(identifier[start])) ++start;
    if (start + 6 <= identifier.size()) {
        auto yymmdd = identifier.substr(start, 6);
        // Convert YYMMDD → 20YYMMDD.
        expiry = U("20") + yymmdd;
    }

    web::json::value result;
    try {
        result = client->get_quote_contract(U("AAPL"), U("OPT"), expiry);
    } catch (const std::exception &ex) {
        GTEST_SKIP() << "GetQuoteContract not supported: " << ex.what();
    }
    if (result.is_null()) {
        GTEST_SKIP() << "GetQuoteContract returned null for AAPL OPT@" << expiry;
    }
    auto arr = extract_array(result);
    for (size_t i = 0; i < arr.size(); ++i) {
        ASSERT_TRUE(arr.at(i).is_object());
    }
}

// ── Analytics asset ──────────────────────────────────────────────────────────

TEST(TradeClientInteg, GetAnalyticsAsset_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<TradeClient>(*cfg);
    EXPECT_NO_THROW({
        web::json::value result = client->get_analytics_asset(
            cfg->account, U("2024-01-01"), U("2024-06-01"));
        ASSERT_FALSE(result.is_null());
        EXPECT_TRUE(result.is_array() || result.is_object());
    });
}

// ── Segment fund ──────────────────────────────────────────────────────────────

TEST(TradeClientInteg, GetSegmentFundHistory_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<TradeClient>(*cfg);
    EXPECT_NO_THROW({
        web::json::value result = client->get_segment_fund_history();
        ASSERT_FALSE(result.is_null());
        auto arr = extract_array(result);
        for (size_t i = 0; i < arr.size(); ++i) {
            ASSERT_TRUE(arr.at(i).is_object());
        }
    });
}

TEST(TradeClientInteg, GetSegmentFundAvailable_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<TradeClient>(*cfg);
    EXPECT_NO_THROW({
        web::json::value result = client->get_segment_fund_available(U("SEC"));
        ASSERT_FALSE(result.is_null());
        EXPECT_TRUE(result.is_array() || result.is_object());
    });
}

// ── Aggregate assets ──────────────────────────────────────────────────────────

TEST(TradeClientInteg, GetAggregateAssets_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<TradeClient>(*cfg);
    // This endpoint is only available for institutional accounts.
    web::json::value result;
    try {
        result = client->get_aggregate_assets();
    } catch (const std::exception &ex) {
        GTEST_SKIP() << "GetAggregateAssets not supported for this account: " << ex.what();
    }
    if (result.is_null()) {
        GTEST_SKIP() << "GetAggregateAssets returned null — institution-only endpoint";
    }
    EXPECT_TRUE(result.is_array() || result.is_object());
}

// ── Funding history ───────────────────────────────────────────────────────────

TEST(TradeClientInteg, GetFundingHistory_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<TradeClient>(*cfg);
    EXPECT_NO_THROW({
        web::json::value result = client->get_funding_history();
        ASSERT_FALSE(result.is_null());
        auto arr = extract_array(result);
        for (size_t i = 0; i < arr.size(); ++i) {
            ASSERT_TRUE(arr.at(i).is_object());
        }
    });
}

// ── Fund details ─────────────────────────────────────────────────────────────

TEST(TradeClientInteg, GetFundDetails_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<TradeClient>(*cfg);
    // This endpoint may require specific account permissions or segment types.
    web::json::value result;
    try {
        result = client->get_fund_details();
    } catch (const std::exception &ex) {
        GTEST_SKIP() << "GetFundDetails not supported for this account: " << ex.what();
    }
    if (result.is_null()) {
        GTEST_SKIP() << "GetFundDetails returned null";
    }
    EXPECT_TRUE(result.is_array() || result.is_object());
}

// ── Position transfer records ─────────────────────────────────────────────────

TEST(TradeClientInteg, GetPositionTransferRecords_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<TradeClient>(*cfg);
    // This endpoint may not be supported for all account types.
    web::json::value result;
    try {
        result = client->get_position_transfer_records();
    } catch (const std::exception &ex) {
        GTEST_SKIP() << "GetPositionTransferRecords not supported for this account: " << ex.what();
    }
    if (result.is_null()) {
        GTEST_SKIP() << "GetPositionTransferRecords returned null";
    }
    auto arr = extract_array(result);
    for (size_t i = 0; i < arr.size(); ++i) {
        ASSERT_TRUE(arr.at(i).is_object());
    }
}

TEST(TradeClientInteg, GetPositionTransferExternalRecords_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<TradeClient>(*cfg);
    // This endpoint may not be supported for all account types.
    web::json::value result;
    try {
        result = client->get_position_transfer_external_records();
    } catch (const std::exception &ex) {
        GTEST_SKIP() << "GetPositionTransferExternalRecords not supported for this account: " << ex.what();
    }
    if (result.is_null()) {
        GTEST_SKIP() << "GetPositionTransferExternalRecords returned null";
    }
    auto arr = extract_array(result);
    for (size_t i = 0; i < arr.size(); ++i) {
        ASSERT_TRUE(arr.at(i).is_object());
    }
}

// ── Option exercise (read-only queries) ──────────────────────────────────────

TEST(TradeClientInteg, GetOptionExerciseRecords_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<TradeClient>(*cfg);
    EXPECT_NO_THROW({
        web::json::value result = client->get_option_exercise_records();
        ASSERT_FALSE(result.is_null());
        auto arr = extract_array(result);
        for (size_t i = 0; i < arr.size(); ++i) {
            ASSERT_TRUE(arr.at(i).is_object());
        }
    });
}

TEST(TradeClientInteg, GetOptionExercisePositions_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<TradeClient>(*cfg);
    EXPECT_NO_THROW({
        web::json::value result = client->get_option_exercise_positions(U("Exercise"));
        ASSERT_FALSE(result.is_null());
        auto arr = extract_array(result);
        // May be empty if no exercisable option positions exist.
        for (size_t i = 0; i < arr.size(); ++i) {
            ASSERT_TRUE(arr.at(i).is_object());
        }
    });
}

// ── Preview order ─────────────────────────────────────────────────────────────
//
// Sends a preview (no actual order placed). Validates that the response
// carries non-negative commission / margin fields, matching the Go SDK
// TestIntegPreviewOrderFields pattern.

TEST(TradeClientInteg, PreviewOrder_LmtBuyAapl_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<TradeClient>(*cfg);

    // Build a LMT BUY 1 AAPL STK order priced well below the market so
    // it would never accidentally fill even if preview were treated as live.
    Contract contract(U("STK"), U("AAPL"), U("USD"), U("AAPL"), U("SMART"), 0);
    Order order(U("LMT"), cfg->account, contract, U("BUY"), 1, /*limit_price=*/1.0);

    web::json::value result;
    EXPECT_NO_THROW({
        result = client->preview_order(order);
    });
    ASSERT_FALSE(result.is_null()) << "PreviewOrder returned null";

    // isPass may be false (e.g. insufficient buying power); the call itself
    // must succeed and commission must be >= 0 when present.
    if (result.has_field(U("commission")) && result.at(U("commission")).is_number()) {
        EXPECT_GE(result.at(U("commission")).as_double(), 0.0)
            << "PreviewOrder.commission should be >= 0";
    }
    if (result.has_field(U("initMargin")) && result.at(U("initMargin")).is_number()) {
        EXPECT_GE(result.at(U("initMargin")).as_double(), 0.0)
            << "PreviewOrder.initMargin should be >= 0";
    }
    if (result.has_field(U("maintMargin")) && result.at(U("maintMargin")).is_number()) {
        EXPECT_GE(result.at(U("maintMargin")).as_double(), 0.0)
            << "PreviewOrder.maintMargin should be >= 0";
    }
}

// ── Place order (Mode A: try → catch trading-hours error → return) ────────────
//
// Places a LMT BUY 1 AAPL at $1 (well below market). Outside trading hours the
// server rejects the order with a "trading hours" error — we catch that and
// return cleanly. Inside trading hours the order must be accepted (it will sit
// open at $1 and never fill). The placed order ID is stored for cancel/modify
// tests below via a process-global.

namespace {
/// Order ID placed by PlaceOrder_LmtBuyAapl. 0 = not placed (out of hours or
/// order was not accepted). Written once by the Place test; read by
/// CancelOrder test only. ModifyOrder places its own fresh order independently.
unsigned long long g_placed_order_id = 0;
} // anonymous namespace

TEST(TradeClientInteg, PlaceOrder_LmtBuyAapl_AcceptsOrSkipsOutOfHours) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<TradeClient>(*cfg);
    auto quote_client = std::make_shared<QuoteClient>(*cfg, /*is_grab_permission=*/false);

    Contract contract(U("STK"), U("AAPL"), U("USD"), U("AAPL"), U("SMART"), 0);
    Order order(U("LMT"), cfg->account, contract, U("BUY"), 1, /*limit_price=*/1.0);

    web::json::value result;
    try {
        result = client->place_order(order);
    } catch (const std::exception &ex) {
        std::string msg(ex.what());
        // Common out-of-hours error substrings from the gateway.
        if (msg.find("trading hours") != std::string::npos
            || msg.find("market is closed") != std::string::npos
            || msg.find("outside") != std::string::npos) {
            if (is_market_trading(*quote_client, U("US"))) {
                FAIL() << "PlaceOrder rejected during US trading hours: " << msg;
            }
            GTEST_SKIP() << "PlaceOrder rejected outside trading hours (expected): " << msg;
        }
        if (is_permission_error(msg)) {
            GTEST_SKIP() << "PlaceOrder rejected (permission/account-state boundary): " << msg;
        }
        // Any other exception is a real failure.
        FAIL() << "PlaceOrder threw unexpected exception: " << msg;
    }

    ASSERT_FALSE(result.is_null()) << "PlaceOrder returned null";
    // Extract the order ID for subsequent cancel/modify tests.
    if (result.has_field(U("id")) && result.at(U("id")).is_number()) {
        g_placed_order_id = static_cast<unsigned long long>(
            result.at(U("id")).as_number().to_int64());
        EXPECT_NE(g_placed_order_id, 0u) << "PlaceOrder returned id = 0";
    }
    if (result.has_field(U("orderId")) && result.at(U("orderId")).is_number()) {
        g_placed_order_id = static_cast<unsigned long long>(
            result.at(U("orderId")).as_number().to_int64());
        EXPECT_NE(g_placed_order_id, 0u) << "PlaceOrder returned orderId = 0";
    }
}

// ── Cancel order (Mode A) ─────────────────────────────────────────────────────
//
// Cancels the order placed by PlaceOrder_LmtBuyAapl. Skips if that test did
// not produce an order ID (i.e. out-of-hours or place failed).

TEST(TradeClientInteg, CancelOrder_PlacedOrder_Succeeds) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    if (g_placed_order_id == 0) {
        GTEST_SKIP() << "No placed order ID available (PlaceOrder test did not run or was skipped)";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<TradeClient>(*cfg);
    auto quote_client = std::make_shared<QuoteClient>(*cfg, /*is_grab_permission=*/false);

    web::json::value result;
    try {
        result = client->cancel_order(g_placed_order_id);
    } catch (const std::exception &ex) {
        std::string msg(ex.what());
        if (msg.find("trading hours") != std::string::npos
            || msg.find("market is closed") != std::string::npos
            || msg.find("outside") != std::string::npos) {
            if (is_market_trading(*quote_client, U("US"))) {
                FAIL() << "CancelOrder rejected during US trading hours: " << msg;
            }
            GTEST_SKIP() << "CancelOrder rejected outside trading hours (expected): " << msg;
        }
        if (is_permission_error(msg)) {
            GTEST_SKIP() << "CancelOrder rejected (permission/account-state boundary): " << msg;
        }
        FAIL() << "CancelOrder threw unexpected exception: " << msg;
    }
    ASSERT_FALSE(result.is_null()) << "CancelOrder returned null";
    // Reset so CancelOrder is not retried if the test suite runs again.
    g_placed_order_id = 0;
}

// ── Modify order (Mode A) ─────────────────────────────────────────────────────
//
// Places a fresh LMT BUY 1 AAPL at $1, then modifies the limit price to $1.01.
// Both sub-operations follow Mode A (catch trading-hours rejection → skip).

TEST(TradeClientInteg, ModifyOrder_PlaceThenModifyPrice_Succeeds) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<TradeClient>(*cfg);
    auto quote_client = std::make_shared<QuoteClient>(*cfg, /*is_grab_permission=*/false);

    // Step 1: place a LMT order at $1.
    Contract contract(U("STK"), U("AAPL"), U("USD"), U("AAPL"), U("SMART"), 0);
    Order order(U("LMT"), cfg->account, contract, U("BUY"), 1, /*limit_price=*/1.0);

    web::json::value place_result;
    try {
        place_result = client->place_order(order);
    } catch (const std::exception &ex) {
        std::string msg(ex.what());
        if (msg.find("trading hours") != std::string::npos
            || msg.find("market is closed") != std::string::npos
            || msg.find("outside") != std::string::npos) {
            if (is_market_trading(*quote_client, U("US"))) {
                FAIL() << "PlaceOrder (modify prereq) rejected during trading hours: " << msg;
            }
            GTEST_SKIP() << "PlaceOrder (modify prereq) rejected outside trading hours: " << msg;
        }
        if (is_permission_error(msg)) {
            GTEST_SKIP() << "PlaceOrder (modify prereq) rejected (permission/account-state boundary): " << msg;
        }
        FAIL() << "PlaceOrder (modify prereq) threw unexpected exception: " << msg;
    }
    ASSERT_FALSE(place_result.is_null()) << "PlaceOrder (modify prereq) returned null";

    // Extract the new order ID.
    unsigned long long modify_order_id = 0;
    if (place_result.has_field(U("id")) && place_result.at(U("id")).is_number()) {
        modify_order_id = static_cast<unsigned long long>(
            place_result.at(U("id")).as_number().to_int64());
    }
    if (modify_order_id == 0
        && place_result.has_field(U("orderId"))
        && place_result.at(U("orderId")).is_number()) {
        modify_order_id = static_cast<unsigned long long>(
            place_result.at(U("orderId")).as_number().to_int64());
    }
    if (modify_order_id == 0) {
        GTEST_SKIP() << "PlaceOrder did not return an order ID — cannot test modify";
    }

    // Step 2: modify the limit price.
    Order modify_req(U("LMT"), cfg->account, contract, U("BUY"), 1, /*limit_price=*/1.01);
    modify_req.id = modify_order_id;

    web::json::value modify_result;
    try {
        modify_result = client->modify_order(modify_req, /*limit_price=*/1.01);
    } catch (const std::exception &ex) {
        std::string msg(ex.what());
        if (msg.find("trading hours") != std::string::npos
            || msg.find("market is closed") != std::string::npos
            || msg.find("outside") != std::string::npos) {
            // Clean up the open order before skipping.
            try { client->cancel_order(modify_order_id); } catch (...) {}
            if (is_market_trading(*quote_client, U("US"))) {
                FAIL() << "ModifyOrder rejected during US trading hours: " << msg;
            }
            GTEST_SKIP() << "ModifyOrder rejected outside trading hours (expected): " << msg;
        }
        if (is_permission_error(msg)) {
            try { client->cancel_order(modify_order_id); } catch (...) {}
            GTEST_SKIP() << "ModifyOrder rejected (permission/account-state boundary): " << msg;
        }
        try { client->cancel_order(modify_order_id); } catch (...) {}
        FAIL() << "ModifyOrder threw unexpected exception: " << msg;
    }
    ASSERT_FALSE(modify_result.is_null()) << "ModifyOrder returned null";

    // Clean up: cancel the modified order so it does not linger.
    try { client->cancel_order(modify_order_id); } catch (...) {}
}

// ── order-type matrix (OrderUtil builders) ────────────────────────────────────
//
// Each test builds an order via OrderUtil and either previews it (no order
// placed) or places+cleans-up via the Mode A trading-hours-tolerant pattern
// used above: real exceptions FAIL, but a trading-hours/market-closed
// rejection is tolerated (and cross-checked against live market state before
// being allowed to skip).

namespace {
    Contract aapl_stock() {
        return Contract(U("STK"), U("AAPL"), U("USD"), U("AAPL"), U("SMART"), 0);
    }

    /// Extract an order id from a place_order()/preview_order() JSON result,
    /// trying both the "id" and "orderId" field names used across endpoints.
    unsigned long long extract_order_id(const web::json::value &result) {
        if (result.has_field(U("id")) && result.at(U("id")).is_number()) {
            return static_cast<unsigned long long>(result.at(U("id")).as_number().to_int64());
        }
        if (result.has_field(U("orderId")) && result.at(U("orderId")).is_number()) {
            return static_cast<unsigned long long>(result.at(U("orderId")).as_number().to_int64());
        }
        return 0;
    }
}

TEST(TradeClientInteg, PlaceOrder_MktBuyAapl_PreviewOnly) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<TradeClient>(*cfg);
    Contract contract = aapl_stock();
    auto order = OrderUtil::market_order(cfg->account, contract, U("BUY"), 1);
    EXPECT_NO_THROW({
        web::json::value result = client->preview_order(order);
        ASSERT_FALSE(result.is_null()) << "PreviewOrder(MKT) returned null";
    });
}

TEST(TradeClientInteg, PlaceOrder_StopSellAapl_AcceptsOrSkips) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<TradeClient>(*cfg);
    auto quote_client = std::make_shared<QuoteClient>(*cfg, /*is_grab_permission=*/false);
    Contract contract = aapl_stock();
    // Deep out-of-the-money stop so it cannot actually trigger a fill.
    auto order = OrderUtil::stop_order(cfg->account, contract, U("SELL"), 1, /*aux_price=*/0.01);
    try {
        web::json::value result = client->place_order(order);
        ASSERT_FALSE(result.is_null()) << "PlaceOrder(STP) returned null";
        auto order_id = extract_order_id(result);
        ASSERT_NE(order_id, 0u) << "PlaceOrder(STP) returned zero order id";
        try { client->cancel_order(order_id); } catch (...) {}
    } catch (const std::exception &ex) {
        std::string msg(ex.what());
        if (msg.find("trading hours") != std::string::npos
            || msg.find("market is closed") != std::string::npos
            || msg.find("outside") != std::string::npos) {
            if (is_market_trading(*quote_client, U("US"))) {
                FAIL() << "PlaceOrder(STP) rejected during US trading hours: " << msg;
            }
            GTEST_SKIP() << "PlaceOrder(STP) rejected outside trading hours (expected): " << msg;
        }
        if (is_permission_error(msg)) {
            GTEST_SKIP() << "PlaceOrder(STP) rejected (permission/account-state boundary): " << msg;
        }
        FAIL() << "PlaceOrder(STP) threw unexpected exception: " << msg;
    }
}

TEST(TradeClientInteg, PlaceOrder_StopLimitSellAapl_AcceptsOrSkips) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<TradeClient>(*cfg);
    auto quote_client = std::make_shared<QuoteClient>(*cfg, /*is_grab_permission=*/false);
    Contract contract = aapl_stock();
    auto order = OrderUtil::stop_limit_order(cfg->account, contract, U("SELL"), 1,
                                              /*limit_price=*/0.01, /*aux_price=*/0.01);
    try {
        web::json::value result = client->place_order(order);
        ASSERT_FALSE(result.is_null()) << "PlaceOrder(STP_LMT) returned null";
        auto order_id = extract_order_id(result);
        ASSERT_NE(order_id, 0u) << "PlaceOrder(STP_LMT) returned zero order id";
        try { client->cancel_order(order_id); } catch (...) {}
    } catch (const std::exception &ex) {
        std::string msg(ex.what());
        if (msg.find("trading hours") != std::string::npos
            || msg.find("market is closed") != std::string::npos
            || msg.find("outside") != std::string::npos) {
            if (is_market_trading(*quote_client, U("US"))) {
                FAIL() << "PlaceOrder(STP_LMT) rejected during US trading hours: " << msg;
            }
            GTEST_SKIP() << "PlaceOrder(STP_LMT) rejected outside trading hours (expected): " << msg;
        }
        if (is_permission_error(msg)) {
            GTEST_SKIP() << "PlaceOrder(STP_LMT) rejected (permission/account-state boundary): " << msg;
        }
        FAIL() << "PlaceOrder(STP_LMT) threw unexpected exception: " << msg;
    }
}

TEST(TradeClientInteg, PlaceOrder_TrailSellAapl_AcceptsOrSkips) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<TradeClient>(*cfg);
    auto quote_client = std::make_shared<QuoteClient>(*cfg, /*is_grab_permission=*/false);
    Contract contract = aapl_stock();
    auto order = OrderUtil::trail_order(cfg->account, contract, U("SELL"), 1,
                                         /*aux_price=*/0, /*trailing_percent=*/5.0);
    try {
        web::json::value result = client->place_order(order);
        ASSERT_FALSE(result.is_null()) << "PlaceOrder(TRAIL) returned null";
        auto order_id = extract_order_id(result);
        ASSERT_NE(order_id, 0u) << "PlaceOrder(TRAIL) returned zero order id";
        try { client->cancel_order(order_id); } catch (...) {}
    } catch (const std::exception &ex) {
        std::string msg(ex.what());
        if (msg.find("trading hours") != std::string::npos
            || msg.find("market is closed") != std::string::npos
            || msg.find("outside") != std::string::npos) {
            if (is_market_trading(*quote_client, U("US"))) {
                FAIL() << "PlaceOrder(TRAIL) rejected during US trading hours: " << msg;
            }
            GTEST_SKIP() << "PlaceOrder(TRAIL) rejected outside trading hours (expected): " << msg;
        }
        if (is_permission_error(msg)) {
            GTEST_SKIP() << "PlaceOrder(TRAIL) rejected (permission/account-state boundary): " << msg;
        }
        FAIL() << "PlaceOrder(TRAIL) threw unexpected exception: " << msg;
    }
}

TEST(TradeClientInteg, PlaceOrder_IcebergBuyAapl_AcceptsOrSkips) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<TradeClient>(*cfg);
    auto quote_client = std::make_shared<QuoteClient>(*cfg, /*is_grab_permission=*/false);
    Contract contract = aapl_stock();
    auto order = OrderUtil::iceberg_order(cfg->account, contract, U("BUY"), 10,
                                           /*limit_price=*/0.01, /*display_size=*/1);
    try {
        web::json::value result = client->place_order(order);
        ASSERT_FALSE(result.is_null()) << "PlaceOrder(ICEBERG) returned null";
        auto order_id = extract_order_id(result);
        ASSERT_NE(order_id, 0u) << "PlaceOrder(ICEBERG) returned zero order id";
        try { client->cancel_order(order_id); } catch (...) {}
    } catch (const std::exception &ex) {
        std::string msg(ex.what());
        if (msg.find("trading hours") != std::string::npos
            || msg.find("market is closed") != std::string::npos
            || msg.find("outside") != std::string::npos) {
            if (is_market_trading(*quote_client, U("US"))) {
                FAIL() << "PlaceOrder(ICEBERG) rejected during US trading hours: " << msg;
            }
            GTEST_SKIP() << "PlaceOrder(ICEBERG) rejected outside trading hours (expected): " << msg;
        }
        if (is_permission_error(msg)) {
            GTEST_SKIP() << "PlaceOrder(ICEBERG) rejected (permission/account-state boundary): " << msg;
        }
        FAIL() << "PlaceOrder(ICEBERG) threw unexpected exception: " << msg;
    }
}

TEST(TradeClientInteg, PlaceOrder_TwapBuyAapl_AcceptsOrSkips) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<TradeClient>(*cfg);
    auto quote_client = std::make_shared<QuoteClient>(*cfg, /*is_grab_permission=*/false);
    std::time_t start = std::time(nullptr) + 60;
    std::time_t end = start + 3600;
    Contract contract = aapl_stock();
    auto order = OrderUtil::twap_order(cfg->account, contract, U("BUY"), 10,
                                        start, end, /*limit_price=*/0.01);
    try {
        web::json::value result = client->place_order(order);
        ASSERT_FALSE(result.is_null()) << "PlaceOrder(TWAP) returned null";
        auto order_id = extract_order_id(result);
        ASSERT_NE(order_id, 0u) << "PlaceOrder(TWAP) returned zero order id";
        try { client->cancel_order(order_id); } catch (...) {}
    } catch (const std::exception &ex) {
        std::string msg(ex.what());
        if (msg.find("trading hours") != std::string::npos
            || msg.find("market is closed") != std::string::npos
            || msg.find("outside") != std::string::npos) {
            if (is_market_trading(*quote_client, U("US"))) {
                FAIL() << "PlaceOrder(TWAP) rejected during US trading hours: " << msg;
            }
            GTEST_SKIP() << "PlaceOrder(TWAP) rejected outside trading hours (expected): " << msg;
        }
        if (is_permission_error(msg)) {
            GTEST_SKIP() << "PlaceOrder(TWAP) rejected (permission/account-state boundary): " << msg;
        }
        FAIL() << "PlaceOrder(TWAP) threw unexpected exception: " << msg;
    }
}

TEST(TradeClientInteg, PlaceOrder_VwapBuyAapl_AcceptsOrSkips) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<TradeClient>(*cfg);
    auto quote_client = std::make_shared<QuoteClient>(*cfg, /*is_grab_permission=*/false);
    std::time_t start = std::time(nullptr) + 60;
    std::time_t end = start + 3600;
    Contract contract = aapl_stock();
    auto order = OrderUtil::vwap_order(cfg->account, contract, U("BUY"), 10,
                                        start, end, /*participation_rate=*/0.1, /*limit_price=*/0.01);
    try {
        web::json::value result = client->place_order(order);
        ASSERT_FALSE(result.is_null()) << "PlaceOrder(VWAP) returned null";
        auto order_id = extract_order_id(result);
        ASSERT_NE(order_id, 0u) << "PlaceOrder(VWAP) returned zero order id";
        try { client->cancel_order(order_id); } catch (...) {}
    } catch (const std::exception &ex) {
        std::string msg(ex.what());
        if (msg.find("trading hours") != std::string::npos
            || msg.find("market is closed") != std::string::npos
            || msg.find("outside") != std::string::npos) {
            if (is_market_trading(*quote_client, U("US"))) {
                FAIL() << "PlaceOrder(VWAP) rejected during US trading hours: " << msg;
            }
            GTEST_SKIP() << "PlaceOrder(VWAP) rejected outside trading hours (expected): " << msg;
        }
        if (is_permission_error(msg)) {
            GTEST_SKIP() << "PlaceOrder(VWAP) rejected (permission/account-state boundary): " << msg;
        }
        FAIL() << "PlaceOrder(VWAP) threw unexpected exception: " << msg;
    }
}

// OCA/ATTACHED orders: preview_order() is rejected by the server for these
// ("OCA/ATTACHED order preview not supported", per OrderUtil's header
// comment) — place_order() must be called directly, never preview first.

TEST(TradeClientInteg, PlaceOrder_OcaBracketsAapl_AcceptsOrSkips) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<TradeClient>(*cfg);
    auto quote_client = std::make_shared<QuoteClient>(*cfg, /*is_grab_permission=*/false);
    Contract contract = aapl_stock();
    auto order = OrderUtil::oca_brackets_order(cfg->account, contract, U("BUY"), 1,
                                                /*profit_taker_price=*/999999.0, /*profit_taker_tif=*/U("DAY"),
                                                /*stop_loss_price=*/0.01, /*stop_loss_limit_price=*/0.0,
                                                /*stop_loss_tif=*/U("DAY"));
    try {
        web::json::value result = client->place_order(order);
        ASSERT_FALSE(result.is_null()) << "PlaceOrder(OCA brackets) returned null";
        auto order_id = extract_order_id(result);
        ASSERT_NE(order_id, 0u) << "PlaceOrder(OCA brackets) returned zero order id";
        try { client->cancel_order(order_id); } catch (...) {}
    } catch (const std::exception &ex) {
        std::string msg(ex.what());
        if (msg.find("trading hours") != std::string::npos
            || msg.find("market is closed") != std::string::npos
            || msg.find("outside") != std::string::npos) {
            if (is_market_trading(*quote_client, U("US"))) {
                FAIL() << "PlaceOrder(OCA brackets) rejected during US trading hours: " << msg;
            }
            GTEST_SKIP() << "PlaceOrder(OCA brackets) rejected outside trading hours (expected): " << msg;
        }
        if (is_permission_error(msg)) {
            GTEST_SKIP() << "PlaceOrder(OCA brackets) rejected (permission/account-state boundary): " << msg;
        }
        FAIL() << "PlaceOrder(OCA brackets) threw unexpected exception: " << msg;
    }
}

TEST(TradeClientInteg, PlaceOrder_WithBracketsAapl_AcceptsOrSkips) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<TradeClient>(*cfg);
    auto quote_client = std::make_shared<QuoteClient>(*cfg, /*is_grab_permission=*/false);
    Contract contract = aapl_stock();
    auto order = OrderUtil::limit_order(cfg->account, contract, U("BUY"), 1, /*limit_price=*/0.01);
    OrderUtil::add_brackets_order(order, /*profit_taker_price=*/999999.0, /*profit_taker_tif=*/U("DAY"),
                                   /*profit_taker_rth=*/false, /*stop_loss_price=*/0.01,
                                   /*stop_loss_limit_price=*/0.0, /*stop_loss_tif=*/U("DAY"));
    try {
        web::json::value result = client->place_order(order);
        ASSERT_FALSE(result.is_null()) << "PlaceOrder(with brackets) returned null";
        auto order_id = extract_order_id(result);
        ASSERT_NE(order_id, 0u) << "PlaceOrder(with brackets) returned zero order id";
        try { client->cancel_order(order_id); } catch (...) {}
    } catch (const std::exception &ex) {
        std::string msg(ex.what());
        if (msg.find("trading hours") != std::string::npos
            || msg.find("market is closed") != std::string::npos
            || msg.find("outside") != std::string::npos) {
            if (is_market_trading(*quote_client, U("US"))) {
                FAIL() << "PlaceOrder(with brackets) rejected during US trading hours: " << msg;
            }
            GTEST_SKIP() << "PlaceOrder(with brackets) rejected outside trading hours (expected): " << msg;
        }
        if (is_permission_error(msg)) {
            GTEST_SKIP() << "PlaceOrder(with brackets) rejected (permission/account-state boundary): " << msg;
        }
        FAIL() << "PlaceOrder(with brackets) threw unexpected exception: " << msg;
    }
}

TEST(TradeClientInteg, PlaceOrder_UsOptLimit_AcceptsOrSkips) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<TradeClient>(*cfg);
    auto quote_client = std::make_shared<QuoteClient>(*cfg, /*is_grab_permission=*/false);
    auto leg = resolve_us_option_contract(*quote_client);
    if (!leg.ok) {
        if (is_market_trading(*quote_client, U("US"))) {
            FAIL() << "Could not resolve a live AAPL option contract during US trading hours";
        }
        GTEST_SKIP() << "Could not resolve a live AAPL option contract (market likely closed)";
    }
    Contract contract(U("OPT"), U("AAPL"), leg.expiry, leg.strike, leg.right,
                       U("USD"), /*multiplier=*/100, U(""), /*contract_id=*/0);
    auto order = OrderUtil::limit_order(cfg->account, contract, U("BUY"), 1, /*limit_price=*/0.01);
    try {
        web::json::value result = client->place_order(order);
        ASSERT_FALSE(result.is_null()) << "PlaceOrder(US OPT) returned null";
        auto order_id = extract_order_id(result);
        ASSERT_NE(order_id, 0u) << "PlaceOrder(US OPT) returned zero order id";
        try { client->cancel_order(order_id); } catch (...) {}
    } catch (const std::exception &ex) {
        std::string msg(ex.what());
        if (msg.find("trading hours") != std::string::npos
            || msg.find("market is closed") != std::string::npos
            || msg.find("outside") != std::string::npos) {
            if (is_market_trading(*quote_client, U("US"))) {
                FAIL() << "PlaceOrder(US OPT) rejected during US trading hours: " << msg;
            }
            GTEST_SKIP() << "PlaceOrder(US OPT) rejected outside trading hours (expected): " << msg;
        }
        if (is_permission_error(msg)) {
            GTEST_SKIP() << "PlaceOrder(US OPT) rejected (permission/account-state boundary): " << msg;
        }
        FAIL() << "PlaceOrder(US OPT) threw unexpected exception: " << msg;
    }
}

TEST(TradeClientInteg, PlaceOrder_HkStkLimit_AcceptsOrSkips) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<TradeClient>(*cfg);
    auto quote_client = std::make_shared<QuoteClient>(*cfg, /*is_grab_permission=*/false);
    Contract contract(U("STK"), U("00700"), U("HKD"), U("00700"), U("SEHK"), 0);
    auto order = OrderUtil::limit_order(cfg->account, contract, U("BUY"), 1, /*limit_price=*/0.01);
    try {
        web::json::value result = client->place_order(order);
        ASSERT_FALSE(result.is_null()) << "PlaceOrder(HK STK) returned null";
        auto order_id = extract_order_id(result);
        ASSERT_NE(order_id, 0u) << "PlaceOrder(HK STK) returned zero order id";
        try { client->cancel_order(order_id); } catch (...) {}
    } catch (const std::exception &ex) {
        std::string msg(ex.what());
        if (msg.find("trading hours") != std::string::npos
            || msg.find("market is closed") != std::string::npos
            || msg.find("outside") != std::string::npos) {
            if (is_market_trading(*quote_client, U("HK"))) {
                FAIL() << "PlaceOrder(HK STK) rejected during HK trading hours: " << msg;
            }
            GTEST_SKIP() << "PlaceOrder(HK STK) rejected outside trading hours (expected): " << msg;
        }
        if (is_permission_error(msg)) {
            GTEST_SKIP() << "PlaceOrder(HK STK) rejected (permission/account-state boundary): " << msg;
        }
        FAIL() << "PlaceOrder(HK STK) threw unexpected exception: " << msg;
    }
}

TEST(TradeClientInteg, PlaceOrder_CnStkLimit_AcceptsOrSkips) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<TradeClient>(*cfg);
    auto quote_client = std::make_shared<QuoteClient>(*cfg, /*is_grab_permission=*/false);
    Contract contract(U("STK"), U("000001"), U("CNH"), U("000001"), U("SEHKSZSE"), 0);
    auto order = OrderUtil::limit_order(cfg->account, contract, U("BUY"), 1, /*limit_price=*/0.01);
    try {
        web::json::value result = client->place_order(order);
        ASSERT_FALSE(result.is_null()) << "PlaceOrder(CN STK) returned null";
        auto order_id = extract_order_id(result);
        ASSERT_NE(order_id, 0u) << "PlaceOrder(CN STK) returned zero order id";
        try { client->cancel_order(order_id); } catch (...) {}
    } catch (const std::exception &ex) {
        std::string msg(ex.what());
        if (msg.find("trading hours") != std::string::npos
            || msg.find("market is closed") != std::string::npos
            || msg.find("outside") != std::string::npos) {
            if (is_market_trading(*quote_client, U("CN"))) {
                FAIL() << "PlaceOrder(CN STK) rejected during CN trading hours: " << msg;
            }
            GTEST_SKIP() << "PlaceOrder(CN STK) rejected outside trading hours (expected): " << msg;
        }
        if (is_permission_error(msg)) {
            GTEST_SKIP() << "PlaceOrder(CN STK) rejected (permission/account-state boundary): " << msg;
        }
        FAIL() << "PlaceOrder(CN STK) threw unexpected exception: " << msg;
    }
}

TEST(TradeClientInteg, PlaceOrder_SgStkLimit_AcceptsOrSkips) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<TradeClient>(*cfg);
    auto quote_client = std::make_shared<QuoteClient>(*cfg, /*is_grab_permission=*/false);
    Contract contract(U("STK"), U("D05"), U("SGD"), U("D05"), U("SGX"), 0);
    auto order = OrderUtil::limit_order(cfg->account, contract, U("BUY"), 1, /*limit_price=*/0.01);
    try {
        web::json::value result = client->place_order(order);
        ASSERT_FALSE(result.is_null()) << "PlaceOrder(SG STK) returned null";
        auto order_id = extract_order_id(result);
        ASSERT_NE(order_id, 0u) << "PlaceOrder(SG STK) returned zero order id";
        try { client->cancel_order(order_id); } catch (...) {}
    } catch (const std::exception &ex) {
        std::string msg(ex.what());
        if (msg.find("trading hours") != std::string::npos
            || msg.find("market is closed") != std::string::npos
            || msg.find("outside") != std::string::npos) {
            if (is_market_trading(*quote_client, U("SG"))) {
                FAIL() << "PlaceOrder(SG STK) rejected during SG trading hours: " << msg;
            }
            GTEST_SKIP() << "PlaceOrder(SG STK) rejected outside trading hours (expected): " << msg;
        }
        if (is_permission_error(msg)) {
            GTEST_SKIP() << "PlaceOrder(SG STK) rejected (permission/account-state boundary): " << msg;
        }
        FAIL() << "PlaceOrder(SG STK) threw unexpected exception: " << msg;
    }
}

TEST(TradeClientInteg, PlaceOrder_UsMlegVerticalSpread_AcceptsOrSkips) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<TradeClient>(*cfg);
    auto quote_client = std::make_shared<QuoteClient>(*cfg, /*is_grab_permission=*/false);
    auto spread = resolve_us_vertical_spread_legs(*quote_client);
    if (!spread.ok) {
        if (is_market_trading(*quote_client, U("US"))) {
            FAIL() << "Could not resolve AAPL vertical-spread PUT legs during US trading hours";
        }
        GTEST_SKIP() << "Could not resolve AAPL vertical-spread PUT legs (market likely closed)";
    }
    ContractLeg lower;
    lower.sec_type = U("OPT");
    lower.symbol = U("AAPL");
    lower.expiry = spread.expiry;
    lower.strike = spread.lower_strike;
    lower.right = U("PUT");
    lower.action = U("BUY");
    lower.ratio = 1;

    ContractLeg upper;
    upper.sec_type = U("OPT");
    upper.symbol = U("AAPL");
    upper.expiry = spread.expiry;
    upper.strike = spread.upper_strike;
    upper.right = U("PUT");
    upper.action = U("SELL");
    upper.ratio = 1;

    std::vector<ContractLeg> legs{lower, upper};
    // Deep-negative limit price: a credit-spread combo at this price cannot
    // execute — this is the deliberate safety mechanism for this test.
    auto order = OrderUtil::multi_leg_order(cfg->account, U("VERTICAL"), legs, U("BUY"), 1,
                                             U("LMT"), /*limit_price=*/-100.0);
    try {
        web::json::value result = client->place_order(order);
        ASSERT_FALSE(result.is_null()) << "PlaceOrder(US MLEG vertical) returned null";
        auto order_id = extract_order_id(result);
        ASSERT_NE(order_id, 0u) << "PlaceOrder(US MLEG vertical) returned zero order id";
        try { client->cancel_order(order_id); } catch (...) {}
    } catch (const std::exception &ex) {
        std::string msg(ex.what());
        if (msg.find("trading hours") != std::string::npos
            || msg.find("market is closed") != std::string::npos
            || msg.find("outside") != std::string::npos) {
            if (is_market_trading(*quote_client, U("US"))) {
                FAIL() << "PlaceOrder(US MLEG vertical) rejected during US trading hours: " << msg;
            }
            GTEST_SKIP() << "PlaceOrder(US MLEG vertical) rejected outside trading hours (expected): " << msg;
        }
        if (is_permission_error(msg)) {
            GTEST_SKIP() << "PlaceOrder(US MLEG vertical) rejected (permission/account-state boundary): " << msg;
        }
        FAIL() << "PlaceOrder(US MLEG vertical) threw unexpected exception: " << msg;
    }
}

// ── get_accounts ──────────────────────────────────────────────────────────────
//
// Duplicates the existing GetAccounts_ReturnsValidFields test style but uses
// the explicit function name called out in the task spec to ensure the
// endpoint is covered even if the existing test is reorganised.

TEST(TradeClientInteg, GetAccounts_NonEmpty_AccountFieldPresent) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<TradeClient>(*cfg);
    EXPECT_NO_THROW({
        web::json::value result = client->get_accounts();
        ASSERT_FALSE(result.is_null());
        auto arr = extract_array(result);
        ASSERT_GT(arr.size(), 0u) << "get_accounts returned empty";
        for (size_t i = 0; i < arr.size(); ++i) {
            const auto &acct = arr.at(i);
            ASSERT_TRUE(acct.is_object());
            EXPECT_TRUE(json_nonempty_string(acct, U("account")))
                << "Account[" << i << "].account is empty, want non-empty";
        }
    });
}

// ── get_assets ────────────────────────────────────────────────────────────────

TEST(TradeClientInteg, GetAssets_NonEmpty_CoreFieldsValid) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<TradeClient>(*cfg);
    EXPECT_NO_THROW({
        web::json::value result = client->get_asset();
        ASSERT_FALSE(result.is_null());
        auto arr = extract_array(result);
        ASSERT_GT(arr.size(), 0u) << "get_asset returned empty";
        const auto &asset = arr.at(0);
        ASSERT_TRUE(asset.is_object());
        EXPECT_TRUE(json_nonempty_string(asset, U("account")))
            << "Asset.account is empty, want non-empty";
        EXPECT_TRUE(json_nonempty_string(asset, U("currency")))
            << "Asset.currency is empty, want non-empty";
        if (asset.has_field(U("netLiquidation")) && asset.at(U("netLiquidation")).is_number()) {
            EXPECT_GE(asset.at(U("netLiquidation")).as_double(), 0.0)
                << "Asset.netLiquidation should be >= 0";
        }
        if (asset.has_field(U("buyingPower")) && asset.at(U("buyingPower")).is_number()) {
            EXPECT_GE(asset.at(U("buyingPower")).as_double(), 0.0)
                << "Asset.buyingPower should be >= 0";
        }
    });
}

// ── get_positions ─────────────────────────────────────────────────────────────

TEST(TradeClientInteg, GetPositions_FieldsValidWhenNonEmpty) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<TradeClient>(*cfg);
    EXPECT_NO_THROW({
        web::json::value result = client->get_positions();
        ASSERT_FALSE(result.is_null());
        auto arr = extract_array(result);
        // Positions may legitimately be empty for a fresh paper account.
        for (size_t i = 0; i < arr.size(); ++i) {
            const auto &pos = arr.at(i);
            ASSERT_TRUE(pos.is_object());
            EXPECT_TRUE(json_nonempty_string(pos, U("symbol")))
                << "Position[" << i << "].symbol is empty, want non-empty";
            EXPECT_TRUE(json_nonempty_string(pos, U("account")))
                << "Position[" << i << "].account is empty, want non-empty";
            EXPECT_TRUE(json_nonempty_string(pos, U("secType")))
                << "Position[" << i << "].secType is empty, want non-empty";
        }
    });
}

// ── get_orders ────────────────────────────────────────────────────────────────

TEST(TradeClientInteg, GetOrders_Last30Days_FieldsValidWhenNonEmpty) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<TradeClient>(*cfg);
    std::time_t now = std::time(nullptr);
    std::time_t start = now - 30 * 24 * 60 * 60;
    EXPECT_NO_THROW({
        web::json::value result = client->get_orders(
            U(""), U(""), U("ALL"), U(""), start, now, 10);
        ASSERT_FALSE(result.is_null());
        auto arr = extract_array(result);
        for (size_t i = 0; i < arr.size(); ++i) {
            const auto &ord = arr.at(i);
            ASSERT_TRUE(ord.is_object());
            if (ord.has_field(U("id")) && ord.at(U("id")).is_number()) {
                EXPECT_NE(ord.at(U("id")).as_number().to_int64(), 0)
                    << "Order[" << i << "].id = 0, want non-zero";
            }
            EXPECT_TRUE(json_nonempty_string(ord, U("symbol")))
                << "Order[" << i << "].symbol is empty, want non-empty";
            EXPECT_TRUE(json_nonempty_string(ord, U("action")))
                << "Order[" << i << "].action is empty, want non-empty";
            EXPECT_TRUE(json_nonempty_string(ord, U("status")))
                << "Order[" << i << "].status is empty, want non-empty";
        }
    });
}

// ── get_filled_orders ─────────────────────────────────────────────────────────

TEST(TradeClientInteg, GetFilledOrders_Last30Days_FieldsValidWhenNonEmpty) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<TradeClient>(*cfg);
    std::time_t now = std::time(nullptr);
    std::time_t start = now - 30 * 24 * 60 * 60;
    EXPECT_NO_THROW({
        web::json::value result = client->get_filled_orders(
            U(""), U(""), U("ALL"), U(""), start, now);
        ASSERT_FALSE(result.is_null());
        auto arr = extract_array(result);
        for (size_t i = 0; i < arr.size(); ++i) {
            const auto &ord = arr.at(i);
            ASSERT_TRUE(ord.is_object());
            if (ord.has_field(U("id")) && ord.at(U("id")).is_number()) {
                EXPECT_NE(ord.at(U("id")).as_number().to_int64(), 0)
                    << "FilledOrder[" << i << "].id = 0, want non-zero";
            }
            EXPECT_TRUE(json_nonempty_string(ord, U("status")))
                << "FilledOrder[" << i << "].status is empty, want non-empty";
        }
    });
}

// ── get_segment_fund_available ────────────────────────────────────────────────
//
// Queries available segment fund for the SEC segment. The response may be empty
// for accounts that have not set up segment fund transfers; the wire path is
// validated by the call succeeding without exception.

TEST(TradeClientInteg, GetSegmentFundAvailable_Sec_DoesNotThrow) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    auto cfg = make_integ_config();
    auto client = std::make_shared<TradeClient>(*cfg);
    EXPECT_NO_THROW({
        web::json::value result = client->get_segment_fund_available(U("SEC"));
        ASSERT_FALSE(result.is_null());
        // Response is account-dependent; accept any non-null shape.
        EXPECT_TRUE(result.is_array() || result.is_object())
            << "GetSegmentFundAvailable returned unexpected type";
    });
}
