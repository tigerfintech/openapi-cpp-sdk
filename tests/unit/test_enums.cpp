#include <gtest/gtest.h>
#include "tigerapi/enums.h"

using namespace TIGER_API;

// ── Market ─────────────────────────────────────────────────────────────────────

TEST(EnumTest, Market_ALL) { EXPECT_EQ(enum_to_str(Market::ALL), U("ALL")); }
TEST(EnumTest, Market_US) { EXPECT_EQ(enum_to_str(Market::US), U("US")); }
TEST(EnumTest, Market_HK) { EXPECT_EQ(enum_to_str(Market::HK), U("HK")); }
TEST(EnumTest, Market_CN) { EXPECT_EQ(enum_to_str(Market::CN), U("CN")); }
TEST(EnumTest, Market_SG) { EXPECT_EQ(enum_to_str(Market::SG), U("SG")); }

// ── License ───────────────────────────────────────────────────────────────────

TEST(EnumTest, License_TBNZ) { EXPECT_EQ(enum_to_str(License::TBNZ), U("TBNZ")); }
TEST(EnumTest, License_TBSG) { EXPECT_EQ(enum_to_str(License::TBSG), U("TBSG")); }
TEST(EnumTest, License_TBHK) { EXPECT_EQ(enum_to_str(License::TBHK), U("TBHK")); }
TEST(EnumTest, License_TBAU) { EXPECT_EQ(enum_to_str(License::TBAU), U("TBAU")); }
TEST(EnumTest, License_TBUS) { EXPECT_EQ(enum_to_str(License::TBUS), U("TBUS")); }

// ── TradingSession ────────────────────────────────────────────────────────────

TEST(EnumTest, TradingSession_PreMarket) { EXPECT_EQ(enum_to_str(TradingSession::PreMarket), U("PreMarket")); }
TEST(EnumTest, TradingSession_Regular) { EXPECT_EQ(enum_to_str(TradingSession::Regular), U("Regular")); }
TEST(EnumTest, TradingSession_AfterHours) { EXPECT_EQ(enum_to_str(TradingSession::AfterHours), U("AfterHours")); }
TEST(EnumTest, TradingSession_OverNight) { EXPECT_EQ(enum_to_str(TradingSession::OverNight), U("OverNight")); }

// ── SecType ────────────────────────────────────────────────────────────────────

TEST(EnumTest, SecType_ALL) { EXPECT_EQ(enum_to_str(SecType::ALL), U("")); }
TEST(EnumTest, SecType_STK) { EXPECT_EQ(enum_to_str(SecType::STK), U("STK")); }
TEST(EnumTest, SecType_OPT) { EXPECT_EQ(enum_to_str(SecType::OPT), U("OPT")); }
TEST(EnumTest, SecType_WAR) { EXPECT_EQ(enum_to_str(SecType::WAR), U("WAR")); }
TEST(EnumTest, SecType_IOPT) { EXPECT_EQ(enum_to_str(SecType::IOPT), U("IOPT")); }
TEST(EnumTest, SecType_FUT) { EXPECT_EQ(enum_to_str(SecType::FUT), U("FUT")); }
TEST(EnumTest, SecType_FOP) { EXPECT_EQ(enum_to_str(SecType::FOP), U("FOP")); }
TEST(EnumTest, SecType_CASH) { EXPECT_EQ(enum_to_str(SecType::CASH), U("CASH")); }

// ── SegmentType ────────────────────────────────────────────────────────────────

TEST(EnumTest, SegmentType_ALL) { EXPECT_EQ(enum_to_str(SegmentType::ALL), U("ALL")); }
TEST(EnumTest, SegmentType_SEC) { EXPECT_EQ(enum_to_str(SegmentType::SEC), U("SEC")); }
TEST(EnumTest, SegmentType_FUT) { EXPECT_EQ(enum_to_str(SegmentType::FUT), U("FUT")); }

// ── Currency ───────────────────────────────────────────────────────────────────

TEST(EnumTest, Currency_ALL) { EXPECT_EQ(enum_to_str(Currency::ALL), U("ALL")); }
TEST(EnumTest, Currency_USD) { EXPECT_EQ(enum_to_str(Currency::USD), U("USD")); }
TEST(EnumTest, Currency_HKD) { EXPECT_EQ(enum_to_str(Currency::HKD), U("HKD")); }
TEST(EnumTest, Currency_CNH) { EXPECT_EQ(enum_to_str(Currency::CNH), U("CNH")); }
TEST(EnumTest, Currency_SGD) { EXPECT_EQ(enum_to_str(Currency::SGD), U("SGD")); }

// ── Language ───────────────────────────────────────────────────────────────────

TEST(EnumTest, Language_zhCN) { EXPECT_EQ(enum_to_str(Language::zh_CN), U("zh_CN")); }
TEST(EnumTest, Language_zhTW) { EXPECT_EQ(enum_to_str(Language::zh_TW), U("zh_TW")); }
TEST(EnumTest, Language_enUS) { EXPECT_EQ(enum_to_str(Language::en_US), U("en_US")); }

// ── QuoteRight ─────────────────────────────────────────────────────────────────

TEST(EnumTest, QuoteRight_br) { EXPECT_EQ(enum_to_str(QuoteRight::br), U("br")); }
TEST(EnumTest, QuoteRight_nr) { EXPECT_EQ(enum_to_str(QuoteRight::nr), U("nr")); }

// ── Right ──────────────────────────────────────────────────────────────────────

TEST(EnumTest, Right_PUT) { EXPECT_EQ(enum_to_str(Right::PUT), U("PUT")); }
TEST(EnumTest, Right_CALL) { EXPECT_EQ(enum_to_str(Right::CALL), U("CALL")); }
TEST(EnumTest, Right_ALL) { EXPECT_EQ(enum_to_str(Right::ALL), U("")); }

// ── TimelinePeriod ─────────────────────────────────────────────────────────────

TEST(EnumTest, TimelinePeriod_Day) { EXPECT_EQ(get_timeline_period_value(TimelinePeriod::DAY), U("day")); }
TEST(EnumTest, TimelinePeriod_FiveDays) { EXPECT_EQ(get_timeline_period_value(TimelinePeriod::FIVE_DAYS), U("5day")); }

// ── BarPeriod ──────────────────────────────────────────────────────────────────

TEST(EnumTest, BarPeriod_Day) { EXPECT_EQ(enum_to_str(BarPeriod::DAY), U("day")); }
TEST(EnumTest, BarPeriod_Week) { EXPECT_EQ(enum_to_str(BarPeriod::WEEK), U("week")); }
TEST(EnumTest, BarPeriod_Month) { EXPECT_EQ(enum_to_str(BarPeriod::MONTH), U("month")); }
TEST(EnumTest, BarPeriod_Year) { EXPECT_EQ(enum_to_str(BarPeriod::YEAR), U("year")); }
TEST(EnumTest, BarPeriod_1Min) { EXPECT_EQ(enum_to_str(BarPeriod::ONE_MINUTE), U("1min")); }
TEST(EnumTest, BarPeriod_3Min) { EXPECT_EQ(enum_to_str(BarPeriod::THREE_MINUTES), U("3min")); }
TEST(EnumTest, BarPeriod_5Min) { EXPECT_EQ(enum_to_str(BarPeriod::FIVE_MINUTES), U("5min")); }
TEST(EnumTest, BarPeriod_10Min) { EXPECT_EQ(enum_to_str(BarPeriod::TEN_MINUTES), U("10min")); }
TEST(EnumTest, BarPeriod_15Min) { EXPECT_EQ(enum_to_str(BarPeriod::FIFTEEN_MINUTES), U("15min")); }
TEST(EnumTest, BarPeriod_30Min) { EXPECT_EQ(enum_to_str(BarPeriod::HALF_HOUR), U("30min")); }
TEST(EnumTest, BarPeriod_45Min) { EXPECT_EQ(enum_to_str(BarPeriod::FORTY_FIVE_MINUTES), U("45min")); }
TEST(EnumTest, BarPeriod_60Min) { EXPECT_EQ(enum_to_str(BarPeriod::ONE_HOUR), U("60min")); }
TEST(EnumTest, BarPeriod_2Hour) { EXPECT_EQ(enum_to_str(BarPeriod::TWO_HOURS), U("2hour")); }
TEST(EnumTest, BarPeriod_3Hour) { EXPECT_EQ(enum_to_str(BarPeriod::THREE_HOURS), U("3hours")); }
TEST(EnumTest, BarPeriod_4Hour) { EXPECT_EQ(enum_to_str(BarPeriod::FOUR_HOURS), U("4hour")); }
TEST(EnumTest, BarPeriod_6Hour) { EXPECT_EQ(enum_to_str(BarPeriod::SIX_HOURS), U("6hour")); }

// ── CapitalPeriod ──────────────────────────────────────────────────────────────

TEST(EnumTest, CapitalPeriod_Intraday) { EXPECT_EQ(enum_to_str(CapitalPeriod::INTRADAY), U("intraday")); }
TEST(EnumTest, CapitalPeriod_Day) { EXPECT_EQ(enum_to_str(CapitalPeriod::DAY), U("day")); }
TEST(EnumTest, CapitalPeriod_Week) { EXPECT_EQ(enum_to_str(CapitalPeriod::WEEK), U("week")); }
TEST(EnumTest, CapitalPeriod_Month) { EXPECT_EQ(enum_to_str(CapitalPeriod::MONTH), U("month")); }
TEST(EnumTest, CapitalPeriod_Year) { EXPECT_EQ(enum_to_str(CapitalPeriod::YEAR), U("year")); }
TEST(EnumTest, CapitalPeriod_Quarter) { EXPECT_EQ(enum_to_str(CapitalPeriod::QUARTER), U("quarter")); }
TEST(EnumTest, CapitalPeriod_HalfYear) { EXPECT_EQ(enum_to_str(CapitalPeriod::HALFAYEAR), U("6month")); }

// ── OrderSortBy ────────────────────────────────────────────────────────────────

TEST(EnumTest, OrderSortBy_LatestCreated) { EXPECT_EQ(enum_to_str(OrderSortBy::LATEST_CREATED), U("LATEST_CREATED")); }
TEST(EnumTest, OrderSortBy_LatestStatusUpdated) { EXPECT_EQ(enum_to_str(OrderSortBy::LATEST_STATUS_UPDATED), U("LATEST_STATUS_UPDATED")); }

// ── OrderStatus ────────────────────────────────────────────────────────────────

TEST(EnumTest, OrderStatus_PendingNew) { EXPECT_EQ(enum_to_str(OrderStatus::PendingNew), U("PendingNew")); }
TEST(EnumTest, OrderStatus_PendingSubmit) { EXPECT_EQ(enum_to_str(OrderStatus::PendingSubmit), U("PendingSubmit")); }
TEST(EnumTest, OrderStatus_Initial) { EXPECT_EQ(enum_to_str(OrderStatus::Initial), U("Initial")); }
TEST(EnumTest, OrderStatus_Submitted) { EXPECT_EQ(enum_to_str(OrderStatus::Submitted), U("Submitted")); }
TEST(EnumTest, OrderStatus_PartiallyFilled) { EXPECT_EQ(enum_to_str(OrderStatus::PartiallyFilled), U("PartiallyFilled")); }
TEST(EnumTest, OrderStatus_Filled) { EXPECT_EQ(enum_to_str(OrderStatus::Filled), U("Filled")); }
TEST(EnumTest, OrderStatus_Cancelled) { EXPECT_EQ(enum_to_str(OrderStatus::Cancelled), U("Cancelled")); }
TEST(EnumTest, OrderStatus_PendingCancel) { EXPECT_EQ(enum_to_str(OrderStatus::PendingCancel), U("PendingCancel")); }
TEST(EnumTest, OrderStatus_Inactive) { EXPECT_EQ(enum_to_str(OrderStatus::Inactive), U("Inactive")); }
TEST(EnumTest, OrderStatus_Invalid) { EXPECT_EQ(enum_to_str(OrderStatus::Invalid), U("Invalid")); }

// ── TickSizeType ───────────────────────────────────────────────────────────────

TEST(EnumTest, TickSizeType_Open) { EXPECT_EQ(enum_to_str(TickSizeType::OPEN), U("OPEN")); }
TEST(EnumTest, TickSizeType_OpenClosed) { EXPECT_EQ(enum_to_str(TickSizeType::OPEN_CLOSED), U("OPEN_CLOSED")); }
TEST(EnumTest, TickSizeType_ClosedOpen) { EXPECT_EQ(enum_to_str(TickSizeType::CLOSED_OPEN), U("CLOSED_OPEN")); }
TEST(EnumTest, TickSizeType_Closed) { EXPECT_EQ(enum_to_str(TickSizeType::CLOSED), U("CLOSED")); }
