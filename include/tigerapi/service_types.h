
#ifndef TIGERAPI_SERVICE_TYPES_H
#define TIGERAPI_SERVICE_TYPES_H


#include <string>

static utility::string_t  ORDER_NO = U("order_no");
static utility::string_t  PREVIEW_ORDER = U("preview_order");
static utility::string_t  PLACE_ORDER = U("place_order");
static utility::string_t  CANCEL_ORDER = U("cancel_order");
static utility::string_t  MODIFY_ORDER = U("modify_order");


// 账户/资产
static utility::string_t  ACCOUNTS = U("accounts");
static utility::string_t  ASSETS = U("assets");
static utility::string_t  PRIME_ASSETS = U("prime_assets");
static utility::string_t  POSITIONS = U("positions");
static utility::string_t  ORDERS = U("orders");
static utility::string_t  ACTIVE_ORDERS = U("active_orders");  // 待成交订单
static utility::string_t  INACTIVE_ORDERS = U("inactive_orders"); // 已撤销订单
static utility::string_t  FILLED_ORDERS = U("filled_orders");  // 已成交订单
static utility::string_t  ORDER_TRANSACTIONS = U("order_transactions");  // 订单成交记录
static utility::string_t  ANALYTICS_ASSET = U("analytics_asset");
static utility::string_t  USER_LICENSE = U("user_license");
static utility::string_t  ESTIMATE_TRADABLE_QUANTITY = U("estimate_tradable_quantity");
static utility::string_t  SEGMENT_FUND_HISTORY = U("segment_fund_history");
static utility::string_t  SEGMENT_FUND_AVAILABLE = U("segment_fund_available");
static utility::string_t  TRANSFER_SEGMENT_FUND = U("transfer_segment_fund");
static utility::string_t  PLACE_FOREX_ORDER = U("place_forex_order");

//合约
static utility::string_t CONTRACT = U("contract");
static utility::string_t CONTRACTS = U("contracts");
static utility::string_t QUOTE_CONTRACT = U("quote_contract");

        
//行情
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
static utility::string_t QUOTE_DEPTH = U("quote_depth");  // level2 深度行情
static utility::string_t GRAB_QUOTE_PERMISSION = U("grab_quote_permission");  // 抢占行情
static utility::string_t MARKET_SCANNER = U("market_scanner");  // 选股器
static utility::string_t GET_QUOTE_PERMISSION = U("get_quote_permission");
static utility::string_t TRADING_CALENDAR = U("trading_calendar");
static utility::string_t STOCK_BROKER = U("stock_broker");  // 港股股票实时经纪队列
static utility::string_t CAPITAL_DISTRIBUTION = U("capital_distribution");  // 股票当日资金分布
static utility::string_t CAPITAL_FLOW = U("capital_flow");  // 股票资金流向
static utility::string_t KLINE_QUOTA = U("kline_quota"); // 历史k线额度

// 期权行情
static utility::string_t OPTION_EXPIRATION = U("option_expiration");
static utility::string_t OPTION_CHAIN = U("option_chain");
static utility::string_t OPTION_BRIEF = U("option_brief");
static utility::string_t OPTION_KLINE = U("option_kline");
static utility::string_t OPTION_TRADE_TICK = U("option_trade_tick");
static utility::string_t WARRANT_FILTER = U("warrant_filter");
static utility::string_t WARRANT_REAL_TIME_QUOTE = U("warrant_real_time_quote");

// 期货行情
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

// 公司行动, 财务数据
static utility::string_t FINANCIAL_DAILY = U("financial_daily");
static utility::string_t FINANCIAL_REPORT = U("financial_report");
static utility::string_t CORPORATE_ACTION = U("corporate_action");

// 行业数据
static utility::string_t INDUSTRY_LIST = U("industry_list");
static utility::string_t INDUSTRY_STOCKS = U("industry_stocks");
static utility::string_t STOCK_INDUSTRY = U("stock_industry");
;

        
#endif //TIGERAPI_SERVICE_TYPES_H
