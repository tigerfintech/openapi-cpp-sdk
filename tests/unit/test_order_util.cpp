#include <gtest/gtest.h>
#include "tigerapi/order_util.h"
#include "tigerapi/contract_util.h"
#include "tigerapi/model.h"

using namespace TIGER_API;

namespace {
    const utility::string_t kAccount = U("DU123456");
    const utility::string_t kBuy     = U("BUY");
    const utility::string_t kSell    = U("SELL");
    Contract aapl() { return ContractUtil::stock_contract(U("AAPL"), U("USD")); }
}

// ── MarketOrder ──────────────────────────────────────────────────────────────

TEST(OrderUtilTest, MarketOrder_OrderType) {
    Contract c = aapl();
    Order o = OrderUtil::market_order(kAccount, c, kBuy, 100);
    EXPECT_EQ(o.order_type, U("MKT"));
}

TEST(OrderUtilTest, MarketOrder_Fields) {
    Contract c = aapl();
    Order o = OrderUtil::market_order(kAccount, c, kBuy, 200);
    EXPECT_EQ(o.account, kAccount);
    EXPECT_EQ(o.action, kBuy);
    EXPECT_EQ(o.total_quantity, 200LL);
}

// ── LimitOrder ───────────────────────────────────────────────────────────────

TEST(OrderUtilTest, LimitOrder_OrderType) {
    Contract c = aapl();
    Order o = OrderUtil::limit_order(kAccount, c, kBuy, 100, 175.5);
    EXPECT_EQ(o.order_type, U("LMT"));
}

TEST(OrderUtilTest, LimitOrder_LimitPrice) {
    Contract c = aapl();
    Order o = OrderUtil::limit_order(kAccount, c, kBuy, 100, 175.5);
    EXPECT_DOUBLE_EQ(o.limit_price, 175.5);
}

// ── StopOrder ────────────────────────────────────────────────────────────────

TEST(OrderUtilTest, StopOrder_OrderType) {
    Contract c = aapl();
    Order o = OrderUtil::stop_order(kAccount, c, kSell, 100, 170.0);
    EXPECT_EQ(o.order_type, U("STP"));
}

TEST(OrderUtilTest, StopOrder_AuxPrice) {
    Contract c = aapl();
    Order o = OrderUtil::stop_order(kAccount, c, kSell, 100, 170.0);
    EXPECT_DOUBLE_EQ(o.aux_price, 170.0);
}

// ── StopLimitOrder ───────────────────────────────────────────────────────────

TEST(OrderUtilTest, StopLimitOrder_OrderType) {
    Contract c = aapl();
    Order o = OrderUtil::stop_limit_order(kAccount, c, kSell, 100, 170.0, 169.0);
    EXPECT_EQ(o.order_type, U("STP_LMT"));
}

TEST(OrderUtilTest, StopLimitOrder_Prices) {
    Contract c = aapl();
    Order o = OrderUtil::stop_limit_order(kAccount, c, kSell, 100, 170.0, 169.0);
    EXPECT_DOUBLE_EQ(o.limit_price, 170.0);
    EXPECT_DOUBLE_EQ(o.aux_price, 169.0);
}

// ── TrailOrder ───────────────────────────────────────────────────────────────

TEST(OrderUtilTest, TrailOrder_OrderType) {
    Contract c = aapl();
    Order o = OrderUtil::trail_order(kAccount, c, kSell, 100, 0, 5.0);
    EXPECT_EQ(o.order_type, U("TRAIL"));
}

TEST(OrderUtilTest, TrailOrder_TrailingPercent) {
    Contract c = aapl();
    Order o = OrderUtil::trail_order(kAccount, c, kSell, 100, 0, 5.0);
    EXPECT_DOUBLE_EQ(o.trailing_percent, 5.0);
}

// ── IcebergOrder ─────────────────────────────────────────────────────────────

TEST(OrderUtilTest, IcebergOrder_OrderType) {
    Contract c = aapl();
    Order o = OrderUtil::iceberg_order(kAccount, c, kBuy, 1000LL, 175.0, 100LL);
    EXPECT_EQ(o.order_type, U("ICEBERG"));
}

TEST(OrderUtilTest, IcebergOrder_DisplaySize) {
    Contract c = aapl();
    Order o = OrderUtil::iceberg_order(kAccount, c, kBuy, 1000LL, 175.0, 100LL);
    EXPECT_EQ(o.display_size, 100LL);
    EXPECT_DOUBLE_EQ(o.limit_price, 175.0);
}

// ── AmountOrder ──────────────────────────────────────────────────────────────

TEST(OrderUtilTest, AmountOrder_CashAmount) {
    Contract c = aapl();
    Order o = OrderUtil::amount_order(kAccount, c, kBuy, 1000.0);
    EXPECT_DOUBLE_EQ(o.cash_amount, 1000.0);
}

TEST(OrderUtilTest, AmountOrder_ZeroThrows) {
    Contract c = aapl();
    EXPECT_THROW(OrderUtil::amount_order(kAccount, c, kBuy, 0.0), std::invalid_argument);
}

TEST(OrderUtilTest, AmountOrder_NegativeThrows) {
    Contract c = aapl();
    EXPECT_THROW(OrderUtil::amount_order(kAccount, c, kBuy, -100.0), std::invalid_argument);
}

// ── TwapOrder ────────────────────────────────────────────────────────────────

TEST(OrderUtilTest, TwapOrder_AlgoStrategy) {
    Contract c = aapl();
    Order o = OrderUtil::twap_order(kAccount, c, kBuy, 1000LL,
                                     1700000000000LL, 1700003600000LL);
    EXPECT_EQ(o.algo_strategy, U("TWAP"));
}

TEST(OrderUtilTest, TwapOrder_OrderType) {
    Contract c = aapl();
    Order o = OrderUtil::twap_order(kAccount, c, kBuy, 1000LL,
                                     1700000000000LL, 1700003600000LL);
    EXPECT_EQ(o.order_type, U("TWAP"));
}

TEST(OrderUtilTest, TwapOrder_HasAlgoParams) {
    Contract c = aapl();
    Order o = OrderUtil::twap_order(kAccount, c, kBuy, 1000LL,
                                     1700000000000LL, 1700003600000LL);
    EXPECT_FALSE(o.algo_params.empty());
}

// ── VwapOrder ────────────────────────────────────────────────────────────────

TEST(OrderUtilTest, VwapOrder_AlgoStrategy) {
    Contract c = aapl();
    Order o = OrderUtil::vwap_order(kAccount, c, kBuy, 1000LL,
                                     1700000000000LL, 1700003600000LL);
    EXPECT_EQ(o.algo_strategy, U("VWAP"));
    EXPECT_EQ(o.order_type, U("VWAP"));
}

// ── MultiLegOrder ─────────────────────────────────────────────────────────────

TEST(OrderUtilTest, MultiLegOrder_ComboType) {
    ContractLeg leg1, leg2;
    leg1.symbol = U("AAPL"); leg1.sec_type = U("OPT");
    leg1.expiry = U("220414"); leg1.strike = U("175.0"); leg1.right = U("C");
    leg1.action = U("BUY"); leg1.ratio = 1;
    leg2.symbol = U("AAPL"); leg2.sec_type = U("OPT");
    leg2.expiry = U("220414"); leg2.strike = U("180.0"); leg2.right = U("C");
    leg2.action = U("SELL"); leg2.ratio = 1;

    std::vector<ContractLeg> legs = {leg1, leg2};
    Order o = OrderUtil::multi_leg_order(kAccount, U("VERTICAL"), legs, kBuy,
                                          1LL, U("LMT"), 2.5);
    EXPECT_EQ(o.combo_type, U("VERTICAL"));
    EXPECT_EQ(o.contract_legs.size(), static_cast<size_t>(2));
    EXPECT_EQ(o.contract.sec_type, U("MLEG"));
}

TEST(OrderUtilTest, MultiLegOrder_EmptyLegsThrows) {
    std::vector<ContractLeg> legs;
    EXPECT_THROW(
        OrderUtil::multi_leg_order(kAccount, U("VERTICAL"), legs, kBuy, 1LL, U("LMT")),
        std::invalid_argument);
}

// ── OCA Brackets Order ────────────────────────────────────────────────────────

TEST(OrderUtilTest, OcaBracketsOrder_TwoSubOrders) {
    Contract c = aapl();
    Order o = OrderUtil::oca_brackets_order(kAccount, c, kBuy, 100LL,
        185.0, U("DAY"), 165.0, 0.0, U("DAY"));
    EXPECT_EQ(o.oca_orders.size(), static_cast<size_t>(2));
}

TEST(OrderUtilTest, OcaBracketsOrder_ProfitTakerIsLmt) {
    Contract c = aapl();
    Order o = OrderUtil::oca_brackets_order(kAccount, c, kBuy, 100LL,
        185.0, U("DAY"), 165.0, 0.0, U("DAY"));
    ASSERT_GE(o.oca_orders.size(), static_cast<size_t>(1));
    EXPECT_EQ(o.oca_orders[0].order_type, U("LMT"));
    EXPECT_DOUBLE_EQ(o.oca_orders[0].limit_price, 185.0);
}

TEST(OrderUtilTest, OcaBracketsOrder_StopLoss_PureStp) {
    Contract c = aapl();
    Order o = OrderUtil::oca_brackets_order(kAccount, c, kBuy, 100LL,
        185.0, U("DAY"), 165.0, 0.0, U("DAY"));
    ASSERT_GE(o.oca_orders.size(), static_cast<size_t>(2));
    EXPECT_EQ(o.oca_orders[1].order_type, U("STP"));
}

TEST(OrderUtilTest, OcaBracketsOrder_StopLoss_StpLmtWhenLimitGiven) {
    Contract c = aapl();
    Order o = OrderUtil::oca_brackets_order(kAccount, c, kBuy, 100LL,
        185.0, U("DAY"), 165.0, 164.0, U("DAY"));
    ASSERT_GE(o.oca_orders.size(), static_cast<size_t>(2));
    EXPECT_EQ(o.oca_orders[1].order_type, U("STP_LMT"));
}

// ── Attached Orders ───────────────────────────────────────────────────────────

TEST(OrderUtilTest, AddProfitTaker_AttachType) {
    Contract c = aapl();
    Order base = OrderUtil::limit_order(kAccount, c, kBuy, 100, 175.0);
    OrderUtil::add_profit_taker_order(base, 185.0);
    EXPECT_EQ(base.attach_type, U("PROFIT"));
    EXPECT_DOUBLE_EQ(base.profit_taker_price, 185.0);
}

TEST(OrderUtilTest, AddStopLoss_AttachType_Stp) {
    Contract c = aapl();
    Order base = OrderUtil::limit_order(kAccount, c, kBuy, 100, 175.0);
    OrderUtil::add_stop_loss_order(base, 165.0);
    EXPECT_EQ(base.attach_type, U("LOSS"));
    EXPECT_EQ(base.stop_loss_order_type, U("STP"));
    EXPECT_DOUBLE_EQ(base.stop_loss_price, 165.0);
}

TEST(OrderUtilTest, AddStopLossLimit_AttachType_StpLmt) {
    Contract c = aapl();
    Order base = OrderUtil::limit_order(kAccount, c, kBuy, 100, 175.0);
    OrderUtil::add_stop_loss_limit_order(base, 165.0, 164.0);
    EXPECT_EQ(base.attach_type, U("LOSS"));
    EXPECT_EQ(base.stop_loss_order_type, U("STP_LMT"));
    EXPECT_DOUBLE_EQ(base.stop_loss_price, 165.0);
    EXPECT_DOUBLE_EQ(base.stop_loss_limit_price, 164.0);
}

TEST(OrderUtilTest, AddStopLossTrail_AttachType_Trail) {
    Contract c = aapl();
    Order base = OrderUtil::limit_order(kAccount, c, kBuy, 100, 175.0);
    OrderUtil::add_stop_loss_trail_order(base, 5.0, 0.0);
    EXPECT_EQ(base.attach_type, U("LOSS"));
    EXPECT_EQ(base.stop_loss_order_type, U("TRAIL"));
    EXPECT_DOUBLE_EQ(base.stop_loss_trailing_percent, 5.0);
}

TEST(OrderUtilTest, AddBrackets_AttachType_Brackets) {
    Contract c = aapl();
    Order base = OrderUtil::limit_order(kAccount, c, kBuy, 100, 175.0);
    OrderUtil::add_brackets_order(base, 185.0, U("DAY"), false, 165.0, 0.0, U("DAY"));
    EXPECT_EQ(base.attach_type, U("BRACKETS"));
    EXPECT_DOUBLE_EQ(base.profit_taker_price, 185.0);
    EXPECT_DOUBLE_EQ(base.stop_loss_price, 165.0);
    EXPECT_EQ(base.stop_loss_order_type, U("STP"));
}

TEST(OrderUtilTest, AddBrackets_StpLmtWhenLimitGiven) {
    Contract c = aapl();
    Order base = OrderUtil::limit_order(kAccount, c, kBuy, 100, 175.0);
    OrderUtil::add_brackets_order(base, 185.0, U("DAY"), false, 165.0, 164.0, U("DAY"));
    EXPECT_EQ(base.stop_loss_order_type, U("STP_LMT"));
}

// ── LimitOrder overloads without account ────────────────────────────────────

TEST(OrderUtilTest, LimitOrder_NoAccount_DoublePice_OrderType) {
    Contract c = aapl();
    Order o = OrderUtil::limit_order(c, kBuy, 100, 175.5);
    EXPECT_EQ(o.order_type, U("LMT"));
    EXPECT_DOUBLE_EQ(o.limit_price, 175.5);
    // account defaults to empty when not specified
    EXPECT_TRUE(o.account.empty());
}

TEST(OrderUtilTest, LimitOrder_NoAccount_StringPrice_OrderType) {
    Contract c = aapl();
    Order o = OrderUtil::limit_order(c, kBuy, 100, U("175.50"));
    EXPECT_EQ(o.order_type, U("LMT"));
    // String price overload sets s_limit_price, not limit_price
    EXPECT_EQ(o.s_limit_price, U("175.50"));
    EXPECT_DOUBLE_EQ(o.limit_price, 0.0);
}

// ── IcebergOrder full params ─────────────────────────────────────────────────

TEST(OrderUtilTest, IcebergOrder_FullParams_DisplaySize) {
    Contract c = aapl();
    // full: account, contract, action, quantity, limit_price, display_size,
    //       min_display_size, check_intervals, price_type, start_time, end_time
    Order o = OrderUtil::iceberg_order(kAccount, c, kBuy,
                                        1000LL, 175.0,
                                        100LL,   // display_size
                                        50LL,    // min_display_size
                                        5LL,     // check_intervals
                                        U("LMT"),// price_type
                                        0LL,     // start_time (0 = omit)
                                        0LL);    // end_time
    EXPECT_EQ(o.order_type, U("ICEBERG"));
    EXPECT_EQ(o.display_size, 100LL);
    EXPECT_DOUBLE_EQ(o.limit_price, 175.0);
}

// ── IcebergOrder edge cases ───────────────────────────────────────────────────

TEST(OrderUtilTest, IcebergOrder_ZeroDisplaySize_Throws) {
    Contract c = aapl();
    EXPECT_THROW(OrderUtil::iceberg_order(kAccount, c, kBuy, 1000LL, 175.0, 0LL),
                 std::invalid_argument);
}

TEST(OrderUtilTest, IcebergOrder_NegativeDisplaySize_Throws) {
    Contract c = aapl();
    EXPECT_THROW(OrderUtil::iceberg_order(kAccount, c, kBuy, 1000LL, 175.0, -1LL),
                 std::invalid_argument);
}

TEST(OrderUtilTest, IcebergOrder_FullParams_ZeroDisplaySize_Throws) {
    Contract c = aapl();
    EXPECT_THROW(OrderUtil::iceberg_order(kAccount, c, kBuy, 1000LL, 175.0,
                                           0LL, 50LL, 5LL, U("LMT"), 0LL, 0LL),
                 std::invalid_argument);
}

TEST(OrderUtilTest, IcebergOrder_FullParams_MinDisplaySize) {
    Contract c = aapl();
    Order o = OrderUtil::iceberg_order(kAccount, c, kBuy, 1000LL, 175.0,
                                        100LL, 50LL, 5LL, U("LMT"), 100LL, 200LL);
    EXPECT_EQ(o.min_display_size, 50LL);
    EXPECT_EQ(o.check_intervals, 5LL);
    EXPECT_EQ(o.start_time, 100LL);
    EXPECT_EQ(o.end_time, 200LL);
}

TEST(OrderUtilTest, IcebergOrder_FullParams_EmptyPriceType_DefaultsToLimitPrice) {
    Contract c = aapl();
    Order o = OrderUtil::iceberg_order(kAccount, c, kBuy, 1000LL, 175.0,
                                        100LL, 0LL, 0LL, U(""), 0LL, 0LL);
    EXPECT_EQ(o.price_type, U("LIMIT_PRICE"));
}

TEST(OrderUtilTest, IcebergOrder_TimeInForce_IsDay) {
    Contract c = aapl();
    Order o = OrderUtil::iceberg_order(kAccount, c, kBuy, 1000LL, 175.0, 100LL);
    EXPECT_EQ(o.time_in_force, U("DAY"));
}

// ── TWAP/VWAP validation ──────────────────────────────────────────────────────

TEST(OrderUtilTest, TwapOrder_EndBeforeStart_Throws) {
    Contract c = aapl();
    EXPECT_THROW(OrderUtil::twap_order(kAccount, c, kBuy, 1000LL, 2000LL, 1000LL),
                 std::invalid_argument);
}

TEST(OrderUtilTest, TwapOrder_EndEqualStart_Throws) {
    Contract c = aapl();
    EXPECT_THROW(OrderUtil::twap_order(kAccount, c, kBuy, 1000LL, 1000LL, 1000LL),
                 std::invalid_argument);
}

TEST(OrderUtilTest, TwapOrder_ZeroStart_Throws) {
    Contract c = aapl();
    EXPECT_THROW(OrderUtil::twap_order(kAccount, c, kBuy, 1000LL, 0LL, 1000LL),
                 std::invalid_argument);
}

TEST(OrderUtilTest, TwapOrder_ZeroEnd_Throws) {
    Contract c = aapl();
    EXPECT_THROW(OrderUtil::twap_order(kAccount, c, kBuy, 1000LL, 1000LL, 0LL),
                 std::invalid_argument);
}

TEST(OrderUtilTest, VwapOrder_EndBeforeStart_Throws) {
    Contract c = aapl();
    EXPECT_THROW(OrderUtil::vwap_order(kAccount, c, kBuy, 1000LL, 2000LL, 1000LL),
                 std::invalid_argument);
}

TEST(OrderUtilTest, VwapOrder_WithParticipationRate) {
    Contract c = aapl();
    Order o = OrderUtil::vwap_order(kAccount, c, kBuy, 1000LL,
                                     1700000000000LL, 1700003600000LL, 0.5);
    EXPECT_FALSE(o.algo_params.empty());
    // Should have 3 params: start_time, end_time, participation_rate
    EXPECT_EQ(o.algo_params.size(), 3u);
}

TEST(OrderUtilTest, VwapOrder_NoParticipationRate) {
    Contract c = aapl();
    Order o = OrderUtil::vwap_order(kAccount, c, kBuy, 1000LL,
                                     1700000000000LL, 1700003600000LL, 0.0);
    // Should have 2 params: start_time, end_time (no participation_rate)
    EXPECT_EQ(o.algo_params.size(), 2u);
}

// ── Multi-leg order with aux_price ────────────────────────────────────────────

TEST(OrderUtilTest, MultiLegOrder_WithLimitAndAuxPrice) {
    ContractLeg leg1, leg2;
    leg1.symbol = U("AAPL"); leg1.sec_type = U("OPT");
    leg1.expiry = U("220414"); leg1.strike = U("175.0"); leg1.right = U("C");
    leg1.action = U("BUY"); leg1.ratio = 1;
    leg2.symbol = U("AAPL"); leg2.sec_type = U("OPT");
    leg2.expiry = U("220414"); leg2.strike = U("180.0"); leg2.right = U("C");
    leg2.action = U("SELL"); leg2.ratio = 1;

    std::vector<ContractLeg> legs = {leg1, leg2};
    Order o = OrderUtil::multi_leg_order(kAccount, U("VERTICAL"), legs, kBuy,
                                          1LL, U("LMT"), 2.5, 1.0);
    EXPECT_DOUBLE_EQ(o.limit_price, 2.5);
    EXPECT_DOUBLE_EQ(o.aux_price, 1.0);
    EXPECT_EQ(o.time_in_force, U("DAY"));
}

// ── OCA brackets edge cases ────────────────────────────────────────────────────

TEST(OrderUtilTest, OcaBracketsOrder_ProfitTakerTif) {
    Contract c = aapl();
    Order o = OrderUtil::oca_brackets_order(kAccount, c, kBuy, 100LL,
        185.0, U("GTC"), 165.0, 0.0, U("GTC"));
    ASSERT_GE(o.oca_orders.size(), 2u);
    EXPECT_EQ(o.oca_orders[0].time_in_force, U("GTC"));
    EXPECT_EQ(o.oca_orders[1].time_in_force, U("GTC"));
}

TEST(OrderUtilTest, OcaBracketsOrder_AccountSet) {
    Contract c = aapl();
    Order o = OrderUtil::oca_brackets_order(kAccount, c, kBuy, 100LL,
        185.0, U("DAY"), 165.0, 0.0, U("DAY"));
    EXPECT_EQ(o.account, kAccount);
    ASSERT_GE(o.oca_orders.size(), 2u);
    EXPECT_EQ(o.oca_orders[0].account, kAccount);
    EXPECT_EQ(o.oca_orders[1].account, kAccount);
}

// ── Attached orders edge cases ─────────────────────────────────────────────────

TEST(OrderUtilTest, AddProfitTaker_WithTif) {
    Contract c = aapl();
    Order base = OrderUtil::limit_order(kAccount, c, kBuy, 100, 175.0);
    OrderUtil::add_profit_taker_order(base, 185.0, U("GTC"));
    EXPECT_EQ(base.profit_taker_tif, U("GTC"));
}

TEST(OrderUtilTest, AddStopLoss_WithTif) {
    Contract c = aapl();
    Order base = OrderUtil::limit_order(kAccount, c, kBuy, 100, 175.0);
    OrderUtil::add_stop_loss_order(base, 165.0, U("GTC"));
    EXPECT_EQ(base.stop_loss_tif, U("GTC"));
}

TEST(OrderUtilTest, AddStopLossLimit_WithTif) {
    Contract c = aapl();
    Order base = OrderUtil::limit_order(kAccount, c, kBuy, 100, 175.0);
    OrderUtil::add_stop_loss_limit_order(base, 165.0, 164.0, U("GTC"));
    EXPECT_EQ(base.stop_loss_tif, U("GTC"));
}

TEST(OrderUtilTest, AddStopLossTrail_WithTif) {
    Contract c = aapl();
    Order base = OrderUtil::limit_order(kAccount, c, kBuy, 100, 175.0);
    OrderUtil::add_stop_loss_trail_order(base, 5.0, 2.0, U("GTC"));
    EXPECT_EQ(base.stop_loss_tif, U("GTC"));
    EXPECT_DOUBLE_EQ(base.stop_loss_trailing_amount, 2.0);
}

TEST(OrderUtilTest, AddBrackets_WithTif) {
    Contract c = aapl();
    Order base = OrderUtil::limit_order(kAccount, c, kBuy, 100, 175.0);
    OrderUtil::add_brackets_order(base, 185.0, U("GTC"), true, 165.0, 0.0, U("GTC"));
    EXPECT_EQ(base.profit_taker_tif, U("GTC"));
    EXPECT_EQ(base.stop_loss_tif, U("GTC"));
    EXPECT_TRUE(base.profit_taker_rth);
}

// ── Order to_string ───────────────────────────────────────────────────────────

TEST(OrderUtilTest, OrderToString_NotEmpty) {
    Contract c = aapl();
    Order o = OrderUtil::market_order(kAccount, c, kBuy, 100);
    utility::string_t s = o.to_string();
    EXPECT_FALSE(s.empty());
}

// ── Market order without account ─────────────────────────────────────────────

TEST(OrderUtilTest, MarketOrder_NoAccount) {
    Contract c = aapl();
    Order o = OrderUtil::market_order(U(""), c, kBuy, 100);
    EXPECT_TRUE(o.account.empty());
    EXPECT_EQ(o.order_type, U("MKT"));
    EXPECT_EQ(o.total_quantity, 100LL);
}

// ── Stop order without account ───────────────────────────────────────────────

TEST(OrderUtilTest, StopOrder_NoAccount) {
    Contract c = aapl();
    Order o = OrderUtil::stop_order(U(""), c, kSell, 100, 170.0);
    EXPECT_TRUE(o.account.empty());
    EXPECT_EQ(o.order_type, U("STP"));
}

// ── Stop limit order without account ──────────────────────────────────────────

TEST(OrderUtilTest, StopLimitOrder_NoAccount) {
    Contract c = aapl();
    Order o = OrderUtil::stop_limit_order(U(""), c, kSell, 100, 170.0, 169.0);
    EXPECT_TRUE(o.account.empty());
    EXPECT_EQ(o.order_type, U("STP_LMT"));
}

// ── Trail order without account ──────────────────────────────────────────────

TEST(OrderUtilTest, TrailOrder_NoAccount) {
    Contract c = aapl();
    Order o = OrderUtil::trail_order(U(""), c, kSell, 100, 0, 5.0);
    EXPECT_TRUE(o.account.empty());
    EXPECT_EQ(o.order_type, U("TRAIL"));
}

// ── Amount order without account ──────────────────────────────────────────────

TEST(OrderUtilTest, AmountOrder_NoAccount) {
    Contract c = aapl();
    Order o = OrderUtil::amount_order(U(""), c, kBuy, 1000.0);
    EXPECT_TRUE(o.account.empty());
    EXPECT_DOUBLE_EQ(o.cash_amount, 1000.0);
}
