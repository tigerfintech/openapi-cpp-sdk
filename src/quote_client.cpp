//
// Created by sukai on 2022/12/21.
//
#include "../include/tigerapi/quote_client.h"
#include "../include/tigerapi/contract_util.h"
#include "../include/tigerapi/utils.h"


namespace TIGER_API {
    QuoteClient::QuoteClient() {};
    QuoteClient::QuoteClient(const ClientConfig &cf, bool is_grab_permission) : TigerClient(cf) {

    }

    value QuoteClient::grab_quote_permission() {
        value obj = value::object(true);
        return post(GRAB_QUOTE_PERMISSION, obj);
    }

    value QuoteClient::get_symbols(Market market) {
        value obj = value::object(true);
        obj[P_MARKET] = value::string(enum_to_str(market));
        return post(ALL_SYMBOLS, obj);
    }

    value QuoteClient::get_quote_stock_trade(const value &symbols) {
        value obj = value::object(true);
        obj[P_SYMBOLS] = symbols;
        return post(QUOTE_STOCK_TRADE, obj);
    }

    value QuoteClient::get_all_symbol_names(Market market) {
        return get_all_symbol_names(enum_to_str(market));
    }

    value QuoteClient::get_all_symbol_names(utility::string_t market) {
        value obj = value::object(true);
        obj[P_MARKET] = value::string(market);
        return post(ALL_SYMBOL_NAMES, obj);
    }

    value QuoteClient::get_market_state(utility::string_t market) {
        value obj = value::object(true);
        obj[P_MARKET] = value::string(market);
        return post(MARKET_STATE, obj);
    }

    value
    QuoteClient::get_brief(const value &symbols, bool include_hour_trading, bool include_ask_bid, QuoteRight right) {
        value obj = value::object(true);
        obj[P_SYMBOLS] = symbols;
        obj[U("include_hour_trading")] = include_hour_trading;
        obj[U("include_ask_bid")] = include_ask_bid;
        obj[P_RIGHT] = value::string(enum_to_str(right));
        return post(BRIEF, obj);
    }

    value QuoteClient::get_stock_detail(const value &symbols) {
        value obj = value::object(true);
        obj[P_SYMBOLS] = symbols;
        return post(STOCK_DETAIL, obj);
    }

    value QuoteClient::get_timeline(const value &symbols, bool include_hour_trading, long begin_time) {
        value obj = value::object(true);
        obj[P_SYMBOLS] = symbols;
        obj[U("include_hour_trading")] = include_hour_trading;
        obj[P_BEGIN_TIME] = begin_time;
        return post(TIMELINE, obj);
    }

    value QuoteClient::get_kline(const value &symbols, utility::string_t period, long begin_time, long end_time, utility::string_t right,
                                 int limit, utility::string_t page_token) {
        value obj = value::object(true);
        obj[P_SYMBOLS] = symbols;
        obj[P_PERIOD] = value::string(period);
        obj[P_BEGIN_TIME] = begin_time;
        obj[P_END_TIME] = end_time;
        obj[P_RIGHT] = value::string(right);
        obj[P_LIMIT] = limit;
        obj[P_PAGE_TOKEN] = value::string(page_token);
        return post(KLINE, obj);
    }

    value
    QuoteClient::get_kline(const value &symbols, BarPeriod period, long begin_time, long end_time, QuoteRight right,
                           int limit, utility::string_t page_token) {
        return get_kline(symbols, enum_to_str(period), begin_time, end_time, enum_to_str(right), limit,
                         page_token);
    }

    value QuoteClient::get_history_timeline(const value &symbols, utility::string_t date, QuoteRight right) {
        value obj = value::object(true);
        obj[P_SYMBOLS] = symbols;
        obj[P_DATE] = value::string(date);
        obj[P_RIGHT] = value::string(enum_to_str(right));
        return post(HISTORY_TIMELINE, obj);
    }

    value
    QuoteClient::get_trade_tick(const value &symbols, TradingSession trade_session, long begin_index, long end_index,
                                int limit) {
        return get_trade_tick(symbols, begin_index, end_index, enum_to_str(trade_session), limit);
    }

    value QuoteClient::get_trade_tick(const value &symbols, long begin_index,
                                                    long end_index, utility::string_t trade_session, int limit) {
        value obj = value::object(true);
        obj[P_SYMBOLS] = symbols;
        obj[P_TRADE_SESSION] = value::string(trade_session);
        obj[P_BEGIN_INDEX] = begin_index;
        obj[P_END_INDEX] = end_index;
        obj[P_LIMIT] = limit;
        return post(TRADE_TICK, obj);
    }

    value QuoteClient::get_quote_real_time(const value &symbols) {
        value obj = value::object(true);
        obj[P_SYMBOLS] = symbols;
        return post(QUOTE_REAL_TIME, obj);
    }

    value QuoteClient::get_quote_delay(const value &symbols) {
        value obj = value::object(true);
        obj[P_SYMBOLS] = symbols;
        return post(QUOTE_DELAY, obj);
    }

    value QuoteClient::get_quote_depth(const value &symbols, Market market) {
        value obj = value::object(true);
        obj[P_SYMBOLS] = symbols;
        return post(QUOTE_DEPTH, obj);
    }

    value QuoteClient::get_quote_shortable_stocks(const value &symbols) {
        value obj = value::object(true);
        obj[P_SYMBOLS] = symbols;
        return post(QUOTE_SHORTABLE_STOCKS, obj);
    }

    value QuoteClient::get_quote_permission() {
        value obj = value::object(true);
        return post(GET_QUOTE_PERMISSION, obj);
    }

    value QuoteClient::get_trading_calendar(Market market, utility::string_t begin_date, utility::string_t end_date) {
        return get_trading_calendar(enum_to_str(market), begin_date, end_date);
    }

    value QuoteClient::get_trading_calendar(utility::string_t market, utility::string_t begin_date, utility::string_t end_date) {
        value obj = value::object(true);
        obj[P_MARKET] = value::string(market);
        obj[P_BEGIN_DATE] = value::string(begin_date);
        obj[P_END_DATE] = value::string(end_date);
        return post(TRADING_CALENDAR, obj);
    }

    value QuoteClient::get_stock_broker(utility::string_t symbol, int limit) {
        value obj = value::object(true);
        obj[P_SYMBOL] = value::string(symbol);
        obj[P_LIMIT] = limit;
        return post(STOCK_BROKER, obj);
    }

    value QuoteClient::get_capital_flow(utility::string_t symbol, Market market, CapitalPeriod period) {
        return get_capital_flow(symbol, enum_to_str(market), enum_to_str(period));
    }

    value QuoteClient::get_capital_flow(utility::string_t symbol, utility::string_t market, utility::string_t period) {
        value obj = value::object(true);
        obj[P_SYMBOL] = value::string(symbol);
        obj[P_MARKET] = value::string(market);
        obj[P_PERIOD] = value::string(period);
        return post(CAPITAL_FLOW, obj);
    }

    value QuoteClient::get_capital_distribution(utility::string_t symbol, Market market) {
        value obj = value::object(true);
        obj[P_SYMBOL] = value::string(symbol);
        obj[P_MARKET] = value::string(enum_to_str(market));
        return post(CAPITAL_DISTRIBUTION, obj);
    }

    value QuoteClient::get_option_expiration(const value &symbols) {
        value obj = value::object(true);
        obj[P_SYMBOLS] = symbols;
        return post(OPTION_EXPIRATION, obj);
    }

    value QuoteClient::get_option_chain(const utility::string_t symbol, long expiry, value option_filter) {
        value obj = value::object(true);
        value basic = value::object(true);
        basic[P_SYMBOL] = value::string(symbol);
        basic[P_EXPIRY] = expiry;
        value option_basic = value::array();
        option_basic[0] = basic;
        obj[U("option_basic")] = option_basic;
        return post(OPTION_CHAIN, obj);
    }

    value QuoteClient::get_option_chain(const utility::string_t symbol, utility::string_t expiry, value option_filter) {
        long expiry_ts = date_string_to_timestamp(expiry);
        return get_option_chain(symbol, expiry_ts, option_filter);
    }

    value QuoteClient::get_option_brief(const utility::string_t identifier) {
        value options = value::array();
        options[0] = value::string(identifier);
        return get_option_brief(options);
    }

    value QuoteClient::get_option_brief(value identifiers) {
        value options = identifiers_to_options(identifiers);
        return post(OPTION_BRIEF, options);
    }

    value QuoteClient::get_option_kline(value identifiers, long begin_time, long end_time) {
        value options = value::array();
        for (size_t i = 0; i < identifiers.size(); ++i) {
            auto identifier = identifiers[i];
            utility::string_t symbol, expiry, right, strike;
            std::tie(symbol, expiry, right, strike) = extract_option_info(identifier.as_string());
            if (symbol.empty() || expiry.empty() || right.empty()) {
                continue;
            }
            value obj = value::object(true);
            obj[P_BEGIN_TIME] = begin_time;
            obj[P_END_TIME] = end_time;
            obj[P_EXPIRY] = date_string_to_timestamp(expiry);
            obj[P_PERIOD] = value::string(U("day"));
            obj[P_RIGHT] = value::string(right);
            obj[P_STRIKE] = value::string(strike);
            obj[P_SYMBOL] = value::string(symbol);
            options[i] = obj;
        }
        return post(OPTION_KLINE, options);
    }

    value QuoteClient::get_option_trade_tick(value identifiers) {
        value options = identifiers_to_options(identifiers);
        return post(OPTION_TRADE_TICK, options);
    }

    value QuoteClient::get_future_exchange(SecType sec_type) {
        value obj = value::object(true);
        obj[P_SEC_TYPE] = value::string(enum_to_str(sec_type));
        return post(FUTURE_EXCHANGE, obj);
    }

    value QuoteClient::get_future_contract_by_contract_code(utility::string_t contract_code) {
        value obj = value::object(true);
        obj[P_CONTRACT_CODE] = value::string(contract_code);
        return post(FUTURE_CONTRACT_BY_CONTRACT_CODE, obj);
    }

    value QuoteClient::get_future_contract_by_exchange_code(utility::string_t exchange_code) {
        value obj = value::object(true);
        obj[P_EXCHANGE_CODE] = value::string(exchange_code);
        return post(FUTURE_CONTRACT_BY_EXCHANGE_CODE, obj);
    }

    value QuoteClient::get_future_contracts(utility::string_t type) {
        value obj = value::object(true);
        obj[P_TYPE] = value::string(type);
        return post(FUTURE_CONTRACTS, obj);
    }

    value QuoteClient::get_future_continuous_contracts(utility::string_t type) {
        value obj = value::object(true);
        obj[P_TYPE] = value::string(type);
        return post(FUTURE_CONTINUOUS_CONTRACTS, obj);
    }

    value QuoteClient::get_future_current_contract(utility::string_t type) {
        value obj = value::object(true);
        obj[P_TYPE] = value::string(type);
        return post(FUTURE_CURRENT_CONTRACT, obj);
    }

    value
    QuoteClient::get_future_kline(value contract_codes, BarPeriod period, long begin_time, long end_time, int limit,
                                  utility::string_t page_token) {
        value obj = value::object(true);
        obj[P_CONTRACT_CODES] = contract_codes;
        obj[P_PERIOD] = value::string(enum_to_str(period));
        obj[P_BEGIN_TIME] = begin_time;
        obj[P_END_TIME] = end_time;
        obj[P_LIMIT] = limit;
        obj[P_PAGE_TOKEN] = value::string(page_token);
        return post(FUTURE_KLINE, obj);
    }

    value QuoteClient::get_future_real_time_quote(value contract_codes) {
        value obj = value::object(true);
        obj[P_CONTRACT_CODES] = contract_codes;
        return post(FUTURE_REAL_TIME_QUOTE, obj);
    }

    value QuoteClient::get_future_tick(utility::string_t contract_code, long begin_index, long end_index, int limit) {
        value obj = value::object(true);
        obj[P_CONTRACT_CODE] = value::string(contract_code);
        obj[P_BEGIN_INDEX] = begin_index;
        obj[P_END_INDEX] = end_index;
        obj[P_LIMIT] = limit;
        return post(FUTURE_TICK, obj);
    }

    value QuoteClient::get_future_trading_date(utility::string_t contract_code, utility::string_t trading_date) {
        value obj = value::object(true);
        obj[P_CONTRACT_CODE] = value::string(contract_code);
        obj[P_TRADING_DATE] = value::string(trading_date);
        return post(FUTURE_TRADING_DATE, obj);
    }


}

