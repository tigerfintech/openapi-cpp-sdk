
#ifndef TIGERAPI_SERVICE_TYPES_H
#define TIGERAPI_SERVICE_TYPES_H


#include <string>

static utility::string_t  ORDER_NO = U("order_no");
static utility::string_t  PREVIEW_ORDER = U("preview_order");
static utility::string_t  PLACE_ORDER = U("place_order");
static utility::string_t  CANCEL_ORDER = U("cancel_order");
static utility::string_t  MODIFY_ORDER = U("modify_order");


// Account/Asset
static utility::string_t  ACCOUNTS = U("accounts");
static utility::string_t  ASSETS = U("assets");
static utility::string_t  PRIME_ASSETS = U("prime_assets");
static utility::string_t  POSITIONS = U("positions");
static utility::string_t  ORDERS = U("orders");
static utility::string_t  ACTIVE_ORDERS = U("active_orders");  // pending orders
static utility::string_t  INACTIVE_ORDERS = U("inactive_orders"); // cancelled orders
static utility::string_t  FILLED_ORDERS = U("filled_orders");  // filled orders
static utility::string_t  ORDER_TRANSACTIONS = U("order_transactions");  // order transaction records
static utility::string_t  ANALYTICS_ASSET = U("analytics_asset");
static utility::string_t  USER_LICENSE = U("user_license");
static utility::string_t  ESTIMATE_TRADABLE_QUANTITY = U("estimate_tradable_quantity");
static utility::string_t  SEGMENT_FUND_HISTORY = U("segment_fund_history");
static utility::string_t  SEGMENT_FUND_AVAILABLE = U("segment_fund_available");
static utility::string_t  TRANSFER_SEGMENT_FUND = U("transfer_segment_fund");
static utility::string_t  CANCEL_SEGMENT_FUND = U("cancel_segment_fund");
static utility::string_t  TRANSFER_FUND = U("transfer_fund");
static utility::string_t  AGGREGATE_ASSETS = U("aggregate_assets");
static utility::string_t  FUND_DETAILS = U("fund_details");
static utility::string_t  POSITION_TRANSFER = U("position_transfer");
static utility::string_t  POSITION_TRANSFER_RECORDS = U("position_transfer_records");
static utility::string_t  POSITION_TRANSFER_DETAIL = U("position_transfer_detail");
static utility::string_t  POSITION_TRANSFER_EXTERNAL_RECORDS = U("position_transfer_external_records");
static utility::string_t  PLACE_FOREX_ORDER = U("place_forex_order");

// Contract
static utility::string_t CONTRACT = U("contract");
static utility::string_t CONTRACTS = U("contracts");
static utility::string_t QUOTE_CONTRACT = U("quote_contract");

        
// Market
static utility::string_t MARKET_STATE = U("market_state");
static utility::string_t ALL_SYMBOLS = U("all_symbols");
static utility::string_t ALL_SYMBOL_NAMES = U("all_symbol_names");
static utility::string_t BRIEF = U("brief");
static utility::string_t STOCK_DETAIL = U("stock_detail");
static utility::string_t TIMELINE = U("timeline");
static utility::string_t HISTORY_TIMELINE = U("history_timeline");
static utility::string_t KLINE = U("kline");
static utility::string_t TRADE_TICK = U("trade_tick");
static utility::string_t QUOTE_REAL_TIME = U("quote_real_time");
static utility::string_t QUOTE_DELAY = U("quote_delay");
static utility::string_t QUOTE_SHORTABLE_STOCKS = U("quote_shortable_stocks");
static utility::string_t QUOTE_STOCK_TRADE = U("quote_stock_trade");
static utility::string_t QUOTE_DEPTH = U("quote_depth");  //  level2 quote
static utility::string_t GRAB_QUOTE_PERMISSION = U("grab_quote_permission");  // grab quote
static utility::string_t MARKET_SCANNER = U("market_scanner");  // stock screener
static utility::string_t MARKET_SCANNER_TAGS = U("market_scanner_tags");  // stock screener tags
static utility::string_t GET_QUOTE_PERMISSION = U("get_quote_permission");
static utility::string_t TRADING_CALENDAR = U("trading_calendar");
static utility::string_t STOCK_BROKER = U("stock_broker");  // HK stock real-time broker queue
static utility::string_t STOCK_FUNDAMENTAL = U("stock_fundamental");  // stock fundamental data
static utility::string_t TRADE_RANK = U("trade_rank");  // trade rank
static utility::string_t QUOTE_OVERNIGHT = U("quote_overnight");  // overnight quote
static utility::string_t BROKER_HOLD = U("broker_hold");  // broker hold data
static utility::string_t CAPITAL_DISTRIBUTION = U("capital_distribution");  // stock capital distribution
static utility::string_t CAPITAL_FLOW = U("capital_flow");  // stock capital flow
static utility::string_t KLINE_QUOTA = U("kline_quota"); // historical k-line quota

// Option
static utility::string_t OPTION_EXPIRATION = U("option_expiration");
static utility::string_t OPTION_CHAIN = U("option_chain");
static utility::string_t OPTION_BRIEF = U("option_brief");
static utility::string_t OPTION_KLINE = U("option_kline");
static utility::string_t OPTION_TRADE_TICK = U("option_trade_tick");
static utility::string_t OPTION_DEPTH = U("option_depth");  // option depth quote
static utility::string_t ALL_HK_OPTION_SYMBOLS = U("all_hk_option_symbols");  // all HK option symbols
static utility::string_t OPTION_TIMELINE = U("option_timeline");  // option timeline
static utility::string_t OPTION_ANALYSIS = U("option_analysis");  // option analysis
static utility::string_t WARRANT_FILTER = U("warrant_filter");
static utility::string_t WARRANT_REAL_TIME_QUOTE = U("warrant_real_time_quote");

// Future
static utility::string_t FUTURE_EXCHANGE = U("future_exchange");
static utility::string_t FUTURE_CONTRACT_BY_CONTRACT_CODE = U("future_contract_by_contract_code");
static utility::string_t FUTURE_CONTRACT_BY_EXCHANGE_CODE = U("future_contract_by_exchange_code");
static utility::string_t FUTURE_CONTRACTS = U("future_contracts");
static utility::string_t FUTURE_CONTINUOUS_CONTRACTS = U("future_continuous_contracts");
static utility::string_t FUTURE_CURRENT_CONTRACT = U("future_current_contract");
static utility::string_t FUTURE_KLINE = U("future_kline");
static utility::string_t FUTURE_REAL_TIME_QUOTE = U("future_real_time_quote");
static utility::string_t FUTURE_TICK = U("future_tick");
static utility::string_t FUTURE_TRADING_DATE = U("future_trading_date");
static utility::string_t FUTURE_DEPTH = U("future_depth");  // future depth quote
static utility::string_t FUTURE_HISTORY_MAIN_CONTRACT = U("future_history_main_contract");  // future history main contract

// Fund
static utility::string_t FUND_ALL_SYMBOLS = U("fund_all_symbols");  // all fund symbols
static utility::string_t FUND_CONTRACTS = U("fund_contracts");  // fund contracts
static utility::string_t FUND_QUOTE = U("fund_quote");  // fund quote
static utility::string_t FUND_HISTORY_QUOTE = U("fund_history_quote");  // fund history quote

// Financial
static utility::string_t FINANCIAL_DAILY = U("financial_daily");
static utility::string_t FINANCIAL_REPORT = U("financial_report");
static utility::string_t CORPORATE_ACTION = U("corporate_action");
static utility::string_t FINANCIAL_CURRENCY = U("financial_currency");  // financial currency
static utility::string_t FINANCIAL_EXCHANGE_RATE = U("financial_exchange_rate");  // financial exchange rate

// Industry
static utility::string_t INDUSTRY_LIST = U("industry_list");
static utility::string_t INDUSTRY_STOCKS = U("industry_stocks");
static utility::string_t STOCK_INDUSTRY = U("stock_industry");


#endif //TIGERAPI_SERVICE_TYPES_H
