//
// Created by sukai on 2022/12/27.
//

#include "tigerapi/order_util.h"
namespace TIGER_API {

    Order
    OrderUtil::market_order(const utility::string_t account, Contract &contract, const utility::string_t action, long quantity) {
        // market order
        return Order(U("MKT"), account, contract, action, quantity);
    }

    Order
    OrderUtil::limit_order(const utility::string_t account, Contract &contract, const utility::string_t action, long quantity,
                double limit_price) {
        // limit order
        return Order(U("LMT"), account, contract, action, quantity, limit_price);
    }

    Order OrderUtil::limit_order(Contract &contract, const utility::string_t action, long quantity, double limit_price) {
        // limit order
        return Order(U("LMT"), U(""), contract, action, quantity, limit_price);
    }

    Order OrderUtil::limit_order(Contract& contract, const utility::string_t action, long quantity, utility::string_t limit_price) {
        // limit order, price is string type
        Order order = Order();
        order.order_type = U("LMT");
        order.contract = contract;
        order.action = action;
        order.total_quantity = quantity;
        order.s_limit_price = limit_price;
        return order;
    }

    Order OrderUtil::stop_order(const utility::string_t account, Contract &contract, const utility::string_t action, long quantity,
                     double aux_price) {
        // stop order
        return Order(U("STP"), account, contract, action, quantity, 0, aux_price);
    }

    Order
    OrderUtil::stop_limit_order(const utility::string_t account, Contract &contract, const utility::string_t action, long quantity,
                     double limit_price, double aux_price) {
        // stop limit order
        return Order(U("STP_LMT"), account, contract, action, quantity, limit_price, aux_price);
    }

    Order
    OrderUtil::trail_order(const utility::string_t account, Contract &contract, const utility::string_t action, long quantity,
                double aux_price, double trailing_percent) {
        // trailing stop order
        return Order(U("TRAIL"), account, contract, action, quantity, 0, aux_price, trailing_percent);
    }

    Order
    OrderUtil::iceberg_order(const utility::string_t account, Contract &contract, const utility::string_t action,
                             long long quantity, double limit_price, long long display_size) {
        if (display_size <= 0) {
            throw std::invalid_argument("iceberg_order: display_size must be a positive integer");
        }
        Order order(U("ICEBERG"), account, contract, action, quantity, limit_price);
        order.time_in_force = U("DAY");
        order.display_size = display_size;
        order.price_type = U("LIMIT_PRICE");
        return order;
    }

    Order
    OrderUtil::iceberg_order(const utility::string_t account, Contract &contract, const utility::string_t action,
                             long long quantity, double limit_price,
                             long long display_size, long long min_display_size,
                             long long check_intervals,
                             const utility::string_t price_type,
                             long long start_time, long long end_time) {
        if (display_size <= 0) {
            throw std::invalid_argument("iceberg_order: display_size must be a positive integer");
        }
        Order order(U("ICEBERG"), account, contract, action, quantity, limit_price);
        order.time_in_force = U("DAY");
        order.display_size = display_size;
        if (min_display_size > 0) order.min_display_size = min_display_size;
        if (check_intervals > 0) order.check_intervals = check_intervals;
        // Default to LIMIT_PRICE when not specified, consistent with the basic overload.
        order.price_type = price_type.empty() ? U("LIMIT_PRICE") : price_type;
        if (start_time > 0) order.start_time = start_time;
        if (end_time > 0) order.end_time = end_time;
        return order;
    }

    Order
    OrderUtil::amount_order(const utility::string_t account, Contract &contract, const utility::string_t action,
                            double cash_amount) {
        if (cash_amount <= 0) {
            throw std::invalid_argument("amount_order: cash_amount must be positive");
        }
        Order order(U("MKT"), account, contract, action, 0);
        order.cash_amount = cash_amount;
        return order;
    }

    Order
    OrderUtil::multi_leg_order(const utility::string_t account, const utility::string_t combo_type,
                               const std::vector<ContractLeg> &legs, const utility::string_t action,
                               long long quantity, const utility::string_t order_type,
                               double limit_price, double aux_price, double trailing_percent) {
        if (legs.empty()) {
            throw std::invalid_argument("multi_leg_order: legs must not be empty");
        }
        Contract combo_contract;
        combo_contract.sec_type = U("MLEG");
        Order order(order_type, account, combo_contract, action, quantity, limit_price, aux_price, trailing_percent);
        order.combo_type = combo_type;
        order.contract_legs = legs;
        order.time_in_force = U("DAY");
        return order;
    }

    Order
    OrderUtil::twap_order(const utility::string_t account, Contract &contract, const utility::string_t action,
                          long long quantity, long long start_time, long long end_time, double limit_price) {
        if (start_time <= 0 || end_time <= 0 || end_time <= start_time) {
            throw std::invalid_argument("twap_order: end_time must be greater than start_time");
        }
        Order order(U("TWAP"), account, contract, action, quantity, limit_price);
        order.time_in_force = U("DAY");
        order.algo_strategy = U("TWAP");
        order.add_algo_param(U("start_time"), utility::conversions::to_string_t(std::to_string(start_time)));
        order.add_algo_param(U("end_time"), utility::conversions::to_string_t(std::to_string(end_time)));
        return order;
    }

    Order
    OrderUtil::vwap_order(const utility::string_t account, Contract &contract, const utility::string_t action,
                          long long quantity, long long start_time, long long end_time,
                          double participation_rate, double limit_price) {
        if (start_time <= 0 || end_time <= 0 || end_time <= start_time) {
            throw std::invalid_argument("vwap_order: end_time must be greater than start_time");
        }
        Order order(U("VWAP"), account, contract, action, quantity, limit_price);
        order.time_in_force = U("DAY");
        order.algo_strategy = U("VWAP");
        order.add_algo_param(U("start_time"), utility::conversions::to_string_t(std::to_string(start_time)));
        order.add_algo_param(U("end_time"), utility::conversions::to_string_t(std::to_string(end_time)));
        if (participation_rate > 0) {
            order.add_algo_param(U("participation_rate"), utility::conversions::to_string_t(std::to_string(participation_rate)));
        }
        return order;
    }

    Order
    OrderUtil::oca_brackets_order(const utility::string_t account, Contract &contract, const utility::string_t action,
                                  long long quantity,
                                  double profit_taker_price, const utility::string_t profit_taker_tif,
                                  double stop_loss_price, double stop_loss_limit_price,
                                  const utility::string_t stop_loss_tif) {
        // NOTE: each sub-order carries its own `account` field (matches Java SDK's
        // buildOCABracketsOrder, which calls buildTradeOrderModel(account, ...) for
        // both profitTakerOrder and stopLossOrder). The outer oca_wrapper.account is
        // also set for consistency/back-compat, but the server reads account from the
        // individual oca_orders entries and rejects the request with "field 'account'
        // cannot be empty" if they are missing it (verified against production).
        Order profit_taker(U("LMT"), account, contract, action, quantity, profit_taker_price);
        profit_taker.time_in_force = profit_taker_tif.empty() ? U("DAY") : profit_taker_tif;

        Order stop_loss(stop_loss_limit_price != 0 ? U("STP_LMT") : U("STP"), account, contract, action, quantity,
                        stop_loss_limit_price, stop_loss_price);
        stop_loss.time_in_force = stop_loss_tif.empty() ? U("DAY") : stop_loss_tif;

        Order oca_wrapper;
        oca_wrapper.account = account;
        oca_wrapper.oca_orders.push_back(profit_taker);
        oca_wrapper.oca_orders.push_back(stop_loss);
        return oca_wrapper;
    }

    void
    OrderUtil::add_profit_taker_order(Order &order, double profit_taker_price,
                                      const utility::string_t profit_taker_tif, bool profit_taker_rth) {
        order.attach_type = U("PROFIT");
        order.profit_taker_price = profit_taker_price;
        order.profit_taker_tif = profit_taker_tif;
        order.profit_taker_rth = profit_taker_rth;
    }

    void
    OrderUtil::add_stop_loss_order(Order &order, double stop_loss_price, const utility::string_t stop_loss_tif) {
        order.attach_type = U("LOSS");
        order.stop_loss_order_type = U("STP");
        order.stop_loss_price = stop_loss_price;
        order.stop_loss_tif = stop_loss_tif;
    }

    void
    OrderUtil::add_stop_loss_limit_order(Order &order, double stop_loss_price, double stop_loss_limit_price,
                                         const utility::string_t stop_loss_tif) {
        order.attach_type = U("LOSS");
        order.stop_loss_order_type = U("STP_LMT");
        order.stop_loss_price = stop_loss_price;
        order.stop_loss_limit_price = stop_loss_limit_price;
        order.stop_loss_tif = stop_loss_tif;
    }

    void
    OrderUtil::add_stop_loss_trail_order(Order &order, double stop_loss_trailing_percent,
                                         double stop_loss_trailing_amount, const utility::string_t stop_loss_tif) {
        order.attach_type = U("LOSS");
        order.stop_loss_order_type = U("TRAIL");
        order.stop_loss_trailing_percent = stop_loss_trailing_percent;
        order.stop_loss_trailing_amount = stop_loss_trailing_amount;
        order.stop_loss_tif = stop_loss_tif;
    }

    void
    OrderUtil::add_brackets_order(Order &order, double profit_taker_price, const utility::string_t profit_taker_tif,
                                  bool profit_taker_rth, double stop_loss_price, double stop_loss_limit_price,
                                  const utility::string_t stop_loss_tif) {
        order.attach_type = U("BRACKETS");
        order.profit_taker_price = profit_taker_price;
        order.profit_taker_tif = profit_taker_tif;
        order.profit_taker_rth = profit_taker_rth;
        // Same STP vs STP_LMT selection as oca_brackets_order's stop_loss sub-order:
        // a non-zero limit price means the caller wants a stop-limit, otherwise a
        // plain stop. Without this the server can't tell which stop-loss order type
        // to place.
        order.stop_loss_order_type = stop_loss_limit_price != 0 ? U("STP_LMT") : U("STP");
        order.stop_loss_price = stop_loss_price;
        order.stop_loss_limit_price = stop_loss_limit_price;
        order.stop_loss_tif = stop_loss_tif;
    }
}
