//
// Created by sukai on 2024/3/8.
//

#ifndef TIGERAPI_PRICEUTIL_H
#define TIGERAPI_PRICEUTIL_H
#include <iostream>
#include <vector>
#include <cmath>
#include <map>
#include "cpprest/json.h"
#include "cpprest/details/basic_types.h"
#include "constants.h"
#include "enums.h"

namespace TIGER_API {


    class PriceUtil {
    public:
        static const utility::string_t INF;
        static const double RELATIVE_TOLERANCE;
        static const utility::string_t TICK_SIZE;

        static bool match_tick_size(double price, const web::json::value &tick_sizes) {
            if (price == 0 || tick_sizes.size() == 0) {
                return false;
            }
            double fixed_price = fix_price_by_tick_size(price, tick_sizes);
            return std::abs(price - fixed_price) <= RELATIVE_TOLERANCE;
        }

        static double fix_price_by_tick_size(double price, const web::json::value tick_sizes, bool is_up = false) {
            if (price == 0) {
                return 0;
            }
            auto tick_size_item = find_tick_size_item(price, tick_sizes);
            if (tick_size_item.is_null()) {
                return price;
            }
            double min_tick = tick_size_item[TICK_SIZE].as_double();

            double begin = stod(tick_size_item[P_BEGIN].as_string());
            return round_with_tick(price, begin, min_tick, is_up);
        }

        static const web::json::value find_tick_size_item(double price, const web::json::value tick_sizes) {
            if (price == 0 || tick_sizes.size() == 0) {
                return {};
            }
            for (const auto& item : tick_sizes.as_array()) {
                utility::string_t type = item.at(P_TYPE).as_string();
                double begin = stod(item.at(P_BEGIN).as_string());
                double end = (item.at(P_END).as_string() == INF) ? INFINITY : stod(item.at(P_END).as_string());
                if (type == enum_to_str(TickSizeType::OPEN)) {
                    if (begin < price && price < end) {
                        return item;
                    }
                } else if  (type == enum_to_str(TickSizeType::CLOSED)) {
                    if (begin <= price && price <= end) {
                        return item;
                    }
                }  else if (type == enum_to_str(TickSizeType::CLOSED_OPEN)) {
                    if (begin <= price && price < end) {
                        return item;
                    }
                } else if (type == enum_to_str(TickSizeType::OPEN_CLOSED)) {
                    if (begin < price && price <= end) {
                        return item;
                    }
                }
            }
            return {};
        }

        static double round_with_tick(double price, double begin, double min_tick, bool is_up) {
            double multiple = (price - begin) / min_tick;
            if (multiple <= 0) {
                return price;
            }
            if (is_up) {
                multiple += 1;
            }
            return std::floor(multiple) * min_tick + begin;
        }
    };

    const utility::string_t PriceUtil::INF = U("Infinity");
    const utility::string_t PriceUtil::TICK_SIZE = U("tickSize");
    const double PriceUtil::RELATIVE_TOLERANCE = 1e-6;

} // TIGER_API

#endif //TIGERAPI_PRICEUTIL_H
