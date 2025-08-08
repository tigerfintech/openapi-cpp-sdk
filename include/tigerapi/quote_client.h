//
// Created by sukai on 2022/12/21.
//

#ifndef TIGERAPI_QUOTE_CLIENT_H
#define TIGERAPI_QUOTE_CLIENT_H

#include "tiger_client.h"
#include "client_config.h"
#include "enums.h"
#include "service_types.h"
#include "model.h"


namespace TIGER_API {
    class OPENAPI_EXPORT QuoteClient : public TigerClient {
    public:
        QuoteClient();
        QuoteClient(const ClientConfig &cf, bool is_grab_permission = true);
        value grab_quote_permission();
        value get_quote_permission();

        /** Stock quote related api **/

        value get_symbols(Market market = Market::ALL, bool include_otc=false);
        value get_all_symbol_names(Market market = Market::ALL, bool include_otc=false);
        value get_all_symbol_names(utility::string_t market = U("ALL"), bool include_otc=false);
        value get_market_state(utility::string_t market);
        value get_trading_calendar(Market market, utility::string_t begin_date, utility::string_t end_date);
        value get_trading_calendar(utility::string_t market, utility::string_t begin_date, utility::string_t end_date);
        value get_quote_stock_trade(const value &symbols);
        value get_brief(const value &symbols, bool include_hour_trading=false, bool include_ask_bid=false, QuoteRight right=QuoteRight::br);
        value get_stock_detail(const value &symbols);
        value get_timeline(const value &symbols, bool include_hour_trading=false, time_t begin_time=-1);
        value get_history_timeline(const value &symbols, utility::string_t date, QuoteRight right=QuoteRight::br);
        value get_kline(const value &symbols, BarPeriod period=BarPeriod::DAY, time_t begin_time=-1, time_t end_time=-1,
                        QuoteRight right=QuoteRight::br, int limit=251, utility::string_t page_token=U(""));
        value get_kline(const value &symbols, utility::string_t period, time_t begin_time=-1, time_t end_time=-1,
                        utility::string_t right=U("br"), int limit=251, utility::string_t page_token=U(""));
        vector<Kline> get_kline(const value &symbols, utility::string_t period, time_t begin_time=-1, time_t end_time=-1,
                              int limit=251, utility::string_t right=U("br"), utility::string_t page_token=U(""));
        value get_trade_tick(const value &symbols, TradingSession trade_session=TradingSession::Regular, long begin_index=-1,
                             long end_index=-1, int limit=100);
        value get_trade_tick(const value &symbols, long begin_index=-1,
                             long end_index=-1, utility::string_t trade_session=U("Regular"), int limit=100);
        value get_quote_real_time_value(const value &symbols);
        vector<RealtimeQuote> get_quote_real_time(const value &symbols);
        value get_quote_delay(const value &symbols);
        value get_quote_shortable_stocks(const value &symbols);
        value get_quote_depth(const value &symbols, Market market = Market::US);
        value get_stock_broker(utility::string_t symbol, int limit=40);
        value get_capital_distribution(utility::string_t symbol, Market market = Market::US);
        value get_capital_flow(utility::string_t symbol, Market market = Market::US, CapitalPeriod period = CapitalPeriod::DAY);
        value get_capital_flow(utility::string_t symbol, utility::string_t market, utility::string_t period = U("day"));

        /** Option quote related api **/
        value get_option_expiration(const value &symbols);
        value get_option_chain(const utility::string_t symbol, time_t expiry, value option_filter= value::null());
        value get_option_chain(const utility::string_t symbol, utility::string_t expiry, value option_filter= value::null());
        value get_option_brief(value identifiers);
        value get_option_brief(const utility::string_t identifier);
        value get_option_kline_value(value identifiers, time_t begin_time, time_t end_time=4070880000000);
        vector<Kline> get_option_kline(value identifiers, time_t begin_time, time_t end_time=4070880000000);
        value get_option_trade_tick(value identifiers);
        value get_warrant_real_time_quote(const value &symbols);
        value get_warrant_real_time_quote(const utility::string_t symbol);
        value get_warrant_filter(const utility::string_t symbol, int page_size = 100, int page = 0,
                                 utility::string_t sort_field_name = U(""), utility::string_t sort_dir = U("SortDir_Ascend"));


        /** Future quote related api **/
        value get_future_exchange(SecType sec_type=SecType::FUT);
        value get_future_contract_by_contract_code(utility::string_t contract_code);
        value get_future_contract_by_exchange_code(utility::string_t exchange_code);
        value get_future_contracts(utility::string_t type);
        value get_future_continuous_contracts(utility::string_t type);
        value get_future_current_contract(utility::string_t type);
        value get_future_kline(value contract_codes, utility::string_t period=U("day"), time_t begin_time=-1, time_t end_time=-1,
                               int limit=251, utility::string_t page_token=U(""));
        vector<Kline> get_future_kline(value contract_codes, BarPeriod period=BarPeriod::DAY, time_t begin_time=-1, time_t end_time=-1,
                               int limit=251, utility::string_t page_token=U(""));
        value get_future_real_time_quote_value(value contract_codes);
        vector<RealtimeQuote> get_future_real_time_quote(value contract_codes);
        value get_future_tick(utility::string_t contract_code, long begin_index=0, long end_index=100, int limit=1000);
        value get_future_trading_date(utility::string_t contract_code, utility::string_t trading_date);

        /** Financial related api **/
        value get_financial_daily();
        value get_financial_report();
        value get_corporate_action();

        value get_industry_list();
        value get_industry_stocks();
        value get_stock_industry();

        // Market scanner
        value get_market_scanner();

        // general
        value get_kline_quota(bool with_details = false);

    };
}
#endif //TIGERAPI_QUOTE_CLIENT_H
