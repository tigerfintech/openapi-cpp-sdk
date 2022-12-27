//
// Created by sukai on 2022/12/27.
//

#ifndef TIGERAPI_ORDER_UTIL_H
#define TIGERAPI_ORDER_UTIL_H


#include "model.h"

Order market_order(const string account, Contract &contract, const string action, long quantity);
Order limit_order(const string account, Contract &contract, const string action, long quantity, double limit_price);
Order limit_order(Contract &contract, const string action, long quantity, double limit_price);
Order stop_order(const string account, Contract &contract, const string action, long quantity, double aux_price);
Order stop_limit_order(const string account, Contract &contract, const string action, long quantity,
                       double limit_price, double aux_price);
Order trail_order(const string account, Contract &contract, const string action, long quantity,
                  double aux_price=0, double trailing_percent=0);

#endif //TIGERAPI_ORDER_UTIL_H
