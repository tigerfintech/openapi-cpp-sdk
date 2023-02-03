//
// Created by sukai on 2022/12/27.
//

#ifndef TIGERAPI_ORDER_UTIL_H
#define TIGERAPI_ORDER_UTIL_H


#include "model.h"

OPENAPI_EXPORT Order market_order(const utility::string_t account, Contract &contract, const utility::string_t action, long quantity);
OPENAPI_EXPORT Order limit_order(const utility::string_t account, Contract &contract, const utility::string_t action, long quantity, double limit_price);
OPENAPI_EXPORT Order limit_order(Contract &contract, const utility::string_t action, long quantity, double limit_price);
OPENAPI_EXPORT Order stop_order(const utility::string_t account, Contract &contract, const utility::string_t action, long quantity, double aux_price);
OPENAPI_EXPORT Order stop_limit_order(const utility::string_t account, Contract &contract, const utility::string_t action, long quantity,
                       double limit_price, double aux_price);
OPENAPI_EXPORT Order trail_order(const utility::string_t account, Contract &contract, const utility::string_t action, long quantity,
                  double aux_price=0, double trailing_percent=0);

#endif //TIGERAPI_ORDER_UTIL_H
