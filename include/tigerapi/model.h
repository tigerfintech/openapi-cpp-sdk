//
// Created by sukai on 2022/12/27.
//

#ifndef TIGERAPI_MODEL_H
#define TIGERAPI_MODEL_H

#include <typeinfo>
#include <any>
#include <string>
#include <cpprest/json.h>

using namespace std;

class Contract {
public:
    Contract() {};

    Contract(const string sec_type, const string symbol, const string currency, const string local_symbol,
             const string exchange, long contract_id) : sec_type(sec_type), symbol(symbol), currency(currency),
                                                        local_symbol(local_symbol), exchange(exchange),
                                                        contract_id(contract_id) {};

    Contract(const string sec_type, const string symbol, const string expiry, double strike, const string right,
             const string currency, long multiplier, const string local_symbol, long contract_id) :
            sec_type(sec_type), symbol(symbol), expiry(expiry), strike(strike), right(right),
            currency(currency), multiplier(multiplier), local_symbol(local_symbol), contract_id(contract_id) {};

    Contract(const string sec_type, const string symbol, const string expiry, long multiplier,
             const string contract_month,
             const string currency, const string exchange, const string local_symbol) :
            sec_type(sec_type), symbol(symbol), expiry(expiry), currency(currency), multiplier(multiplier),
            exchange(exchange), contract_month(contract_month), local_symbol(local_symbol) {};

    string symbol;
    string currency;
    string sec_type;
    string exchange;
    string local_symbol;
    string expiry;
    double strike = 0;
    string right;
    int multiplier = 0;
    string contract_month;
    long contract_id;
    string identifier;
    string market;
};

class Order {
public:
    Order() {};

    Order(const string order_type, const string account, Contract &contract, const string action, long total_quantity) :
            order_type(order_type), account(account), contract(contract), action(action),
            total_quantity(total_quantity) {};

    Order(const string order_type, const string account, Contract &contract, const string action, long total_quantity,
          double limit_price, double aux_price = 0, double trailing_percent = 0) :
            order_type(order_type), account(account), contract(contract), action(action),
            total_quantity(total_quantity),
            limit_price(limit_price), aux_price(aux_price), trailing_percent(trailing_percent) {};

    Contract contract;
    std::string account;
    long id;
    long order_id;
    /**  订单类型, 'MKT' 市价单 / 'LMT' 限价单 / 'STP' 止损单 / 'STP_LMT' 止损限价单 / 'TRAIL' 跟踪止损单 **/
    std::string order_type;
    /** 交易方向, 'BUY' / 'SELL' **/
    std::string action;
    /** 下单数量 **/
    long total_quantity;
    /** 限价单价格 **/
    double limit_price;
    /** 在止损单中, 表示触发止损单的价格, 在移动止损单中, 表示跟踪的价差 **/
    double aux_price;
    double trail_stop_price;
    double trailing_percent;
    double percent_offset;
    /** 有效期,'DAY' 日内有效 / 'GTC' good till cancel  / 'GTD' good till date **/
    std::string time_in_force;
    /** 是否允许盘前盘后交易(outside of regular trading hours 美股专属). True 允许, False 不允许 **/
    bool outside_rth;
    /**
     * 价格微调幅度（默认为0表示不调整，正数为向上调整，负数向下调整），对传入价格自动调整到合法价位上.
          例如：0.001 代表向上调整且幅度不超过 0.1%；-0.001 代表向下调整且幅度不超过 0.1%。默认 0 表示不调整
     */
    bool adjust_limit;
    std::string user_mark;
    long expire_time;

    // 订单状态
    std::string status;
    // 主订单id, 目前只用于 TigerTrade App端的附加订单中
    long parent_id;
    // 下单时间
    long open_time;
    // 下单失败时, 会返回失败原因的描述
    std::string reason;
    // 最新成交时间
    long latest_time;
    // order updated time
    long update_time;
    // 成交数量
    long filled_quantity;
    // 包含佣金的平均成交价
    double avg_fill_price;
    // 实现盈亏
    double realized_pnl;
    std::string secret_key;
    web::json::value sub_ids;
    string algo_strategy;
    double commission;

    std::string to_string() {
        std::stringstream ss;
        ss << U("Order(id=") << this->id << U(" status=") << this->status << U(" total_quantity: ") << this->total_quantity
           << U(" limit_price: ") << this->limit_price << ")" << endl;
        return ss.str();
    };

    Order(const web::json::value &json) {
        if (json.has_field(U("id"))) {
            id = json.at(U("id")).as_number().to_uint64();
        }
        if (json.has_field(U("orderType"))) {
            order_type = json.at(U("orderType")).as_string();
        }
        if (json.has_field(U("account"))) {
            account = json.at(U("account")).as_string();
        }
        if (json.has_field(U("symbol"))) {
            string symbol = json.at(U("symbol")).as_string();
            string sec_type = json.at(U("secType")).as_string();
            string market = json.at(U("market")).as_string();
            string currency = json.at(U("currency")).as_string();
            contract = Contract();
            contract.symbol = symbol;
            contract.sec_type = sec_type;
            contract.market = market;
            contract.currency = currency;
        }
        if (json.has_field(U("action"))) {
            action = json.at(U("action")).as_string();
        }
        if (json.has_field(U("limitPrice"))) {
            limit_price = json.at(U("limitPrice")).as_double();
        }
        if (json.has_field(U("auxPrice"))) {
            aux_price = json.at(U("auxPrice")).as_double();
        }
        if (json.has_field(U("trailingPercent"))) {
            trailing_percent = json.at(U("trailingPercent")).as_double();
        }
        if (json.has_field(U("timeInForce"))) {
            time_in_force = json.at(U("timeInForce")).as_string();
        }
        if (json.has_field(U("outsideRth"))) {
            outside_rth = json.at(U("outsideRth")).as_bool();
        }
        if (json.has_field(U("adjustLimit"))) {
            adjust_limit = json.at(U("adjustLimit")).as_bool();
        }
        if (json.has_field(U("userMark"))) {
            user_mark = json.at(U("userMark")).as_string();
        }
        if (json.has_field(U("expireTime"))) {
            expire_time = json.at(U("expireTime")).as_integer();
        }
        if (json.has_field(U("status"))) {
            status = json.at(U("status")).as_string();
        }
        if (json.has_field(U("parentId"))) {
            parent_id = json.at(U("parentId")).as_integer();
        }
        if (json.has_field(U("openTime"))) {
            open_time = json.at(U("openTime")).as_integer();
        }
        if (json.has_field(U("reason"))) {
            reason = json.at(U("reason")).as_string();
        }
        if (json.has_field(U("latestTime"))) {
            latest_time = json.at(U("latestTime")).as_integer();
        }
        if (json.has_field(U("updateTime"))) {
            update_time = json.at(U("updateTime")).as_integer();
        }
        if (json.has_field(U("filledQuantity"))) {
            filled_quantity = json.at(U("filledQuantity")).as_integer();
        }
        if (json.has_field(U("totalQuantity"))) {
            total_quantity = json.at(U("totalQuantity")).as_integer();
        }
        if (json.has_field(U("avgFillPrice"))) {
            avg_fill_price = json.at(U("avgFillPrice")).as_double();
        }
        if (json.has_field(U("realizedPnl"))) {
            realized_pnl = json.at(U("realizedPnl")).as_double();
        }
        if (json.has_field(U("commission"))) {
            commission = json.at(U("commission")).as_double();
        }
        if (json.has_field(U("subIds"))) {
            sub_ids = json.at(U("subIds"));
        }
        if (json.has_field(U("algoStrategy"))) {
            algo_strategy = json.at(U("algoStrategy")).as_string();
        }
    };

};

class Position {
public:
    Position() {};

    Position(const std::string &account, const Contract &contract, int position = 0, double average_cost = 0.0,
             double latest_price = 0.0, double market_value = 0.0, double realized_pnl = 0.0,
             double unrealized_pnl = 0.0)
            : account(account), contract(contract), position(position), average_cost(average_cost),
              latest_price(latest_price), market_value(market_value), realized_pnl(realized_pnl),
              unrealized_pnl(unrealized_pnl) {};

    Position(const web::json::value &json) {
        if (json.has_field(U("account"))) {
            account = json.at(U("account")).as_string();
        }
        if (json.has_field(U("symbol"))) {
            string symbol = json.at(U("symbol")).as_string();
            string sec_type = json.at(U("secType")).as_string();
            string market = json.at(U("market")).as_string();
            string currency = json.at(U("currency")).as_string();
            string identifier = json.at(U("identifier")).as_string();
            long contract_id = json.at(U("contractId")).as_integer();
            long multiplier = (long) json.at(U("multiplier")).as_number().to_uint64();
            contract = Contract();
            contract.symbol = symbol;
            contract.sec_type = sec_type;
            contract.market = market;
            contract.currency = currency;
            contract.contract_id = contract_id;
            contract.identifier = identifier;
            contract.multiplier = multiplier;
        }
        if (json.has_field(U("position"))) {
            position = json.at(U("position")).as_number().to_uint64();
        }
        if (json.has_field(U("averageCost"))) {
            average_cost = json.at(U("averageCost")).as_double();
        }
        if (json.has_field(U("marketValue"))) {
            market_value = json.at(U("marketValue")).as_double();
        }
        if (json.has_field(U("realizedPnl"))) {
            realized_pnl = json.at(U("realizedPnl")).as_double();
        }
        if (json.has_field(U("unrealizedPnl"))) {
            unrealized_pnl = json.at(U("unrealizedPnl")).as_double();
        }
        if (json.has_field(U("latestPrice"))) {
            latest_price = json.at(U("latestPrice")).as_double();
        }
        if (json.has_field(U("updateTimestamp"))) {
            update_timestamp = (long) json.at(U("updateTimestamp")).as_number().to_uint64();
        }
        if (json.has_field(U("status"))) {
            status = json.at(U("status")).as_integer();
        }
    }

    std::string account;
    Contract contract;
    long position;
    double average_cost;
    double market_value;
    double realized_pnl;
    double unrealized_pnl;
    double latest_price;
    int status;
    long update_timestamp;


    std::string to_string() {
        std::stringstream ss;
        ss << U("Position(symbol=") << this->contract.symbol << U(" position=") << this->position << ")" << endl;
        return ss.str();
    };
};

class CurrencyAsset {
public:
    CurrencyAsset() {};

    CurrencyAsset(const web::json::value &json) {
        if (json.has_field(U("cashAvailableForTrade"))) {
            this->cash_available_for_trade = json.at(U("cashAvailableForTrade")).as_double();
        }
        if (json.has_field(U("cashBalance"))) {
            this->cash_balance = json.at(U("cashBalance")).as_double();
        }
        if (json.has_field(U("currency"))) {
            this->currency = json.at(U("currency")).as_string();
        }
        if (json.has_field(U("grossPositionValue"))) {
            this->gross_position_value = json.at(U("grossPositionValue")).as_double();
        }
        if (json.has_field(U("optionMarketValue"))) {
            this->option_market_value = json.at(U("optionMarketValue")).as_double();
        }
        if (json.has_field(U("realizedPL"))) {
            this->realized_pl = json.at(U("realizedPL")).as_double();
        }
        if (json.has_field(U("stockMarketValue"))) {
            this->stock_market_value = json.at(U("stockMarketValue")).as_double();
        }
        if (json.has_field(U("unrealizedPL"))) {
            this->unrealized_pl = json.at(U("unrealizedPL")).as_double();
        }
    }


    double cash_available_for_trade = 0;
    double cash_balance = 0;
    string currency;
    double gross_position_value = 0;
    double option_market_value = 0;
    double realized_pl = 0;
    double stock_market_value = 0;
    double unrealized_pl = 0;
};

class Segment {
public:
    Segment() {};
    Segment(const web::json::value &segment_json) {
        if (segment_json.has_field(U("buyingPower"))) {
            this->buying_power = segment_json.at(U("buyingPower")).as_double();
        }
        if (segment_json.has_field(U("capability"))) {
            this->capability = segment_json.at(U("capability")).as_string();
        }
        if (segment_json.has_field(U("cashAvailableForTrade"))) {
            this->cash_available_for_trade = segment_json.at(U("cashAvailableForTrade")).as_double();
        }
        if (segment_json.has_field(U("cashAvailableForWithdrawal"))) {
            this->cash_available_for_withdrawal = segment_json.at(U("cashAvailableForWithdrawal")).as_double();
        }
        if (segment_json.has_field(U("cashBalance"))) {
            this->cash_balance = segment_json.at(U("cashBalance")).as_double();
        }
        if (segment_json.has_field(U("category"))) {
            this->category = segment_json.at(U("category")).as_string();
        }
        if (segment_json.has_field(U("currency"))) {
            this->currency = segment_json.at(U("currency")).as_string();
        }
        if (segment_json.has_field(U("equityWithLoan"))) {
            this->equity_with_loan = segment_json.at(U("equityWithLoan")).as_double();
        }
        if (segment_json.has_field(U("excessLiquidation"))) {
            this->excess_liquidation = segment_json.at(U("excessLiquidation")).as_double();
        }
        if (segment_json.has_field(U("grossPositionValue"))) {
            this->gross_position_value = segment_json.at(U("grossPositionValue")).as_double();
        }
        if (segment_json.has_field(U("initMargin"))) {
            this->init_margin = segment_json.at(U("initMargin")).as_double();
        }
        if (segment_json.has_field(U("leverage"))) {
            this->leverage = segment_json.at(U("leverage")).as_double();
        }
        if (segment_json.has_field(U("lockedFunds"))) {
            this->locked_funds = segment_json.at(U("lockedFunds")).as_double();
        }
        if (segment_json.has_field(U("maintainMargin"))) {
            this->maintain_margin = segment_json.at(U("maintainMargin")).as_double();
        }
        if (segment_json.has_field(U("netLiquidation"))) {
            net_liquidation = segment_json.at(U("netLiquidation")).as_double();
        }
        if (segment_json.has_field(U("overnightLiquidation"))) {
            overnight_liquidation = segment_json.at(U("overnightLiquidation")).as_double();
        }
        if (segment_json.has_field(U("overnightMargin"))) {
            overnight_margin = segment_json.at(U("overnightMargin")).as_double();
        }
        if (segment_json.has_field(U("realizedPL"))) {
            realized_pl = segment_json.at(U("realizedPL")).as_double();
        }
        if (segment_json.has_field(U("totalTodayPL"))) {
            total_today_pl = segment_json.at(U("totalTodayPL")).as_double();
        }
        if (segment_json.has_field(U("unrealizedPL"))) {
            unrealized_pl = segment_json.at(U("unrealizedPL")).as_double();
        }
        if (segment_json.has_field(U("unrealizedPLByCostOfCarry"))) {
            unrealized_plby_cost_of_carry = segment_json.at(U("unrealizedPLByCostOfCarry")).as_double();
        }
    };

    string category;
    string capability;
    string currency;
    double buying_power = 0;
    double cash_available_for_trade = 0;
    double cash_available_for_withdrawal = 0;
    double cash_balance = 0;
    double equity_with_loan = 0;
    double excess_liquidation = 0;
    double gross_position_value = 0;
    double init_margin = 0;
    double leverage = 0;
    double locked_funds = 0;
    double maintain_margin = 0;
    double net_liquidation = 0;
    double overnight_liquidation = 0;
    double overnight_margin = 0;
    double realized_pl = 0;
    double total_today_pl = 0;
    double unrealized_pl = 0;
    double unrealized_plby_cost_of_carry = 0;
    vector<CurrencyAsset> currency_assets;

    std::string to_string() {
        std::stringstream ss;
        ss << U("Segment(category=") << this->category << U(" currency=") << this->currency << ")" << endl;
        return ss.str();
    };

};

class PortfolioAccount {
public:
    PortfolioAccount(){};
    string account;
    long update_timestamp;
    vector<Segment> segments;

    std::string to_string() {
        std::stringstream ss;
        ss << U("PortfolioAccount(account=") << this->account << U(" update_timestamp=") << this->update_timestamp << ")" << endl;
        return ss.str();
    };
};



#endif //TIGERAPI_MODEL_H
