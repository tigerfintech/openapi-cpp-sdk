//
// Created by sukai on 2022/12/27.
//

#ifndef TIGERAPI_MODEL_H
#define TIGERAPI_MODEL_H

#include <typeinfo>
//#include <any>
#include <string>
#include "cpprest/json.h"
#include "cpprest/details/basic_types.h"
#include "win32.h"

using namespace std;

namespace TIGER_API {

    class OPENAPI_EXPORT Contract {
    public:
        Contract() {};

        Contract(const utility::string_t sec_type, const utility::string_t symbol, const utility::string_t currency,
                 const utility::string_t local_symbol,
                 const utility::string_t exchange, long contract_id) : sec_type(sec_type), symbol(symbol),
                                                                       currency(currency),
                                                                       local_symbol(local_symbol), exchange(exchange),
                                                                       contract_id(contract_id) {};

        Contract(const utility::string_t sec_type, const utility::string_t symbol, const utility::string_t expiry,
                 const utility::string_t strike, const utility::string_t right,
                 const utility::string_t currency, long multiplier, const utility::string_t local_symbol,
                 long contract_id) :
                sec_type(sec_type), symbol(symbol), expiry(expiry), strike(strike), right(right),
                currency(currency), multiplier(multiplier), local_symbol(local_symbol), contract_id(contract_id) {};

        Contract(const utility::string_t sec_type, const utility::string_t symbol, const utility::string_t expiry,
                 long multiplier,
                 const utility::string_t contract_month,
                 const utility::string_t currency, const utility::string_t exchange,
                 const utility::string_t local_symbol) :
                sec_type(sec_type), symbol(symbol), expiry(expiry), currency(currency), multiplier(multiplier),
                exchange(exchange), contract_month(contract_month), local_symbol(local_symbol) {};

        utility::string_t symbol;
        utility::string_t currency;
        utility::string_t sec_type;
        utility::string_t exchange;
        utility::string_t local_symbol;
        utility::string_t expiry;
        utility::string_t strike;
        utility::string_t right;
        int multiplier = 0;
        utility::string_t contract_month;
        long contract_id = 0;
        utility::string_t identifier;
        utility::string_t market;
    };

    class OPENAPI_EXPORT Order {
    public:
        Order() {};

        Order(const utility::string_t order_type, const utility::string_t account, Contract &contract,
              const utility::string_t action, long long total_quantity) :
                order_type(order_type), account(account), contract(contract), action(action),
                total_quantity(total_quantity) {};

        Order(const utility::string_t order_type, const utility::string_t account, Contract &contract,
              const utility::string_t action, long long total_quantity,
              double limit_price, double aux_price = 0, double trailing_percent = 0) :
                order_type(order_type), account(account), contract(contract), action(action),
                total_quantity(total_quantity),
                limit_price(limit_price), aux_price(aux_price), trailing_percent(trailing_percent) {};

        Contract contract;
        utility::string_t account;
        unsigned long long id = 0;
        long order_id = 0;
        /**  订单类型, 'MKT' 市价单 / 'LMT' 限价单 / 'STP' 止损单 / 'STP_LMT' 止损限价单 / 'TRAIL' 跟踪止损单 **/
        utility::string_t order_type;
        /** 交易方向, 'BUY' / 'SELL' **/
        utility::string_t action;
        /** 下单数量 **/
        long long total_quantity = 0;
        /** 下单数量偏移位数，如 total_quantity 为 5, total_quantity_scale 为 1， 则实际下单数量为 5 * 10^-1 = 0.5 **/
        long total_quantity_scale = 0;
        /** 限价单价格 **/
        double limit_price = 0;
        utility::string_t s_limit_price;
        /** 在止损单中, 表示触发止损单的价格, 在移动止损单中, 表示跟踪的价差 **/
        double aux_price = 0;
        double trail_stop_price = 0;
        double trailing_percent = 0;
        double percent_offset = 0;
        /** 有效期,'DAY' 日内有效 / 'GTC' good till cancel  / 'GTD' good till date **/
        utility::string_t time_in_force;
        /** 是否允许盘前盘后交易(outside of regular trading hours 美股专属). True 允许, False 不允许 **/
        bool outside_rth;
        /**
         * 价格微调幅度（默认为0表示不调整，正数为向上调整，负数向下调整），对传入价格自动调整到合法价位上.
              例如：0.001 代表向上调整且幅度不超过 0.1%；-0.001 代表向下调整且幅度不超过 0.1%。默认 0 表示不调整
         */
        double adjust_limit;
        utility::string_t user_mark;
        time_t expire_time = 0;

        // 订单状态
        utility::string_t status;
        // 主订单id, 目前只用于 TigerTrade App端的附加订单中
        unsigned long long parent_id;
        // 下单时间
        time_t open_time;
        // 下单失败时, 会返回失败原因的描述
        utility::string_t reason;
        // 最新成交时间
        time_t latest_time;
        // order updated time
        time_t update_time;
        // 成交数量
        long long filled_quantity;
        // 成交金额偏移位数
        long filled_quantity_scale;
        // 包含佣金的平均成交价
        double avg_fill_price;
        // 实现盈亏
        double realized_pnl;
        utility::string_t secret_key;
        web::json::value sub_ids;
        utility::string_t algo_strategy;
        double commission;
        double gst;

        utility::string_t to_string() {
            utility::stringstream_t ss;
            ss << U("Order(id=") << this->id << U(" status=") << this->status << U(" total_quantity: ")
               << this->total_quantity
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
                utility::string_t symbol = json.at(U("symbol")).as_string();
                utility::string_t sec_type = json.at(U("secType")).as_string();
                utility::string_t market = json.at(U("market")).as_string();
                utility::string_t currency = json.at(U("currency")).as_string();
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
                adjust_limit = json.at(U("adjustLimit")).as_double();
            }
            if (json.has_field(U("userMark"))) {
                user_mark = json.at(U("userMark")).as_string();
            }
            if (json.has_field(U("expireTime"))) {
                expire_time = json.at(U("expireTime")).as_number().to_int64();
            }
            if (json.has_field(U("status"))) {
                status = json.at(U("status")).as_string();
            }
            if (json.has_field(U("parentId"))) {
                parent_id = json.at(U("parentId")).as_number().to_uint64();
            }
            if (json.has_field(U("openTime"))) {
                open_time = json.at(U("openTime")).as_number().to_int64();
            }
            if (json.has_field(U("reason"))) {
                reason = json.at(U("reason")).as_string();
            }
            if (json.has_field(U("latestTime"))) {
                latest_time = json.at(U("latestTime")).as_number().to_int64();
            }
            if (json.has_field(U("updateTime"))) {
                update_time = json.at(U("updateTime")).as_number().to_int64();
            }
            if (json.has_field(U("filledQuantity"))) {
                filled_quantity = json.at(U("filledQuantity")).as_number().to_int64();
            }
            if (json.has_field(U("totalQuantity"))) {
                total_quantity = json.at(U("totalQuantity")).as_number().to_int64();
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
            if (json.has_field(U("gst"))) {
                gst = json.at(U("gst")).as_double();
            }
            if (json.has_field(U("subIds"))) {
                sub_ids = json.at(U("subIds"));
            }
            if (json.has_field(U("algoStrategy"))) {
                algo_strategy = json.at(U("algoStrategy")).as_string();
            }
            if (json.has_field(U("total_quantity_scale"))) {
                total_quantity_scale = json.at(U("total_quantity_scale")).as_number().to_int64();
            }
            if (json.has_field(U("filled_quantity_scale"))) {
                filled_quantity_scale = json.at(U("filled_quantity_scale")).as_number().to_int64();
            }
            if (json.has_field(U("secret_key"))) {
                secret_key = json.at(U("secret_key")).as_string();
            }

        };

    };

    class OPENAPI_EXPORT Position {
    public:
        Position() {};

        Position(const utility::string_t &account, const Contract &contract, long long position = 0,
                 double average_cost = 0.0,
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
                utility::string_t symbol = json.at(U("symbol")).as_string();
                utility::string_t sec_type = json.at(U("secType")).as_string();
                utility::string_t market = json.at(U("market")).as_string();
                utility::string_t currency = json.at(U("currency")).as_string();
                utility::string_t identifier = json.at(U("identifier")).as_string();
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
                update_timestamp = json.at(U("updateTimestamp")).as_number().to_uint64();
            }
            if (json.has_field(U("status"))) {
                status = json.at(U("status")).as_integer();
            }
        }

        utility::string_t account;
        Contract contract;
        long long position;
        double average_cost;
        double market_value;
        double realized_pnl;
        double unrealized_pnl;
        double latest_price;
        int status;
        time_t update_timestamp;


        utility::string_t to_string() {
            utility::stringstream_t ss;
            ss << U("Position(symbol=") << this->contract.symbol << U(" position=") << this->position << ")" << endl;
            return ss.str();
        };
    };

    class OPENAPI_EXPORT CurrencyAsset {
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
        utility::string_t currency;
        double gross_position_value = 0;
        double option_market_value = 0;
        double realized_pl = 0;
        double stock_market_value = 0;
        double unrealized_pl = 0;
    };

    class OPENAPI_EXPORT Segment {
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

        utility::string_t category;
        utility::string_t capability;
        utility::string_t currency;
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

        utility::string_t to_string() {
            utility::stringstream_t ss;
            ss << U("Segment(category=") << this->category << U(" currency=") << this->currency << ")" << endl;
            return ss.str();
        };

    };

    class OPENAPI_EXPORT PortfolioAccount {
    public:
        PortfolioAccount() {};
        utility::string_t account;
        long update_timestamp;
        vector<Segment> segments;

        utility::string_t to_string() {
            utility::stringstream_t ss;
            ss << U("PortfolioAccount(account=") << this->account << U(" update_timestamp=") << this->update_timestamp
               << ")" << endl;
            return ss.str();
        };
    };

    class OPENAPI_EXPORT KlineItem {
    public:
        double open = 0;
        double high = 0;
        double low = 0;
        double close = 0;
        time_t time;
        long long volume = 0;
        time_t last_time;
        long long open_interest = 0;
        double settlement = 0;

        void from_json(const web::json::value& j) {
            if (j.has_field(U("open"))) {
                open = j.at(U("open")).as_double();
            }
            if (j.has_field(U("high"))) {
                high = j.at(U("high")).as_double();
            }
            if (j.has_field(U("low"))) {
                low = j.at(U("low")).as_double();
            }
            if (j.has_field(U("close"))) {
                close = j.at(U("close")).as_double();
            }
            if (j.has_field(U("time"))) {
                time = j.at(U("time")).as_number().to_int64();
            }
            if (j.has_field(U("lastTime"))) {
                last_time = j.at(U("lastTime")).as_number().to_int64();
            }
            if (j.has_field(U("volume"))) {
                volume = j.at(U("volume")).as_number().to_int64();
            }
            if (j.has_field(U("settlement"))) {
                settlement = j.at(U("settlement")).as_double();
            }
            if (j.has_field(U("openInterest"))) {
                open_interest = j.at(U("openInterest")).as_number().to_int64();
            }

        }
    };

    class OPENAPI_EXPORT Kline {
    public:
        vector<KlineItem> items;
        utility::string_t symbol;
        utility::string_t contract_code;
        utility::string_t period;
        time_t expiry;
        utility::string_t right;
        utility::string_t strike;

        utility::string_t to_string() {
            return U("Kline of ") + symbol;
        }
    };

    class OPENAPI_EXPORT RealtimeQuote {
    public:
        double open = 0;
        double high = 0;
        double low = 0;
        double close = 0;
        long long volume = 0;
        double adj_pre_close = 0;
        double pre_close = 0;
        double ask_price = 0;
        double ask_size = 0;
        double bid_price = 0;
        double bid_size = 0;
        double latest_price = 0;
        time_t latest_time;
        long long latest_size = 0;
        utility::string_t status;
        utility::string_t symbol;

        utility::string_t contract_code;
        long long open_interest = 0;
        int limit_down = 0;
        int limit_up = 0;


        void from_json(const web::json::value &j) {
            if (j.has_field(U("open")))
                open = j.at(U("open")).as_double();

            if (j.has_field(U("high")))
                high = j.at(U("high")).as_double();

            if (j.has_field(U("low")))
                low = j.at(U("low")).as_double();

            if (j.has_field(U("close")))
                close = j.at(U("close")).as_double();

            if (j.has_field(U("volume")))
                volume = j.at(U("volume")).as_number().to_int64();

            if (j.has_field(U("adjPreClose")))
                adj_pre_close = j.at(U("adjPreClose")).as_double();

            if (j.has_field(U("preClose")))
                pre_close = j.at(U("preClose")).as_double();

            if (j.has_field(U("askPrice")))
                ask_price = j.at(U("askPrice")).as_double();

            if (j.has_field(U("askSize")))
                ask_size = j.at(U("askSize")).as_double();

            if (j.has_field(U("bidPrice")))
                bid_price = j.at(U("bidPrice")).as_double();

            if (j.has_field(U("bidSize")))
                bid_size = j.at(U("bidSize")).as_double();

            if (j.has_field(U("latestPrice")))
                latest_price = j.at(U("latestPrice")).as_double();

            if (j.has_field(U("latestTime")))
                latest_time = j.at(U("latestTime")).as_number().to_int64();

            if (j.has_field(U("latestSize")))
                latest_size = j.at(U("latestSize")).as_number().to_int64();

            if (j.has_field(U("status")))
                status = j.at(U("status")).as_string();

            if (j.has_field(U("symbol")))
                symbol = j.at(U("symbol")).as_string();

            if (j.has_field(U("contractCode")))
                contract_code = j.at(U("contractCode")).as_string();

            if (j.has_field(U("openInterest")))
                open_interest = j.at(U("openInterest")).as_number().to_int64();

            if (j.has_field(U("limitDown")))
                limit_down = j.at(U("limitDown")).as_integer();

            if (j.has_field(U("limitUp")))
                limit_up = j.at(U("limitUp")).as_integer();
        }

        utility::string_t to_string() {
            return U("RealtimeQuote<") + symbol + U("> ");
        }
    };



    class OPENAPI_EXPORT TradeTickItem {
    public:
        std::string tick_type;
        double price;
        int volume;
        std::string part_code;
        std::string part_code_name;
        std::string cond;
        std::string time;
        std::string sn;

    public:
        // Constructor
        TradeTickItem() : 
            price(0), 
            volume(0) 
        {}

        // Getters
        const std::string& get_ticktype() const { return tick_type; }
        double get_price() const { return price; }
        int get_volume() const { return volume; }
        const std::string& get_partcode() const { return part_code; }
        const std::string& get_partcodename() const { return part_code_name; }
        const std::string& get_cond() const { return cond; }
        const std::string& get_time() const { return time; }
        const std::string& get_sn() const { return sn; }

        // Setters
        void set_ticktype(const std::string& value) { tick_type = value; }
        void set_price(double value) { price = value; }
        void set_volume(int value) { volume = value; }
        void set_partcode(const std::string& value) { part_code = value; }
        void set_partcodename(const std::string& value) { part_code_name = value; }
        void set_cond(const std::string& value) { cond = value; }
        void set_time(const std::string& value) { time = value; }
        void set_sn(const std::string& value) { sn = value; }

        // toString equivalent
        std::string to_string() const {
            std::ostringstream oss;
            oss << "TradeTickItem<{tick_type: " << tick_type
            << ", price: " << price
            << ", volume: " << volume
            << ", part_code: " << part_code
            << ", part_code_name: " << part_code_name
            << ", cond: " << cond
            << ", time: " << time
            << ", sn: " << sn
            << "}>";
        return oss.str();
        }
    };

    class OPENAPI_EXPORT TradeTick {
    public:
        std::string symbol;
        std::string sec_type;
        std::string quote_level;
        std::string timestamp;
        std::vector<TradeTickItem> ticks;

    public:
        // Constructor
        TradeTick() {}

        // Getters
        const std::string& get_symbol() const { return symbol; }
        const std::string& get_sectype() const { return sec_type; }
        const std::string& get_quotelevel() const { return quote_level; }
        const std::string& get_timestamp() const { return timestamp; }
        const std::vector<TradeTickItem>& get_ticks() const { return ticks; }

        // Setters
        void set_symbol(const std::string& value) { symbol = value; }
        void set_sectype(const std::string& value) { sec_type = value; }
        void set_quotelevel(const std::string& value) { quote_level = value; }
        void set_timestamp(const std::string& value) { timestamp = value; }
        void set_ticks(const std::vector<TradeTickItem>& value) { ticks = value; }

    // Add single tick
        void addTick(const TradeTickItem& tick) {
            ticks.push_back(tick);
        }

        // toString equivalent
        std::string to_string() const {
            std::ostringstream oss;
            oss << "TradeTick<{symbol: " << symbol
            << ", sec_type: " << sec_type
            << ", quote_level: " << quote_level
            << ", timestamp: " << timestamp
            << ", ticks: [";
        
        for (size_t i = 0; i < ticks.size(); ++i) {
            if (i > 0) oss << ", ";
            oss << ticks[i].toString();
        }
        oss << "]}>";
        return oss.str();
    }
};


}


#endif //TIGERAPI_MODEL_H
