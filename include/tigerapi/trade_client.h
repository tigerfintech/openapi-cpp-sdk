//
// Created by sukai on 2022/12/23.
//

#ifndef TIGERAPI_TRADE_CLIENT_H
#define TIGERAPI_TRADE_CLIENT_H


#include "tiger_client.h"
#include "enums.h"
#include "service_types.h"
#include "utils.h"
#include "model.h"


namespace TIGER_API {
    class OPENAPI_EXPORT TradeClient : public TigerClient {
    public:
        TradeClient();
        TradeClient(const ClientConfig &cf);

        /** Get account list **/
        value get_accounts();

        /**
         * Get asset (applicable to prime/paper accounts)
         * @param account
         * @param base_currency Currency
         * @return
         */
        value get_prime_asset(const utility::string_t & account = U(""), const utility::string_t & base_currency = U("USD"));

        value get_prime_asset(const utility::string_t &account, Currency base_currency = Currency::USD);

        PortfolioAccount get_prime_portfolio(const utility::string_t &account = U(""), const utility::string_t &base_currency = U("USD"));

        /**
         * Get asset (applicable to global/prime/paper accounts)
         */
        value get_asset(utility::string_t account = U(""), const value &sub_accounts = value::array(), bool segment = false,
                        bool market_value = false);

        /**
         * Get position
         * @param account Account, default is account in config file
         * @param sec_type Security type STK/FUT/OPT
         * @param currency
         * @param market
         * @param symbol
         * @param sub_accounts
         * @param expiry
         * @param strike
         * @param right
         * @return
         */
        value get_positions(utility::string_t account = U(""), SecType sec_type = SecType::ALL, Currency currency = Currency::ALL,
                            Market market = Market::ALL,
                            utility::string_t symbol = U(""), const value &sub_accounts = value::array(), time_t expiry = -1,
                            utility::string_t strike = U(""), Right right = Right::ALL);

        value get_positions(const utility::string_t &account, const utility::string_t &sec_type = U(""), const utility::string_t &currency = U("ALL"),
                            const utility::string_t &market = U("ALL"),
                            const utility::string_t &symbol = U(""), const value &sub_accounts = value::array(), time_t expiry = -1,
                            utility::string_t strike = U(""), const utility::string_t &right = U(""));

        vector<Position> get_position_list(utility::string_t account = U(""), utility::string_t sec_type = U(""), utility::string_t currency = U("ALL"),
                            utility::string_t market = U("ALL"),
                            utility::string_t symbol = U(""), const value &sub_accounts = value::array(), time_t expiry = -1,
                                           utility::string_t strike = U(""), utility::string_t right = U(""));
        /**
         * Get order list
         * @param account
         * @param sec_type
         * @param market
         * @param symbol
         * @param start_date
         * @param end_date
         * @param limit
         * @param is_brief
         * @param states
         * @param sort_by
         * @param seg_type
         * @return
         */
        value get_orders(const utility::string_t &account = U(""), const utility::string_t &sec_type = U(""),
                         const utility::string_t &market = U("ALL"),
                         const utility::string_t &symbol = U(""), time_t start_date = -1, time_t end_date = -1, int limit = 100,
                         bool is_brief = false, const value &states = value::array(), const utility::string_t &sort_by = U(""),
                         const utility::string_t &seg_type = U(""));

        value get_orders(utility::string_t account, SecType sec_type = SecType::ALL,
                         Market market = Market::ALL,
                         utility::string_t symbol = U(""), time_t start_date = -1, time_t end_date = -1, int limit = 100,
                         bool is_brief = false, const value &states = value::array(),
                         OrderSortBy sort_by = OrderSortBy::LATEST_STATUS_UPDATED,
                         SegmentType seg_type = SegmentType::SEC);

        /** Get active orders **/
        value get_active_orders(utility::string_t account = U(""), utility::string_t sec_type = U(""),
                                utility::string_t market = U("ALL"),
                                utility::string_t symbol = U(""), time_t start_date = -1, time_t end_date = -1, unsigned long long parent_id = 0,
                                utility::string_t sort_by = U(""),
                                utility::string_t seg_type = U(""));

        value get_active_orders(utility::string_t account, SecType sec_type = SecType::ALL,
                                Market market = Market::ALL,
                                utility::string_t symbol = U(""), time_t start_date = -1, time_t end_date = -1, unsigned long long parent_id = 0,
                                OrderSortBy sort_by = OrderSortBy::LATEST_STATUS_UPDATED,
                                SegmentType seg_type = SegmentType::SEC);

        /** Get filled orders **/
        value get_filled_orders(utility::string_t account = U(""), utility::string_t sec_type = U(""),
                                utility::string_t market = U("ALL"),
                                utility::string_t symbol = U(""), time_t start_date = -1, time_t end_date = -1, unsigned long long parent_id = 0,
                                utility::string_t sort_by = U(""),
                                utility::string_t seg_type = U(""));

        value get_filled_orders(utility::string_t account, SecType sec_type = SecType::ALL,
                                Market market = Market::ALL,
                                utility::string_t symbol = U(""), time_t start_date = -1, time_t end_date = -1, unsigned long long parent_id = 0,
                                OrderSortBy sort_by = OrderSortBy::LATEST_STATUS_UPDATED,
                                SegmentType seg_type = SegmentType::SEC);

        /** Get inactive orders **/
        value get_inactive_orders(utility::string_t account = U(""), utility::string_t sec_type = U(""),
                                  utility::string_t market = U("ALL"),
                                  utility::string_t symbol = U(""), time_t start_date = -1, time_t end_date = -1, unsigned long long parent_id = 0,
                                  utility::string_t sort_by = U(""),
                                  utility::string_t seg_type = U(""));

        value get_inactive_orders(utility::string_t account, SecType sec_type = SecType::ALL,
                                  Market market = Market::ALL,
                                  utility::string_t symbol = U(""), time_t start_date = -1, time_t end_date = -1, unsigned long long parent_id = 0,
                                  OrderSortBy sort_by = OrderSortBy::LATEST_STATUS_UPDATED,
                                  SegmentType seg_type = SegmentType::SEC);

        Order get_order(unsigned long long id, bool is_brief=false);
        value get_transactions(utility::string_t account, long long order_id);
        value get_transactions(utility::string_t account, utility::string_t symbol, utility::string_t sec_type = U(""),
                               long start_time = -1, time_t end_time = -1,
                               int limit = 100, utility::string_t expiry = U(""), utility::string_t strike = U(""), utility::string_t right = U(""),
                               long long order_id = 0);

        value cancel_order(unsigned long long id);
        value place_order(value &order);
        value place_order(Order &order);
        value modify_order(Order &order);
        value modify_order(Order &order, double limit_price=0, long total_quantity=0,  double aux_price=0,
                           double trail_stop_price=0, double trailing_percent=0, double percent_offset=0,
                           utility::string_t time_in_force=U(""), bool outside_rth=false, time_t expire_time=0);


        value get_contract(utility::string_t symbol, utility::string_t sec_type, utility::string_t currency = U(""), utility::string_t exchange = U(""), time_t expiry = -1,
                           utility::string_t strike = U(""), utility::string_t right = U(""));
        value
        get_contract(utility::string_t symbol, SecType sec_type = SecType::STK, Currency currency = Currency::ALL, utility::string_t exchange = U(""), time_t expiry = -1,
                     utility::string_t strike = U(""), Right right = Right::ALL);
        value get_contracts(const value &symbols, utility::string_t sec_type, utility::string_t currency = U(""), utility::string_t exchange = U(""), time_t expiry = -1,
                            utility::string_t strike = U(""), utility::string_t right = U(""));
        value get_quote_contract(utility::string_t symbol, utility::string_t sec_type, utility::string_t expiry);

        value get_estimate_tradable_quantity(Order &order, utility::string_t seg_type = U("SEC"));
        value get_analytics_asset(utility::string_t account, utility::string_t start_date, utility::string_t end_date,
                                  utility::string_t seg_type = U("SEC"), utility::string_t currency = U("USD"),
                                  utility::string_t sub_account = U(""));

        value get_segment_fund_history(int limit = 0);
        value get_segment_fund_available(utility::string_t from_segment, utility::string_t currency = U("USD"));
        value transfer_segment_fund(utility::string_t from_segment, utility::string_t to_segment, double amount, utility::string_t currency = U("USD"));
        value place_forex_order(utility::string_t seg_type, utility::string_t source_currency, utility::string_t target_currency, double source_amount);


    private:
        value get_account_param(const utility::string_t &account=U(""));

        void set_secret_key(value &obj);
    };
}


#endif //TIGERAPI_TRADE_CLIENT_H
