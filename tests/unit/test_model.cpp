#include <gtest/gtest.h>
#include "tigerapi/model.h"
#include "tigerapi/contract_util.h"
#include "tigerapi/order_util.h"
#include <string>

using namespace TIGER_API;

// ── Contract constructors ──────────────────────────────────────────────────────

TEST(ModelTest, Contract_DefaultConstructor) {
    Contract c;
    EXPECT_EQ(c.sec_type, U(""));
    EXPECT_EQ(c.multiplier, 0);
    EXPECT_EQ(c.contract_id, 0);
}

TEST(ModelTest, Contract_StockConstructor) {
    Contract c(U("STK"), U("AAPL"), U("USD"), U("AAPL"), U("NASDAQ"), 265598);
    EXPECT_EQ(c.sec_type, U("STK"));
    EXPECT_EQ(c.symbol, U("AAPL"));
    EXPECT_EQ(c.currency, U("USD"));
    EXPECT_EQ(c.local_symbol, U("AAPL"));
    EXPECT_EQ(c.exchange, U("NASDAQ"));
    EXPECT_EQ(c.contract_id, 265598);
}

TEST(ModelTest, Contract_OptionConstructor) {
    Contract c(U("OPT"), U("AAPL"), U("220414"), U("175.0"), U("CALL"),
               U("USD"), 100, U(""), 0);
    EXPECT_EQ(c.sec_type, U("OPT"));
    EXPECT_EQ(c.symbol, U("AAPL"));
    EXPECT_EQ(c.expiry, U("220414"));
    EXPECT_EQ(c.strike, U("175.0"));
    EXPECT_EQ(c.right, U("CALL"));
    EXPECT_EQ(c.currency, U("USD"));
    EXPECT_EQ(c.multiplier, 100);
}

TEST(ModelTest, Contract_FutureConstructor) {
    Contract c(U("FUT"), U("CL"), U("20250117"), 1000, U("202501"), U("USD"), U("NYMEX"), U(""));
    EXPECT_EQ(c.sec_type, U("FUT"));
    EXPECT_EQ(c.symbol, U("CL"));
    EXPECT_EQ(c.expiry, U("20250117"));
    EXPECT_EQ(c.multiplier, 1000);
    EXPECT_EQ(c.contract_month, U("202501"));
    EXPECT_EQ(c.currency, U("USD"));
    EXPECT_EQ(c.exchange, U("NYMEX"));
}

// ── ContractLeg ─────────────────────────────────────────────────────────────────

TEST(ModelTest, ContractLeg_DefaultConstructor) {
    ContractLeg leg;
    EXPECT_EQ(leg.ratio, 0);
}

TEST(ModelTest, ContractLeg_SimpleConstructor) {
    ContractLeg leg(U("OPT"), U("AAPL"), U("BUY"), 1);
    EXPECT_EQ(leg.sec_type, U("OPT"));
    EXPECT_EQ(leg.symbol, U("AAPL"));
    EXPECT_EQ(leg.action, U("BUY"));
    EXPECT_EQ(leg.ratio, 1);
}

TEST(ModelTest, ContractLeg_FullConstructor) {
    ContractLeg leg(U("OPT"), U("AAPL"), U("175.0"), U("220414"), U("CALL"), U("BUY"), 1);
    EXPECT_EQ(leg.strike, U("175.0"));
    EXPECT_EQ(leg.expiry, U("220414"));
    EXPECT_EQ(leg.right, U("CALL"));
}

TEST(ModelTest, ContractLeg_ToJson_Full) {
    ContractLeg leg(U("OPT"), U("AAPL"), U("175.0"), U("220414"), U("CALL"), U("BUY"), 1);
    web::json::value j = leg.to_json();
    EXPECT_EQ(j[U("symbol")].as_string(), U("AAPL"));
    EXPECT_EQ(j[U("sec_type")].as_string(), U("OPT"));
    EXPECT_EQ(j[U("expiry")].as_string(), U("220414"));
    EXPECT_EQ(j[U("strike")].as_string(), U("175.0"));
    EXPECT_EQ(j[U("right")].as_string(), U("CALL"));
    EXPECT_EQ(j[U("action")].as_string(), U("BUY"));
    EXPECT_EQ(j[U("ratio")].as_integer(), 1);
}

TEST(ModelTest, ContractLeg_ToJson_Empty) {
    ContractLeg leg;
    web::json::value j = leg.to_json();
    // Empty leg should produce empty object (all fields are empty/zero)
    EXPECT_FALSE(j.has_field(U("symbol")));
    EXPECT_FALSE(j.has_field(U("sec_type")));
    EXPECT_FALSE(j.has_field(U("ratio")));
}

// ── AlgoParam ──────────────────────────────────────────────────────────────────

TEST(ModelTest, AlgoParam_DefaultConstructor) {
    AlgoParam ap;
    EXPECT_TRUE(ap.tag.empty());
    EXPECT_TRUE(ap.value.empty());
}

TEST(ModelTest, AlgoParam_Constructor) {
    AlgoParam ap(U("start_time"), U("1700000000000"));
    EXPECT_EQ(ap.tag, U("start_time"));
    EXPECT_EQ(ap.value, U("1700000000000"));
}

TEST(ModelTest, AlgoParam_ToJson) {
    AlgoParam ap(U("end_time"), U("1700003600000"));
    web::json::value j = ap.to_json();
    EXPECT_EQ(j[U("tag")].as_string(), U("end_time"));
    EXPECT_EQ(j[U("value")].as_string(), U("1700003600000"));
}

// ── Order from JSON ────────────────────────────────────────────────────────────

TEST(ModelTest, Order_FromJson_BasicFields) {
    web::json::value j = web::json::value::object(true);
    j[U("id")] = web::json::value::number(12345);
    j[U("orderType")] = web::json::value::string(U("LMT"));
    j[U("account")] = web::json::value::string(U("DU123"));
    j[U("action")] = web::json::value::string(U("BUY"));
    j[U("limitPrice")] = web::json::value::number(175.5);
    j[U("auxPrice")] = web::json::value::number(170.0);
    j[U("totalQuantity")] = web::json::value::number(100);

    Order o(j);
    EXPECT_EQ(o.id, 12345ULL);
    EXPECT_EQ(o.order_type, U("LMT"));
    EXPECT_EQ(o.account, U("DU123"));
    EXPECT_EQ(o.action, U("BUY"));
    EXPECT_DOUBLE_EQ(o.limit_price, 175.5);
    EXPECT_DOUBLE_EQ(o.aux_price, 170.0);
    EXPECT_EQ(o.total_quantity, 100LL);
}

TEST(ModelTest, Order_FromJson_ContractFields) {
    web::json::value j = web::json::value::object(true);
    j[U("symbol")] = web::json::value::string(U("AAPL"));
    j[U("secType")] = web::json::value::string(U("STK"));
    j[U("market")] = web::json::value::string(U("US"));
    j[U("currency")] = web::json::value::string(U("USD"));

    Order o(j);
    EXPECT_EQ(o.contract.symbol, U("AAPL"));
    EXPECT_EQ(o.contract.sec_type, U("STK"));
    EXPECT_EQ(o.contract.market, U("US"));
    EXPECT_EQ(o.contract.currency, U("USD"));
}

TEST(ModelTest, Order_FromJson_AllAdvancedFields) {
    web::json::value j = web::json::value::object(true);
    j[U("id")] = web::json::value::number(999);
    j[U("orderType")] = web::json::value::string(U("LMT"));
    j[U("account")] = web::json::value::string(U("ACCT"));
    j[U("action")] = web::json::value::string(U("SELL"));
    j[U("limitPrice")] = web::json::value::number(200.0);
    j[U("auxPrice")] = web::json::value::number(195.0);
    j[U("trailingPercent")] = web::json::value::number(5.0);
    j[U("timeInForce")] = web::json::value::string(U("GTC"));
    j[U("outsideRth")] = web::json::value::boolean(true);
    j[U("adjustLimit")] = web::json::value::number(0.001);
    j[U("userMark")] = web::json::value::string(U("mark123"));
    j[U("expireTime")] = web::json::value::number(1700000000000LL);
    j[U("status")] = web::json::value::string(U("Filled"));
    j[U("parentId")] = web::json::value::number(0);
    j[U("openTime")] = web::json::value::number(1699000000000LL);
    j[U("reason")] = web::json::value::string(U(""));
    j[U("latestTime")] = web::json::value::number(1699500000000LL);
    j[U("updateTime")] = web::json::value::number(1699600000000LL);
    j[U("filledQuantity")] = web::json::value::number(100);
    j[U("totalQuantity")] = web::json::value::number(100);
    j[U("avgFillPrice")] = web::json::value::number(198.5);
    j[U("realizedPnl")] = web::json::value::number(350.0);
    j[U("commission")] = web::json::value::number(1.5);
    j[U("gst")] = web::json::value::number(0.15);
    j[U("displaySize")] = web::json::value::number(50);
    j[U("minDisplaySize")] = web::json::value::number(25);
    j[U("checkIntervals")] = web::json::value::number(5);
    j[U("priceType")] = web::json::value::string(U("LIMIT_PRICE"));
    j[U("startTime")] = web::json::value::number(1000);
    j[U("endTime")] = web::json::value::number(2000);
    j[U("totalQuantityScale")] = web::json::value::number(1);
    j[U("filledQuantityScale")] = web::json::value::number(1);
    j[U("cashAmount")] = web::json::value::number(5000.0);
    j[U("comboType")] = web::json::value::string(U("VERTICAL"));
    j[U("attachType")] = web::json::value::string(U("PROFIT"));
    j[U("profitTakerOrderId")] = web::json::value::number(888);
    j[U("profitTakerPrice")] = web::json::value::number(210.0);
    j[U("profitTakerTif")] = web::json::value::string(U("DAY"));
    j[U("profitTakerRth")] = web::json::value::boolean(true);
    j[U("stopLossOrderType")] = web::json::value::string(U("STP"));
    j[U("stopLossOrderId")] = web::json::value::number(777);
    j[U("stopLossPrice")] = web::json::value::number(190.0);
    j[U("stopLossLimitPrice")] = web::json::value::number(189.0);
    j[U("stopLossTif")] = web::json::value::string(U("DAY"));
    j[U("stopLossTrailingPercent")] = web::json::value::number(3.0);
    j[U("stopLossTrailingAmount")] = web::json::value::number(5.0);

    Order o(j);
    EXPECT_EQ(o.id, 999ULL);
    EXPECT_EQ(o.order_type, U("LMT"));
    EXPECT_DOUBLE_EQ(o.trailing_percent, 5.0);
    EXPECT_EQ(o.time_in_force, U("GTC"));
    EXPECT_TRUE(o.outside_rth);
    EXPECT_DOUBLE_EQ(o.adjust_limit, 0.001);
    EXPECT_EQ(o.user_mark, U("mark123"));
    EXPECT_EQ(o.expire_time, 1700000000000);
    EXPECT_EQ(o.status, U("Filled"));
    EXPECT_EQ(o.filled_quantity, 100);
    EXPECT_DOUBLE_EQ(o.avg_fill_price, 198.5);
    EXPECT_DOUBLE_EQ(o.realized_pnl, 350.0);
    EXPECT_DOUBLE_EQ(o.commission, 1.5);
    EXPECT_DOUBLE_EQ(o.gst, 0.15);
    EXPECT_EQ(o.display_size, 50LL);
    EXPECT_EQ(o.min_display_size, 25LL);
    EXPECT_EQ(o.check_intervals, 5LL);
    EXPECT_EQ(o.price_type, U("LIMIT_PRICE"));
    EXPECT_EQ(o.start_time, 1000LL);
    EXPECT_EQ(o.end_time, 2000LL);
    EXPECT_EQ(o.total_quantity_scale, 1);
    EXPECT_EQ(o.filled_quantity_scale, 1);
    EXPECT_DOUBLE_EQ(o.cash_amount, 5000.0);
    EXPECT_EQ(o.combo_type, U("VERTICAL"));
    EXPECT_EQ(o.attach_type, U("PROFIT"));
    EXPECT_EQ(o.profit_taker_order_id, 888);
    EXPECT_DOUBLE_EQ(o.profit_taker_price, 210.0);
    EXPECT_EQ(o.profit_taker_tif, U("DAY"));
    EXPECT_TRUE(o.profit_taker_rth);
    EXPECT_EQ(o.stop_loss_order_type, U("STP"));
    EXPECT_EQ(o.stop_loss_order_id, 777);
    EXPECT_DOUBLE_EQ(o.stop_loss_price, 190.0);
    EXPECT_DOUBLE_EQ(o.stop_loss_limit_price, 189.0);
    EXPECT_EQ(o.stop_loss_tif, U("DAY"));
    EXPECT_DOUBLE_EQ(o.stop_loss_trailing_percent, 3.0);
    EXPECT_DOUBLE_EQ(o.stop_loss_trailing_amount, 5.0);
}

TEST(ModelTest, Order_FromJson_AlgoParams) {
    web::json::value j = web::json::value::object(true);
    j[U("algoStrategy")] = web::json::value::string(U("TWAP"));
    web::json::value params = web::json::value::array(2);
    web::json::value p1 = web::json::value::object(true);
    p1[U("tag")] = web::json::value::string(U("start_time"));
    p1[U("value")] = web::json::value::string(U("1000"));
    web::json::value p2 = web::json::value::object(true);
    p2[U("tag")] = web::json::value::string(U("end_time"));
    p2[U("value")] = web::json::value::string(U("2000"));
    params[0] = p1;
    params[1] = p2;
    j[U("algoParams")] = params;

    Order o(j);
    EXPECT_EQ(o.algo_strategy, U("TWAP"));
    EXPECT_EQ(o.algo_params.size(), 2u);
    EXPECT_EQ(o.algo_params[0].tag, U("start_time"));
    EXPECT_EQ(o.algo_params[0].value, U("1000"));
    EXPECT_EQ(o.algo_params[1].tag, U("end_time"));
    EXPECT_EQ(o.algo_params[1].value, U("2000"));
}

TEST(ModelTest, Order_FromJson_ContractLegs) {
    web::json::value j = web::json::value::object(true);
    web::json::value legs = web::json::value::array(1);
    web::json::value leg = web::json::value::object(true);
    leg[U("symbol")] = web::json::value::string(U("AAPL"));
    leg[U("secType")] = web::json::value::string(U("OPT"));
    leg[U("expiry")] = web::json::value::string(U("220414"));
    leg[U("strike")] = web::json::value::string(U("175"));
    leg[U("right")] = web::json::value::string(U("CALL"));
    leg[U("action")] = web::json::value::string(U("BUY"));
    leg[U("ratio")] = web::json::value::number(1);
    legs[0] = leg;
    j[U("contractLegs")] = legs;

    Order o(j);
    EXPECT_EQ(o.contract_legs.size(), 1u);
    EXPECT_EQ(o.contract_legs[0].symbol, U("AAPL"));
    EXPECT_EQ(o.contract_legs[0].sec_type, U("OPT"));
    EXPECT_EQ(o.contract_legs[0].ratio, 1);
}

TEST(ModelTest, Order_FromJson_OcaOrders) {
    web::json::value j = web::json::value::object(true);
    web::json::value oca = web::json::value::array(1);
    web::json::value sub = web::json::value::object(true);
    sub[U("orderType")] = web::json::value::string(U("LMT"));
    sub[U("id")] = web::json::value::number(42);
    oca[0] = sub;
    j[U("ocaOrders")] = oca;

    Order o(j);
    EXPECT_EQ(o.oca_orders.size(), 1u);
    EXPECT_EQ(o.oca_orders[0].order_type, U("LMT"));
    EXPECT_EQ(o.oca_orders[0].id, 42ULL);
}

TEST(ModelTest, Order_FromJson_SubIds) {
    web::json::value j = web::json::value::object(true);
    web::json::value subIds = web::json::value::array(2);
    subIds[0] = web::json::value::number(1);
    subIds[1] = web::json::value::number(2);
    j[U("subIds")] = subIds;

    Order o(j);
    EXPECT_FALSE(o.sub_ids.is_null());
    EXPECT_EQ(o.sub_ids.size(), 2u);
}

TEST(ModelTest, Order_ToString) {
    Order o;
    o.id = 123;
    o.status = U("Filled");
    o.total_quantity = 100;
    o.limit_price = 175.5;
    utility::string_t s = o.to_string();
    EXPECT_FALSE(s.empty());
    EXPECT_NE(s.find(U("123")), utility::string_t::npos);
}

TEST(ModelTest, Order_AddAlgoParam_EmptyTagIgnored) {
    Order o;
    o.add_algo_param(U(""), U("value"));
    o.add_algo_param(U("tag"), U(""));
    EXPECT_EQ(o.algo_params.size(), 0u);
}

TEST(ModelTest, Order_AddAlgoParam_Valid) {
    Order o;
    o.add_algo_param(U("start_time"), U("1000"));
    EXPECT_EQ(o.algo_params.size(), 1u);
    EXPECT_EQ(o.algo_params[0].tag, U("start_time"));
}

// ── Position from JSON ─────────────────────────────────────────────────────────

TEST(ModelTest, Position_FromJson_Basic) {
    web::json::value j = web::json::value::object(true);
    j[U("account")] = web::json::value::string(U("ACCT"));
    j[U("symbol")] = web::json::value::string(U("AAPL"));
    j[U("secType")] = web::json::value::string(U("STK"));
    j[U("market")] = web::json::value::string(U("US"));
    j[U("currency")] = web::json::value::string(U("USD"));
    j[U("identifier")] = web::json::value::string(U("AAPL"));
    j[U("contractId")] = web::json::value::number(265598);
    j[U("multiplier")] = web::json::value::number(1);
    j[U("position")] = web::json::value::number(100);
    j[U("averageCost")] = web::json::value::number(170.0);
    j[U("marketValue")] = web::json::value::number(17500.0);
    j[U("realizedPnl")] = web::json::value::number(500.0);
    j[U("unrealizedPnl")] = web::json::value::number(500.0);
    j[U("latestPrice")] = web::json::value::number(175.0);
    j[U("updateTimestamp")] = web::json::value::number(1700000000000LL);
    j[U("status")] = web::json::value::number(1);

    Position p(j);
    EXPECT_EQ(p.account, U("ACCT"));
    EXPECT_EQ(p.contract.symbol, U("AAPL"));
    EXPECT_EQ(p.contract.sec_type, U("STK"));
    EXPECT_EQ(p.contract.market, U("US"));
    EXPECT_EQ(p.contract.currency, U("USD"));
    EXPECT_EQ(p.contract.contract_id, 265598);
    EXPECT_EQ(p.position, 100LL);
    EXPECT_DOUBLE_EQ(p.average_cost, 170.0);
    EXPECT_DOUBLE_EQ(p.market_value, 17500.0);
    EXPECT_DOUBLE_EQ(p.realized_pnl, 500.0);
    EXPECT_DOUBLE_EQ(p.unrealized_pnl, 500.0);
    EXPECT_DOUBLE_EQ(p.latest_price, 175.0);
    EXPECT_EQ(p.update_timestamp, 1700000000000LL);
    EXPECT_EQ(p.status, 1);
}

TEST(ModelTest, Position_DefaultConstructor) {
    Position p;
    // Default values - should not crash
    EXPECT_TRUE(p.account.empty());
}

TEST(ModelTest, Position_ToString) {
    Position p;
    p.contract.symbol = U("AAPL");
    p.position = 100;
    utility::string_t s = p.to_string();
    EXPECT_FALSE(s.empty());
    EXPECT_NE(s.find(U("AAPL")), utility::string_t::npos);
}

// ── CurrencyAsset ──────────────────────────────────────────────────────────────

TEST(ModelTest, CurrencyAsset_FromJson) {
    web::json::value j = web::json::value::object(true);
    j[U("cashAvailableForTrade")] = web::json::value::number(10000.0);
    j[U("cashBalance")] = web::json::value::number(5000.0);
    j[U("currency")] = web::json::value::string(U("USD"));
    j[U("grossPositionValue")] = web::json::value::number(20000.0);
    j[U("optionMarketValue")] = web::json::value::number(3000.0);
    j[U("realizedPL")] = web::json::value::number(500.0);
    j[U("stockMarketValue")] = web::json::value::number(15000.0);
    j[U("unrealizedPL")] = web::json::value::number(800.0);

    CurrencyAsset ca(j);
    EXPECT_DOUBLE_EQ(ca.cash_available_for_trade, 10000.0);
    EXPECT_DOUBLE_EQ(ca.cash_balance, 5000.0);
    EXPECT_EQ(ca.currency, U("USD"));
    EXPECT_DOUBLE_EQ(ca.gross_position_value, 20000.0);
    EXPECT_DOUBLE_EQ(ca.option_market_value, 3000.0);
    EXPECT_DOUBLE_EQ(ca.realized_pl, 500.0);
    EXPECT_DOUBLE_EQ(ca.stock_market_value, 15000.0);
    EXPECT_DOUBLE_EQ(ca.unrealized_pl, 800.0);
}

TEST(ModelTest, CurrencyAsset_DefaultConstructor) {
    CurrencyAsset ca;
    EXPECT_DOUBLE_EQ(ca.cash_balance, 0.0);
    EXPECT_TRUE(ca.currency.empty());
}

// ── Segment ─────────────────────────────────────────────────────────────────────

TEST(ModelTest, Segment_FromJson_AllFields) {
    web::json::value j = web::json::value::object(true);
    j[U("buyingPower")] = web::json::value::number(50000.0);
    j[U("capability")] = web::json::value::string(U("trade"));
    j[U("cashAvailableForTrade")] = web::json::value::number(10000.0);
    j[U("cashAvailableForWithdrawal")] = web::json::value::number(8000.0);
    j[U("cashBalance")] = web::json::value::number(5000.0);
    j[U("category")] = web::json::value::string(U("SEC"));
    j[U("currency")] = web::json::value::string(U("USD"));
    j[U("equityWithLoan")] = web::json::value::number(20000.0);
    j[U("excessLiquidation")] = web::json::value::number(5000.0);
    j[U("grossPositionValue")] = web::json::value::number(15000.0);
    j[U("initMargin")] = web::json::value::number(3000.0);
    j[U("leverage")] = web::json::value::number(2.0);
    j[U("lockedFunds")] = web::json::value::number(1000.0);
    j[U("maintainMargin")] = web::json::value::number(2500.0);
    j[U("netLiquidation")] = web::json::value::number(25000.0);
    j[U("overnightLiquidation")] = web::json::value::number(24000.0);
    j[U("overnightMargin")] = web::json::value::number(2800.0);
    j[U("realizedPL")] = web::json::value::number(500.0);
    j[U("totalTodayPL")] = web::json::value::number(300.0);
    j[U("unrealizedPL")] = web::json::value::number(800.0);
    j[U("unrealizedPLByCostOfCarry")] = web::json::value::number(100.0);

    Segment s(j);
    EXPECT_DOUBLE_EQ(s.buying_power, 50000.0);
    EXPECT_EQ(s.capability, U("trade"));
    EXPECT_DOUBLE_EQ(s.cash_available_for_trade, 10000.0);
    EXPECT_DOUBLE_EQ(s.cash_available_for_withdrawal, 8000.0);
    EXPECT_DOUBLE_EQ(s.cash_balance, 5000.0);
    EXPECT_EQ(s.category, U("SEC"));
    EXPECT_EQ(s.currency, U("USD"));
    EXPECT_DOUBLE_EQ(s.equity_with_loan, 20000.0);
    EXPECT_DOUBLE_EQ(s.excess_liquidation, 5000.0);
    EXPECT_DOUBLE_EQ(s.gross_position_value, 15000.0);
    EXPECT_DOUBLE_EQ(s.init_margin, 3000.0);
    EXPECT_DOUBLE_EQ(s.leverage, 2.0);
    EXPECT_DOUBLE_EQ(s.locked_funds, 1000.0);
    EXPECT_DOUBLE_EQ(s.maintain_margin, 2500.0);
    EXPECT_DOUBLE_EQ(s.net_liquidation, 25000.0);
    EXPECT_DOUBLE_EQ(s.overnight_liquidation, 24000.0);
    EXPECT_DOUBLE_EQ(s.overnight_margin, 2800.0);
    EXPECT_DOUBLE_EQ(s.realized_pl, 500.0);
    EXPECT_DOUBLE_EQ(s.total_today_pl, 300.0);
    EXPECT_DOUBLE_EQ(s.unrealized_pl, 800.0);
    EXPECT_DOUBLE_EQ(s.unrealized_plby_cost_of_carry, 100.0);
}

TEST(ModelTest, Segment_DefaultConstructor) {
    Segment s;
    EXPECT_DOUBLE_EQ(s.buying_power, 0.0);
    EXPECT_TRUE(s.category.empty());
}

// ── KlineItem ──────────────────────────────────────────────────────────────────

TEST(ModelTest, KlineItem_FromJson) {
    web::json::value j = web::json::value::object(true);
    j[U("open")] = web::json::value::number(170.0);
    j[U("high")] = web::json::value::number(175.0);
    j[U("low")] = web::json::value::number(168.0);
    j[U("close")] = web::json::value::number(172.0);
    j[U("time")] = web::json::value::number(1700000000000LL);
    j[U("lastTime")] = web::json::value::number(1700000060000LL);
    j[U("volume")] = web::json::value::number(1000000LL);
    j[U("settlement")] = web::json::value::number(171.5);
    j[U("openInterest")] = web::json::value::number(50000LL);

    KlineItem ki;
    ki.from_json(j);
    EXPECT_DOUBLE_EQ(ki.open, 170.0);
    EXPECT_DOUBLE_EQ(ki.high, 175.0);
    EXPECT_DOUBLE_EQ(ki.low, 168.0);
    EXPECT_DOUBLE_EQ(ki.close, 172.0);
    EXPECT_EQ(ki.time, 1700000000000LL);
    EXPECT_EQ(ki.last_time, 1700000060000LL);
    EXPECT_EQ(ki.volume, 1000000LL);
    EXPECT_DOUBLE_EQ(ki.settlement, 171.5);
    EXPECT_EQ(ki.open_interest, 50000LL);
}

TEST(ModelTest, KlineItem_DefaultValues) {
    KlineItem ki;
    EXPECT_DOUBLE_EQ(ki.open, 0.0);
    EXPECT_DOUBLE_EQ(ki.high, 0.0);
    EXPECT_EQ(ki.volume, 0LL);
}

// ── Kline ──────────────────────────────────────────────────────────────────────

TEST(ModelTest, Kline_ToString) {
    Kline k;
    k.symbol = U("AAPL");
    EXPECT_NE(k.to_string().find(U("AAPL")), utility::string_t::npos);
}

// ── RealtimeQuote ──────────────────────────────────────────────────────────────

TEST(ModelTest, RealtimeQuote_FromJson_AllFields) {
    web::json::value j = web::json::value::object(true);
    j[U("open")] = web::json::value::number(170.0);
    j[U("high")] = web::json::value::number(175.0);
    j[U("low")] = web::json::value::number(168.0);
    j[U("close")] = web::json::value::number(172.0);
    j[U("volume")] = web::json::value::number(1000000LL);
    j[U("adjPreClose")] = web::json::value::number(169.0);
    j[U("preClose")] = web::json::value::number(169.5);
    j[U("askPrice")] = web::json::value::number(172.1);
    j[U("askSize")] = web::json::value::number(500);
    j[U("bidPrice")] = web::json::value::number(171.9);
    j[U("bidSize")] = web::json::value::number(400);
    j[U("latestPrice")] = web::json::value::number(172.0);
    j[U("latestTime")] = web::json::value::number(1700000000000LL);
    j[U("latestSize")] = web::json::value::number(100);
    j[U("status")] = web::json::value::string(U("Active"));
    j[U("symbol")] = web::json::value::string(U("AAPL"));
    j[U("contractCode")] = web::json::value::string(U("CLZ5"));
    j[U("openInterest")] = web::json::value::number(50000LL);
    j[U("limitDown")] = web::json::value::number(0);
    j[U("limitUp")] = web::json::value::number(1);

    RealtimeQuote rq;
    rq.from_json(j);
    EXPECT_DOUBLE_EQ(rq.open, 170.0);
    EXPECT_DOUBLE_EQ(rq.high, 175.0);
    EXPECT_DOUBLE_EQ(rq.low, 168.0);
    EXPECT_DOUBLE_EQ(rq.close, 172.0);
    EXPECT_EQ(rq.volume, 1000000LL);
    EXPECT_DOUBLE_EQ(rq.adj_pre_close, 169.0);
    EXPECT_DOUBLE_EQ(rq.pre_close, 169.5);
    EXPECT_DOUBLE_EQ(rq.ask_price, 172.1);
    EXPECT_DOUBLE_EQ(rq.ask_size, 500);
    EXPECT_DOUBLE_EQ(rq.bid_price, 171.9);
    EXPECT_DOUBLE_EQ(rq.bid_size, 400);
    EXPECT_DOUBLE_EQ(rq.latest_price, 172.0);
    EXPECT_EQ(rq.latest_time, 1700000000000LL);
    EXPECT_EQ(rq.latest_size, 100LL);
    EXPECT_EQ(rq.status, U("Active"));
    EXPECT_EQ(rq.symbol, U("AAPL"));
    EXPECT_EQ(rq.contract_code, U("CLZ5"));
    EXPECT_EQ(rq.open_interest, 50000LL);
    EXPECT_EQ(rq.limit_down, 0);
    EXPECT_EQ(rq.limit_up, 1);
}

TEST(ModelTest, RealtimeQuote_DefaultValues) {
    RealtimeQuote rq;
    EXPECT_DOUBLE_EQ(rq.open, 0.0);
    EXPECT_EQ(rq.volume, 0LL);
    EXPECT_TRUE(rq.symbol.empty());
}

TEST(ModelTest, RealtimeQuote_ToString) {
    RealtimeQuote rq;
    rq.symbol = U("AAPL");
    EXPECT_NE(rq.to_string().find(U("AAPL")), utility::string_t::npos);
}

// ── TradeTickItem ──────────────────────────────────────────────────────────────

TEST(ModelTest, TradeTickItem_DefaultConstructor) {
    TradeTickItem item;
    EXPECT_EQ(item.price, 0.0);
    EXPECT_EQ(item.volume, 0);
    EXPECT_TRUE(item.tick_type.empty());
}

TEST(ModelTest, TradeTickItem_GettersSetters) {
    TradeTickItem item;
    item.set_ticktype("trade");
    item.set_price(175.5);
    item.set_volume(100);
    item.set_partcode("n");
    item.set_partcodename("NYSE");
    item.set_cond("regular");
    item.set_time("10:30:00");
    item.set_sn("12345");

    EXPECT_EQ(item.get_ticktype(), "trade");
    EXPECT_DOUBLE_EQ(item.get_price(), 175.5);
    EXPECT_EQ(item.get_volume(), 100LL);
    EXPECT_EQ(item.get_partcode(), "n");
    EXPECT_EQ(item.get_partcodename(), "NYSE");
    EXPECT_EQ(item.get_cond(), "regular");
    EXPECT_EQ(item.get_time(), "10:30:00");
    EXPECT_EQ(item.get_sn(), "12345");
}

TEST(ModelTest, TradeTickItem_ToString) {
    TradeTickItem item;
    item.set_ticktype("trade");
    item.set_price(175.5);
    item.set_volume(100);
    std::string s = item.to_string();
    EXPECT_FALSE(s.empty());
    EXPECT_NE(s.find("trade"), std::string::npos);
    EXPECT_NE(s.find("175.5"), std::string::npos);
}

// ── TradeTick ──────────────────────────────────────────────────────────────────

TEST(ModelTest, TradeTick_DefaultConstructor) {
    TradeTick tt;
    EXPECT_TRUE(tt.get_symbol().empty());
    EXPECT_TRUE(tt.get_ticks().empty());
}

TEST(ModelTest, TradeTick_GettersSetters) {
    TradeTick tt;
    tt.set_symbol("AAPL");
    tt.set_sectype("STK");
    tt.set_quotelevel("us_basic");
    tt.set_timestamp("20240101");

    EXPECT_EQ(tt.get_symbol(), "AAPL");
    EXPECT_EQ(tt.get_sectype(), "STK");
    EXPECT_EQ(tt.get_quotelevel(), "us_basic");
    EXPECT_EQ(tt.get_timestamp(), "20240101");
}

TEST(ModelTest, TradeTick_AddTick) {
    TradeTick tt;
    TradeTickItem item;
    item.set_price(175.0);
    tt.addTick(item);
    EXPECT_EQ(tt.get_ticks().size(), 1u);
    EXPECT_DOUBLE_EQ(tt.get_ticks()[0].get_price(), 175.0);
}

TEST(ModelTest, TradeTick_ToString) {
    TradeTick tt;
    tt.set_symbol("AAPL");
    tt.set_sectype("STK");
    TradeTickItem item;
    item.set_price(175.0);
    tt.addTick(item);
    std::string s = tt.to_string();
    EXPECT_FALSE(s.empty());
    EXPECT_NE(s.find("AAPL"), std::string::npos);
}

// ── PortfolioAccount ───────────────────────────────────────────────────────────

TEST(ModelTest, PortfolioAccount_DefaultConstructor) {
    PortfolioAccount pa;
    EXPECT_TRUE(pa.account.empty());
    EXPECT_TRUE(pa.segments.empty());
}

TEST(ModelTest, PortfolioAccount_ToString) {
    PortfolioAccount pa;
    pa.account = U("ACCT");
    pa.update_timestamp = 1700000000000;
    utility::string_t s = pa.to_string();
    EXPECT_FALSE(s.empty());
    EXPECT_NE(s.find(U("ACCT")), utility::string_t::npos);
}
