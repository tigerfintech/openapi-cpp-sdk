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
    EXPECT_EQ(o.order_type, U("LMT"));
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
