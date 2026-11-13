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
    // Implementation prepends "20" and adds dashes: "2022-04-14"
    EXPECT_EQ(std::get<1>(result), U("2022-04-14"));
    EXPECT_EQ(std::get<2>(result), U("CALL"));
    EXPECT_FALSE(std::get<3>(result).empty());    // strike
}

TEST(ContractUtilTest, ExtractOptionInfo_Put) {
    auto result = ContractUtil::extract_option_info(U("JD 211015P00045000"));
    EXPECT_EQ(std::get<0>(result), U("JD"));
    // Implementation prepends "20" and adds dashes: "2021-10-15"
    EXPECT_EQ(std::get<1>(result), U("2021-10-15"));
    EXPECT_EQ(std::get<2>(result), U("PUT"));
    EXPECT_FALSE(std::get<3>(result).empty());
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

TEST(ContractUtilTest, FutureContract_WithMultiplier) {
    Contract c = ContractUtil::future_contract(U("ES"), U("USD"), U("20250117"), U("CME"),
                                                U("202501"), 50, U("ES1!"));
    EXPECT_EQ(c.sec_type, U("FUT"));
    EXPECT_EQ(c.symbol, U("ES"));
    EXPECT_EQ(c.multiplier, 50);
    EXPECT_EQ(c.contract_month, U("202501"));
    EXPECT_EQ(c.local_symbol, U("ES1!"));
}

// ── option_contract from identifier with dashes ──────────────────────────────

TEST(ContractUtilTest, OptionContract_FromIdentifier_WithDashes) {
    // extract_option_info returns expiry as "YYYY-MM-DD"; option_contract(identifier)
    // strips dashes → expiry becomes "YYYYMMDD"
    Contract c = ContractUtil::option_contract(U("AAPL 220414C0017500"));
    // After dash removal, expiry should be 8 chars without dashes
    EXPECT_EQ(c.expiry.find(U("-")), utility::string_t::npos);
}

TEST(ContractUtilTest, OptionContract_FromIdentifier_WithMultiplier) {
    Contract c = ContractUtil::option_contract(U("AAPL 220414C0017500"), 100, U("USD"));
    EXPECT_EQ(c.multiplier, 100);
    EXPECT_EQ(c.currency, U("USD"));
}

TEST(ContractUtilTest, OptionContract_PutFromIdentifier) {
    Contract c = ContractUtil::option_contract(U("JD 211015P00045000"));
    EXPECT_EQ(c.right, U("PUT"));
    EXPECT_EQ(c.symbol, U("JD"));
}

// ── extract_option_info edge cases ─────────────────────────────────────────────

TEST(ContractUtilTest, ExtractOptionInfo_EmptyString_ReturnsEmpty) {
    auto result = ContractUtil::extract_option_info(U(""));
    EXPECT_EQ(std::get<0>(result), U(""));
    EXPECT_EQ(std::get<1>(result), U(""));
    EXPECT_EQ(std::get<2>(result), U(""));
    EXPECT_EQ(std::get<3>(result), U(""));
}

TEST(ContractUtilTest, ExtractOptionInfo_InvalidFormat_ReturnsEmpty) {
    auto result = ContractUtil::extract_option_info(U("not_an_option"));
    EXPECT_EQ(std::get<0>(result), U(""));
    EXPECT_EQ(std::get<1>(result), U(""));
}

TEST(ContractUtilTest, ExtractOptionInfo_WithDotInSymbol) {
    // Symbol with dot, e.g. "BRK.A 220414C0010000"
    auto result = ContractUtil::extract_option_info(U("BRK.A 220414C0010000"));
    EXPECT_EQ(std::get<0>(result), U("BRK.A"));
    EXPECT_EQ(std::get<2>(result), U("CALL"));
}

TEST(ContractUtilTest, ExtractOptionInfo_StrikeFormatting) {
    // Strike 0017500 → std::stod("0017500")=17500 → 17500/1000=17.5 → "17.5"
    auto result = ContractUtil::extract_option_info(U("AAPL 220414C0017500"));
    utility::string_t strike = std::get<3>(result);
    EXPECT_FALSE(strike.empty());
    EXPECT_EQ(strike, U("17.5"));
}

TEST(ContractUtilTest, ExtractOptionInfo_CallRight) {
    auto result = ContractUtil::extract_option_info(U("AAPL 220414C0017500"));
    EXPECT_EQ(std::get<2>(result), U("CALL"));
}

TEST(ContractUtilTest, ExtractOptionInfo_PutRight) {
    auto result = ContractUtil::extract_option_info(U("AAPL 220414P0017500"));
    EXPECT_EQ(std::get<2>(result), U("PUT"));
}

TEST(ContractUtilTest, ExtractOptionInfo_ExpiryHasDashes) {
    // The implementation formats expiry as "YYYY-MM-DD"
    auto result = ContractUtil::extract_option_info(U("AAPL 220414C0017500"));
    utility::string_t expiry = std::get<1>(result);
    EXPECT_NE(expiry.find(U("-")), utility::string_t::npos);
    EXPECT_EQ(expiry.size(), 10u);  // "2022-04-14"
}
