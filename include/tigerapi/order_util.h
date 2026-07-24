//
// Created by sukai on 2022/12/27.
//

#ifndef TIGERAPI_ORDER_UTIL_H
#define TIGERAPI_ORDER_UTIL_H


#include <vector>
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

        /** Amount order: order by cash amount instead of share quantity (e.g. fund subscription). */
        static Order
        amount_order(const utility::string_t account, Contract &contract, const utility::string_t action,
                     double cash_amount);

        /**
         * Multi-leg (combo) order, e.g. an option spread.
         * @param combo_type One of COVERED/PROTECTIVE/VERTICAL/STRADDLE/STRANGLE/CALENDAR/DIAGONAL/SYNTHETIC/CUSTOM.
         * @param legs       Each leg's contract, action, and ratio.
         */
        static Order
        multi_leg_order(const utility::string_t account, const utility::string_t combo_type,
                        const std::vector<ContractLeg> &legs, const utility::string_t action,
                        long long quantity, const utility::string_t order_type,
                        double limit_price = 0, double aux_price = 0, double trailing_percent = 0);

        /** TWAP algo order: time-weighted average price execution between start_time and end_time (epoch ms). */
        static Order
        twap_order(const utility::string_t account, Contract &contract, const utility::string_t action,
                   long long quantity, long long start_time, long long end_time, double limit_price = 0);

        /**
         * VWAP algo order: volume-weighted average price execution between start_time and end_time (epoch ms).
         * @param participation_rate Optional target participation rate (0 to omit).
         */
        static Order
        vwap_order(const utility::string_t account, Contract &contract, const utility::string_t action,
                   long long quantity, long long start_time, long long end_time,
                   double participation_rate = 0, double limit_price = 0);

        /**
         * OCA (one-cancels-all) bracket order: a profit-taker and a stop-loss order that are
         * siblings — when either fills, the other is automatically cancelled.
         * NOTE: the server rejects TradeClient::preview_order() for OCA orders with
         * "OCA/ATTACHED order preview not supported" (verified against production);
         * use place_order() directly.
         */
        static Order
        oca_brackets_order(const utility::string_t account, Contract &contract, const utility::string_t action,
                           long long quantity,
                           double profit_taker_price, const utility::string_t profit_taker_tif,
                           double stop_loss_price, double stop_loss_limit_price,
                           const utility::string_t stop_loss_tif);

        /**
         * Attach a profit-taker order to an existing (already-built) order.
         * NOTE: the server rejects TradeClient::preview_order() for attached orders with
         * "OCA/ATTACHED order preview not supported" (verified against production);
         * use place_order() directly.
         */
        static void
        add_profit_taker_order(Order &order, double profit_taker_price,
                               const utility::string_t profit_taker_tif = U(""), bool profit_taker_rth = false);

        /** Attach a stop-loss (STP) order to an existing (already-built) order. See add_profit_taker_order() for the preview_order caveat. */
        static void
        add_stop_loss_order(Order &order, double stop_loss_price, const utility::string_t stop_loss_tif = U(""));

        /** Attach a stop-limit (STP_LMT) stop-loss order to an existing (already-built) order. See add_profit_taker_order() for the preview_order caveat. */
        static void
        add_stop_loss_limit_order(Order &order, double stop_loss_price, double stop_loss_limit_price,
                                  const utility::string_t stop_loss_tif = U(""));

        /** Attach a trailing-stop-loss (TRAIL) order to an existing (already-built) order. See add_profit_taker_order() for the preview_order caveat. */
        static void
        add_stop_loss_trail_order(Order &order, double stop_loss_trailing_percent,
                                  double stop_loss_trailing_amount, const utility::string_t stop_loss_tif = U(""));

        /** Attach both a profit-taker and a stop-loss order (bracket) to an existing (already-built) order. See add_profit_taker_order() for the preview_order caveat. */
        static void
        add_brackets_order(Order &order, double profit_taker_price, const utility::string_t profit_taker_tif,
                           bool profit_taker_rth, double stop_loss_price, double stop_loss_limit_price,
                           const utility::string_t stop_loss_tif);
    };
}
#endif //TIGERAPI_ORDER_UTIL_H
