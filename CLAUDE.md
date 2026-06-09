# CLAUDE.md - TigerOpen C++ SDK

## 项目概述

老虎证券 OpenAPI 官方 C++ SDK，提供交易、行情、长连接推送等功能。支持跨平台（macOS、Linux、Windows）。

## 技术栈

- C++17
- CMake 3.15+
- WebSocket（实时推送）
- HTTP REST API

## 核心依赖

| 依赖 | 版本 | 用途 |
|------|------|------|
| Boost | 1.86.0 | system, thread, log, program_options |
| OpenSSL | 1.0.1+ | 加密通信 |
| cpprestsdk | latest | HTTP 客户端 |
| Protobuf | v25.1 | 数据序列化 |
| Abseil | 20240722+ | Protobuf 运行时依赖 |

## 目录结构

```
openapi-cpp-sdk/
├── include/              # 头文件
├── src/                  # 源码
├── demo/                 # 示例代码
├── scripts/              # 构建脚本
├── CMakeLists.txt        # CMake 配置
├── vcpkg.json            # Windows 依赖管理
└── README.md
```

## 主要特性

- 完整的交易接口支持（下单、撤单、查询持仓）
- 实时行情数据获取（K线、报价、逐笔成交）
- WebSocket 长连接推送（持仓、订单、行情实时推送）
- 跨平台支持（macOS、Linux、Windows）
- 类型安全的 C++ API 设计
- 支持静态库和动态库编译

## 构建

### macOS/Linux

```bash
chmod +x scripts/build_linux_mac.sh
./scripts/build_linux_mac.sh

# Release 模式
BUILD_TYPE=Release ./scripts/build_linux_mac.sh
```

### Windows

使用 Visual Studio 打开 `openapi-cpp-sdk.sln`，或使用 CMake：

```bash
cmake -B build -DCMAKE_TOOLCHAIN_FILE=[vcpkg-root]/scripts/buildsystems/vcpkg.cmake
cmake --build build
```

## 文档

- 开发者平台：https://developer.itigerup.com/
