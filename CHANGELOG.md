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
