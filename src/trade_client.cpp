//
// Created by sukai on 2022/12/23.
//

#include "../include/tigerapi/trade_client.h"

namespace TIGER_API {
    TradeClient::TradeClient() {}

    TradeClient::TradeClient(Config &cf) : TigerClient(cf) {}

    value TradeClient::get_prime_asset(string account, string base_currency) {
        value obj = value::object(true);
        obj[P_ACCOUNT] = get_account_param(account);
        return post(PRIME_ASSETS, obj);
    }

    value TradeClient::get_prime_asset(string account, Currency base_currency) {
        return get_prime_asset(account, enum_to_str(base_currency));
    }

    value TradeClient::get_asset(string account, const value &sub_accounts, bool segment, bool market_value) {
        value obj = value::object(true);
        obj[P_ACCOUNT] = get_account_param(account);
        obj["segment"] = segment;
        obj["market_value"] = market_value;
        return post(PRIME_ASSETS, obj);
    }

    value TradeClient::get_account_param(string account) {
        if (account.empty()) {
            return value::string(client_config.account);
        }
        return value::string(account);
    }

    value TradeClient::get_positions(string account, string sec_type, string currency, string market, string symbol,
                                     const value &sub_accounts, long expiry, double strike, string right) {
        value obj = value::object(true);
        obj[P_ACCOUNT] = get_account_param(account);
        if (!sec_type.empty()) {
            obj[P_SEC_TYPE] = value::string(sec_type);
        }
        if (!currency.empty()) {
            obj[P_CURRENCY] = value::string(currency);
        }
        if (!market.empty()) {
            obj[P_MARKET] = value::string(market);
        }
        if (!symbol.empty()) {
            obj[P_SYMBOL] = value::string(symbol);
        }
        if (!sub_accounts.is_null() && sub_accounts.size() > 0) {
            obj["sub_accounts"] = sub_accounts;
        }
        if (expiry != -1 && expiry != 0) {
            obj[P_EXPIRY] = expiry;
        }
        if (strike != 0) {
            obj[P_STRIKE] = strike;
        }
        if (!right.empty()) {
            obj[P_RIGHT] = value::string(right);
        }
        return post(POSITIONS, obj)[P_ITEMS];
    }

    value TradeClient::get_positions(string account, SecType sec_type, Currency currency, Market market, string symbol,
                                     const value &sub_accounts, long expiry, double strike, Right right) {
        return get_positions(account, enum_to_str(sec_type), enum_to_str(currency), enum_to_str(market),
                             symbol, sub_accounts, expiry, strike, enum_to_str(right));
    }

    value TradeClient::get_orders(string account, string sec_type, string market, string symbol, long start_time,
                                  long end_time, int limit, bool is_brief, const value &states, string sort_by,
                                  string seg_type) {
        value obj = value::object(true);
        obj[P_ACCOUNT] = get_account_param(account);
        if (!sec_type.empty()) {
            obj[P_SEC_TYPE] = value::string(sec_type);
        }
        if (!market.empty()) {
            obj[P_MARKET] = value::string(market);
        }
        if (!symbol.empty()) {
            obj[P_SYMBOL] = value::string(symbol);
        }
        if (start_time != -1 && start_time != 0) {
            obj[P_START_TIME] = start_time;
        }
        if (end_time != -1 && end_time != 0) {
            obj[P_END_TIME] = end_time;
        }
        if (limit != 0) {
            obj[P_LIMIT] = limit;
        }
        if (!states.is_null() && states.size() > 0) {
            obj["states"] = states;
        }
        if (!sort_by.empty()) {
            obj["sort_by"] = value::string(sort_by);
        }
        if (!seg_type.empty()) {
            obj[P_SEG_TYPE] = value::string(seg_type);
        }
        if (is_brief) {
            obj["is_brief"] = is_brief;
        }
        return post(ORDERS, obj)[P_ITEMS];
    }

    value TradeClient::get_orders(string account, SecType sec_type, Market market, string symbol, long start_time,
                                  long end_time, int limit, bool is_brief, const value &states, OrderSortBy sort_by,
                                  SegmentType seg_type) {
        return get_orders(account, enum_to_str(sec_type), enum_to_str(market), symbol, start_time, end_time, limit,
                          is_brief, states, enum_to_str(sort_by), enum_to_str(seg_type));
    }

    value TradeClient::get_active_orders(string account, string sec_type, string market, string symbol, long start_time,
                                         long end_time, long parent_id, string sort_by, string seg_type) {
        value obj = value::object(true);
        obj[P_ACCOUNT] = get_account_param(account);
        if (!sec_type.empty()) {
            obj[P_SEC_TYPE] = value::string(sec_type);
        }
        if (!market.empty()) {
            obj[P_MARKET] = value::string(market);
        }
        if (!symbol.empty()) {
            obj[P_SYMBOL] = value::string(symbol);
        }
        if (start_time != -1 && start_time != 0) {
            obj[P_START_TIME] = start_time;
        }
        if (end_time != -1 && end_time != 0) {
            obj[P_END_TIME] = end_time;
        }
        if (parent_id != 0) {
            obj["parent_id"] = parent_id;
        }
        if (!sort_by.empty()) {
            obj["sort_by"] = value::string(sort_by);
        }
        if (!seg_type.empty()) {
            obj[P_SEG_TYPE] = value::string(seg_type);
        }
        return post(ACTIVE_ORDERS, obj)[P_ITEMS];
    }

    value
    TradeClient::get_active_orders(string account, SecType sec_type, Market market, string symbol, long start_time,
                                   long end_time, long parent_id, OrderSortBy sort_by, SegmentType seg_type) {
        return get_active_orders(account, enum_to_str(sec_type), enum_to_str(market), symbol, start_time, end_time,
                                 parent_id, enum_to_str(sort_by), enum_to_str(seg_type));
    }


    value TradeClient::get_contract(string symbol, string sec_type, string currency, string exchange, long expiry,
                                    double strike, string right) {
        value obj = value::object(true);
        obj[P_ACCOUNT] = get_account_param();
        if (!symbol.empty()) {
            obj[P_SYMBOL] = value::string(symbol);
        }
        if (!sec_type.empty()) {
            obj[P_SEC_TYPE] = value::string(sec_type);
        }
        if (!currency.empty()) {
            obj[P_CURRENCY] = value::string(currency);
        }
        if (!exchange.empty()) {
            obj[P_EXCHANGE] = value::string(exchange);
        }
        if (expiry != -1 && expiry != 0) {
            obj[P_EXPIRY] = expiry;
        }
        if (strike != -1 && strike != 0) {
            obj[P_STRIKE] = strike;
        }
        if (!right.empty()) {
            obj[P_RIGHT] = value::string(right);
        }
        return post(CONTRACT, obj);
    }

    value TradeClient::get_contract(string symbol, SecType sec_type, Currency currency, string exchange, long expiry,
                                    double strike, Right right) {
        return get_contract(symbol, enum_to_str(sec_type), enum_to_str(currency), exchange, expiry, strike,
                            enum_to_str(right));
    }

    value TradeClient::place_order(value &order) {
        value obj = value::object(true);
        for (const auto &kvp: order.as_object()) {
            obj[kvp.first] = kvp.second;
        }
        obj[P_ACCOUNT] = get_account_param(order[P_ACCOUNT].as_string());

        value res = post(PLACE_ORDER, obj);
     }

}
