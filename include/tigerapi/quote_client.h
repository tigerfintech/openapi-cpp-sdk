//
// Created by sukai on 2022/12/21.
//

#ifndef TIGERAPI_QUOTE_CLIENT_H
#define TIGERAPI_QUOTE_CLIENT_H

#include "tiger_client.h"
#include "client_config.h"
#include "enums.h"
#include "../include/tigerapi/service_types.h"

namespace TIGER_API {
    class QuoteClient : public TigerClient {
    public:
        QuoteClient();
        QuoteClient(const ClientConfig &cf, bool is_grab_permission = true);
        value grab_quote_permission();
        value get_quote_permission();

        /** 股票行情 Stock quote related api **/

        value get_symbols(Market market = Market::ALL);
        value get_all_symbol_names(Market market = Market::ALL);
        value get_all_symbol_names(string market="ALL");
        value get_quote_stock_trade(const value &symbols);
        value get_brief(const value &symbols, bool include_hour_trading=false, bool include_ask_bid=false, QuoteRight right=QuoteRight::br);
        value get_stock_detail(const value &symbols);
        value get_timeline(const value &symbols, bool include_hour_trading=false, long begin_time=-1);
        value get_history_timeline(const value &symbols, string date, QuoteRight right=QuoteRight::br);
        value get_kline(const value &symbols, BarPeriod period=BarPeriod::DAY, long begin_time=-1, long end_time=-1,
                        QuoteRight right=QuoteRight::br, int limit=251, string page_token="");
        value get_kline(const value &symbols, string period, long begin_time=-1, long end_time=-1,
                        string right="br", int limit=251, string page_token="");
        value get_trade_tick(const value &symbols, TradingSession trade_session=TradingSession::Regular, long begin_index=-1,
                             long end_index=-1, int limit=100);
        value get_quote_real_time(const value &symbols);
        value get_quote_delay(const value &symbols);
        value get_quote_shortable_stocks(const value &symbols);
        value get_quote_depth(const value &symbols, Market market = Market::US);
        value get_trading_calendar(Market market, string begin_date, string end_date);
        value get_stock_broker(string symbol, int limit=40);
        value get_capital_distribution(string symbol, Market market = Market::US);
        value get_capital_flow(string symbol, Market market = Market::US, CapitalPeriod period = CapitalPeriod::DAY);

        /** 期权行情 Option quote related api **/
        value get_option_expiration(const value &symbols);
        value get_option_chain(const value &symbols, long expiry, value option_filter=value::null());
        value get_option_brief(value identifiers);
        value get_option_kline(value identifiers, long begin_time=-1, long end_time=-1);
        value get_option_trade_tick(value identifiers);

        /** 期货行情 Future quote related api **/
        value get_future_exchange(SecType sec_type=SecType::FUT);
        value get_future_contract_by_contract_code(string contract_code);
        value get_future_contract_by_exchange_code(string exchange_code);
        value get_future_contracts(string type);
        value get_future_continuous_contracts(string type);
        value get_future_current_contract(string type);
        value get_future_kline(value contract_codes, BarPeriod period=BarPeriod::DAY, long begin_time=-1, long end_time=-1,
                               int limit=251, string page_token=nullptr);
        value get_future_real_time_quote(value contract_codes);
        value get_future_tick(string contract_code, long begin_index=-1, long end_index=-1, int limit=1000);
        value get_future_trading_date(string contract_code, string trading_date);

        /** 财务 **/
        value get_financial_daily();
        value get_financial_report();
        value get_corporate_action();

        value get_industry_list();
        value get_industry_stocks();
        value get_stock_industry();

        // 选股
        value get_market_scanner();
    };
}
#endif //TIGERAPI_QUOTE_CLIENT_H
