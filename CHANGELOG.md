## 1.2.3 (2026-06-10)
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

## 1.2.2 (2026-05-07)
### New
- `OrderStatusData` 推送消息补齐字段：`timeInForce` (43)、`updateTime` (44, 订单信息更新时间戳 ms)、`latestTime` (45, 订单状态更新时间戳 ms)

## 1.2.1 (2026-04-13)
### New
- 同时支持 PKCS#8（`private_key_pk8`）和 PKCS#1（`private_key_pk1`）私钥格式，优先使用 PKCS#8
- `get_props_path()` 兼容输入完整文件路径（直接使用）或目录路径（自动追加默认文件名）
- Demo 默认从 `~/.tigeropen/` 读取配置文件
