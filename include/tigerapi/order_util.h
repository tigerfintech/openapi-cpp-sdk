//
// Created by sukai on 2022/12/27.
//

#ifndef TIGERAPI_ORDER_UTIL_H
#define TIGERAPI_ORDER_UTIL_H


#include "model.h"

namespace TIGER_API {
    class OPENAPI_EXPORT OrderUtil {

    public:
        static Order
        market_order(const utility::string_t account, Contract &contract, const utility::string_t action,
                     long quantity);

        static Order
        limit_order(const utility::string_t account, Contract &contract, const utility::string_t action, long quantity,
                    double limit_price);

        static Order
        limit_order(Contract &contract, const utility::string_t action, long quantity, double limit_price);

        static Order
            limit_order(Contract& contract, const utility::string_t action, long quantity, const utility::string_t limit_price);


        static Order
        stop_order(const utility::string_t account, Contract &contract, const utility::string_t action, long quantity,
                   double aux_price);

        static Order
        stop_limit_order(const utility::string_t account, Contract &contract, const utility::string_t action,
                         long quantity,
                         double limit_price, double aux_price);

        static Order
        trail_order(const utility::string_t account, Contract &contract, const utility::string_t action, long quantity,
                    double aux_price = 0, double trailing_percent = 0);

        /** Iceberg order (basic) — only display_size required. */
        static Order
        iceberg_order(const utility::string_t account, Contract &contract, const utility::string_t action,
                      long long quantity, double limit_price, long long display_size);

        /** Iceberg order (full params). Pass 0 for optional numeric fields to omit them. */
        static Order
        iceberg_order(const utility::string_t account, Contract &contract, const utility::string_t action,
                      long long quantity, double limit_price,
                      long long display_size, long long min_display_size,
                      long long check_intervals,
                      const utility::string_t price_type,
                      long long start_time, long long end_time);
    };
}
#endif //TIGERAPI_ORDER_UTIL_H
