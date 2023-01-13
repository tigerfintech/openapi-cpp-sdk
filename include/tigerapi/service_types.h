
#ifndef TIGERAPI_SERVICE_TYPES_H
#define TIGERAPI_SERVICE_TYPES_H


#include <string>

static std::string  ORDER_NO = U("order_no");
static std::string  PREVIEW_ORDER = U("preview_order");
static std::string  PLACE_ORDER = U("place_order");
static std::string  CANCEL_ORDER = U("cancel_order");
static std::string  MODIFY_ORDER = U("modify_order");


// 账户/资产
static std::string  ACCOUNTS = U("accounts");
static std::string  ASSETS = U("assets");
static std::string  PRIME_ASSETS = U("prime_assets");
static std::string  POSITIONS = U("positions");
static std::string  ORDERS = U("orders");
static std::string  ACTIVE_ORDERS = U("active_orders");  // 待成交订单
static std::string  INACTIVE_ORDERS = U("inactive_orders"); // 已撤销订单
static std::string  FILLED_ORDERS = U("filled_orders");  // 已成交订单
static std::string  ORDER_TRANSACTIONS = U("order_transactions");  // 订单成交记录
static std::string  ANALYTICS_ASSET = U("analytics_asset");
static std::string  USER_LICENSE = U("user_license");

//合约
static std::string CONTRACT = U("contract");
static std::string CONTRACTS = U("contracts");
static std::string QUOTE_CONTRACT = U("quote_contract");

        
//行情
static std::string MARKET_STATE = U("market_state");
static std::string ALL_SYMBOLS = U("all_symbols");
static std::string ALL_SYMBOL_NAMES = U("all_symbol_names");
static std::string BRIEF = U("brief");
static std::string STOCK_DETAIL = U("stock_detail");
static std::string TIMELINE = U("timeline");
static std::string HISTORY_TIMELINE = U("history_timeline");
static std::string KLINE = U("kline");
static std::string TRADE_TICK = U("trade_tick");
static std::string QUOTE_REAL_TIME = U("quote_real_time");
static std::string QUOTE_DELAY = U("quote_delay");
static std::string QUOTE_SHORTABLE_STOCKS = U("quote_shortable_stocks");
static std::string QUOTE_STOCK_TRADE = U("quote_stock_trade");
static std::string QUOTE_DEPTH = U("quote_depth");  // level2 深度行情
static std::string GRAB_QUOTE_PERMISSION = U("grab_quote_permission");  // 抢占行情
static std::string MARKET_SCANNER = U("market_scanner");  // 选股器
static std::string GET_QUOTE_PERMISSION = U("get_quote_permission");
static std::string TRADING_CALENDAR = U("trading_calendar");
static std::string STOCK_BROKER = U("stock_broker");  // 港股股票实时经纪队列
static std::string CAPITAL_DISTRIBUTION = U("capital_distribution");  // 股票当日资金分布
static std::string CAPITAL_FLOW = U("capital_flow");  // 股票资金流向

// 期权行情
static std::string OPTION_EXPIRATION = U("option_expiration");
static std::string OPTION_CHAIN = U("option_chain");
static std::string OPTION_BRIEF = U("option_brief");
static std::string OPTION_KLINE = U("option_kline");
static std::string OPTION_TRADE_TICK = U("option_trade_tick");

// 期货行情
static std::string FUTURE_EXCHANGE = U("future_exchange");
static std::string FUTURE_CONTRACT_BY_CONTRACT_CODE = U("future_contract_by_contract_code");
static std::string FUTURE_CONTRACT_BY_EXCHANGE_CODE = U("future_contract_by_exchange_code");
static std::string FUTURE_CONTRACTS = U("future_contracts");
static std::string FUTURE_CONTINUOUS_CONTRACTS = U("future_continuous_contracts");
static std::string FUTURE_CURRENT_CONTRACT = U("future_current_contract");
static std::string FUTURE_KLINE = U("future_kline");
static std::string FUTURE_REAL_TIME_QUOTE = U("future_real_time_quote");
static std::string FUTURE_TICK = U("future_tick");
static std::string FUTURE_TRADING_DATE = U("future_trading_date");

// 公司行动, 财务数据
static std::string FINANCIAL_DAILY = U("financial_daily");
static std::string FINANCIAL_REPORT = U("financial_report");
static std::string CORPORATE_ACTION = U("corporate_action");

// 行业数据
static std::string INDUSTRY_LIST = U("industry_list");
static std::string INDUSTRY_STOCKS = U("industry_stocks");
static std::string STOCK_INDUSTRY = U("stock_industry");
;

        
#endif //TIGERAPI_SERVICE_TYPES_H
