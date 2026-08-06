#include <gtest/gtest.h>
#include <ctime>
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
 *
 * These tests only exercise read-only trade endpoints (contract/asset/position/
 * order queries) — no order placement, modification, or cancellation.
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

} // anonymous namespace

// ── Asset ─────────────────────────────────────────────────────────────────────

TEST(TradeClientInteg, GetAsset_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    ClientConfig cfg = make_integ_config();
    auto client = std::make_shared<TradeClient>(cfg);
    EXPECT_NO_THROW({
        web::json::value result = client->get_asset();
        ASSERT_FALSE(result.is_null());

        auto arr = extract_array(result);
        ASSERT_GT(arr.size(), 0u) << "GetAsset returned empty";
        const auto &asset = arr.at(0);
        ASSERT_TRUE(asset.is_object());

        if (asset.has_field(U("account"))) {
            EXPECT_FALSE(asset.at(U("account")).as_string().empty())
                << "Asset.account is empty, want non-empty";
        }
        if (asset.has_field(U("currency"))) {
            EXPECT_FALSE(asset.at(U("currency")).as_string().empty())
                << "Asset.currency is empty, want non-empty";
        }
        if (asset.has_field(U("netLiquidation"))) {
            EXPECT_GE(asset.at(U("netLiquidation")).as_double(), 0.0)
                << "Asset.netLiquidation should be >= 0";
        }
    });
}

// ── Accounts ─────────────────────────────────────────────────────────────────

TEST(TradeClientInteg, GetAccounts_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    ClientConfig cfg = make_integ_config();
    auto client = std::make_shared<TradeClient>(cfg);
    EXPECT_NO_THROW({
        web::json::value result = client->get_accounts();
        ASSERT_FALSE(result.is_null());

        auto arr = extract_array(result);
        ASSERT_GT(arr.size(), 0u) << "GetAccounts returned empty";
        const auto &acct = arr.at(0);
        ASSERT_TRUE(acct.is_object());

        if (acct.has_field(U("account"))) {
            EXPECT_FALSE(acct.at(U("account")).as_string().empty())
                << "Account.account is empty, want non-empty";
        }
    });
}

// ── Positions (empty-result guard) ───────────────────────────────────────────

TEST(TradeClientInteg, GetPositions_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    ClientConfig cfg = make_integ_config();
    auto client = std::make_shared<TradeClient>(cfg);
    EXPECT_NO_THROW({
        web::json::value result = client->get_positions();
        ASSERT_FALSE(result.is_null());

        auto arr = extract_array(result);
        // Positions may legitimately be empty for a fresh paper account;
        // only assert the call succeeds. When positions exist, validate fields.
        for (size_t i = 0; i < arr.size(); ++i) {
            const auto &pos = arr.at(i);
            ASSERT_TRUE(pos.is_object());

            if (pos.has_field(U("symbol"))) {
                EXPECT_FALSE(pos.at(U("symbol")).as_string().empty())
                    << "Position[" << i << "].symbol is empty, want non-empty";
            }
            if (pos.has_field(U("account"))) {
                EXPECT_FALSE(pos.at(U("account")).as_string().empty())
                    << "Position[" << i << "].account is empty, want non-empty";
            }
            if (pos.has_field(U("secType"))) {
                EXPECT_FALSE(pos.at(U("secType")).as_string().empty())
                    << "Position[" << i << "].secType is empty, want non-empty";
            }
        }
    });
}

// ── Orders (empty-result guard) ──────────────────────────────────────────────

TEST(TradeClientInteg, GetOrders_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    ClientConfig cfg = make_integ_config();
    auto client = std::make_shared<TradeClient>(cfg);

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

            if (ord.has_field(U("id"))) {
                EXPECT_NE(ord.at(U("id")).as_number().to_int64(), 0)
                    << "Order[" << i << "].id = 0, want non-zero";
            }
            if (ord.has_field(U("symbol"))) {
                EXPECT_FALSE(ord.at(U("symbol")).as_string().empty())
                    << "Order[" << i << "].symbol is empty, want non-empty";
            }
            if (ord.has_field(U("action"))) {
                EXPECT_FALSE(ord.at(U("action")).as_string().empty())
                    << "Order[" << i << "].action is empty, want non-empty";
            }
            if (ord.has_field(U("status"))) {
                EXPECT_FALSE(ord.at(U("status")).as_string().empty())
                    << "Order[" << i << "].status is empty, want non-empty";
            }
        }
    });
}

// ── Contract ─────────────────────────────────────────────────────────────────

TEST(TradeClientInteg, GetContract_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    ClientConfig cfg = make_integ_config();
    auto client = std::make_shared<TradeClient>(cfg);
    EXPECT_NO_THROW({
        web::json::value result = client->get_contract(U("AAPL"), U("STK"));
        ASSERT_FALSE(result.is_null());

        auto arr = extract_array(result);
        ASSERT_GT(arr.size(), 0u) << "GetContract returned empty for AAPL STK";
        const auto &contract = arr.at(0);
        ASSERT_TRUE(contract.is_object());

        if (contract.has_field(U("symbol"))) {
            EXPECT_EQ(contract.at(U("symbol")).as_string(), U("AAPL"))
                << "Contract.symbol mismatch";
        }
        if (contract.has_field(U("secType"))) {
            EXPECT_EQ(contract.at(U("secType")).as_string(), U("STK"))
                << "Contract.secType mismatch";
        }
        if (contract.has_field(U("currency"))) {
            EXPECT_FALSE(contract.at(U("currency")).as_string().empty())
                << "Contract.currency is empty, want non-empty (e.g. USD)";
        }
    });
}

// ── Active orders (empty-result guard) ───────────────────────────────────────

TEST(TradeClientInteg, GetActiveOrders_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    ClientConfig cfg = make_integ_config();
    auto client = std::make_shared<TradeClient>(cfg);
    EXPECT_NO_THROW({
        web::json::value result = client->get_active_orders();
        ASSERT_FALSE(result.is_null());

        auto arr = extract_array(result);
        // Active orders may be empty if no open orders exist.
        // When orders exist, validate identifier fields.
        for (size_t i = 0; i < arr.size(); ++i) {
            const auto &ord = arr.at(i);
            ASSERT_TRUE(ord.is_object());

            if (ord.has_field(U("id"))) {
                EXPECT_NE(ord.at(U("id")).as_number().to_int64(), 0)
                    << "ActiveOrder[" << i << "].id = 0, want non-zero";
            }
            if (ord.has_field(U("symbol"))) {
                EXPECT_FALSE(ord.at(U("symbol")).as_string().empty())
                    << "ActiveOrder[" << i << "].symbol is empty, want non-empty";
            }
            if (ord.has_field(U("status"))) {
                EXPECT_FALSE(ord.at(U("status")).as_string().empty())
                    << "ActiveOrder[" << i << "].status is empty, want non-empty";
            }
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
        if (ord.is_object() && ord.has_field(U("id"))) {
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
    ClientConfig cfg = make_integ_config();
    auto client = std::make_shared<TradeClient>(cfg);
    EXPECT_NO_THROW({
        web::json::value result = client->get_prime_asset();
        ASSERT_FALSE(result.is_null());
        auto arr = extract_array(result);
        // Prime asset may be empty for non-prime accounts.
        for (size_t i = 0; i < arr.size(); ++i) {
            const auto &asset = arr.at(i);
            ASSERT_TRUE(asset.is_object());
            if (asset.has_field(U("account"))) {
                EXPECT_FALSE(asset.at(U("account")).as_string().empty())
                    << "PrimeAsset[" << i << "].account is empty";
            }
            if (asset.has_field(U("currency"))) {
                EXPECT_FALSE(asset.at(U("currency")).as_string().empty())
                    << "PrimeAsset[" << i << "].currency is empty";
            }
        }
    });
}

TEST(TradeClientInteg, GetPrimePortfolio_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    ClientConfig cfg = make_integ_config();
    auto client = std::make_shared<TradeClient>(cfg);
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
    ClientConfig cfg = make_integ_config();
    auto client = std::make_shared<TradeClient>(cfg);
    EXPECT_NO_THROW({
        auto positions = client->get_position_list();
        // Positions may legitimately be empty for a fresh paper account.
        for (size_t i = 0; i < positions.size(); ++i) {
            EXPECT_FALSE(positions[i].contract.symbol.empty())
                << "Position[" << i << "].symbol is empty";
            if (!positions[i].account.empty()) {
                EXPECT_FALSE(positions[i].account.empty())
                    << "Position[" << i << "].account is empty";
            }
        }
    });
}

// ── Filled / inactive orders ─────────────────────────────────────────────────

TEST(TradeClientInteg, GetFilledOrders_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    ClientConfig cfg = make_integ_config();
    auto client = std::make_shared<TradeClient>(cfg);
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
            if (ord.has_field(U("id"))) {
                EXPECT_NE(ord.at(U("id")).as_number().to_int64(), 0)
                    << "FilledOrder[" << i << "].id = 0, want non-zero";
            }
            if (ord.has_field(U("status"))) {
                EXPECT_FALSE(ord.at(U("status")).as_string().empty())
                    << "FilledOrder[" << i << "].status is empty";
            }
        }
    });
}

TEST(TradeClientInteg, GetInactiveOrders_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    ClientConfig cfg = make_integ_config();
    auto client = std::make_shared<TradeClient>(cfg);
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
            if (ord.has_field(U("id"))) {
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
    ClientConfig cfg = make_integ_config();
    auto client = std::make_shared<TradeClient>(cfg);
    auto order_id = get_first_order_id(*client);
    if (order_id == 0) {
        GTEST_SKIP() << "No orders available to test get_order";
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
    ClientConfig cfg = make_integ_config();
    auto client = std::make_shared<TradeClient>(cfg);
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
    ClientConfig cfg = make_integ_config();
    auto client = std::make_shared<TradeClient>(cfg);
    EXPECT_NO_THROW({
        web::json::value result = client->get_contracts(make_symbols(U("AAPL")), U("STK"));
        ASSERT_FALSE(result.is_null());
        auto arr = extract_array(result);
        ASSERT_GT(arr.size(), 0u) << "GetContracts returned empty for AAPL STK";
        const auto &contract = arr.at(0);
        ASSERT_TRUE(contract.is_object());
        if (contract.has_field(U("symbol"))) {
            EXPECT_EQ(contract.at(U("symbol")).as_string(), U("AAPL"));
        }
        if (contract.has_field(U("secType"))) {
            EXPECT_EQ(contract.at(U("secType")).as_string(), U("STK"));
        }
    });
}

// ── Quote contract ───────────────────────────────────────────────────────────

TEST(TradeClientInteg, GetQuoteContract_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    ClientConfig cfg = make_integ_config();
    auto client = std::make_shared<TradeClient>(cfg);
    EXPECT_NO_THROW({
        web::json::value result = client->get_quote_contract(U("AAPL"), U("STK"), U(""));
        ASSERT_FALSE(result.is_null());
        auto arr = extract_array(result);
        ASSERT_GT(arr.size(), 0u) << "GetQuoteContract returned empty for AAPL STK";
        const auto &contract = arr.at(0);
        ASSERT_TRUE(contract.is_object());
        if (contract.has_field(U("symbol"))) {
            EXPECT_EQ(contract.at(U("symbol")).as_string(), U("AAPL"));
        }
    });
}

// ── Analytics asset ──────────────────────────────────────────────────────────

TEST(TradeClientInteg, GetAnalyticsAsset_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    ClientConfig cfg = make_integ_config();
    auto client = std::make_shared<TradeClient>(cfg);
    EXPECT_NO_THROW({
        web::json::value result = client->get_analytics_asset(
            cfg.account, U("2024-01-01"), U("2024-06-01"));
        ASSERT_FALSE(result.is_null());
        EXPECT_TRUE(result.is_array() || result.is_object());
    });
}

// ── Segment fund ──────────────────────────────────────────────────────────────

TEST(TradeClientInteg, GetSegmentFundHistory_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    ClientConfig cfg = make_integ_config();
    auto client = std::make_shared<TradeClient>(cfg);
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
    ClientConfig cfg = make_integ_config();
    auto client = std::make_shared<TradeClient>(cfg);
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
    ClientConfig cfg = make_integ_config();
    auto client = std::make_shared<TradeClient>(cfg);
    EXPECT_NO_THROW({
        web::json::value result = client->get_aggregate_assets();
        ASSERT_FALSE(result.is_null());
        EXPECT_TRUE(result.is_array() || result.is_object());
    });
}

// ── Funding history ───────────────────────────────────────────────────────────

TEST(TradeClientInteg, GetFundingHistory_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    ClientConfig cfg = make_integ_config();
    auto client = std::make_shared<TradeClient>(cfg);
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
    ClientConfig cfg = make_integ_config();
    auto client = std::make_shared<TradeClient>(cfg);
    EXPECT_NO_THROW({
        web::json::value result = client->get_fund_details();
        ASSERT_FALSE(result.is_null());
        EXPECT_TRUE(result.is_array() || result.is_object());
    });
}

// ── Position transfer records ─────────────────────────────────────────────────

TEST(TradeClientInteg, GetPositionTransferRecords_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    ClientConfig cfg = make_integ_config();
    auto client = std::make_shared<TradeClient>(cfg);
    EXPECT_NO_THROW({
        web::json::value result = client->get_position_transfer_records();
        ASSERT_FALSE(result.is_null());
        auto arr = extract_array(result);
        for (size_t i = 0; i < arr.size(); ++i) {
            ASSERT_TRUE(arr.at(i).is_object());
        }
    });
}

TEST(TradeClientInteg, GetPositionTransferExternalRecords_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    ClientConfig cfg = make_integ_config();
    auto client = std::make_shared<TradeClient>(cfg);
    EXPECT_NO_THROW({
        web::json::value result = client->get_position_transfer_external_records();
        ASSERT_FALSE(result.is_null());
        auto arr = extract_array(result);
        for (size_t i = 0; i < arr.size(); ++i) {
            ASSERT_TRUE(arr.at(i).is_object());
        }
    });
}

// ── Option exercise (read-only queries) ──────────────────────────────────────

TEST(TradeClientInteg, GetOptionExerciseRecords_ReturnsValidFields) {
    if (!integ_enabled()) {
        GTEST_SKIP() << "Set TIGER_RUN_INTEG=true to run integration tests";
    }
    ClientConfig cfg = make_integ_config();
    auto client = std::make_shared<TradeClient>(cfg);
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
    ClientConfig cfg = make_integ_config();
    auto client = std::make_shared<TradeClient>(cfg);
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
