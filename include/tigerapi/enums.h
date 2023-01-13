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
            return U("ALL");
        case Market::US:
            return U("US");
        case Market::HK:
            return U("HK");
        case Market::CN:
            return U("CN");
        case Market::SG:
            return U("SG");
        default:
            return U("");
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
            return U("PreMarket");
        case TradingSession::Regular:
            return U("Regular");
        case TradingSession::AfterHours:
            return U("AfterHours");
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
            name = U("");
            break;
        case SecType::STK:
            name = U("STK");
            break;
        case SecType::OPT:
            name = U("OPT");
            break;
        case SecType::WAR:
            name = U("WAR");
            break;
        case SecType::IOPT:
            name = U("IOPT");
            break;
        case SecType::FUT:
            name = U("FUT");
            break;
        case SecType::FOP:
            name = U("FOP");
            break;
        case SecType::CASH:
            name = U("CASH");
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
            name = U("ALL");
            break;
        case SegmentType::SEC:
            name = U("SEC");
            break;
        case SegmentType::FUT:
            name = U("FUT");
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
            name = U("ALL");
            break;
        case Currency::USD:
            name = U("USD");
            break;
        case Currency::HKD:
            name = U("HKD");
            break;
        case Currency::CNH:
            name = U("CNH");
            break;
        case Currency::SGD:
            name = U("SGD");
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
            name = U("zh_CN");
            break;
        case Language::zh_TW:
            name = U("zh_TW");
            break;
        case Language::en_US:
            name = U("en_US");
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
            return U("br");
        case QuoteRight::nr:
            return U("nr");
        default:
            return U("");
    }
}

enum class Right {
    PUT,
    CALL,
    ALL,
};

inline std::string enum_to_str(Right right) {
    switch (right) {
        case Right::ALL:
            return U("");
        case Right::PUT:
            return U("PUT");
        case Right::CALL:
            return U("CALL");
        default:
            return U("");
    }
}

enum class TimelinePeriod {
    DAY = 1,
    FIVE_DAYS = 2,
};

// 定义字符串数组
static const std::string timeline_period_names[] = {
        U(""),  // 由于数组的下标从0开始，因此第一个元素需要留空
        U("day"),
        U("5day"),
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
            return U("day");
        case BarPeriod::WEEK:
            return U("week");
        case BarPeriod::MONTH:
            return U("month");
        case BarPeriod::YEAR:
            return U("year");
        case BarPeriod::ONE_MINUTE:
            return U("1min");
        case BarPeriod::THREE_MINUTES:
            return U("3min");
        case BarPeriod::FIVE_MINUTES:
            return U("5min");
        case BarPeriod::TEN_MINUTES:
            return U("10min");
        case BarPeriod::FIFTEEN_MINUTES:
            return U("15min");
        case BarPeriod::HALF_HOUR:
            return U("30min");
        case BarPeriod::FORTY_FIVE_MINUTES:
            return U("45min");
        case BarPeriod::ONE_HOUR:
            return U("60min");
        case BarPeriod::TWO_HOURS:
            return U("2hour");
        case BarPeriod::THREE_HOURS:
            return U("3hours");
        case BarPeriod::FOUR_HOURS:
            return U("4hour");
        case BarPeriod::SIX_HOURS:
            return U("6hour");
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
            return U("intraday");
        case CapitalPeriod::DAY:
            return U("day");
        case CapitalPeriod::WEEK:
            return U("week");
        case CapitalPeriod::MONTH:
            return U("month");
        case CapitalPeriod::YEAR:
            return U("year");
        case CapitalPeriod::QUARTER:
            return U("quarter");
        case CapitalPeriod::HALFAYEAR:
            return U("6month");
    }
}

enum class OrderSortBy {
    LATEST_CREATED,
    LATEST_STATUS_UPDATED
};

inline std::string enum_to_str(OrderSortBy sortBy) {
    switch (sortBy) {
        case OrderSortBy::LATEST_CREATED:
            return U("LATEST_CREATED");
        case OrderSortBy::LATEST_STATUS_UPDATED:
            return U("LATEST_STATUS_UPDATED");
        default:
            return U("");
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

inline std::string enum_to_str(OrderStatus status) {
    switch (status) {
        case OrderStatus::PendingNew:
            return U("PendingNew");
        case OrderStatus::Initial:
            return U("Initial");
        case OrderStatus::Submitted:
            return U("Submitted");
        case OrderStatus::PartiallyFilled:
            return U("PartiallyFilled");
        case OrderStatus::Filled:
            return U("Filled");
        case OrderStatus::Cancelled:
            return U("Cancelled");
        case OrderStatus::PendingCancel:
            return U("PendingCancel");
        case OrderStatus::Inactive:
            return U("Inactive");
        case OrderStatus::Invalid:
            return U("Invalid");
        default:
            return U("");
    }
}


#endif //TIGERAPI_ENUMS_H
