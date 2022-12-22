//
// Created by sukai on 2022/12/21.
//

#ifndef TIGERAPI_QUOTE_CLIENT_H
#define TIGERAPI_QUOTE_CLIENT_H

#include "tiger_client.h"
#include "enums.h"

namespace TIGERAPI {
    class QuoteClient : public TigerClient {
    public:
        QuoteClient();
        QuoteClient(struct Config &cf, bool is_grab_permission);
        value grab_quote_permission();
        value get_symbols(Market market = Market::ALL);
        value get_all_symbol_names(Market market = Market::ALL);
        value get_quote_stock_trade(value symbols);
        value get_brief(value symbols, bool include_hour_trading=false, bool include_ask_bid=false, QuoteRight right=QuoteRight::br);
        value get_stock_detail(value symbols);
        value get_timeline(value symbols, bool include_hour_trading=false, long begin_time=-1);
        value get_history_timeline(value symbols, string date, QuoteRight right=QuoteRight::br);
        value get_kline(value symbols, BarPeriod period=BarPeriod::DAY, long begin_time=-1, long end_time=-1,
                        QuoteRight right=QuoteRight::br, int limit=251, string page_token=nullptr);
        value get_trade_tick(value symbols, TradingSession trade_session=TradingSession::Regular, long begin_index=-1,
                             long end_index=-1, int limit=100);
        value get_quote_real_time(value symbols);
        value get_quote_delay(value symbols);
        value get_quote_shortable_stocks(value symbols);
        value get_quote_depth(value symbols, Market market = Market::US);
        value get_market_scanner();
        value get_quote_permission();
        value get_trading_calendar();
        value get_stock_broker();
        value get_capital_distribution();
        value get_capital_flow();

        value get_option_expiration(value symbols);
        value get_option_chain(value symbols, long expiry, value option_filter=value::null());
        value get_option_brief(value identifiers);
        value get_option_kline(value identifiers, long begin_time=-1, long end_time=-1);
        value get_option_trade_tick(value identifiers);

        value get_future_exchange(SecurityType sec_type=SecurityType::FUT);
        value get_future_contract_by_contract_code(string contract_code);
        value get_future_contract_by_exchange_code(string exchange_code);
        value get_future_contracts(string future_type);
        value get_future_continuous_contracts();
        value get_future_current_contract(string future_type);
        value get_future_kline(value identifiers, BarPeriod period=BarPeriod::DAY, long begin_time=-1, long end_time=-1,
                               int limit=251, string page_token=nullptr);
        value get_future_real_time_quote(value identifiers);
        value get_future_tick(string identifier, long begin_index=-1, long end_index=-1, int limit=1000);
        value get_future_trading_date(string contract_code, long trading_date);

        value get_financial_daily();
        value get_financial_report();
        value get_corporate_action();

        value get_industry_list();
        value get_industry_stocks();
        value get_stock_industry();

    };
}
#endif //TIGERAPI_QUOTE_CLIENT_H
