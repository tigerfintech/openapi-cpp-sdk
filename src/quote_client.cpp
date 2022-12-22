//
// Created by sukai on 2022/12/21.
//
#include "../include/tigerapi/quote_client.h"
#include "../include/tigerapi/service_types.h"


namespace TIGERAPI {
    QuoteClient::QuoteClient() {};
    QuoteClient::QuoteClient(struct Config &cf, bool is_grab_permission) : TigerClient(cf) {};

    value QuoteClient::grab_quote_permission() {
        value obj = value::object(true);
        return post(GRAB_QUOTE_PERMISSION, obj);
    }

    value QuoteClient::get_symbols(Market market) {
        value obj = value::object(true);
        obj[P_MARKET] = value::string(enum_to_str(market));
        return post(ALL_SYMBOLS, obj);
    }

    value QuoteClient::get_quote_stock_trade(value symbols) {
        value obj = value::object(true);
        obj[P_SYMBOLS] = symbols;
        return post(QUOTE_STOCK_TRADE, obj);
    }

    value QuoteClient::get_all_symbol_names(Market market) {
        value obj = value::object(true);
        obj[P_MARKET] = value::string(enum_to_str(market));
        return post(ALL_SYMBOL_NAMES, obj);
    }

    value QuoteClient::get_brief(value symbols, bool include_hour_trading, bool include_ask_bid, QuoteRight right) {
        value obj = value::object(true);
        obj[P_SYMBOLS] = symbols;
        obj["include_hour_trading"] = include_hour_trading;
        obj["include_ask_bid"] = include_ask_bid;
        obj["right"] = value::string(enum_to_str(right));
        return post(BRIEF, obj);
    }

    value QuoteClient::get_stock_detail(value symbols) {
        value obj = value::object(true);
        obj[P_SYMBOLS] = symbols;
        return post(STOCK_DETAIL, obj);
    }

    value QuoteClient::get_timeline(value symbols, bool include_hour_trading, long begin_time) {
        value obj = value::object(true);
        obj[P_SYMBOLS] = symbols;
        obj["include_hour_trading"] = include_hour_trading;
        obj["begin_time"] = begin_time;
        return post(TIMELINE, obj);
    }

    value
    QuoteClient::get_kline(value symbols, BarPeriod period, long begin_time, long end_time, QuoteRight right, int limit,
                           string page_token) {
        value obj = value::object(true);
        obj[P_SYMBOLS] = symbols;
        obj[P_PERIOD] = value::string(get_bar_period_value(period));
        obj[P_BEGIN_TIME] = begin_time;
        obj[P_END_TIME] = end_time;
        obj["right"] = value::string(enum_to_str(right));
        obj[P_LIMIT] = limit;
        obj["page_token"] = value::string(page_token);
        return post(TIMELINE, obj);
    }

    value QuoteClient::get_history_timeline(value symbols, string date, QuoteRight right) {
        value obj = value::object(true);
        obj[P_SYMBOLS] = symbols;
        obj["date"] = value::string(date);
        obj["right"] = value::string(enum_to_str(right));
        return post(HISTORY_TIMELINE, obj);
    }

    value QuoteClient::get_trade_tick(value symbols, TradingSession trade_session, long begin_index, long end_index,
                                      int limit) {
        value obj = value::object(true);
        obj[P_SYMBOLS] = symbols;
        obj["trade_session"] = value::string(enum_to_str(trade_session));
        obj["begin_index"] = begin_index;
        obj["end_index"] = end_index;
        obj[P_LIMIT] = limit;
        return post(TRADE_TICK, obj);
    }

    value QuoteClient::get_quote_real_time(value symbols) {
        value obj = value::object(true);
        obj[P_SYMBOLS] = symbols;
        return post(QUOTE_REAL_TIME, obj);
    }

    value QuoteClient::get_quote_delay(value symbols) {
        value obj = value::object(true);
        obj[P_SYMBOLS] = symbols;
        return post(QUOTE_DELAY, obj);
    }
}

