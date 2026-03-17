## 1.2.0

### QuoteClient - 参数补充 (2025-03-05 更新)
- `get_capital_flow` - 新增 `begin_time`、`end_time`、`limit` 参数
- `get_stock_broker` - 新增 `lang`、`sec_type` 参数
- `get_capital_distribution` - 新增 `lang` 参数
- `get_stock_detail` - 新增 `lang`、`sec_type` 参数
- `get_industry_list` - 新增 `industry_level`、`lang` 参数 + 完整实现
- `get_industry_stocks` - 新增 `industry_id`、`market`、`lang` 参数 + 完整实现
- `get_stock_industry` - 新增 `symbol`、`market`、`sec_type`、`lang` 参数 + 完整实现

### QuoteClient - 新增接口
#### 期权
- `get_option_symbols` - 获取所有港股期权标的
- `get_option_depth` - 获取期权深度行情数据
- `get_option_timeline` - 获取期权分时数据
- `get_option_analysis` - 获取期权分析数据

#### 期货
- `get_future_depth` - 获取期货深度行情数据
- `get_future_history_main_contract` - 获取期货历史主力合约

#### 基金（新增类别）
- `get_fund_symbols` - 获取所有基金标的
- `get_fund_contracts` - 获取基金合约
- `get_fund_quote` - 获取基金实时行情
- `get_fund_history_quote` - 获取基金历史行情

#### 财经
- `get_financial_currency` - 获取财经货币数据
- `get_financial_exchange_rate` - 获取财经汇率数据

#### 其他
- `get_stock_fundamental` - 获取股票基本面数据
- `get_trade_rank` - 获取交易排行数据
- `get_quote_overnight` - 获取隔夜行情数据
- `get_broker_hold` - 获取券商持仓数据
- `get_market_scanner_tags` - 获取市场扫描标签

### TradeClient - 新增接口
- `preview_order` - 下单前预览订单
- `get_aggregate_assets` - 获取跨账户汇总资产
- `cancel_segment_fund` - 取消分段资金划转
- `get_funding_history` - 获取资金划转历史
- `get_fund_details` - 获取资金明细记录
- `transfer_position` - 账户间持仓划转
- `get_position_transfer_records` - 获取持仓划转记录
- `get_position_transfer_detail` - 获取持仓划转详情
- `get_position_transfer_external_records` - 获取外部持仓划转记录

### PushClient - 新增功能
- `subscribe_cc` / `unsubscribe_cc` - 加密货币行情订阅
- `set_heartbeat_callback` - 心跳事件回调支持
- `subscribe_stock_top` / `unsubscribe_stock_top` - 新增 `indicators` 参数
- `subscribe_option_top` / `unsubscribe_option_top` - 新增 `indicators` 参数

### Protobuf
- 在 `SocketCommon.proto` DataType 枚举中新增 `Cc = 13`，支持加密货币

### 服务类型
- 新增约 25 个服务类型常量：`MARKET_SCANNER_TAGS`、`STOCK_FUNDAMENTAL`、`TRADE_RANK`、`QUOTE_OVERNIGHT`、`BROKER_HOLD`、`OPTION_DEPTH`、`ALL_HK_OPTION_SYMBOLS`、`OPTION_TIMELINE`、`OPTION_ANALYSIS`、`FUTURE_DEPTH`、`FUTURE_HISTORY_MAIN_CONTRACT`、`FUND_ALL_SYMBOLS`、`FUND_CONTRACTS`、`FUND_QUOTE`、`FUND_HISTORY_QUOTE`、`FINANCIAL_CURRENCY`、`FINANCIAL_EXCHANGE_RATE`、`CANCEL_SEGMENT_FUND`、`TRANSFER_FUND`、`AGGREGATE_ASSETS`、`FUND_DETAILS`、`POSITION_TRANSFER`、`POSITION_TRANSFER_RECORDS`、`POSITION_TRANSFER_DETAIL`、`POSITION_TRANSFER_EXTERNAL_RECORDS`


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
