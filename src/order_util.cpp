//
// Created by sukai on 2022/12/27.
//

#include "../include/tigerapi/order_util.h"
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
}
