## 1.2.3 (2026-06-08)
### New
- 新增期权提前行权接口：`submit_option_exercise` 提交行权/放弃申请、`cancel_option_exercise` 撤销申请、`check_option_exercise` 行权检验（预估持仓变化）、`get_option_exercise_records` 分页查询行权记录、`get_option_exercise_positions` 查询可行权持仓

## 1.2.2 (2026-05-07)
### New
- `OrderStatusData` 推送消息补齐字段：`timeInForce` (43)、`updateTime` (44, 订单信息更新时间戳 ms)、`latestTime` (45, 订单状态更新时间戳 ms)

## 1.2.1 (2026-04-13)
### New
- 同时支持 PKCS#8（`private_key_pk8`）和 PKCS#1（`private_key_pk1`）私钥格式，优先使用 PKCS#8
- `get_props_path()` 兼容输入完整文件路径（直接使用）或目录路径（自动追加默认文件名）
- Demo 默认从 `~/.tigeropen/` 读取配置文件
