#include <gtest/gtest.h>
#include "tigerapi/tick_util.h"
#include "tigerapi/enums.h"

using namespace TIGER_API;

// ── get_part_code ──────────────────────────────────────────────────────────────

TEST(TickUtilTest, GetPartCode_NYSE) {
    EXPECT_EQ(get_part_code("n"), "NYSE");
}

TEST(TickUtilTest, GetPartCode_NASDAQ) {
    EXPECT_EQ(get_part_code("t"), "NSDQ");
}

TEST(TickUtilTest, GetPartCode_AMEX) {
    EXPECT_EQ(get_part_code("a"), "AMEX");
}

TEST(TickUtilTest, GetPartCode_ARCA) {
    EXPECT_EQ(get_part_code("p"), "ARCA");
}

TEST(TickUtilTest, GetPartCode_IEX) {
    EXPECT_EQ(get_part_code("v"), "IEX");
}

TEST(TickUtilTest, GetPartCode_Unknown) {
    EXPECT_EQ(get_part_code("zzz"), "");
}

TEST(TickUtilTest, GetPartCode_Empty) {
    EXPECT_EQ(get_part_code(""), "");
}

TEST(TickUtilTest, GetPartCode_AllMappings) {
    // Verify all known codes
    EXPECT_EQ(get_part_code("b"), "BX");
    EXPECT_EQ(get_part_code("c"), "NSX");
    EXPECT_EQ(get_part_code("d"), "ADF");
    EXPECT_EQ(get_part_code("h"), "MIAX");
    EXPECT_EQ(get_part_code("i"), "ISE");
    EXPECT_EQ(get_part_code("j"), "EDGA");
    EXPECT_EQ(get_part_code("k"), "EDGX");
    EXPECT_EQ(get_part_code("l"), "LTSE");
    EXPECT_EQ(get_part_code("m"), "CHO");
    EXPECT_EQ(get_part_code("s"), "CTS");
    EXPECT_EQ(get_part_code("u"), "MEMX");
    EXPECT_EQ(get_part_code("w"), "CBSX");
    EXPECT_EQ(get_part_code("x"), "PSX");
    EXPECT_EQ(get_part_code("y"), "BYX");
    EXPECT_EQ(get_part_code("z"), "BZX");
}

// ── get_part_code_name ─────────────────────────────────────────────────────────

TEST(TickUtilTest, GetPartCodeName_NYSE) {
    EXPECT_NE(get_part_code_name("n"), "");
    EXPECT_NE(get_part_code_name("n").find("NYSE"), std::string::npos);
}

TEST(TickUtilTest, GetPartCodeName_NASDAQ) {
    EXPECT_NE(get_part_code_name("t"), "");
    EXPECT_NE(get_part_code_name("t").find("NASDAQ"), std::string::npos);
}

TEST(TickUtilTest, GetPartCodeName_Unknown) {
    EXPECT_EQ(get_part_code_name("zzz"), "");
}

TEST(TickUtilTest, GetPartCodeName_Empty) {
    EXPECT_EQ(get_part_code_name(""), "");
}

// ── get_trade_condition_map ────────────────────────────────────────────────────

TEST(TickUtilTest, GetTradeConditionMap_EmptyLevel_ReturnsUS) {
    auto& m = get_trade_condition_map("");
    // US_TRADE_COND_MAP should be returned for empty level
    EXPECT_NE(m.find("B"), m.end());
}

TEST(TickUtilTest, GetTradeConditionMap_USLevel_ReturnsUS) {
    auto& m = get_trade_condition_map("us");
    EXPECT_NE(m.find("B"), m.end());
    EXPECT_EQ(m.at("B"), "US_BUNCHED_TRADE");
}

TEST(TickUtilTest, GetTradeConditionMap_HKLevel_ReturnsHK) {
    auto& m = get_trade_condition_map("hk_level_1");
    // HK_TRADE_COND_MAP should be returned for hk prefix
    EXPECT_NE(m.find("D"), m.end());
    EXPECT_EQ(m.at("D"), "HK_ODD_LOT_TRADE");
}

TEST(TickUtilTest, GetTradeConditionMap_HKLevel_CaseInsensitive) {
    auto& m = get_trade_condition_map("HK");
    EXPECT_NE(m.find("D"), m.end());
}

TEST(TickUtilTest, GetTradeConditionMap_NonHKPrefix_ReturnsUS) {
    auto& m = get_trade_condition_map("sg");
    EXPECT_NE(m.find("B"), m.end());
    EXPECT_EQ(m.at("B"), "US_BUNCHED_TRADE");
}

// ── get_trade_condition ────────────────────────────────────────────────────────

TEST(TickUtilTest, GetTradeCondition_RegularSale_US) {
    auto& m = get_trade_condition_map("us");
    EXPECT_EQ(get_trade_condition(" ", m), "US_REGULAR_SALE");
}

TEST(TickUtilTest, GetTradeCondition_CashTrade_US) {
    auto& m = get_trade_condition_map("us");
    EXPECT_EQ(get_trade_condition("C", m), "US_CASH_TRADE");
}

TEST(TickUtilTest, GetTradeCondition_UnknownCondition_US) {
    auto& m = get_trade_condition_map("us");
    EXPECT_EQ(get_trade_condition("ZZZ", m), "");
}

TEST(TickUtilTest, GetTradeCondition_OddLot_HK) {
    auto& m = get_trade_condition_map("hk");
    EXPECT_EQ(get_trade_condition("D", m), "HK_ODD_LOT_TRADE");
}

TEST(TickUtilTest, GetTradeCondition_AuctionTrade_HK) {
    auto& m = get_trade_condition_map("hk");
    EXPECT_EQ(get_trade_condition("U", m), "HK_AUCTION_TRADE");
}

TEST(TickUtilTest, GetTradeCondition_UnknownCondition_HK) {
    auto& m = get_trade_condition_map("hk");
    EXPECT_EQ(get_trade_condition("ZZZ", m), "");
}
