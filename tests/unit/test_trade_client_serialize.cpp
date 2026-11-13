// This test file accesses a private static method via the #define private public
// technique.  The trick is scoped: all standard-library and project headers are
// included FIRST, then private is redefined only for the two headers that
// declare the private members we need to reach.

#include <gtest/gtest.h>
#include "cpprest/http_client.h"
#include "cpprest/json.h"

// Include ALL transitive headers before the define so they are unaffected.
#include <gtest/gtest.h>
#include "cpprest/http_client.h"
#include "cpprest/filestream.h"
#include "cpprest/json.h"
#include "tigerapi/client_config.h"
#include "tigerapi/constants.h"
#include "tigerapi/contract_util.h"
#include "tigerapi/enums.h"
#include "tigerapi/model.h"
#include "tigerapi/order_util.h"
#include "tigerapi/utils.h"
#include "tigerapi/service_types.h"
#include "tigerapi/properties.h"
#include "tigerapi/logger.h"
#include "tigerapi/win32.h"
#include "support/test_support.h"

// ── Access private members of TigerClient and TradeClient ──────────────────────
#define private public
#include "tigerapi/tiger_client.h"
#include "tigerapi/trade_client.h"
#undef private

using namespace TIGER_API;

// ── TigerClient private methods ────────────────────────────────────────────────

TEST(TigerClientPrivateTest, BuildSignContent_SingleKey) {
    auto cfg = test::make_offline_config();
    TigerClient client(*cfg);

    value obj = value::object(true);
    obj[U("b")] = value::string(U("2"));
    obj[U("a")] = value::string(U("1"));
    obj[U("c")] = value::string(U("3"));

    utility::string_t result = client.build_sign_content(obj);
    // Keys should be sorted alphabetically
    EXPECT_EQ(result, U("a=1&b=2&c=3"));
}

TEST(TigerClientPrivateTest, BuildSignContent_NonStringValue) {
    auto cfg = test::make_offline_config();
    TigerClient client(*cfg);

    value obj = value::object(true);
    obj[U("num")] = value::number(42);
    obj[U("str")] = value::string(U("hello"));

    utility::string_t result = client.build_sign_content(obj);
    // Non-string values use serialize()
    EXPECT_NE(result.find(U("num")), utility::string_t::npos);
    EXPECT_NE(result.find(U("42")), utility::string_t::npos);
    EXPECT_NE(result.find(U("str=hello")), utility::string_t::npos);
}

TEST(TigerClientPrivateTest, BuildSignContent_EmptyObject) {
    auto cfg = test::make_offline_config();
    TigerClient client(*cfg);

    value obj = value::object(true);
    utility::string_t result = client.build_sign_content(obj);
    EXPECT_TRUE(result.empty());
}

TEST(TigerClientPrivateTest, BuildSignContent_SingleEntry) {
    auto cfg = test::make_offline_config();
    TigerClient client(*cfg);

    value obj = value::object(true);
    obj[U("key")] = value::string(U("value"));

    utility::string_t result = client.build_sign_content(obj);
    EXPECT_EQ(result, U("key=value"));
}

TEST(TigerClientPrivateTest, BuildCommonParams_HasTigerId) {
    auto cfg = test::make_offline_config();
    TigerClient client(*cfg);

    value params = client.build_common_params();
    EXPECT_EQ(params[U("tiger_id")].as_string(), test::FAKE_TIGER_ID);
}

TEST(TigerClientPrivateTest, BuildCommonParams_HasCharset) {
    auto cfg = test::make_offline_config();
    TigerClient client(*cfg);

    value params = client.build_common_params();
    EXPECT_EQ(params[U("charset")].as_string(), U("UTF-8"));
}

TEST(TigerClientPrivateTest, BuildCommonParams_HasSignType) {
    auto cfg = test::make_offline_config();
    TigerClient client(*cfg);

    value params = client.build_common_params();
    EXPECT_EQ(params[U("sign_type")].as_string(), U("RSA"));
}

TEST(TigerClientPrivateTest, BuildCommonParams_HasVersion) {
    auto cfg = test::make_offline_config();
    TigerClient client(*cfg);

    value params = client.build_common_params();
    EXPECT_EQ(params[U("version")].as_string(), OPEN_API_SERVICE_VERSION);
}

// Per-endpoint version overrides for endpoints that do not use the global version.
// The following endpoints must pin API v2.0 or the gateway rejects the call (matches Java V2_0 and Rust
// VERSION_V2 fixes).
TEST(TigerClientPrivateTest, BuildCommonParams_FinancialReport_UsesV2) {
    auto cfg = test::make_offline_config();
    TigerClient client(*cfg);

    value params = client.build_common_params(U("financial_report"));
    EXPECT_EQ(params[U("version")].as_string(), U("2.0"));
}

TEST(TigerClientPrivateTest, BuildCommonParams_FinancialDaily_UsesV2) {
    auto cfg = test::make_offline_config();
    TigerClient client(*cfg);

    value params = client.build_common_params(U("financial_daily"));
    EXPECT_EQ(params[U("version")].as_string(), U("2.0"));
}

TEST(TigerClientPrivateTest, BuildCommonParams_TradingCalendar_UsesV2) {
    auto cfg = test::make_offline_config();
    TigerClient client(*cfg);

    value params = client.build_common_params(U("trading_calendar"));
    EXPECT_EQ(params[U("version")].as_string(), U("2.0"));
}

TEST(TigerClientPrivateTest, BuildCommonParams_QuoteOvernight_UsesV1) {
    auto cfg = test::make_offline_config();
    TigerClient client(*cfg);

    value params = client.build_common_params(U("quote_overnight"));
    EXPECT_EQ(params[U("version")].as_string(), U("1.0"));
}

TEST(TigerClientPrivateTest, BuildCommonParams_QuoteRealTime_UsesV2) {
    auto cfg = test::make_offline_config();
    TigerClient client(*cfg);

    value params = client.build_common_params(U("quote_real_time"));
    EXPECT_EQ(params[U("version")].as_string(), U("2.0"));
}

TEST(TigerClientPrivateTest, BuildCommonParams_UnknownMethod_UsesGlobalVersion) {
    auto cfg = test::make_offline_config();
    TigerClient client(*cfg);

    // Non-overridden endpoints must keep the global default (currently V3.0).
    value params = client.build_common_params(U("unknown_method"));
    EXPECT_EQ(params[U("version")].as_string(), OPEN_API_SERVICE_VERSION);
}

TEST(TigerClientPrivateTest, BuildCommonParams_HasDeviceId) {
    auto cfg = test::make_offline_config();
    TigerClient client(*cfg);

    value params = client.build_common_params();
    EXPECT_FALSE(params[U("device_id")].as_string().empty());
}

// ── TradeClient::serialize_advanced_order_fields ──────────────────────────────

namespace {
    TradeClient make_trade_client() {
        auto cfg = test::make_offline_config();
        return TradeClient(*cfg);
    }
}

TEST(TradeClientSerializeTest, IcebergFields) {
    TradeClient tc = make_trade_client();
    Contract c = ContractUtil::stock_contract(U("AAPL"), U("USD"));
    Order o = OrderUtil::iceberg_order(U("ACCT"), c, U("BUY"), 1000LL, 175.0, 100LL, 50LL, 5LL, U("LMT"), 100LL, 200LL);

    value obj = value::object(true);
    obj[U("account")] = value::string(U("ACCT"));
    TradeClient::serialize_advanced_order_fields(obj, o, U("ACCT"));

    EXPECT_EQ(obj[U("display_size")].as_number().to_int64(), 100LL);
    EXPECT_EQ(obj[U("min_display_size")].as_number().to_int64(), 50LL);
    EXPECT_EQ(obj[U("check_intervals")].as_number().to_int64(), 5LL);
    EXPECT_EQ(obj[U("price_type")].as_string(), U("LMT"));
    EXPECT_EQ(obj[U("start_time")].as_number().to_int64(), 100LL);
    EXPECT_EQ(obj[U("end_time")].as_number().to_int64(), 200LL);
}

TEST(TradeClientSerializeTest, AmountOrderField) {
    TradeClient tc = make_trade_client();
    Contract c = ContractUtil::stock_contract(U("AAPL"), U("USD"));
    Order o = OrderUtil::amount_order(U("ACCT"), c, U("BUY"), 5000.0);

    value obj = value::object(true);
    TradeClient::serialize_advanced_order_fields(obj, o, U("ACCT"));

    EXPECT_DOUBLE_EQ(obj[U("cash_amount")].as_double(), 5000.0);
}

TEST(TradeClientSerializeTest, ComboOrderFields) {
    TradeClient tc = make_trade_client();
    ContractLeg leg1(U("OPT"), U("AAPL"), U("175.0"), U("220414"), U("CALL"), U("BUY"), 1);
    ContractLeg leg2(U("OPT"), U("AAPL"), U("180.0"), U("220414"), U("CALL"), U("SELL"), 1);
    std::vector<ContractLeg> legs = {leg1, leg2};
    Order o = OrderUtil::multi_leg_order(U("ACCT"), U("VERTICAL"), legs, U("BUY"), 1LL, U("LMT"), 2.5);

    value obj = value::object(true);
    TradeClient::serialize_advanced_order_fields(obj, o, U("ACCT"));

    EXPECT_EQ(obj[U("combo_type")].as_string(), U("VERTICAL"));
    EXPECT_TRUE(obj.has_field(U("contract_legs")));
    EXPECT_EQ(obj[U("contract_legs")].size(), 2u);
}

TEST(TradeClientSerializeTest, AlgoOrderFields) {
    TradeClient tc = make_trade_client();
    Contract c = ContractUtil::stock_contract(U("AAPL"), U("USD"));
    Order o = OrderUtil::twap_order(U("ACCT"), c, U("BUY"), 1000LL,
                                    1700000000000LL, 1700003600000LL);

    value obj = value::object(true);
    TradeClient::serialize_advanced_order_fields(obj, o, U("ACCT"));

    EXPECT_EQ(obj[U("algo_strategy")].as_string(), U("TWAP"));
    EXPECT_TRUE(obj.has_field(U("algo_params")));
    EXPECT_EQ(obj[U("algo_params")].size(), 2u);  // start_time, end_time
}

TEST(TradeClientSerializeTest, OcaOrdersFields) {
    TradeClient tc = make_trade_client();
    Contract c = ContractUtil::stock_contract(U("AAPL"), U("USD"));
    Order o = OrderUtil::oca_brackets_order(U("ACCT"), c, U("BUY"), 100LL,
        185.0, U("DAY"), 165.0, 0.0, U("DAY"));

    value obj = value::object(true);
    TradeClient::serialize_advanced_order_fields(obj, o, U("ACCT"));

    EXPECT_TRUE(obj.has_field(U("oca_orders")));
    EXPECT_EQ(obj[U("oca_orders")].size(), 2u);
    // Each sub-order should have account
    EXPECT_EQ(obj[U("oca_orders")][0][U("account")].as_string(), U("ACCT"));
    EXPECT_EQ(obj[U("oca_orders")][1][U("account")].as_string(), U("ACCT"));
}

TEST(TradeClientSerializeTest, AttachedProfitTaker) {
    TradeClient tc = make_trade_client();
    Contract c = ContractUtil::stock_contract(U("AAPL"), U("USD"));
    Order o = OrderUtil::limit_order(U("ACCT"), c, U("BUY"), 100, 175.0);
    OrderUtil::add_profit_taker_order(o, 185.0, U("DAY"), false);

    value obj = value::object(true);
    TradeClient::serialize_advanced_order_fields(obj, o, U("ACCT"));

    EXPECT_EQ(obj[U("attach_type")].as_string(), U("PROFIT"));
    EXPECT_DOUBLE_EQ(obj[U("profit_taker_price")].as_double(), 185.0);
    EXPECT_EQ(obj[U("profit_taker_tif")].as_string(), U("DAY"));
}

TEST(TradeClientSerializeTest, AttachedStopLoss) {
    TradeClient tc = make_trade_client();
    Contract c = ContractUtil::stock_contract(U("AAPL"), U("USD"));
    Order o = OrderUtil::limit_order(U("ACCT"), c, U("BUY"), 100, 175.0);
    OrderUtil::add_stop_loss_order(o, 165.0, U("DAY"));

    value obj = value::object(true);
    TradeClient::serialize_advanced_order_fields(obj, o, U("ACCT"));

    EXPECT_EQ(obj[U("attach_type")].as_string(), U("LOSS"));
    EXPECT_EQ(obj[U("stop_loss_order_type")].as_string(), U("STP"));
    EXPECT_DOUBLE_EQ(obj[U("stop_loss_price")].as_double(), 165.0);
}

TEST(TradeClientSerializeTest, AttachedStopLossLimit) {
    TradeClient tc = make_trade_client();
    Contract c = ContractUtil::stock_contract(U("AAPL"), U("USD"));
    Order o = OrderUtil::limit_order(U("ACCT"), c, U("BUY"), 100, 175.0);
    OrderUtil::add_stop_loss_limit_order(o, 165.0, 164.0, U("DAY"));

    value obj = value::object(true);
    TradeClient::serialize_advanced_order_fields(obj, o, U("ACCT"));

    EXPECT_EQ(obj[U("stop_loss_order_type")].as_string(), U("STP_LMT"));
    EXPECT_DOUBLE_EQ(obj[U("stop_loss_limit_price")].as_double(), 164.0);
}

TEST(TradeClientSerializeTest, AttachedStopLossTrail) {
    TradeClient tc = make_trade_client();
    Contract c = ContractUtil::stock_contract(U("AAPL"), U("USD"));
    Order o = OrderUtil::limit_order(U("ACCT"), c, U("BUY"), 100, 175.0);
    OrderUtil::add_stop_loss_trail_order(o, 5.0, 2.0, U("DAY"));

    value obj = value::object(true);
    TradeClient::serialize_advanced_order_fields(obj, o, U("ACCT"));

    EXPECT_EQ(obj[U("stop_loss_order_type")].as_string(), U("TRAIL"));
    EXPECT_DOUBLE_EQ(obj[U("stop_loss_trailing_percent")].as_double(), 5.0);
    EXPECT_DOUBLE_EQ(obj[U("stop_loss_trailing_amount")].as_double(), 2.0);
}

TEST(TradeClientSerializeTest, AttachedBrackets) {
    TradeClient tc = make_trade_client();
    Contract c = ContractUtil::stock_contract(U("AAPL"), U("USD"));
    Order o = OrderUtil::limit_order(U("ACCT"), c, U("BUY"), 100, 175.0);
    OrderUtil::add_brackets_order(o, 185.0, U("DAY"), true, 165.0, 164.0, U("DAY"));

    value obj = value::object(true);
    EXPECT_NO_THROW(TradeClient::serialize_advanced_order_fields(obj, o, U("ACCT")));

    EXPECT_EQ(obj[U("attach_type")].as_string(), U("BRACKETS"));
    EXPECT_DOUBLE_EQ(obj[U("profit_taker_price")].as_double(), 185.0);
    EXPECT_EQ(obj[U("stop_loss_order_type")].as_string(), U("STP_LMT"));
    EXPECT_DOUBLE_EQ(obj[U("stop_loss_limit_price")].as_double(), 164.0);
}

TEST(TradeClientSerializeTest, EmptyOrder_NoAdvancedFields) {
    TradeClient tc = make_trade_client();
    Order o;  // Default order with no advanced fields

    value obj = value::object(true);
    TradeClient::serialize_advanced_order_fields(obj, o, U("ACCT"));

    // No advanced fields should be present
    EXPECT_FALSE(obj.has_field(U("display_size")));
    EXPECT_FALSE(obj.has_field(U("cash_amount")));
    EXPECT_FALSE(obj.has_field(U("combo_type")));
    EXPECT_FALSE(obj.has_field(U("algo_strategy")));
    EXPECT_FALSE(obj.has_field(U("oca_orders")));
    EXPECT_FALSE(obj.has_field(U("attach_type")));
}

TEST(TradeClientSerializeTest, OcaSubOrderContractFields) {
    TradeClient tc = make_trade_client();
    Contract c = ContractUtil::stock_contract(U("AAPL"), U("USD"));
    c.exchange = U("NASDAQ");
    Order o = OrderUtil::oca_brackets_order(U("ACCT"), c, U("BUY"), 100LL,
        185.0, U("DAY"), 165.0, 0.0, U("DAY"));

    value obj = value::object(true);
    TradeClient::serialize_advanced_order_fields(obj, o, U("ACCT"));

    value oca0 = obj[U("oca_orders")][0];
    EXPECT_EQ(oca0[U("symbol")].as_string(), U("AAPL"));
    EXPECT_EQ(oca0[U("currency")].as_string(), U("USD"));
    EXPECT_EQ(oca0[U("sec_type")].as_string(), U("STK"));
    EXPECT_EQ(oca0[U("exchange")].as_string(), U("NASDAQ"));
    EXPECT_EQ(oca0[U("order_type")].as_string(), U("LMT"));
    EXPECT_EQ(oca0[U("action")].as_string(), U("BUY"));
    EXPECT_EQ(oca0[U("total_quantity")].as_number().to_int64(), 100LL);
    EXPECT_DOUBLE_EQ(oca0[U("limit_price")].as_double(), 185.0);
    EXPECT_EQ(oca0[U("time_in_force")].as_string(), U("DAY"));
}

// ── Asset quote type request builders ─────────────────────────────────────────
// These cover the asset_quote_type additions on the pull side.  Each endpoint is
// asserted twice: the pre-existing overloads must NOT emit the field (so old
// callers keep sending the exact same payload), the new ones must emit the enum
// name verbatim -- the server maps the name, not a code.

TEST(AssetQuoteTypeRequestTest, Positions_WithQuoteType) {
    TradeClient tc = make_trade_client();
    value obj = tc.build_positions_request(U("ACCT"), U(""), U("ALL"), U("ALL"), U(""),
                                           value::array(), -1, U(""), U(""), U("OVERNIGHT"));

    ASSERT_TRUE(obj.has_field(U("asset_quote_type")));
    EXPECT_EQ(obj[U("asset_quote_type")].as_string(), U("OVERNIGHT"));
    EXPECT_EQ(obj[U("account")].as_string(), U("ACCT"));
}

TEST(AssetQuoteTypeRequestTest, Positions_WithoutQuoteType) {
    TradeClient tc = make_trade_client();
    value obj = tc.build_positions_request(U("ACCT"), U(""), U("ALL"), U("ALL"), U(""),
                                           value::array(), -1, U(""), U(""), U(""));

    EXPECT_FALSE(obj.has_field(U("asset_quote_type")));
}

TEST(AssetQuoteTypeRequestTest, Positions_KeepsExistingFilters) {
    TradeClient tc = make_trade_client();
    value obj = tc.build_positions_request(U("ACCT"), U("OPT"), U("USD"), U("US"), U("AAPL"),
                                           value::array(), 1700000000LL, U("175.0"), U("CALL"), U("RTH"));

    EXPECT_EQ(obj[U("sec_type")].as_string(), U("OPT"));
    EXPECT_EQ(obj[U("currency")].as_string(), U("USD"));
    EXPECT_EQ(obj[U("market")].as_string(), U("US"));
    EXPECT_EQ(obj[U("symbol")].as_string(), U("AAPL"));
    EXPECT_EQ(obj[U("strike")].as_string(), U("175.0"));
    EXPECT_EQ(obj[U("right")].as_string(), U("CALL"));
    EXPECT_EQ(obj[U("expiry")].as_number().to_int64(), 1700000000LL);
    EXPECT_EQ(obj[U("asset_quote_type")].as_string(), U("RTH"));
}

TEST(AssetQuoteTypeRequestTest, PrimeAsset_WithQuoteType) {
    TradeClient tc = make_trade_client();
    value obj = tc.build_prime_asset_request(U("ACCT"), U("USD"), U("ETH"));

    EXPECT_EQ(obj[U("base_currency")].as_string(), U("USD"));
    EXPECT_EQ(obj[U("asset_quote_type")].as_string(), U("ETH"));
}

TEST(AssetQuoteTypeRequestTest, PrimeAsset_WithoutQuoteType) {
    TradeClient tc = make_trade_client();
    value obj = tc.build_prime_asset_request(U("ACCT"), U("USD"), U(""));

    EXPECT_EQ(obj[U("base_currency")].as_string(), U("USD"));
    EXPECT_FALSE(obj.has_field(U("asset_quote_type")));
}

TEST(AssetQuoteTypeRequestTest, Asset_WithQuoteType) {
    TradeClient tc = make_trade_client();
    value obj = tc.build_asset_request(U("ACCT"), true, false, U("OVERNIGHT"));

    // json::value's bool constructor is explicit, so these two go on the wire as
    // the numbers 1 / 0 rather than true / false. Long-standing behaviour, asserted
    // here so the refactoring above is provably payload-preserving.
    EXPECT_EQ(obj[U("segment")].as_integer(), 1);
    EXPECT_EQ(obj[U("market_value")].as_integer(), 0);
    EXPECT_EQ(obj[U("asset_quote_type")].as_string(), U("OVERNIGHT"));
}

TEST(AssetQuoteTypeRequestTest, Asset_WithoutQuoteType) {
    TradeClient tc = make_trade_client();
    value obj = tc.build_asset_request(U("ACCT"), true, false, U(""));

    EXPECT_FALSE(obj.has_field(U("asset_quote_type")));
}

TEST(AssetQuoteTypeRequestTest, AggregateAssets_WithQuoteType) {
    TradeClient tc = make_trade_client();
    value obj = tc.build_aggregate_assets_request(U("ACCT"), U("SEC"), U("USD"), U("OVERNIGHT"));

    EXPECT_EQ(obj[U("seg_type")].as_string(), U("SEC"));
    EXPECT_EQ(obj[U("base_currency")].as_string(), U("USD"));
    EXPECT_EQ(obj[U("asset_quote_type")].as_string(), U("OVERNIGHT"));
}

TEST(AssetQuoteTypeRequestTest, AggregateAssets_WithoutQuoteType) {
    TradeClient tc = make_trade_client();
    value obj = tc.build_aggregate_assets_request(U("ACCT"), U("SEC"), U("USD"), U(""));

    EXPECT_FALSE(obj.has_field(U("asset_quote_type")));
}
