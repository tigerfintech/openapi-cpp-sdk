## 1.3.1 (2026-08-19)
### New
- `get_option_chain` response adds `markPrice`, `preMarkPrice`, `markTimestamp`, `midPrice`, `preMidPrice`, `midTimestamp` fields
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
- `Order` 新增 `cash_amount`、`combo_type`、`contract_legs`、`algo_params`、`oca_orders`、`attach_type` 及止盈/止损相关字段，`place_order`/`modify_order`/`preview_order` 均支持序列化这些字段

### Fixed
- `oca_brackets_order` 序列化时子订单（`oca_orders` 数组内的止盈单/止损单）缺少 `account` 字段，导致服务端返回 `biz param error(field 'account' cannot be empty)`；现在每个子订单都携带与外层订单一致的已解析账号（参考 Java SDK `buildOCABracketsOrder` 对每个子订单调用 `buildTradeOrderModel(account, ...)` 的方式），已用真实账号验证下单成功

### Changed
- `TigerClient` 复用持久化的 `cpprestsdk::http_client` 及其内置连接池，减少重复建连开销
- 全平台以 C++14 为最低构建基线，支持 C++17 用户工程直接编译和链接；依赖固定为 Boost 1.86.0、Protobuf 5.28.3 和 Abseil 20240722.0

### Notes
- OCA 组合单与附加止盈/止损单（`add_profit_taker_order` 等）不支持 `preview_order` 预览，服务端会返回 `OCA/ATTACHED order preview not supported`（已用真实账号验证），请直接使用 `place_order`

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
- 修复多线程并发写日志时的崩溃（`el::LogMessage::logger()` 空指针解引用）：easyloggingpp 的线程安全开关 `ELPP_THREAD_SAFE` 此前从未启用，token 自动刷新的后台线程与主线程同时写日志会触发该竞态
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
- `json_format()` 修复空字符串时 `length() - 1` 下溢 crash；添加 `reserve()` 消除 O(n²) 字符追加
- `identifiers_to_options()` 修复稀疏数组问题（插入索引与循环索引分离）
- `send_request()` 修复 `result[P_SIGN]` 缺少 `has_field` 检查导致的异常
- `build_sign_content()` 修复对非字符串字段调用 `as_string()` 的类型错误

### Refactor
- `TigerClient` 线程安全：`client_config` 改为 `const` 成员，构造时一次性初始化；`http_client` 提升为成员变量只初始化一次
- `ClientConfig` 的 `check()`、`check_account()`、`get_server_url()` 等方法补充 `const` 限定符
- `Utils::add_start_end()` / `get_sign()` / `fill_private_key_marker()` 参数改为 `const` 引用

### Build
- vendored `include/google/protobuf/` 升级为 protobuf 5.28.3
- CMakeLists 调整 include 顺序，确保系统 protobuf 头文件优先于 vendored 副本
- demo/CMakeLists.txt 修正 protobuf `find_path` 搜索路径顺序
- Windows 工程改为使用已安装的 Windows SDK `10.0.22621.0`，避免 `MSB8036` 找不到 `10.0.19041.0` 或 `10.0.22621.1`
- Windows vcpkg 依赖统一到 Protobuf 5.28.3，并为 Protobuf DLL 链接补充 `PROTOBUF_USE_DLLS` / `abseil_dll.lib`
- Visual Studio 工程禁用自动 vcpkg include/autolink，保留项目 include 优先、vcpkg include 放最后
- `scripts/build_windows.ps1` 保留 CMake 构建路径，默认使用 vcpkg manifest 中的 Protobuf 5.28.3/Abseil/OpenSSL/cpprestsdk，同时保留 `-ProtobufProvider Source` 源码安装路径，并为 demo CMake 补充 Windows 支持
- 启用 MSBuild/CL 并行编译（`/m`、`/MP`），并通过 `/FS`、`/Zi` 和 per-source PDB 设置解决 Debug 并行构建 PDB 竞争
- 修复 `openapi_cpp_test` Win32/x64 vcpkg 路径混用问题，Release-MD Win32/x64 demo 均可生成 `openapi_cpp_test.exe`

## 1.2.2 (2026-05-07)
### New
- `OrderStatusData` 推送消息补齐字段：`timeInForce` (43)、`updateTime` (44, 订单信息更新时间戳 ms)、`latestTime` (45, 订单状态更新时间戳 ms)

## 1.2.1 (2026-04-13)
### New
- 同时支持 PKCS#8（`private_key_pk8`）和 PKCS#1（`private_key_pk1`）私钥格式，优先使用 PKCS#8
- `get_props_path()` 兼容输入完整文件路径（直接使用）或目录路径（自动追加默认文件名）
- Demo 默认从 `~/.tigeropen/` 读取配置文件

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
