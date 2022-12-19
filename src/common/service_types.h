
#ifndef TIGERAPI_SERVICE_TYPES_H
#define TIGERAPI_SERVICE_TYPES_H


#include <string>

static std::string  ORDER_NO = "order_no";
static std::string  PREVIEW_ORDER = "preview_order";
static std::string  PLACE_ORDER = "place_order";
static std::string  CANCEL_ORDER = "cancel_order";
static std::string  MODIFY_ORDER = "modify_order";


// 账户/资产
static std::string  ACCOUNTS = "accounts";
static std::string  ASSETS = "assets";
static std::string  PRIME_ASSETS = "prime_assets";
static std::string  POSITIONS = "positions";
static std::string  ORDERS = "orders";
static std::string  ACTIVE_ORDERS = "active_orders";  // 待成交订单
static std::string  INACTIVE_ORDERS = "inactive_orders"; // 已撤销订单
static std::string  FILLED_ORDERS = "filled_orders";  // 已成交订单
static std::string  ORDER_TRANSACTIONS = "order_transactions";  // 订单成交记录
static std::string  ANALYTICS_ASSET = "analytics_asset";
static std::string  USER_LICENSE = "user_license";

//合约
static std::string CONTRACT = "contract";
static std::string CONTRACTS = "contracts";
static std::string QUOTE_CONTRACT = "quote_contract";

        
//行情
static std::string MARKET_STATE = "market_state";
static std::string ALL_SYMBOLS = "all_symbols";
static std::string ALL_SYMBOL_NAMES = "all_symbol_names";
static std::string BRIEF = "brief";
static std::string STOCK_DETAIL = "stock_detail";
static std::string TIMELINE = "timeline";
static std::string HISTORY_TIMELINE = "history_timeline";
static std::string KLINE = "kline";
static std::string TRADE_TICK = "trade_tick";
static std::string QUOTE_REAL_TIME = "quote_real_time";
static std::string QUOTE_DELAY = "quote_delay";
static std::string QUOTE_SHORTABLE_STOCKS = "quote_shortable_stocks";
static std::string QUOTE_STOCK_TRADE = "quote_stock_trade";
static std::string QUOTE_DEPTH = "quote_depth";  // level2 深度行情
static std::string GRAB_QUOTE_PERMISSION = "grab_quote_permission";  // 抢占行情
static std::string MARKET_SCANNER = "market_scanner";  // 选股器
static std::string GET_QUOTE_PERMISSION = "get_quote_permission";
static std::string TRADING_CALENDAR = "trading_calendar";
static std::string STOCK_BROKER = "stock_broker";  // 港股股票实时经纪队列
static std::string CAPITAL_DISTRIBUTION = "capital_distribution";  // 股票当日资金分布
static std::string CAPITAL_FLOW = "capital_flow";  // 股票资金流向

// 期权行情
static std::string OPTION_EXPIRATION = "option_expiration";
static std::string OPTION_CHAIN = "option_chain";
static std::string OPTION_BRIEF = "option_brief";
static std::string OPTION_KLINE = "option_kline";
static std::string OPTION_TRADE_TICK = "option_trade_tick";

// 期货行情
static std::string FUTURE_EXCHANGE = "future_exchange";
static std::string FUTURE_CONTRACT_BY_CONTRACT_CODE = "future_contract_by_contract_code";
static std::string FUTURE_CONTRACT_BY_EXCHANGE_CODE = "future_contract_by_exchange_code";
static std::string FUTURE_CONTRACTS = "future_contracts";
static std::string FUTURE_CONTINUOUS_CONTRACTS = "future_continuous_contracts";
static std::string FUTURE_CURRENT_CONTRACT = "future_current_contract";
static std::string FUTURE_KLINE = "future_kline";
static std::string FUTURE_REAL_TIME_QUOTE = "future_real_time_quote";
static std::string FUTURE_TICK = "future_tick";
static std::string FUTURE_TRADING_DATE = "future_trading_date";

// 公司行动, 财务数据
static std::string FINANCIAL_DAILY = "financial_daily";
static std::string FINANCIAL_REPORT = "financial_report";
static std::string CORPORATE_ACTION = "corporate_action";

// 行业数据
static std::string INDUSTRY_LIST = "industry_list";
static std::string INDUSTRY_STOCKS = "industry_stocks";
static std::string STOCK_INDUSTRY = "stock_industry";
;

        
#endif //TIGERAPI_SERVICE_TYPES_H
