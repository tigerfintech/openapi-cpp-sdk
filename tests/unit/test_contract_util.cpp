#include <gtest/gtest.h>
#include "tigerapi/contract_util.h"
#include "tigerapi/model.h"
#include <tuple>

using namespace TIGER_API;

// ── stock_contract ────────────────────────────────────────────────────────────

TEST(ContractUtilTest, StockContract_SecType) {
    Contract c = ContractUtil::stock_contract(U("AAPL"), U("USD"));
    EXPECT_EQ(c.sec_type, U("STK"));
}

TEST(ContractUtilTest, StockContract_SymbolAndCurrency) {
    Contract c = ContractUtil::stock_contract(U("TSLA"), U("USD"));
    EXPECT_EQ(c.symbol, U("TSLA"));
    EXPECT_EQ(c.currency, U("USD"));
}

TEST(ContractUtilTest, StockContract_HKD) {
    Contract c = ContractUtil::stock_contract(U("9988"), U("HKD"));
    EXPECT_EQ(c.symbol, U("9988"));
    EXPECT_EQ(c.currency, U("HKD"));
}

TEST(ContractUtilTest, StockContract_OptionalFields) {
    Contract c = ContractUtil::stock_contract(U("AAPL"), U("USD"), U("AAPL"), U("NASDAQ"), 265598);
    EXPECT_EQ(c.local_symbol, U("AAPL"));
    EXPECT_EQ(c.exchange, U("NASDAQ"));
    EXPECT_EQ(c.contract_id, 265598);
}

// ── option_contract ───────────────────────────────────────────────────────────

TEST(ContractUtilTest, OptionContract_SecType) {
    Contract c = ContractUtil::option_contract(U("AAPL"), U("220414"), U("175.0"), U("CALL"));
    EXPECT_EQ(c.sec_type, U("OPT"));
}

TEST(ContractUtilTest, OptionContract_Fields) {
    Contract c = ContractUtil::option_contract(U("AAPL"), U("220414"), U("175.0"), U("CALL"),
                                               U("USD"), 100);
    EXPECT_EQ(c.symbol, U("AAPL"));
    EXPECT_EQ(c.expiry, U("220414"));
    EXPECT_EQ(c.strike, U("175.0"));
    EXPECT_EQ(c.right, U("CALL"));
    EXPECT_EQ(c.currency, U("USD"));
    EXPECT_EQ(c.multiplier, 100);
}

TEST(ContractUtilTest, OptionContract_FromIdentifier_SecType) {
    // Identifier format: "AAPL 220414C0017500"
    Contract c = ContractUtil::option_contract(U("AAPL 220414C0017500"));
    EXPECT_EQ(c.sec_type, U("OPT"));
}

TEST(ContractUtilTest, OptionContract_FromIdentifier_Symbol) {
    Contract c = ContractUtil::option_contract(U("AAPL 220414C0017500"));
    EXPECT_EQ(c.symbol, U("AAPL"));
}

TEST(ContractUtilTest, ExtractOptionInfo_ParsesIdentifier) {
    auto result = ContractUtil::extract_option_info(U("AAPL 220414C0017500"));
    EXPECT_EQ(std::get<0>(result), U("AAPL"));   // symbol
    EXPECT_EQ(std::get<1>(result), U("220414"));  // expiry
    EXPECT_FALSE(std::get<2>(result).empty());    // right ("C" or "CALL")
    EXPECT_FALSE(std::get<3>(result).empty());    // strike
}

TEST(ContractUtilTest, ExtractOptionInfo_Put) {
    auto result = ContractUtil::extract_option_info(U("JD 211015P00045000"));
    EXPECT_EQ(std::get<0>(result), U("JD"));
    EXPECT_EQ(std::get<1>(result), U("211015"));
    EXPECT_FALSE(std::get<2>(result).empty());
}

// ── future_contract ───────────────────────────────────────────────────────────

TEST(ContractUtilTest, FutureContract_SecType) {
    Contract c = ContractUtil::future_contract(U("CL"), U("USD"));
    EXPECT_EQ(c.sec_type, U("FUT"));
}

TEST(ContractUtilTest, FutureContract_Fields) {
    Contract c = ContractUtil::future_contract(U("CL"), U("USD"), U("20250117"), U("NYMEX"));
    EXPECT_EQ(c.symbol, U("CL"));
    EXPECT_EQ(c.currency, U("USD"));
    EXPECT_EQ(c.expiry, U("20250117"));
    EXPECT_EQ(c.exchange, U("NYMEX"));
}
