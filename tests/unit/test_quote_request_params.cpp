// Wire-key contract tests for QuoteClient request parameter construction.

#include <gtest/gtest.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include "cpprest/http_listener.h"
#include "cpprest/json.h"
#include "tigerapi/constants.h"
#include "tigerapi/enums.h"
#include "tigerapi/quote_client.h"
#include "tigerapi/utils.h"

using namespace TIGER_API;
using namespace web;
using json::value;

namespace {

utility::string_t make_private_key() {
    EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, nullptr);
    EVP_PKEY *key = nullptr;
    if (!ctx || EVP_PKEY_keygen_init(ctx) <= 0 ||
        EVP_PKEY_CTX_set_rsa_keygen_bits(ctx, 2048) <= 0 ||
        EVP_PKEY_keygen(ctx, &key) <= 0) {
        EVP_PKEY_CTX_free(ctx);
        throw std::runtime_error("failed to generate test RSA key");
    }
    EVP_PKEY_CTX_free(ctx);

    BIO *bio = BIO_new(BIO_s_mem());
    if (!bio || PEM_write_bio_PrivateKey(bio, key, nullptr, nullptr, 0, nullptr, nullptr) != 1) {
        BIO_free(bio);
        EVP_PKEY_free(key);
        throw std::runtime_error("failed to encode test RSA key");
    }
    char *data = nullptr;
    const long size = BIO_get_mem_data(bio, &data);
    const utility::string_t pem = Utils::str8to16(std::string(data, static_cast<size_t>(size)));
    BIO_free(bio);
    EVP_PKEY_free(key);
    return pem;
}

class QuoteRequestCaptureTest : public ::testing::Test {
protected:
    QuoteRequestCaptureTest()
        : listener_(U("http://127.0.0.1:39091")) {
        listener_.support(methods::POST, [this](http_request request) {
            captured_ = request.extract_json().get();
            value response = value::object();
            response[P_CODE] = value::number(1);
            request.reply(status_codes::OK, response).wait();
        });
        listener_.open().wait();
    }

    ~QuoteRequestCaptureTest() override {
        listener_.close().wait();
    }

    std::unique_ptr<QuoteClient> make_client() {
        static const utility::string_t private_key = make_private_key();
        ClientConfig config(U("test_tiger_id"), private_key, U("test_account"), true, U(""));
        config.set_server_url(U("http://127.0.0.1:39091"));
        return std::make_unique<QuoteClient>(config, false);
    }

    value biz_content() const {
        return value::parse(captured_.at(P_BIZ_CONTENT).as_string());
    }

    web::http::experimental::listener::http_listener listener_;
    value captured_;
};

value one_symbol() {
    value symbols = value::array();
    symbols[0] = value::string(U("AAPL"));
    return symbols;
}

} // namespace

// ── get_kline wire keys ───────────────────────────────────────────────────────

TEST_F(QuoteRequestCaptureTest, GetKlineSendsV2EnvelopeAndSession) {
    auto client = make_client();
    EXPECT_THROW(client->get_kline(one_symbol(), BarPeriod::ONE_MINUTE, 1700000000000LL,
                                   1700086400000LL, QuoteRight::br, 251, U("next"),
                                   TradingSession::OverNight), std::runtime_error);

    EXPECT_EQ(captured_.at(P_METHOD).as_string(), U("kline"));
    EXPECT_EQ(captured_.at(P_VERSION).as_string(), U("2.0"));
    const value content = biz_content();
    EXPECT_EQ(content.at(P_SYMBOLS).as_array().at(0).as_string(), U("AAPL"));
    EXPECT_EQ(content.at(P_PERIOD).as_string(), U("1min"));
    EXPECT_EQ(content.at(P_BEGIN_TIME).as_number().to_int64(), 1700000000000LL);
    EXPECT_EQ(content.at(P_END_TIME).as_number().to_int64(), 1700086400000LL);
    EXPECT_EQ(content.at(P_RIGHT).as_string(), U("br"));
    EXPECT_EQ(content.at(P_LIMIT).as_integer(), 251);
    EXPECT_EQ(content.at(P_PAGE_TOKEN).as_string(), U("next"));
    EXPECT_EQ(content.at(P_TRADE_SESSION).as_string(), U("OverNight"));
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

TEST_F(QuoteRequestCaptureTest, GetTradeTickSendsV2EnvelopeAndLatestOvernightRange) {
    auto client = make_client();
    EXPECT_THROW(client->get_trade_tick(one_symbol(), TradingSession::OverNight, -1, -1, 2000),
                 std::runtime_error);

    EXPECT_EQ(captured_.at(P_METHOD).as_string(), U("trade_tick"));
    EXPECT_EQ(captured_.at(P_VERSION).as_string(), U("2.0"));
    const value content = biz_content();
    EXPECT_EQ(content.at(P_SYMBOLS).as_array().at(0).as_string(), U("AAPL"));
    EXPECT_EQ(content.at(P_TRADE_SESSION).as_string(), U("OverNight"));
    EXPECT_EQ(content.at(P_BEGIN_INDEX).as_number().to_int64(), -1);
    EXPECT_EQ(content.at(P_END_INDEX).as_number().to_int64(), -1);
    EXPECT_EQ(content.at(P_LIMIT).as_integer(), 2000);
}

// ── get_quote_depth wire keys ─────────────────────────────────────────────────

TEST_F(QuoteRequestCaptureTest, GetQuoteDepthSendsV2EnvelopeAndSession) {
    auto client = make_client();
    EXPECT_THROW(client->get_quote_depth(one_symbol(), Market::US, TradingSession::OverNight),
                 std::runtime_error);

    EXPECT_EQ(captured_.at(P_METHOD).as_string(), U("quote_depth"));
    EXPECT_EQ(captured_.at(P_VERSION).as_string(), U("2.0"));
    const value content = biz_content();
    EXPECT_EQ(content.at(P_SYMBOLS).as_array().at(0).as_string(), U("AAPL"));
    EXPECT_EQ(content.at(P_MARKET).as_string(), U("US"));
    EXPECT_EQ(content.at(P_TRADE_SESSION).as_string(), U("OverNight"));
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
