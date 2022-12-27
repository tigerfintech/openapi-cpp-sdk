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
    class TradeClient : public TigerClient {
    public:
        TradeClient();

        TradeClient(struct Config &cf);

        /**
         * 获取资产(适用综合/模拟账户) get asset
         * @param account
         * @param base_currency 币种
         * @return
         */
        value get_prime_asset(string account = "", string base_currency = "USD");

        value get_prime_asset(string account, Currency base_currency = Currency::USD);

        /**
         * 获取资产(适用于环球/综合/模拟账户)
         */
        value get_asset(string account = "", const value &sub_accounts = value::array(), bool segment = false,
                        bool market_value = false);

        /**
         * 获取持仓
         * @param account 资金账户，不传则默认适用配置文件里的account
         * @param sec_type 证券类型 STK/FUT/OPT
         * @param currency
         * @param market
         * @param symbol
         * @param sub_accounts
         * @param expiry
         * @param strike
         * @param right
         * @return
         */
        value get_positions(string account = "", SecType sec_type = SecType::ALL, Currency currency = Currency::ALL,
                            Market market = Market::ALL,
                            string symbol = "", const value &sub_accounts = value::array(), long expiry = -1,
                            double strike = 0, Right right = Right::ALL);

        value get_positions(string account, string sec_type = "", string currency = "ALL",
                            string market = "ALL",
                            string symbol = "", const value &sub_accounts = value::array(), long expiry = -1,
                            double strike = 0, string right = "");

        /**
         * 获取订单列表
         * @param account
         * @param sec_type
         * @param market
         * @param symbol
         * @param start_time
         * @param end_time
         * @param limit
         * @param is_brief
         * @param states
         * @param sort_by
         * @param seg_type
         * @return
         */
        value get_orders(string account = "", string sec_type = "",
                         string market = "ALL",
                         string symbol = "", long start_time = -1, long end_time = -1, int limit = 100,
                         bool is_brief = false, const value &states = value::array(), string sort_by = "",
                         string seg_type = "");

        value get_orders(string account, SecType sec_type = SecType::ALL,
                         Market market = Market::ALL,
                         string symbol = "", long start_time = -1, long end_time = -1, int limit = 100,
                         bool is_brief = false, const value &states = value::array(),
                         OrderSortBy sort_by = OrderSortBy::LATEST_STATUS_UPDATED,
                         SegmentType seg_type = SegmentType::SEC);

        /** 获取未成交订单 **/
        value get_active_orders(string account = "", string sec_type = "",
                                string market = "ALL",
                                string symbol = "", long start_time = -1, long end_time = -1, long parent_id = 0,
                                string sort_by = "",
                                string seg_type = "");

        value get_active_orders(string account, SecType sec_type = SecType::ALL,
                                Market market = Market::ALL,
                                string symbol = "", long start_time = -1, long end_time = -1, long parent_id = 0,
                                OrderSortBy sort_by = OrderSortBy::LATEST_STATUS_UPDATED,
                                SegmentType seg_type = SegmentType::SEC);

        Order get_order(long id, bool is_brief=false);
        value cancel_order(long id);
        value place_order(value &order);
        value place_order(Order &order);
        value modify_order(Order &order);
        value modify_order(Order &order, double limit_price=0, long total_quantity=0,  double aux_price=0,
                           double trail_stop_price=0, double trailing_percent=0, double percent_offset=0,
                           string time_in_force="", bool outside_rth=false, long expire_time=0);


        value get_contract(string symbol, string sec_type, string currency = "", string exchange = "", long expiry = -1,
                     double strike = 0, string right = "");
        value
        get_contract(string symbol, SecType sec_type = SecType::STK, Currency currency = Currency::ALL, string exchange = "", long expiry = -1,
                     double strike = 0, Right right = Right::ALL);




    private:
        value get_account_param(string account="");
    };
}


#endif //TIGERAPI_TRADE_CLIENT_H
