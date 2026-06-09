//
// Created by sukai on 2022/12/23.
//

#include "tigerapi/trade_client.h"

namespace TIGER_API {
    TradeClient::TradeClient() {}

    TradeClient::TradeClient(const ClientConfig &cf) : TigerClient(cf) {
        this->client_config.check_account();
    }

    value TradeClient::get_accounts() {
        value obj = value::object(true);
        set_secret_key(obj);
        return post(ACCOUNTS, obj);
    }

    value TradeClient::get_prime_asset(const utility::string_t &account, const utility::string_t &base_currency) {
        value obj = value::object(true);
        obj[P_ACCOUNT] = get_account_param(account);
        set_secret_key(obj);
        obj[U("base_currency")] = value::string(base_currency);
        return post(PRIME_ASSETS, obj);
    }

    value TradeClient::get_prime_asset(const utility::string_t &account, Currency base_currency) {
        return get_prime_asset(account, enum_to_str(base_currency));
    }

    PortfolioAccount TradeClient::get_prime_portfolio(const utility::string_t &account, const utility::string_t &base_currency) {
        value asset = get_prime_asset(account, base_currency);
        PortfolioAccount portfolio = PortfolioAccount();
        portfolio.account = asset.at(U("accountId")).as_string();
        portfolio.update_timestamp = (long) asset.at(U("updateTimestamp")).as_number().to_uint64();
        vector<Segment> segments;
        for (const auto &element: asset.at(U("segments")).as_array()) {
            Segment s = Segment(element);
            vector<CurrencyAsset> currency_assets;
            for (const auto &c: element.at(U("currencyAssets")).as_array()) {
                currency_assets.push_back(CurrencyAsset(c));
            }
            s.currency_assets = currency_assets;
            segments.push_back(s);
        }
        portfolio.segments = segments;
        return portfolio;
    }

    value TradeClient::get_asset(utility::string_t account, const value &sub_accounts, bool segment, bool market_value) {
        value obj = value::object(true);
        obj[P_ACCOUNT] = get_account_param(account);
        set_secret_key(obj);
        obj[U("segment")] = segment;
        obj[U("market_value")] = market_value;
        return post(ASSETS, obj);
    }

    value TradeClient::get_account_param(const utility::string_t &account) {
        if (account.empty()) {
            return value::string(client_config.account);
        }
        return value::string(account);
    }

    void TradeClient::set_secret_key(value &obj) {
        if (!client_config.secret_key.empty()) {
            obj[U("secret_key")] = value::string(client_config.secret_key);
        }
    }

    value TradeClient::get_positions(const utility::string_t &account, const utility::string_t &sec_type, const utility::string_t &currency,
                                     const utility::string_t &market, const utility::string_t &symbol,
                                     const value &sub_accounts, time_t expiry, utility::string_t strike, const utility::string_t &right) {
        value obj = value::object(true);
        obj[P_ACCOUNT] = get_account_param(account);
        set_secret_key(obj);
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
            obj[U("sub_accounts")] = sub_accounts;
        }
        if (expiry != -1 && expiry != 0) {
            obj[P_EXPIRY] = value::number(static_cast<int64_t>(expiry));
        }
        if (!strike.empty()) {
            obj[P_STRIKE] = value::string(strike);
        }
        if (!right.empty()) {
            obj[P_RIGHT] = value::string(right);
        }
        return post(POSITIONS, obj)[P_ITEMS];
    }

    value TradeClient::get_positions(utility::string_t account, SecType sec_type, Currency currency, Market market, utility::string_t symbol,
                                     const value &sub_accounts, time_t expiry, utility::string_t strike, Right right) {
        return get_positions(account, enum_to_str(sec_type), enum_to_str(currency), enum_to_str(market),
                             symbol, sub_accounts, expiry, strike, enum_to_str(right));
    }


    vector<Position>
    TradeClient::get_position_list(utility::string_t account, utility::string_t sec_type, utility::string_t currency, utility::string_t market, utility::string_t symbol,
                                   const value &sub_accounts, time_t expiry, utility::string_t strike, utility::string_t right) {
        std::vector<Position> vec;
        value positions = get_positions(account, sec_type, currency, market, symbol,
                                        sub_accounts, expiry, strike, right);
        for (const auto &element: positions.as_array()) {
            Position p = Position(element);
            vec.push_back(p);
        }
        return vec;
    }

    value TradeClient::get_orders(const utility::string_t &account, const utility::string_t &sec_type, const utility::string_t &market, const utility::string_t &symbol, time_t start_date,
                                  time_t end_date, int limit, bool is_brief, const value &states, const utility::string_t &sort_by,
                                  const utility::string_t &seg_type) {
        value obj = value::object(true);
        obj[P_ACCOUNT] = get_account_param(account);
        set_secret_key(obj);
        if (!sec_type.empty()) {
            obj[P_SEC_TYPE] = value::string(sec_type);
        }
        if (!market.empty()) {
            obj[P_MARKET] = value::string(market);
        }
        if (!symbol.empty()) {
            obj[P_SYMBOL] = value::string(symbol);
        }
        if (start_date != -1 && start_date != 0) {
            obj[P_START_DATE] = value::number(static_cast<int64_t>(start_date));
        }
        if (end_date != -1 && end_date != 0) {
            obj[P_END_DATE] = value::number(static_cast<int64_t>(end_date));
        }
        if (limit != 0) {
            obj[P_LIMIT] = limit;
        }
        if (!states.is_null() && states.size() > 0) {
            obj[U("states")] = states;
        }
        if (!sort_by.empty()) {
            obj[U("sort_by")] = value::string(sort_by);
        }
        if (!seg_type.empty()) {
            obj[P_SEG_TYPE] = value::string(seg_type);
        }
        if (is_brief) {
            obj[U("is_brief")] = is_brief;
        }
        return post(ORDERS, obj)[P_ITEMS];
    }

    value TradeClient::get_orders(utility::string_t account, SecType sec_type, Market market, utility::string_t symbol, time_t start_date,
                                  time_t end_date, int limit, bool is_brief, const value &states, OrderSortBy sort_by,
                                  SegmentType seg_type) {
        return get_orders(account, enum_to_str(sec_type), enum_to_str(market), symbol, start_date, end_date, limit,
                          is_brief, states, enum_to_str(sort_by), enum_to_str(seg_type));
    }

    value TradeClient::get_active_orders(utility::string_t account, utility::string_t sec_type, utility::string_t market, utility::string_t symbol, time_t start_date,
                                         time_t end_date, unsigned long long parent_id, utility::string_t sort_by, utility::string_t seg_type) {
        value obj = value::object(true);
        obj[P_ACCOUNT] = get_account_param(account);
        set_secret_key(obj);
        if (!sec_type.empty()) {
            obj[P_SEC_TYPE] = value::string(sec_type);
        }
        if (!market.empty()) {
            obj[P_MARKET] = value::string(market);
        }
        if (!symbol.empty()) {
            obj[P_SYMBOL] = value::string(symbol);
        }
        if (start_date != -1 && start_date != 0) {
            obj[P_START_DATE] = value::number(static_cast<int64_t>(start_date));
        }
        if (end_date != -1 && end_date != 0) {
            obj[P_END_DATE] = value::number(static_cast<int64_t>(end_date));
        }
        if (!sort_by.empty()) {
            obj[U("sort_by")] = value::string(sort_by);
        }
        if (!seg_type.empty()) {
            obj[P_SEG_TYPE] = value::string(seg_type);
        }
        return post(ACTIVE_ORDERS, obj)[P_ITEMS];
    }

    value
    TradeClient::get_active_orders(utility::string_t account, SecType sec_type, Market market, utility::string_t symbol, time_t start_date,
                                   time_t end_date, unsigned long long parent_id, OrderSortBy sort_by, SegmentType seg_type) {
        return get_active_orders(account, enum_to_str(sec_type), enum_to_str(market), symbol, start_date, end_date,
                                 parent_id, enum_to_str(sort_by), enum_to_str(seg_type));
    }


    value
    TradeClient::get_filled_orders(utility::string_t account, utility::string_t sec_type, utility::string_t market,
                                   utility::string_t symbol, time_t start_date, time_t end_date, unsigned long long parent_id,
                                   utility::string_t sort_by, utility::string_t seg_type) {
        value obj = value::object(true);
        obj[P_ACCOUNT] = get_account_param(account);
        set_secret_key(obj);
        if (!sec_type.empty()) {
            obj[P_SEC_TYPE] = value::string(sec_type);
        }
        if (!market.empty()) {
            obj[P_MARKET] = value::string(market);
        }
        if (!symbol.empty()) {
            obj[P_SYMBOL] = value::string(symbol);
        }
        if (start_date != -1 && start_date != 0) {
            obj[P_START_DATE] = value::number(static_cast<int64_t>(start_date));
        }
        if (end_date != -1 && end_date != 0) {
            obj[P_END_DATE] = value::number(static_cast<int64_t>(end_date));
        }
        if (!sort_by.empty()) {
            obj[U("sort_by")] = value::string(sort_by);
        }
        if (!seg_type.empty()) {
            obj[P_SEG_TYPE] = value::string(seg_type);
        }
        return post(FILLED_ORDERS, obj)[P_ITEMS];
    }

    value
    TradeClient::get_filled_orders(utility::string_t account, SecType sec_type, Market market, utility::string_t symbol, time_t start_date,
                                   time_t end_date, unsigned long long parent_id, OrderSortBy sort_by, SegmentType seg_type) {
        return get_filled_orders(account, enum_to_str(sec_type), enum_to_str(market), symbol, start_date, end_date,
                                 parent_id, enum_to_str(sort_by), enum_to_str(seg_type));
    }


    value
    TradeClient::get_inactive_orders(utility::string_t account, utility::string_t sec_type, utility::string_t market,
                                     utility::string_t symbol, time_t start_date, time_t end_date, unsigned long long parent_id,
                                     utility::string_t sort_by, utility::string_t seg_type) {
        value obj = value::object(true);
        obj[P_ACCOUNT] = get_account_param(account);
        set_secret_key(obj);
        if (!sec_type.empty()) {
            obj[P_SEC_TYPE] = value::string(sec_type);
        }
        if (!market.empty()) {
            obj[P_MARKET] = value::string(market);
        }
        if (!symbol.empty()) {
            obj[P_SYMBOL] = value::string(symbol);
        }
        if (start_date != -1 && start_date != 0) {
            obj[P_START_DATE] = value::number(static_cast<int64_t>(start_date));
        }
        if (end_date != -1 && end_date != 0) {
            obj[P_END_DATE] = value::number(static_cast<int64_t>(end_date));
        }
        if (parent_id != 0) {
            obj[U("parent_id")] = value::number(static_cast<uint64_t>(parent_id));
        }
        if (!sort_by.empty()) {
            obj[U("sort_by")] = value::string(sort_by);
        }
        if (!seg_type.empty()) {
            obj[P_SEG_TYPE] = value::string(seg_type);
        }
        return post(INACTIVE_ORDERS, obj)[P_ITEMS];
    }

    value
    TradeClient::get_inactive_orders(utility::string_t account, SecType sec_type, Market market, utility::string_t symbol, time_t start_date,
                                     time_t end_date, unsigned long long parent_id, OrderSortBy sort_by, SegmentType seg_type) {
        return get_inactive_orders(account, enum_to_str(sec_type), enum_to_str(market), symbol, start_date, end_date,
                                   parent_id, enum_to_str(sort_by), enum_to_str(seg_type));
    }

    value TradeClient::get_transactions(utility::string_t account, long long order_id) {
        value obj = value::object(true);
        obj[P_ACCOUNT] = get_account_param(account);
        set_secret_key(obj);
        obj[U("order_id")] = value::number(static_cast<int64_t>(order_id));
        return post(ORDER_TRANSACTIONS, obj)[P_ITEMS];
    }

    value TradeClient::get_transactions(utility::string_t account, utility::string_t symbol, utility::string_t sec_type,
                                        long start_time, time_t end_time,
                                        int limit, utility::string_t expiry, utility::string_t strike, utility::string_t right,
                                        long long order_id) {
        value obj = value::object(true);
        obj[P_ACCOUNT] = get_account_param(account);
        set_secret_key(obj);
        if (order_id != 0) {
            obj[U("order_id")] = value::number(static_cast<int64_t>(order_id));
        }
        if (!sec_type.empty()) {
            obj[U("sec_type")] = value::string(sec_type);
        }
        if (!symbol.empty()) {
            obj[U("symbol")] = value::string(symbol);
        }
        if (!expiry.empty()) {
            obj[U("expiry")] = value::string(expiry);
        }
        if (!strike.empty()) {
            obj[U("strike")] = value::string(strike);
        }
        if (!right.empty()) {
            obj[U("right")] = value::string(right);
        }
        if (start_time != 0) {
            obj[U("start_time")] = value::number(static_cast<int64_t>(start_time));
        }
        if (end_time != 0) {
            obj[U("end_time")] = value::number(static_cast<int64_t>(end_time));
        }
        if (limit != 0) {
            obj[U("limit")] = value::number(static_cast<int64_t>(limit));
        }
        return post(ORDER_TRANSACTIONS, obj)[P_ITEMS];
    }

    value TradeClient::get_contract(utility::string_t symbol, utility::string_t sec_type, utility::string_t currency, utility::string_t exchange, time_t expiry,
                                    utility::string_t strike, utility::string_t right) {
        value obj = value::object(true);
        obj[P_ACCOUNT] = get_account_param();
        set_secret_key(obj);
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
            obj[P_EXPIRY] = value::number(static_cast<int64_t>(expiry));
        }
        if (!strike.empty()) {
            obj[P_STRIKE] = value::string(strike);
        }
        if (!right.empty()) {
            obj[P_RIGHT] = value::string(right);
        }
        return post(CONTRACT, obj);
    }

    value TradeClient::get_contract(utility::string_t symbol, SecType sec_type, Currency currency, utility::string_t exchange, time_t expiry,
                                    utility::string_t strike, Right right) {
        return get_contract(symbol, enum_to_str(sec_type), enum_to_str(currency), exchange, expiry, strike,
                            enum_to_str(right));
    }

    value TradeClient::get_contracts(const value &symbols, utility::string_t sec_type, utility::string_t currency,
                                     utility::string_t exchange, time_t expiry, utility::string_t strike,
                                     utility::string_t right) {
        value obj = value::object(true);
        obj[P_ACCOUNT] = get_account_param();
        set_secret_key(obj);
        obj[P_SYMBOLS] = symbols;
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
            obj[P_EXPIRY] = value::number(static_cast<int64_t>(expiry));
        }
        if (!strike.empty()) {
            obj[P_STRIKE] = value::string(strike);
        }
        if (!right.empty()) {
            obj[P_RIGHT] = value::string(right);
        }
        return post(CONTRACTS, obj)[P_ITEMS];
    }

    value
    TradeClient::get_quote_contract(utility::string_t symbol, utility::string_t sec_type, utility::string_t expiry) {
        value obj = value::object(true);
        obj[P_ACCOUNT] = get_account_param();
        set_secret_key(obj);
        if (!symbol.empty()) {
            obj[P_SYMBOL] = value::string(symbol);
        }
        if (!sec_type.empty()) {
            obj[P_SEC_TYPE] = value::string(sec_type);
        }
        if (!expiry.empty()) {
            obj[P_EXPIRY] = value::string(expiry);
        }
        return post(QUOTE_CONTRACT, obj)[P_ITEMS];
    }

    value TradeClient::place_order(value &order) {
        value obj = value::object(true);
        for (const auto &kvp: order.as_object()) {
            obj[kvp.first] = kvp.second;
        }
        obj[P_ACCOUNT] = get_account_param(order[P_ACCOUNT].as_string());
        set_secret_key(obj);
        value res = post(PLACE_ORDER, obj);
        return res;
    }

    value TradeClient::place_order(Order &order) {
        value obj = value::object(true);
        Contract contract = order.contract;
        if (!contract.symbol.empty()) {
            obj[U("symbol")] = value::string(contract.symbol);
        }
        if (!contract.currency.empty()) {
            obj[U("currency")] = value::string(contract.currency);
        }
        if (!contract.sec_type.empty()) {
            obj[U("sec_type")] = value::string(contract.sec_type);
        }
        if (!contract.exchange.empty()) {
            obj[U("exchange")] = value::string(contract.exchange);
        }
        if (!contract.expiry.empty()) {
            obj[U("expiry")] = value::string(contract.expiry);
        }
        if (!contract.strike.empty()) {
            obj[U("strike")] = value::string(contract.strike);
        }
        if (!contract.right.empty()) {
            obj[U("right")] = value::string(contract.right);
        }
        if (contract.multiplier != 0) {
            obj[U("multiplier")] = contract.multiplier;
        }

        obj[P_ACCOUNT] = get_account_param(order.account);
        set_secret_key(obj);

        if (!order.secret_key.empty()) {
            obj[U("secret_key")] = value::string(order.secret_key);
        }

        if (order.order_id != 0) {
            obj[U("order_id")] = value::number(static_cast<int64_t>(order.order_id));
        }
        if (order.id != 0) {
            obj[U("id")] = value::number(static_cast<uint64_t>(order.id));
        }
        if (!order.order_type.empty()) {
            obj[U("order_type")] = value::string(order.order_type);
        }
        if (!order.action.empty()) {
            obj[U("action")] = value::string(order.action);
        }
        if (order.total_quantity) {
            obj[U("total_quantity")] = value::number(static_cast<int64_t>(order.total_quantity));
        }
        if (order.limit_price != 0) {
           obj[U("limit_price")] = order.limit_price;
        }
        if (!order.s_limit_price.empty()) {
            obj[U("limit_price")] = value::string(order.s_limit_price);
        }
        if (order.aux_price != 0) {
            obj[U("aux_price")] = order.aux_price;
        }
        if (order.trail_stop_price != 0) {
            obj[U("trail_stop_price")] = order.trail_stop_price;
        }
        if (order.trailing_percent != 0) {
            obj[U("trailing_percent")] = order.trailing_percent;
        }
        if (order.percent_offset != 0) {
            obj[U("percent_offset")] = order.percent_offset;
        }
        if (!order.time_in_force.empty()) {
            obj[U("time_in_force")] = value::string(order.time_in_force);
        }
        if (order.outside_rth) {
            obj[U("outside_rth")] = order.outside_rth;
        }
        if (order.adjust_limit) {
            obj[U("adjust_limit")] = value::number(order.adjust_limit);
        }
        if (!order.user_mark.empty()) {
            obj[U("user_mark")] = value::string(order.user_mark);
        }
        if (order.expire_time) {
            obj[U("expire_time")] = value::number(static_cast<int64_t>(order.expire_time));
        }
        value res = post(PLACE_ORDER, obj);
        try {
            order.id = res[U("id")].as_number().to_uint64();
            order.sub_ids = res[U("subIds")];
        } catch (...) {
            ucout << U("Warn: id not returned") << endl;
        }
        return res;
    }

    Order TradeClient::get_order(unsigned long long id, bool is_brief) {
        value obj = value::object(true);
        obj[P_ACCOUNT] = get_account_param();
        set_secret_key(obj);
        obj[U("id")] = value::number(static_cast<uint64_t>(id));
        obj[U("is_brief")] = is_brief;
        value res = post(ORDERS, obj);
        if (!res.is_null()) {
            Order order = Order(res);
            return order;
        } else {
            ucout << U("Exception: order ") << id << U(" not exist, result: ") << res << endl;
            throw std::runtime_error(Utils::str16to8(res.serialize()).c_str());
        }
    }

    value TradeClient::cancel_order(unsigned long long id) {
        value obj = value::object(true);
        obj[P_ACCOUNT] = get_account_param();
        set_secret_key(obj);
        obj[U("id")] = value::number(static_cast<uint64_t>(id));
        value res = post(CANCEL_ORDER, obj);
        return res;
    }

    value TradeClient::modify_order(Order &order) {
        value obj = value::object(true);

        obj[P_ACCOUNT] = get_account_param(order.account);
        set_secret_key(obj);
        if (order.id != 0) {
            obj[U("id")] = value::number(static_cast<uint64_t>(order.id));
        }
        if (!order.order_type.empty()) {
            obj[U("order_type")] = value::string(order.order_type);
        }
        if (!order.action.empty()) {
            obj[U("action")] = value::string(order.action);
        }
        if (order.total_quantity) {
            obj[U("total_quantity")] = value::number(static_cast<int64_t>(order.total_quantity));
        }
        if (order.limit_price != 0) {
            obj[U("limit_price")] = order.limit_price;
        }
        if (order.aux_price != 0) {
            obj[U("aux_price")] = order.aux_price;
        }
        if (order.trail_stop_price != 0) {
            obj[U("trail_stop_price")] = order.trail_stop_price;
        }
        if (order.trailing_percent != 0) {
            obj[U("trailing_percent")] = order.trailing_percent;
        }
        if (order.percent_offset != 0) {
            obj[U("percent_offset")] = order.percent_offset;
        }
        if (!order.time_in_force.empty()) {
            obj[U("time_in_force")] = value::string(order.time_in_force);
        }
        if (order.outside_rth) {
            obj[U("outside_rth")] = order.outside_rth;
        }
        if (order.adjust_limit) {
            obj[U("adjust_limit")] = order.adjust_limit;
        }
        if (!order.user_mark.empty()) {
            obj[U("user_mark")] = value::string(order.user_mark);
        }
        if (order.expire_time) {
            obj[U("expire_time")] = value::number(static_cast<int64_t>(order.expire_time));
        }
        value res = post(MODIFY_ORDER, obj);
        return res;
    }

    value TradeClient::modify_order(Order &order, double limit_price, long total_quantity, double aux_price,
                                    double trail_stop_price, double trailing_percent, double percent_offset,
                                    utility::string_t time_in_force, bool outside_rth, time_t expire_time) {
        value obj = value::object(true);
        obj[P_ACCOUNT] = get_account_param(order.account);
        set_secret_key(obj);
        if (order.id != 0) {
            obj[U("id")] = value::number(static_cast<uint64_t>(order.id));
        }
        if (!order.order_type.empty()) {
            obj[U("order_type")] = value::string(order.order_type);
        }
        if (!order.action.empty()) {
            obj[U("action")] = value::string(order.action);
        }
        if (total_quantity != 0) {
            obj[U("total_quantity")] = value::number(static_cast<int64_t>(total_quantity));
        } else {
            obj[U("total_quantity")] = value::number(static_cast<int64_t>(order.total_quantity));
        }
        if (limit_price != 0) {
            obj[U("limit_price")] = limit_price;
        }
        if (aux_price != 0) {
            obj[U("aux_price")] = aux_price;
        }
        if (trail_stop_price != 0) {
            obj[U("trail_stop_price")] = trail_stop_price;
        }
        if (trailing_percent != 0) {
            obj[U("trailing_percent")] = trailing_percent;
        }
        if (percent_offset != 0) {
            obj[U("percent_offset")] = percent_offset;
        }
        if (!time_in_force.empty()) {
            obj[U("time_in_force")] = value::string(time_in_force);
        }
        if (outside_rth) {
            obj[U("outside_rth")] = outside_rth;
        }
        if (expire_time) {
            obj[U("expire_time")] = value::number(static_cast<int64_t>(expire_time));
        }
        value res = post(MODIFY_ORDER, obj);
        return res;
    }

    value TradeClient::get_estimate_tradable_quantity(Order &order, utility::string_t seg_type) {
        value obj = value::object(true);
        obj[P_ACCOUNT] = get_account_param(order.account);
        set_secret_key(obj);
        obj[U("seg_type")] = value::string(seg_type);
        Contract contract = order.contract;
        if (!contract.symbol.empty()) {
            obj[U("symbol")] = value::string(contract.symbol);
        }
        if (!contract.sec_type.empty()) {
            obj[U("sec_type")] = value::string(contract.sec_type);
        }
        if (!contract.expiry.empty()) {
            obj[U("expiry")] = value::string(contract.expiry);
        }
        if (!contract.strike.empty()) {
            obj[U("strike")] = value::string(contract.strike);
        }
        if (!contract.right.empty()) {
            obj[U("right")] = value::string(contract.right);
        }
        if (!order.order_type.empty()) {
            obj[U("order_type")] = value::string(order.order_type);
        }
        if (!order.action.empty()) {
            obj[U("action")] = value::string(order.action);
        }
        if (order.total_quantity) {
            obj[U("total_quantity")] = value::number(static_cast<int64_t>(order.total_quantity));
        }
        if (order.limit_price != 0) {
            obj[U("limit_price")] = order.limit_price;
        }
        if (order.aux_price != 0) {
            obj[U("stop_price")] = order.aux_price;
        }
        return post(ESTIMATE_TRADABLE_QUANTITY, obj);
    }

    value TradeClient::get_analytics_asset(utility::string_t account, utility::string_t start_date,
                                           utility::string_t end_date, utility::string_t seg_type,
                                           utility::string_t currency, utility::string_t sub_account) {
        value obj = value::object(true);
        obj[P_ACCOUNT] = get_account_param(account);
        set_secret_key(obj);
        if (!start_date.empty()) {
            obj[U("start_date")] = value::string(start_date);
        }
        if (!end_date.empty()) {
            obj[U("end_date")] = value::string(end_date);
        }
        if (!seg_type.empty()) {
            obj[U("seg_type")] = value::string(seg_type);
        }
        if (!currency.empty()) {
            obj[U("currency")] = value::string(currency);
        }
        if (!sub_account.empty()) {
            obj[U("sub_account")] = value::string(sub_account);
        }
        return post(ANALYTICS_ASSET, obj);
    }

    value TradeClient::get_segment_fund_history(int limit) {
        value obj = value::object(true);
        obj[P_ACCOUNT] = get_account_param();
        set_secret_key(obj);
        if (limit > 0) {
            obj[U("limit")] = limit;
        }
        return post(SEGMENT_FUND_HISTORY, obj);
    }

    value TradeClient::get_segment_fund_available(utility::string_t from_segment, utility::string_t currency) {
        value obj = value::object(true);
        obj[P_ACCOUNT] = get_account_param();
        set_secret_key(obj);
        if (!from_segment.empty()) {
            obj[U("from_segment")] = value::string(from_segment);
        }
        if (!currency.empty()) {
            obj[U("currency")] = value::string(currency);
        }
        return post(SEGMENT_FUND_AVAILABLE, obj);
    }

    value
    TradeClient::transfer_segment_fund(utility::string_t from_segment, utility::string_t to_segment, double amount,
                                       utility::string_t currency) {
        value obj = value::object(true);
        obj[P_ACCOUNT] = get_account_param();
        set_secret_key(obj);
        if (!from_segment.empty()) {
            obj[U("from_segment")] = value::string(from_segment);
        }
        if (!to_segment.empty()) {
            obj[U("to_segment")] = value::string(to_segment);
        }
        if (!currency.empty()) {
            obj[U("currency")] = value::string(currency);
        }
        if (amount != 0) {
            obj[U("amount")] = amount;
        }
        return post(TRANSFER_SEGMENT_FUND, obj);
    }

    value TradeClient::place_forex_order(utility::string_t seg_type, utility::string_t source_currency,
                                         utility::string_t target_currency, double source_amount) {
        value obj = value::object(true);
        obj[P_ACCOUNT] = get_account_param();
        set_secret_key(obj);
        if (!seg_type.empty()) {
            obj[U("seg_type")] = value::string(seg_type);
        }
        if (!source_currency.empty()) {
            obj[U("source_currency")] = value::string(source_currency);
        }
        if (!target_currency.empty()) {
            obj[U("target_currency")] = value::string(target_currency);
        }
        if (source_amount != 0) {
            obj[U("source_amount")] = source_amount;
        }
        return post(PLACE_FOREX_ORDER, obj);
    }

    value TradeClient::preview_order(Order &order) {
        value obj = value::object(true);
        Contract contract = order.contract;
        if (!contract.symbol.empty()) {
            obj[U("symbol")] = value::string(contract.symbol);
        }
        if (!contract.currency.empty()) {
            obj[U("currency")] = value::string(contract.currency);
        }
        if (!contract.sec_type.empty()) {
            obj[U("sec_type")] = value::string(contract.sec_type);
        }
        if (!contract.exchange.empty()) {
            obj[U("exchange")] = value::string(contract.exchange);
        }
        if (!contract.expiry.empty()) {
            obj[U("expiry")] = value::string(contract.expiry);
        }
        if (!contract.strike.empty()) {
            obj[U("strike")] = value::string(contract.strike);
        }
        if (!contract.right.empty()) {
            obj[U("right")] = value::string(contract.right);
        }
        if (contract.multiplier != 0) {
            obj[U("multiplier")] = contract.multiplier;
        }

        obj[P_ACCOUNT] = get_account_param(order.account);
        set_secret_key(obj);

        if (!order.order_type.empty()) {
            obj[U("order_type")] = value::string(order.order_type);
        }
        if (!order.action.empty()) {
            obj[U("action")] = value::string(order.action);
        }
        if (order.total_quantity) {
            obj[U("total_quantity")] = value::number(static_cast<int64_t>(order.total_quantity));
        }
        if (order.limit_price != 0) {
            obj[U("limit_price")] = order.limit_price;
        }
        if (!order.s_limit_price.empty()) {
            obj[U("limit_price")] = value::string(order.s_limit_price);
        }
        if (order.aux_price != 0) {
            obj[U("aux_price")] = order.aux_price;
        }
        if (order.trail_stop_price != 0) {
            obj[U("trail_stop_price")] = order.trail_stop_price;
        }
        if (order.trailing_percent != 0) {
            obj[U("trailing_percent")] = order.trailing_percent;
        }
        if (order.percent_offset != 0) {
            obj[U("percent_offset")] = order.percent_offset;
        }
        if (!order.time_in_force.empty()) {
            obj[U("time_in_force")] = value::string(order.time_in_force);
        }
        if (order.outside_rth) {
            obj[U("outside_rth")] = order.outside_rth;
        }
        if (order.expire_time) {
            obj[U("expire_time")] = value::number(static_cast<int64_t>(order.expire_time));
        }
        return post(PREVIEW_ORDER, obj);
    }

    value TradeClient::get_aggregate_assets(utility::string_t account, utility::string_t seg_type, utility::string_t base_currency) {
        value obj = value::object(true);
        obj[P_ACCOUNT] = get_account_param(account);
        set_secret_key(obj);
        if (!seg_type.empty()) {
            obj[P_SEG_TYPE] = value::string(seg_type);
        }
        if (!base_currency.empty()) {
            obj[U("base_currency")] = value::string(base_currency);
        }
        return post(AGGREGATE_ASSETS, obj);
    }

    value TradeClient::cancel_segment_fund(long long id) {
        value obj = value::object(true);
        obj[P_ACCOUNT] = get_account_param();
        set_secret_key(obj);
        obj[U("id")] = value::number(static_cast<int64_t>(id));
        return post(CANCEL_SEGMENT_FUND, obj);
    }

    value TradeClient::get_funding_history(utility::string_t seg_type) {
        value obj = value::object(true);
        obj[P_ACCOUNT] = get_account_param();
        set_secret_key(obj);
        if (!seg_type.empty()) {
            obj[P_SEG_TYPE] = value::string(seg_type);
        }
        return post(TRANSFER_FUND, obj);
    }

    value TradeClient::get_fund_details(const value &seg_types, utility::string_t account,
                                        utility::string_t fund_type, utility::string_t currency,
                                        int start, int limit, utility::string_t start_date, utility::string_t end_date) {
        value obj = value::object(true);
        obj[P_ACCOUNT] = get_account_param(account);
        set_secret_key(obj);
        if (!seg_types.is_null() && seg_types.size() > 0) {
            obj[U("seg_types")] = seg_types;
        }
        if (!fund_type.empty()) {
            obj[U("fund_type")] = value::string(fund_type);
        }
        if (!currency.empty()) {
            obj[P_CURRENCY] = value::string(currency);
        }
        if (start > 0) {
            obj[U("start")] = start;
        }
        if (limit > 0) {
            obj[P_LIMIT] = limit;
        }
        if (!start_date.empty()) {
            obj[P_START_DATE] = value::string(start_date);
        }
        if (!end_date.empty()) {
            obj[P_END_DATE] = value::string(end_date);
        }
        return post(FUND_DETAILS, obj);
    }

    value TradeClient::transfer_position(utility::string_t from_account, utility::string_t to_account,
                                         const value &transfers, utility::string_t market) {
        value obj = value::object(true);
        obj[P_ACCOUNT] = get_account_param();
        set_secret_key(obj);
        if (!from_account.empty()) {
            obj[U("from_account")] = value::string(from_account);
        }
        if (!to_account.empty()) {
            obj[U("to_account")] = value::string(to_account);
        }
        if (!transfers.is_null() && transfers.size() > 0) {
            obj[U("transfers")] = transfers;
        }
        if (!market.empty()) {
            obj[P_MARKET] = value::string(market);
        }
        return post(POSITION_TRANSFER, obj);
    }

    value TradeClient::get_position_transfer_records(utility::string_t since_date, utility::string_t to_date,
                                                     utility::string_t status, utility::string_t market,
                                                     utility::string_t symbol, utility::string_t account_id) {
        value obj = value::object(true);
        obj[U("account_id")] = get_account_param(account_id);
        set_secret_key(obj);
        if (!since_date.empty()) {
            obj[U("since_date")] = value::string(since_date);
        }
        if (!to_date.empty()) {
            obj[U("to_date")] = value::string(to_date);
        }
        if (!status.empty()) {
            obj[U("status")] = value::string(status);
        }
        if (!market.empty()) {
            obj[P_MARKET] = value::string(market);
        }
        if (!symbol.empty()) {
            obj[P_SYMBOL] = value::string(symbol);
        }
        return post(POSITION_TRANSFER_RECORDS, obj);
    }

    value TradeClient::get_position_transfer_detail(utility::string_t transfer_id, utility::string_t account_id) {
        value obj = value::object(true);
        obj[U("account_id")] = get_account_param(account_id);
        set_secret_key(obj);
        if (!transfer_id.empty()) {
            obj[U("transfer_id")] = value::string(transfer_id);
        }
        return post(POSITION_TRANSFER_DETAIL, obj);
    }

    value TradeClient::get_position_transfer_external_records(utility::string_t since_date, utility::string_t to_date,
                                                              utility::string_t status, utility::string_t market,
                                                              utility::string_t symbol, utility::string_t account_id) {
        value obj = value::object(true);
        obj[U("account_id")] = get_account_param(account_id);
        set_secret_key(obj);
        if (!since_date.empty()) {
            obj[U("since_date")] = value::string(since_date);
        }
        if (!to_date.empty()) {
            obj[U("to_date")] = value::string(to_date);
        }
        if (!status.empty()) {
            obj[U("status")] = value::string(status);
        }
        if (!market.empty()) {
            obj[P_MARKET] = value::string(market);
        }
        if (!symbol.empty()) {
            obj[P_SYMBOL] = value::string(symbol);
        }
        return post(POSITION_TRANSFER_EXTERNAL_RECORDS, obj);
    }

    value TradeClient::submit_option_exercise(long long contract_id, utility::string_t type, double quantity,
                                              utility::string_t executing_date, int is_force,
                                              int itm_rate, utility::string_t account) {
        value obj = value::object(true);
        obj[P_ACCOUNT] = get_account_param(account);
        set_secret_key(obj);
        obj[U("contract_id")] = value::number(contract_id);
        obj[U("type")] = value::string(type);
        obj[U("quantity")] = value::number(quantity);
        if (!executing_date.empty()) {
            obj[U("executing_date")] = value::string(executing_date);
        }
        if (is_force >= 0) {
            obj[U("is_force")] = value::boolean(is_force != 0);
        }
        if (itm_rate >= 0) {
            obj[U("itm_rate")] = value::number(itm_rate);
        }
        return post(OPTION_EXERCISE_SUBMIT, obj);
    }

    value TradeClient::check_option_exercise(long long contract_id, utility::string_t type, double quantity,
                                             utility::string_t executing_date, int is_force,
                                             int itm_rate, utility::string_t account) {
        value obj = value::object(true);
        obj[P_ACCOUNT] = get_account_param(account);
        set_secret_key(obj);
        obj[U("contract_id")] = value::number(contract_id);
        obj[U("type")] = value::string(type);
        obj[U("quantity")] = value::number(quantity);
        if (!executing_date.empty()) {
            obj[U("executing_date")] = value::string(executing_date);
        }
        if (is_force >= 0) {
            obj[U("is_force")] = value::boolean(is_force != 0);
        }
        if (itm_rate >= 0) {
            obj[U("itm_rate")] = value::number(itm_rate);
        }
        return post(OPTION_EXERCISE_CHECK, obj);
    }

    value TradeClient::get_option_exercise_records(utility::string_t exercise_type, utility::string_t status,
                                                   utility::string_t symbol, utility::string_t order_by,
                                                   int page, int size, utility::string_t account) {
        value obj = value::object(true);
        obj[P_ACCOUNT] = get_account_param(account);
        set_secret_key(obj);
        if (!exercise_type.empty()) {
            obj[U("type")] = value::string(exercise_type);
        }
        if (!status.empty()) {
            obj[U("status")] = value::string(status);
        }
        if (!symbol.empty()) {
            obj[P_SYMBOL] = value::string(symbol);
        }
        if (!order_by.empty()) {
            obj[U("order_by")] = value::string(order_by);
        }
        if (page > 0) {
            obj[U("page")] = value::number(page);
        }
        if (size > 0) {
            obj[U("size")] = value::number(size);
        }
        return post(OPTION_EXERCISE_RECORD, obj);
    }

    value TradeClient::get_option_exercise_positions(utility::string_t type, utility::string_t account) {
        value obj = value::object(true);
        obj[P_ACCOUNT] = get_account_param(account);
        set_secret_key(obj);
        obj[U("type")] = value::string(type);
        return post(OPTION_EXERCISE_POSITION, obj);
    }

    value TradeClient::cancel_option_exercise(long long exercise_id, utility::string_t account) {
        value obj = value::object(true);
        obj[P_ACCOUNT] = get_account_param(account);
        set_secret_key(obj);
        obj[U("id")] = value::number(exercise_id);
        return post(OPTION_EXERCISE_CANCEL, obj);
    }

}
