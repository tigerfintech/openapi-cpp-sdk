## 1.4.1 (2026-11-02)
### Added
- US option quotes and option-chain legs expose cumulative volume/amount and extended-session quotes for GTH/CURB.
- Protobuf option callbacks read GTH/CURB prices and session volume/amount from existing fields with PreMarket/AfterHours tags; cumulative totals remain independent. The unpublished nested push object is removed. HTTP snapshots and option-chain objects remain available.
- Option minute-bar requests can include GTH/CURB through the optional with_hour_trading parameter; existing calls retain the server default.

## 1.4.0 (2026-09-22)
### New
- 资产与持仓支持指定行情口径 `AssetQuoteType`（`ETH` / `RTH` / `OVERNIGHT`），用于夜盘时段按夜盘价估值。仅美股有夜盘时段。
  - 拉取侧：`get_prime_asset`、`get_prime_portfolio`、`get_asset`、`get_positions`、`get_position_list`、`get_aggregate_assets` 各新增一个末位参数为 `AssetQuoteType` 的重载。
  - 订阅侧：`IPushClient` 新增 `subscribe_asset(account, AssetQuoteType)` 与 `subscribe_position(account, AssetQuoteType)`；订阅报文 `Subscribe` 新增字段 5 `assetQuoteType`。
  - 全部为新增重载，**已有方法签名未改动**：不传口径时请求内容与旧版本完全一致，服务端按 `ETH` 处理。新的纯虚函数追加在 `IPushClient` 类体末尾，只在 vtable 尾部新增槽位。

## 1.3.2 (2026-08-27)
### New
- `RealtimeQuote` 新增 `amount` 字段，支持股票和数字货币实时行情成交额；`get_quote_real_time` 新增 `SecType` overload 用于数字货币实时行情。

## 1.3.1 (2026-08-19)
### New
- HTTP K-line and current-timeline requests support cryptocurrency symbols through `SecType::CC`; K-line pagination propagates `sec_type`, and typed `KlineItem` results expose fractional `volume_decimal` parsed from `volumeDecimal`. Existing overloads remain available and omit `sec_type` as before.
- `get_option_chain` response adds `markPrice`, `preMarkPrice`, `markTimestamp`, `midPrice`, `preMidPrice`, `midTimestamp` fields

### Breaking
- ABI change: adding `KlineItem::volume_decimal` changes the exported structure layout. Rebuild applications and libraries against the updated SDK.
- `TradingSession::OverNight` and overnight-session support for `get_kline`, `get_quote_depth`, and the existing enum overload of `get_trade_tick`
### Fixed
- `get_quote_overnight` now uses its required API version 1.0

## 1.3.0 (2026-07-24)
### New
- `OrderUtil::amount_order` — 按金额下市价单（如基金申购），不指定股数
- `OrderUtil::multi_leg_order` — 多腿组合单（期权价差等），支持 `ContractLeg` 描述每条腿
- `OrderUtil::twap_order` / `vwap_order` — TWAP/VWAP 算法单，按时间区间拆单执行
- `OrderUtil::oca_brackets_order` — OCA 止盈止损组合单，任一单成交后自动取消另一单
- `OrderUtil::add_profit_taker_order` / `add_stop_loss_order` / `add_stop_loss_limit_order` / `add_stop_loss_trail_order` / `add_brackets_order` — 给已有订单挂载附加止盈/止损单
- `Order` 补充组合单、算法单、止盈止损相关字段，`place_order`/`modify_order`/`preview_order` 均已支持

### Fixed
- 修复 `oca_brackets_order` 提交时子订单缺少账号字段导致下单失败的问题

### Changed
- 优化 HTTP 请求性能，减少重复建连开销
- 全平台以 C++14 为最低构建基线，支持 C++17 用户工程直接编译和链接；依赖固定为 Boost 1.86.0、Protobuf 5.28.3 和 Abseil 20240722.0

### Notes
- OCA 组合单与附加止盈/止损单（`add_profit_taker_order` 等）不支持 `preview_order` 预览，请直接使用 `place_order`

## 1.2.6 (2026-07-24)
### New
- `OrderUtil::iceberg_order` — 冰山单构造，支持基础参数（display_size）和完整参数（min_display_size、check_intervals、price_type、start_time、end_time）两种重载
- `TradeClient::place_order` / `modify_order` 支持冰山单字段的下单与改单

### Fixed
- 修复订单响应解析中 `totalQuantityScale` / `filledQuantityScale` 字段名大小写错误，导致碎股等场景下数量精度字段（`total_quantity_scale`/`filled_quantity_scale`）始终解析为默认值 0

## 1.2.5 (2026-07-23)
### New
- Token 自动刷新：`ClientConfig::start_token_refresh` / `stop_token_refresh`，支持自定义刷新间隔和本地文件持久化
- `tiger_openapi_config.properties` 新增 `server_url` / `server_public_key` 字段，用于指定自定义网关地址和响应签名校验公钥

### Fixed
- 修复多线程并发写日志时可能崩溃的问题
- 修复请求异常被重新抛出为通用 `std::exception`、丢失原始错误信息的问题

## 1.2.4 (2026-07-20)
### New
- `get_corporate_symbol_change(symbols, market, begin_date, end_date)` — 股票代码变更查询
- `get_corporate_delisting(symbols, market, begin_date, end_date)` — 退市事件查询
- `get_corporate_ipo(symbols, market, begin_date, end_date)` — 新股上市查询

### Breaking
- `get_corporate_action()` 签名变更：原接口无参数，现在需要传入 `symbols`（`value`）、`market`、`action_type`、`begin_date`、`end_date`。
  现有调用方需更新为：
  ```cpp
  // before
  client.get_corporate_action();
  // after
  value symbols = value::array();
  symbols[0] = value::string(U("AAPL"));
  client.get_corporate_action(symbols, U("US"), U("dividend"), U("2024-01-01"), U("2024-12-31"));
  ```

## 1.2.3 (2026-06-15)
### New
- 新增期权提前行权接口：`submit_option_exercise` 提交行权/放弃申请、`cancel_option_exercise` 撤销申请、`check_option_exercise` 行权检验（预估持仓变化）、`get_option_exercise_records` 分页查询行权记录、`get_option_exercise_positions` 查询可行权持仓

### Fix
- 修复 `json_format()` 处理空字符串时可能崩溃的问题
- 修复 `identifiers_to_options()` 稀疏数组场景下解析结果错误的问题
- 修复 `send_request()` 在特定响应场景下抛出异常的问题
- 修复 `build_sign_content()` 对非字符串字段处理时的类型错误

## 1.2.2 (2026-05-07)
### New
- `OrderStatusData` 推送消息补充生效时间类型、订单信息更新时间、订单状态更新时间字段

## 1.2.1 (2026-04-13)
### New
- 同时支持 PKCS#8（`private_key_pk8`）和 PKCS#1（`private_key_pk1`）私钥格式，优先使用 PKCS#8
- `get_props_path()` 兼容输入完整文件路径（直接使用）或目录路径（自动追加默认文件名）
- Demo 默认从 `~/.tigeropen/` 读取配置文件

## 1.2.0

### New
- 新增期权、期货行情查询接口，覆盖标的列表、深度行情、分时数据、期权分析和期货历史主力合约
- 新增基金行情能力，支持基金标的列表、合约、实时行情和历史行情查询
- 新增财经数据接口，支持货币和汇率查询
- 行情查询补充股票基本面、交易排行、隔夜行情、券商持仓、市场扫描标签等接口，并为多个现有查询接口补充语言、证券类型等筛选参数
- 交易能力新增下单预览、跨账户资产汇总、资金划转历史与明细查询、账户间持仓划转及划转记录查询
- 长连接推送新增加密货币行情订阅、心跳回调，并为热门排行订阅新增指标参数
- 新增加密货币相关的数据类型和服务类型常量，配合上述新接口使用

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
