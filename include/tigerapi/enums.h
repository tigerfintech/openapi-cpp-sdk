//
// Created by sukai on 2022/12/22.
//

#ifndef TIGERAPI_ENUMS_H
#define TIGERAPI_ENUMS_H


enum class Market
{
    ALL,
    US,  // 美股
    HK,  // 港股
    CN,  // A股
    SG   // 新加坡
};

inline std::string enum_to_str(Market market) {
    switch (market) {
        case Market::ALL:
            return "ALL";
        case Market::US:
            return "US";
        case Market::HK:
            return "HK";
        case Market::CN:
            return "CN";
        case Market::SG:
            return "SG";
        default:
            return "";
    }
}


enum class TradingSession
{
    PreMarket,  // 盘前
    Regular,    // 盘中
    AfterHours  // 盘后
};

inline std::string enum_to_str(TradingSession session) {
    switch (session) {
        case TradingSession::PreMarket:
            return "PreMarket";
        case TradingSession::Regular:
            return "Regular";
        case TradingSession::AfterHours:
            return "AfterHours";
    }
}

enum class SecType
{
    ALL,
    STK,  // 股票
    OPT,  // 期权
    WAR,  // 窝轮
    IOPT, // 权证(牛熊证)
    FUT,  // 期货
    FOP,  // 期货期权
    CASH  // 外汇
};

inline std::string enum_to_str(SecType secType) {
    std::string name;
    switch (secType) {
        case SecType::ALL:
            name = "ALL";
            break;
        case SecType::STK:
            name = "STK";
            break;
        case SecType::OPT:
            name = "OPT";
            break;
        case SecType::WAR:
            name = "WAR";
            break;
        case SecType::IOPT:
            name = "IOPT";
            break;
        case SecType::FUT:
            name = "FUT";
            break;
        case SecType::FOP:
            name = "FOP";
            break;
        case SecType::CASH:
            name = "CASH";
            break;
    }
    return name;
}

enum class SegmentType
{
    ALL,
    SEC,
    FUT
};
inline std::string enum_to_str(SegmentType segmentType) {
    std::string name;
    switch (segmentType) {
        case SegmentType::ALL:
            name = "ALL";
            break;
        case SegmentType::SEC:
            name = "SEC";
            break;
        case SegmentType::FUT:
            name = "FUT";
            break;
    }
    return name;
}

// Currency 枚举类
enum class Currency {
    ALL,
    USD,
    HKD,
    CNH,
    SGD,
};
inline std::string enum_to_str(Currency currency) {
    std::string name;
    switch (currency) {
        case Currency::ALL:
            name = "ALL";
            break;
        case Currency::USD:
            name = "USD";
            break;
        case Currency::HKD:
            name = "HKD";
            break;
        case Currency::CNH:
            name = "CNH";
            break;
        case Currency::SGD:
            name = "SGD";
            break;
    }
    return name;
}

// Language 枚举类
enum class Language {
    zh_CN,
    zh_TW,
    en_US,
};
inline std::string enum_to_str(Language language) {
    std::string name;
    switch (language) {
        case Language::zh_CN:
            name = "zh_CN";
            break;
        case Language::zh_TW:
            name = "zh_TW";
            break;
        case Language::en_US:
            name = "en_US";
            break;
    }
    return name;
}

// QuoteRight 枚举类
enum class QuoteRight {
    br,
    nr,
};

inline std::string enum_to_str(QuoteRight quoteRight) {
    switch (quoteRight) {
        case QuoteRight::br:
            return "br";
        case QuoteRight::nr:
            return "nr";
        default:
            return "";
    }
}


enum class TimelinePeriod {
    DAY = 1,
    FIVE_DAYS = 2,
};

// 定义字符串数组
static const std::string timeline_period_names[] = {
        "",  // 由于数组的下标从0开始，因此第一个元素需要留空
        "day",
        "5day",
};

// 获取 TimelinePeriod 名称的函数
inline std::string get_timeline_period_value(TimelinePeriod period) {
    return timeline_period_names[static_cast<int>(period)];
}


// BarPeriod 枚举类
enum class BarPeriod {
    DAY,
    WEEK,
    MONTH,
    YEAR,
    ONE_MINUTE,
    THREE_MINUTES,
    FIVE_MINUTES,
    TEN_MINUTES,
    FIFTEEN_MINUTES,
    HALF_HOUR,
    FORTY_FIVE_MINUTES,
    ONE_HOUR,
    TWO_HOURS,
    THREE_HOURS,
    FOUR_HOURS,
    SIX_HOURS,
};

// 获取 BarPeriod 名称的函数
inline std::string enum_to_str(BarPeriod period) {
    switch (period) {
        case BarPeriod::DAY:
            return "day";
        case BarPeriod::WEEK:
            return "week";
        case BarPeriod::MONTH:
            return "month";
        case BarPeriod::YEAR:
            return "year";
        case BarPeriod::ONE_MINUTE:
            return "1min";
        case BarPeriod::THREE_MINUTES:
            return "3min";
        case BarPeriod::FIVE_MINUTES:
            return "5min";
        case BarPeriod::TEN_MINUTES:
            return "10min";
        case BarPeriod::FIFTEEN_MINUTES:
            return "15min";
        case BarPeriod::HALF_HOUR:
            return "30min";
        case BarPeriod::FORTY_FIVE_MINUTES:
            return "45min";
        case BarPeriod::ONE_HOUR:
            return "60min";
        case BarPeriod::TWO_HOURS:
            return "2hour";
        case BarPeriod::THREE_HOURS:
            return "3hours";
        case BarPeriod::FOUR_HOURS:
            return "4hour";
        case BarPeriod::SIX_HOURS:
            return "6hour";
    }
}


enum class CapitalPeriod {
    INTRADAY,
    DAY,
    WEEK,
    MONTH,
    YEAR,
    QUARTER,
    HALFAYEAR,
};

inline std::string enum_to_str(CapitalPeriod period) {
    switch (period) {
        case CapitalPeriod::INTRADAY:
            return "intraday";
        case CapitalPeriod::DAY:
            return "day";
        case CapitalPeriod::WEEK:
            return "week";
        case CapitalPeriod::MONTH:
            return "month";
        case CapitalPeriod::YEAR:
            return "year";
        case CapitalPeriod::QUARTER:
            return "quarter";
        case CapitalPeriod::HALFAYEAR:
            return "6month";
    }
}

enum class OrderStatus {
    PendingNew,
    Initial,
    Submitted,
    PartiallyFilled,
    Filled,
    Cancelled,
    PendingCancel,
    Inactive,
    Invalid
};

#endif //TIGERAPI_ENUMS_H
