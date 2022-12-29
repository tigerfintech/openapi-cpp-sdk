//
// Created by sukai on 2022/12/23.
//

#include "../include/tigerapi/trade_client.h"

namespace TIGER_API {
    TradeClient::TradeClient() {}

//    TradeClient::TradeClient(Config &cf) : TigerClient(cf) {}
    TradeClient::TradeClient(const ClientConfig &cf) : TigerClient(cf) {}

    value TradeClient::get_prime_asset(const string &account, const string &base_currency) {
        value obj = value::object(true);
        obj[P_ACCOUNT] = get_account_param(account);
        obj["base_currency"] = value::string(base_currency);
        return post(PRIME_ASSETS, obj);
    }

    value TradeClient::get_prime_asset(const string &account, Currency base_currency) {
        return get_prime_asset(account, enum_to_str(base_currency));
    }

    PortfolioAccount TradeClient::get_prime_portfolio(const string &account, const string &base_currency) {
        value asset = get_prime_asset(account, base_currency);
        PortfolioAccount portfolio = PortfolioAccount();
        portfolio.account = asset.at("accountId").as_string();
        portfolio.update_timestamp = (long) asset.at("updateTimestamp").as_number().to_uint64();
        vector<Segment> segments;
        for (const auto &element: asset.at("segments").as_array()) {
            Segment s = Segment(element);
            vector<CurrencyAsset> currency_assets;
            for (const auto &c: element.at("currencyAssets").as_array()) {
                currency_assets.push_back(CurrencyAsset(c));
            }
            s.currency_assets = currency_assets;
            segments.push_back(s);
        }
        portfolio.segments = segments;
        return portfolio;
    }

    value TradeClient::get_asset(string account, const value &sub_accounts, bool segment, bool market_value) {
        value obj = value::object(true);
        obj[P_ACCOUNT] = get_account_param(account);
        obj["segment"] = segment;
        obj["market_value"] = market_value;
        return post(PRIME_ASSETS, obj);
    }

    value TradeClient::get_account_param(const string &account) {
        if (account.empty()) {
            return value::string(client_config.account);
        }
        return value::string(account);
    }

    value TradeClient::get_positions(const string &account, const string &sec_type, const string &currency,
                                     const string &market, const string &symbol,
                                     const value &sub_accounts, long expiry, double strike, const string &right) {
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


    vector<Position>
    TradeClient::get_position_list(string account, string sec_type, string currency, string market, string symbol,
                                   const value &sub_accounts, long expiry, double strike, string right) {
        std::vector<Position> vec;
        value positions = get_positions(account, sec_type, currency, market, symbol,
                                        sub_accounts, expiry, strike, right);
        for (const auto &element: positions.as_array()) {
            Position p = Position(element);
            vec.push_back(p);
        }
        return vec;
    }

    value TradeClient::get_orders(const string &account, const string &sec_type, const string &market, const string &symbol, long start_time,
                                  long end_time, int limit, bool is_brief, const value &states, const string &sort_by,
                                  const string &seg_type) {
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
        return res;
    }

    value TradeClient::place_order(Order &order) {
        value obj = value::object(true);
        Contract contract = order.contract;
        if (!contract.symbol.empty()) {
            obj["symbol"] = value::string(contract.symbol);
        }
        if (!contract.currency.empty()) {
            obj["currency"] = value::string(contract.currency);
        }
        if (!contract.sec_type.empty()) {
            obj["sec_type"] = value::string(contract.sec_type);
        }
        if (!contract.exchange.empty()) {
            obj["exchange"] = value::string(contract.exchange);
        }
        if (!contract.expiry.empty()) {
            obj["expiry"] = value::string(contract.expiry);
        }
        if (contract.strike != 0) {
            obj["strike"] = contract.strike;
        }
        if (!contract.right.empty()) {
            obj["right"] = value::string(contract.right);
        }
        if (contract.multiplier != 0) {
            obj["multiplier"] = contract.multiplier;
        }

        obj[P_ACCOUNT] = get_account_param(order.account);

        if (!order.secret_key.empty()) {
            obj["secret_key"] = value::string(order.secret_key);
        }

        if (order.order_id != 0) {
            obj["order_id"] = order.order_id;
        }
        if (order.id != 0) {
            obj["id"] = order.id;
        }
        if (!order.order_type.empty()) {
            obj["order_type"] = value::string(order.order_type);
        }
        if (!order.action.empty()) {
            obj["action"] = value::string(order.action);
        }
        if (order.total_quantity) {
            obj["total_quantity"] = order.total_quantity;
        }
        if (order.limit_price != 0) {
            obj["limit_price"] = order.limit_price;
        }
        if (order.aux_price != 0) {
            obj["aux_price"] = order.aux_price;
        }
        if (order.trail_stop_price != 0) {
            obj["trail_stop_price"] = order.trail_stop_price;
        }
        if (order.trailing_percent != 0) {
            obj["trailing_percent"] = order.trailing_percent;
        }
        if (order.percent_offset != 0) {
            obj["percent_offset"] = order.percent_offset;
        }
        if (!order.time_in_force.empty()) {
            obj["time_in_force"] = value::string(order.time_in_force);
        }
        if (order.outside_rth) {
            obj["outside_rth"] = order.outside_rth;
        }
        if (order.adjust_limit) {
            obj["adjust_limit"] = order.adjust_limit;
        }
        if (!order.user_mark.empty()) {
            obj["user_mark"] = value::string(order.user_mark);
        }
        if (order.expire_time) {
            obj["expire_time"] = order.expire_time;
        }
        value res = post(PLACE_ORDER, obj);
        try {
            order.id = res["id"].as_number().to_uint64();
            order.sub_ids = res["subIds"];
        } catch (...) {
            cout << "Warn: id not returned" << endl;
        }
        return res;
    }

    Order TradeClient::get_order(long id, bool is_brief) {
        value obj = value::object(true);
        obj[P_ACCOUNT] = get_account_param();
        obj["id"] = id;
        obj["is_brief"] = is_brief;
        value res = post(ORDERS, obj);
        if (!res.is_null()) {
            Order order = Order(res);
            return order;
        } else {
            cout << "Exception: order " << id << " not exist, result: " << res << endl;
            exit(-1);
        }
    }

    value TradeClient::cancel_order(long id) {
        value obj = value::object(true);
        obj[P_ACCOUNT] = get_account_param();
        obj["id"] = id;
        value res = post(CANCEL_ORDER, obj);
        return res;
    }

    value TradeClient::modify_order(Order &order) {
        value obj = value::object(true);

        obj[P_ACCOUNT] = get_account_param(order.account);

        if (order.id != 0) {
            obj["id"] = order.id;
        }
        if (!order.order_type.empty()) {
            obj["order_type"] = value::string(order.order_type);
        }
        if (!order.action.empty()) {
            obj["action"] = value::string(order.action);
        }
        if (order.total_quantity) {
            obj["total_quantity"] = order.total_quantity;
        }
        if (order.limit_price != 0) {
            obj["limit_price"] = order.limit_price;
        }
        if (order.aux_price != 0) {
            obj["aux_price"] = order.aux_price;
        }
        if (order.trail_stop_price != 0) {
            obj["trail_stop_price"] = order.trail_stop_price;
        }
        if (order.trailing_percent != 0) {
            obj["trailing_percent"] = order.trailing_percent;
        }
        if (order.percent_offset != 0) {
            obj["percent_offset"] = order.percent_offset;
        }
        if (!order.time_in_force.empty()) {
            obj["time_in_force"] = value::string(order.time_in_force);
        }
        if (order.outside_rth) {
            obj["outside_rth"] = order.outside_rth;
        }
        if (order.adjust_limit) {
            obj["adjust_limit"] = order.adjust_limit;
        }
        if (!order.user_mark.empty()) {
            obj["user_mark"] = value::string(order.user_mark);
        }
        if (order.expire_time) {
            obj["expire_time"] = order.expire_time;
        }
        value res = post(MODIFY_ORDER, obj);
        return res;
    }

    value TradeClient::modify_order(Order &order, double limit_price, long total_quantity, double aux_price,
                                    double trail_stop_price, double trailing_percent, double percent_offset,
                                    string time_in_force, bool outside_rth, long expire_time) {
        value obj = value::object(true);
        obj[P_ACCOUNT] = get_account_param(order.account);

        if (order.id != 0) {
            obj["id"] = order.id;
        }
        if (!order.order_type.empty()) {
            obj["order_type"] = value::string(order.order_type);
        }
        if (!order.action.empty()) {
            obj["action"] = value::string(order.action);
        }
        if (total_quantity != 0) {
            obj["total_quantity"] = total_quantity;
        } else {
            obj["total_quantity"] = order.total_quantity;
        }
        if (limit_price != 0) {
            obj["limit_price"] = limit_price;
        }
        if (aux_price != 0) {
            obj["aux_price"] = aux_price;
        }
        if (trail_stop_price != 0) {
            obj["trail_stop_price"] = trail_stop_price;
        }
        if (trailing_percent != 0) {
            obj["trailing_percent"] = trailing_percent;
        }
        if (percent_offset != 0) {
            obj["percent_offset"] = percent_offset;
        }
        if (!time_in_force.empty()) {
            obj["time_in_force"] = value::string(time_in_force);
        }
        if (outside_rth) {
            obj["outside_rth"] = outside_rth;
        }
        if (expire_time) {
            obj["expire_time"] = expire_time;
        }
        value res = post(MODIFY_ORDER, obj);
        return res;
    }


}