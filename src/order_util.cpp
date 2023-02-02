#include "../pch.h"
//
// Created by sukai on 2022/12/27.
//

#include "../include/tigerapi/order_util.h"

Order market_order(const utility::string_t account, Contract &contract, const utility::string_t action, long quantity) {
    // 市价单
    return Order(U("MKT"), account, contract, action, quantity);
}

Order limit_order(const utility::string_t account, Contract &contract, const utility::string_t action, long quantity, double limit_price) {
    // 限价单
    return Order( U("LMT"), account, contract, action, quantity, limit_price);
}

Order limit_order(Contract &contract, const utility::string_t action, long quantity, double limit_price) {
    // 限价单
    return Order( U("LMT"), U(""), contract, action, quantity, limit_price);
}

Order stop_order(const utility::string_t account, Contract &contract, const utility::string_t action, long quantity, double aux_price) {
    // 止损单
    return Order(U("STP"), account, contract, action, quantity, 0, aux_price);
}

Order stop_limit_order(const utility::string_t account, Contract &contract, const utility::string_t action, long quantity, double limit_price, double aux_price) {
    // 限价止损单
    return Order( U("STP_LMT"), account, contract, action, quantity, limit_price, aux_price);
}

Order trail_order(const utility::string_t account, Contract &contract, const utility::string_t action, long quantity,
                  double aux_price, double trailing_percent) {
    // 移动止损单
    return Order( U("TRAIL"), account, contract, action, quantity, 0,  aux_price, trailing_percent);
}
