//
// Created by sukai on 2022/12/22.
//

#ifndef TIGERAPI_ENUMS_H
#define TIGERAPI_ENUMS_H

#include <string>
#include <map>

// Quote level prefixes
const std::string HK_QUOTE_LEVEL_PREFIX = "hk";
const std::string US_QUOTE_LEVEL_PREFIX = "us";


namespace TIGER_API {

    enum class Market {
        ALL,
        US,  // 美股
        HK,  // 港股
        CN,  // A股
        SG   // 新加坡
    };

    inline utility::string_t enum_to_str(Market market) {
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


    enum class TradingSession {
        PreMarket,  // 盘前
        Regular,    // 盘中
        AfterHours  // 盘后
    };

    inline utility::string_t enum_to_str(TradingSession session) {
        switch (session) {
            case TradingSession::PreMarket:
                return U("PreMarket");
            case TradingSession::Regular:
                return U("Regular");
            case TradingSession::AfterHours:
                return U("AfterHours");
        }
    }

    enum class SecType {
        ALL,
        STK,  // 股票
        OPT,  // 期权
        WAR,  // 窝轮
        IOPT, // 权证(牛熊证)
        FUT,  // 期货
        FOP,  // 期货期权
        CASH  // 外汇
    };

    inline utility::string_t enum_to_str(SecType secType) {
        utility::string_t name;
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

    enum class SegmentType {
        ALL,
        SEC,
        FUT
    };

    inline utility::string_t enum_to_str(SegmentType segmentType) {
        utility::string_t name;
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

    inline utility::string_t enum_to_str(Currency currency) {
        utility::string_t name;
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

    inline utility::string_t enum_to_str(Language language) {
        utility::string_t name;
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

    inline utility::string_t enum_to_str(QuoteRight quoteRight) {
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

    inline utility::string_t enum_to_str(Right right) {
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
    static const utility::string_t timeline_period_names[] = {
            U(""),  // 由于数组的下标从0开始，因此第一个元素需要留空
            U("day"),
            U("5day"),
    };

// 获取 TimelinePeriod 名称的函数
    inline utility::string_t get_timeline_period_value(TimelinePeriod period) {
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
    inline utility::string_t enum_to_str(BarPeriod period) {
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

    inline utility::string_t enum_to_str(CapitalPeriod period) {
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

    inline utility::string_t enum_to_str(OrderSortBy sortBy) {
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
        PendingSubmit,
        Initial,
        Submitted,
        PartiallyFilled,
        Filled,
        Cancelled,
        PendingCancel,
        Inactive,
        Invalid
    };

    inline utility::string_t enum_to_str(OrderStatus status) {
        switch (status) {
            case OrderStatus::PendingNew:
                return U("PendingNew");
            case OrderStatus::PendingSubmit:
                return U("PendingSubmit");
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

    enum class TickSizeType {
        OPEN,
        OPEN_CLOSED,
        CLOSED_OPEN,
        CLOSED
    };

    inline  utility::string_t enum_to_str(TickSizeType type) {
        switch (type) {
            case TickSizeType::OPEN: return U("OPEN"); break;
            case TickSizeType::OPEN_CLOSED: return U("OPEN_CLOSED"); break;
            case TickSizeType::CLOSED_OPEN: return U("CLOSED_OPEN"); break;
            case TickSizeType::CLOSED: return U("CLOSED"); break;
            default: return U(""); break;
        }
    };



    // Part code name mapping
    const std::map<std::string, std::string> PART_CODE_NAME_MAP = {
        {"a", "NYSE American, LLC (NYSE American)"},
        {"b", "NASDAQ OMX BX, Inc. (NASDAQ OMX BX)"},
        {"c", "NYSE National, Inc. (NYSE National)"},
        {"d", "FINRA Alternative Display Facility (ADF)"},
        {"h", "MIAX Pearl Exchange, LLC (MIAX)"},
        {"i", "International Securities Exchange, LLC (ISE)"},
        {"j", "Cboe EDGA Exchange, Inc. (Cboe EDGA)"},
        {"k", "Cboe EDGX Exchange, Inc. (Cboe EDGX)"},
        {"l", "Long-Term Stock Exchange, Inc. (LTSE)"},
        {"m", "NYSE Chicago, Inc. (NYSE Chicago)"},
        {"n", "New York Stock Exchange, LLC (NYSE)"},
        {"p", "NYSE Arca, Inc. (NYSE Arca)"},
        {"s", "Consolidated Tape System (CTS)"},
        {"t", "NASDAQ Stock Market, LLC (NASDAQ)"},
        {"u", "Members Exchange, LLC (MEMX)"},
        {"v", "Investors' Exchange, LLC. (IEX)"},
        {"w", "CBOE Stock Exchange, Inc. (CBSX)"},
        {"x", "NASDAQ OMX PSX, Inc. (NASDAQ OMX PSX)"},
        {"y", "Cboe BYX Exchange, Inc. (Cboe BYX)"},
        {"z", "Cboe BZX Exchange, Inc. (Cboe BZX)"}
    };

    // Part code mapping
    const std::map<std::string, std::string> PART_CODE_MAP = {
        {"a", "AMEX"},
        {"b", "BX"},
        {"c", "NSX"},
        {"d", "ADF"},
        {"h", "MIAX"},
        {"i", "ISE"},
        {"j", "EDGA"},
        {"k", "EDGX"},
        {"l", "LTSE"},
        {"m", "CHO"},
        {"n", "NYSE"},
        {"p", "ARCA"},
        {"s", "CTS"},
        {"t", "NSDQ"},
        {"u", "MEMX"},
        {"v", "IEX"},
        {"w", "CBSX"},
        {"x", "PSX"},
        {"y", "BYX"},
        {"z", "BZX"}
    };

    // US trade condition mapping
    const std::map<std::string, std::string> US_TRADE_COND_MAP = {
        {" ", "US_REGULAR_SALE"},         // 自动对盘
        {"B", "US_BUNCHED_TRADE"},        // 批量交易
        {"C", "US_CASH_TRADE"},          // 现金交易
        {"F", "US_INTERMARKET_SWEEP"},    // 跨市场交易
        {"G", "US_BUNCHED_SOLD_TRADE"},   // 批量卖出
        {"H", "US_PRICE_VARIATION_TRADE"},// 离价交易
        {"I", "US_ODD_LOT_TRADE"},       // 碎股交易
        {"K", "US_RULE_127_OR_155_TRADE"},// 纽交所 第127条交易 或 第155条交易
        {"L", "US_SOLD_LAST"},           // 延迟交易
        {"M", "US_MARKET_CENTER_CLOSE_PRICE"},// 中央收市价
        {"N", "US_NEXT_DAY_TRADE"},      // 隔日交易
        {"O", "US_MARKET_CENTER_OPENING_TRADE"},// 中央开盘价交易
        {"P", "US_PRIOR_REFERENCE_PRICE"},// 前参考价
        {"Q", "US_MARKET_CENTER_OPEN_PRICE"},// 中央开盘价
        {"R", "US_SELLER"},              // 卖方
        {"T", "US_FORM_T"},              // 盘前盘后交易
        {"U", "US_EXTENDED_TRADING_HOURS"},// 延长交易时段
        {"V", "US_CONTINGENT_TRADE"},    // 合单交易
        {"W", "US_AVERAGE_PRICE_TRADE"}, // 均价交易
        {"X", "US_CROSS_TRADE"},         //
        {"Z", "US_SOLD_OUT_OF_SEQUENCE"},// 场外售出
        {"0", "US_ODD_LOST_CROSS_TRADE"},// 碎股跨市场交易
        {"4", "US_DERIVATIVELY_PRICED"}, // 衍生工具定价
        {"5", "US_MARKET_CENTER_RE_OPENING_TRADE"},// 再开盘定价
        {"6", "US_MARKET_CENTER_CLOSING_TRADE"},   // 收盘定价
        {"7", "US_QUALIFIED_CONTINGENT_TRADE"},    // 合单交易
        {"9", "US_CONSOLIDATED_LAST_PRICE_PER_LISTING_PACKET"}// 综合延迟价格
    };

    // HK trade condition mapping
    const std::map<std::string, std::string> HK_TRADE_COND_MAP = {
        {" ", "HK_AUTOMATCH_NORMAL"},         // 自动对盘
        {"D", "HK_ODD_LOT_TRADE"},           // 碎股交易
        {"U", "HK_AUCTION_TRADE"},           // 竞价交易
        {"*", "HK_OVERSEAS_TRADE"},          // 场外交易
        {"P", "HK_LATE_TRADE_OFF_EXCHG"},    // 开市前成交
        {"M", "HK_NON_DIRECT_OFF_EXCHG_TRADE"}, // 非自动对盘
        {"X", "HK_DIRECT_OFF_EXCHG_TRADE"},     // 同券商自动对盘
        {"Y", "HK_AUTOMATIC_INTERNALIZED"}      // 同券商非自动对盘
    };
}


#endif //TIGERAPI_ENUMS_H
