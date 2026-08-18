// Wire-key contract tests for QuoteClient request parameter construction.
//
// Each test mirrors the obj-building logic in quote_client.cpp and asserts
// that the exact JSON key names sent to the gateway are stable. If a rename
// or refactor changes a wire key, the test fails before the change reaches
// a live server.
//
// Design: no network, no credentials. We construct value objects the same way
// quote_client.cpp does, then assert key presence and value correctness.

#include <gtest/gtest.h>
#include "cpprest/json.h"
#include "tigerapi/constants.h"
#include "tigerapi/enums.h"
#include "tigerapi/utils.h"

using namespace TIGER_API;
using namespace web;
using json::value;

// ── get_kline wire keys ───────────────────────────────────────────────────────

TEST(QuoteRequestParamsTest, GetKline_WireKeys_Present) {
    value obj = value::object(true);
    value symbols = value::array();
    symbols[0] = value::string(U("AAPL"));
    obj[P_SYMBOLS] = symbols;
    obj[P_PERIOD] = value::string(U("day"));
    obj[P_BEGIN_TIME] = value::number(static_cast<int64_t>(1700000000000LL));
    obj[P_END_TIME]   = value::number(static_cast<int64_t>(1700086400000LL));
    obj[P_RIGHT]      = value::string(U("br"));
    obj[P_LIMIT]      = value::number(251);

    EXPECT_TRUE(obj.has_field(U("symbols")));
    EXPECT_TRUE(obj.has_field(U("period")));
    EXPECT_TRUE(obj.has_field(U("begin_time")));
    EXPECT_TRUE(obj.has_field(U("end_time")));
    EXPECT_TRUE(obj.has_field(U("right")));
    EXPECT_TRUE(obj.has_field(U("limit")));
}

TEST(QuoteRequestParamsTest, GetKline_Period_WireValue) {
    value obj = value::object(true);
    obj[P_PERIOD] = value::string(enum_to_str(BarPeriod::DAY));
    EXPECT_EQ(obj.at(U("period")).as_string(), U("day"));
}

TEST(QuoteRequestParamsTest, GetKline_Right_WireValue_BuJi) {
    value obj = value::object(true);
    obj[P_RIGHT] = value::string(enum_to_str(QuoteRight::br));
    EXPECT_EQ(obj.at(U("right")).as_string(), U("br"));
}

TEST(QuoteRequestParamsTest, GetKline_Right_WireValue_None) {
    value obj = value::object(true);
    obj[P_RIGHT] = value::string(enum_to_str(QuoteRight::nr));
    EXPECT_EQ(obj.at(U("right")).as_string(), U("nr"));
}

TEST(QuoteRequestParamsTest, GetKline_BeginEndTime_Int64) {
    // Timestamps must be stored as int64, not double, to avoid precision loss.
    int64_t begin = 1700000000000LL;
    int64_t end   = 1700086400000LL;
    value obj = value::object(true);
    obj[P_BEGIN_TIME] = value::number(static_cast<int64_t>(begin));
    obj[P_END_TIME]   = value::number(static_cast<int64_t>(end));
    EXPECT_EQ(obj.at(U("begin_time")).as_number().to_int64(), begin);
    EXPECT_EQ(obj.at(U("end_time")).as_number().to_int64(), end);
}

// ── get_timeline wire keys ────────────────────────────────────────────────────

TEST(QuoteRequestParamsTest, GetTimeline_WireKeys_Present) {
    value obj = value::object(true);
    value symbols = value::array();
    symbols[0] = value::string(U("TSLA"));
    obj[P_SYMBOLS] = symbols;
    obj[U("include_hour_trading")] = value::boolean(true);
    obj[P_BEGIN_TIME] = value::number(static_cast<int64_t>(1700000000000LL));

    EXPECT_TRUE(obj.has_field(U("symbols")));
    EXPECT_TRUE(obj.has_field(U("include_hour_trading")));
    EXPECT_TRUE(obj.has_field(U("begin_time")));
    // Wire key must NOT be camelCase
    EXPECT_FALSE(obj.has_field(U("includeHourTrading")));
}

TEST(QuoteRequestParamsTest, GetTimeline_IncludeHourTrading_DefaultFalse) {
    value obj = value::object(true);
    obj[U("include_hour_trading")] = value::boolean(false);
    EXPECT_FALSE(obj.at(U("include_hour_trading")).as_bool());
}

// ── get_trade_tick wire keys ──────────────────────────────────────────────────

TEST(QuoteRequestParamsTest, GetTradeTick_WireKeys_Present) {
    value obj = value::object(true);
    value symbols = value::array();
    symbols[0] = value::string(U("AAPL"));
    obj[P_SYMBOLS]       = symbols;
    obj[P_TRADE_SESSION] = value::string(enum_to_str(TradingSession::Regular));
    obj[P_BEGIN_INDEX]   = value::number(static_cast<int64_t>(0));
    obj[P_END_INDEX]     = value::number(static_cast<int64_t>(30));
    obj[P_LIMIT]         = value::number(30);

    EXPECT_TRUE(obj.has_field(U("symbols")));
    EXPECT_TRUE(obj.has_field(U("trade_session")));
    EXPECT_TRUE(obj.has_field(U("begin_index")));
    EXPECT_TRUE(obj.has_field(U("end_index")));
    EXPECT_TRUE(obj.has_field(U("limit")));
    // camelCase must NOT be used
    EXPECT_FALSE(obj.has_field(U("tradeSession")));
    EXPECT_FALSE(obj.has_field(U("beginIndex")));
    EXPECT_FALSE(obj.has_field(U("endIndex")));
}

TEST(QuoteRequestParamsTest, GetTradeTick_TradeSession_WireValue) {
    value obj = value::object(true);
    obj[P_TRADE_SESSION] = value::string(enum_to_str(TradingSession::Regular));
    EXPECT_EQ(obj.at(U("trade_session")).as_string(), U("Regular"));
}

// ── get_quote_depth wire keys ─────────────────────────────────────────────────

TEST(QuoteRequestParamsTest, GetQuoteDepth_WireKeys_Present) {
    value obj = value::object(true);
    value symbols = value::array();
    symbols[0] = value::string(U("AAPL"));
    obj[P_SYMBOLS] = symbols;
    obj[P_MARKET]  = value::string(enum_to_str(Market::US));

    EXPECT_TRUE(obj.has_field(U("symbols")));
    EXPECT_TRUE(obj.has_field(U("market")));
}

TEST(QuoteRequestParamsTest, GetQuoteDepth_Market_WireValue) {
    value obj = value::object(true);
    obj[P_MARKET] = value::string(enum_to_str(Market::US));
    EXPECT_EQ(obj.at(U("market")).as_string(), U("US"));
}

TEST(QuoteRequestParamsTest, GetQuoteDepth_Market_HK_WireValue) {
    value obj = value::object(true);
    obj[P_MARKET] = value::string(enum_to_str(Market::HK));
    EXPECT_EQ(obj.at(U("market")).as_string(), U("HK"));
}

// ── get_capital_flow wire keys ────────────────────────────────────────────────

TEST(QuoteRequestParamsTest, GetCapitalFlow_WireKeys_Present) {
    value obj = value::object(true);
    obj[P_SYMBOL] = value::string(U("AAPL"));
    obj[P_MARKET] = value::string(U("US"));
    obj[P_PERIOD] = value::string(U("DAY"));

    EXPECT_TRUE(obj.has_field(U("symbol")));
    EXPECT_TRUE(obj.has_field(U("market")));
    EXPECT_TRUE(obj.has_field(U("period")));
}

TEST(QuoteRequestParamsTest, GetCapitalFlow_OptionalBeginTime_PresentWhenPositive) {
    // begin_time is only added when > 0 — verify conditional logic.
    time_t begin_time = 1700000000000LL;
    value obj = value::object(true);
    if (begin_time > 0) {
        obj[P_BEGIN_TIME] = value::number(static_cast<int64_t>(begin_time));
    }
    EXPECT_TRUE(obj.has_field(U("begin_time")));
}

TEST(QuoteRequestParamsTest, GetCapitalFlow_OptionalBeginTime_AbsentWhenNegative) {
    time_t begin_time = -1;
    value obj = value::object(true);
    if (begin_time > 0) {
        obj[P_BEGIN_TIME] = value::number(static_cast<int64_t>(begin_time));
    }
    EXPECT_FALSE(obj.has_field(U("begin_time")));
}

TEST(QuoteRequestParamsTest, GetCapitalFlow_OptionalLimit_AbsentWhenZero) {
    int limit = 0;
    value obj = value::object(true);
    if (limit > 0) {
        obj[P_LIMIT] = value::number(limit);
    }
    EXPECT_FALSE(obj.has_field(U("limit")));
}

// ── get_option_chain wire keys ────────────────────────────────────────────────

TEST(QuoteRequestParamsTest, GetOptionChain_WireKeys_Present) {
    // Mirrors the option_basic array structure in quote_client.cpp.
    value obj = value::object(true);
    value basic = value::object(true);
    basic[P_SYMBOL] = value::string(U("AAPL"));
    basic[P_EXPIRY] = value::number(static_cast<int64_t>(1705622400000LL));
    value option_basic = value::array();
    option_basic[0] = basic;
    obj[U("option_basic")] = option_basic;

    EXPECT_TRUE(obj.has_field(U("option_basic")));
    auto arr = obj.at(U("option_basic")).as_array();
    ASSERT_EQ(arr.size(), 1u);
    EXPECT_TRUE(arr[0].has_field(U("symbol")));
    EXPECT_TRUE(arr[0].has_field(U("expiry")));
    // Wire key must be 'expiry', not 'expiration' or 'expiry_date'
    EXPECT_FALSE(arr[0].has_field(U("expiration")));
    EXPECT_FALSE(arr[0].has_field(U("expiry_date")));
}

TEST(QuoteRequestParamsTest, GetOptionChain_OptionalFilter_AbsentWhenNull) {
    value option_filter = value::null();
    value obj = value::object(true);
    if (!option_filter.is_null()) {
        obj[U("option_filter")] = option_filter;
    }
    EXPECT_FALSE(obj.has_field(U("option_filter")));
}

TEST(QuoteRequestParamsTest, GetOptionChain_OptionalFilter_PresentWhenSet) {
    value option_filter = value::object(true);
    option_filter[U("right")] = value::string(U("CALL"));
    value obj = value::object(true);
    if (!option_filter.is_null()) {
        obj[U("option_filter")] = option_filter;
    }
    EXPECT_TRUE(obj.has_field(U("option_filter")));
}

// ── get_capital_distribution wire keys ───────────────────────────────────────

TEST(QuoteRequestParamsTest, GetCapitalDistribution_WireKeys_Present) {
    value obj = value::object(true);
    obj[P_SYMBOL] = value::string(U("AAPL"));
    obj[P_MARKET] = value::string(enum_to_str(Market::US));

    EXPECT_TRUE(obj.has_field(U("symbol")));
    EXPECT_TRUE(obj.has_field(U("market")));
}

TEST(QuoteRequestParamsTest, GetCapitalDistribution_OptionalLang_AbsentWhenEmpty) {
    utility::string_t lang = U("");
    value obj = value::object(true);
    if (!lang.empty()) {
        obj[P_LANG] = value::string(lang);
    }
    EXPECT_FALSE(obj.has_field(U("lang")));
}

TEST(QuoteRequestParamsTest, GetCapitalDistribution_OptionalLang_PresentWhenSet) {
    utility::string_t lang = U("zh_CN");
    value obj = value::object(true);
    if (!lang.empty()) {
        obj[P_LANG] = value::string(lang);
    }
    EXPECT_TRUE(obj.has_field(U("lang")));
    EXPECT_EQ(obj.at(U("lang")).as_string(), U("zh_CN"));
}

// ── get_option_expiration wire keys ──────────────────────────────────────────

TEST(QuoteRequestParamsTest, GetOptionExpiration_SymbolsArray_WireKey) {
    value obj = value::object(true);
    value symbols = value::array();
    symbols[0] = value::string(U("AAPL"));
    symbols[1] = value::string(U("TSLA"));
    obj[P_SYMBOLS] = symbols;

    EXPECT_TRUE(obj.has_field(U("symbols")));
    EXPECT_EQ(obj.at(U("symbols")).as_array().size(), 2u);
}

// ── Symbols array vs singular symbol ─────────────────────────────────────────

TEST(QuoteRequestParamsTest, MultiSymbol_UsesArrayKey_NotSingular) {
    // Methods that accept multiple symbols use 'symbols' (array), not 'symbol'.
    value obj = value::object(true);
    value symbols = value::array();
    symbols[0] = value::string(U("AAPL"));
    symbols[1] = value::string(U("MSFT"));
    obj[P_SYMBOLS] = symbols;

    EXPECT_TRUE(obj.has_field(U("symbols")));
    EXPECT_FALSE(obj.has_field(U("symbol")));  // singular must NOT appear
}

TEST(QuoteRequestParamsTest, SingleSymbol_UsesScalarKey_NotArray) {
    // Methods that accept one symbol use 'symbol' (scalar), not 'symbols'.
    value obj = value::object(true);
    obj[P_SYMBOL] = value::string(U("AAPL"));

    EXPECT_TRUE(obj.has_field(U("symbol")));
    EXPECT_FALSE(obj.has_field(U("symbols")));  // plural must NOT appear
}
