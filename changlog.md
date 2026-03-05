## 1.2.0
### New 
  1. include/tigerapi/service_types.h — 新增 25 个服务类型常量                          
  分类: Quote 行情                                                                      
  新增常量: MARKET_SCANNER_TAGS, STOCK_FUNDAMENTAL, TRADE_RANK, QUOTE_OVERNIGHT,        
  BROKER_HOLD                                                                           
  ────────────────────────────────────────
  分类: Option 期权                                                                     
  新增常量: OPTION_DEPTH, ALL_HK_OPTION_SYMBOLS, OPTION_TIMELINE, OPTION_ANALYSIS       
  ────────────────────────────────────────                                              
  分类: Future 期货
  新增常量: FUTURE_DEPTH, FUTURE_HISTORY_MAIN_CONTRACT
  ────────────────────────────────────────
  分类: Fund 基金
  新增常量: FUND_ALL_SYMBOLS, FUND_CONTRACTS, FUND_QUOTE, FUND_HISTORY_QUOTE
  ────────────────────────────────────────
  分类: Financial 财务
  新增常量: FINANCIAL_CURRENCY, FINANCIAL_EXCHANGE_RATE
  ────────────────────────────────────────
  分类: Trade 交易
  新增常量: CANCEL_SEGMENT_FUND, TRANSFER_FUND, AGGREGATE_ASSETS, FUND_DETAILS,
    POSITION_TRANSFER, POSITION_TRANSFER_RECORDS, POSITION_TRANSFER_DETAIL,
    POSITION_TRANSFER_EXTERNAL_RECORDS
  2. 已有接口补充缺失参数

  - get_capital_flow — 新增 begin_time, end_time, limit
  - get_stock_broker — 新增 lang, sec_type
  - get_capital_distribution — 新增 lang

  3. QuoteClient 新增 17 个方法

  期权: get_option_symbols, get_option_depth, get_option_timeline, get_option_analysis
  期货: get_future_depth, get_future_history_main_contract
  基金: get_fund_symbols, get_fund_contracts, get_fund_quote, get_fund_history_quote
  其他: get_stock_fundamental, get_trade_rank, get_quote_overnight, get_broker_hold,
  get_market_scanner_tags, get_financial_currency, get_financial_exchange_rate

  4. TradeClient 新增 9 个方法

  preview_order, get_aggregate_assets, cancel_segment_fund, get_funding_history,
  get_fund_details, transfer_position, get_position_transfer_records,
  get_position_transfer_detail, get_position_transfer_external_records


## 1.1.0 (2025-08-08)
### New
- 支持 TBUS 配置

## 1.0.4 (2025-05-12)
### Breaking
- 长连接订阅请求由 bool 改为 unsigned int，表示订阅请求的 id

## 1.0.3 (2025-02-20)
- properties 配置支持；
- log配置优化；
- kickout回调

## 1.0.2 (2025-01-15)
### Fix
- 长连接DNS解析问题

## 1.0.1 (2025-01-02)
### Modify
- 中文注释改为英文注释

## 1.0.0 (2024-11-28)
### New
- 支持长连接推送

## 0.1.6 (2024-03-08)
### New
- 新增根据ticksize调整下单价格的工具 PriceUtil


## 0.1.5 (2024-02-04)
### Modify
- 交易接口支持 `secret_key`


## 0.1.0 (2023-02-15)
### New
- Beta版本，支持交易、行情接口

