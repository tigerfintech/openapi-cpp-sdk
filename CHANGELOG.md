## 1.2.1 (2026-04-13)
### New
- 同时支持 PKCS#8（`private_key_pk8`）和 PKCS#1（`private_key_pk1`）私钥格式，优先使用 PKCS#8
- `get_props_path()` 兼容输入完整文件路径（直接使用）或目录路径（自动追加默认文件名）
- Demo 默认从 `~/.tigeropen/` 读取配置文件
