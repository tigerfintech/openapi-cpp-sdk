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
| Protobuf | 5.28.3 | 数据序列化 |
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
├── CHANGELOG.md          # 版本更新历史
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

使用 Visual Studio 打开 `openapi-cpp-sdk.sln`，或在 Developer PowerShell 中使用 MSBuild：

```powershell
msbuild openapi-cpp-sdk.vcxproj /t:Rebuild /p:Configuration=Release-MD /p:Platform=x64 /m /nr:false
msbuild demo\openapi_cpp_test\openapi_cpp_test.vcxproj /t:Rebuild /p:Configuration=Release-MD /p:Platform=x64 /m /nr:false
```

Visual Studio 工程使用 vcpkg manifest 依赖，Windows SDK 目标版本为 `10.0.22621.0`。Win32 使用 `vcpkg_installed_x86`，x64 使用 `vcpkg_installed`。工程保留项目 include 优先，vcpkg include 放最后，并通过 `PROTOBUF_USE_DLLS` 匹配 vcpkg DLL 版 Protobuf。

CMake 路径保留在 `scripts/build_windows.ps1`，脚本默认使用 vcpkg manifest 安装 Protobuf 5.28.3、Abseil、OpenSSL 和 cpprestsdk，Boost 1.86.0 仍由脚本源码构建并缓存到 `.deps/`。如需和 macOS/Linux 保持源码安装路径，可使用 `-ProtobufProvider Source`。常用命令：

```powershell
Set-ExecutionPolicy -Scope Process -ExecutionPolicy Bypass -Force
.\scripts\build_windows.ps1 -Triplet x64-windows -BuildType Release -Runtime MD
```

Windows 输出包位于 `output/Windows/{Win32,x64}`，每个平台包含 `Debug-MD`、`Debug-MT`、`Release-MD`、`Release-MT` 四个 zip。`output/` 下只跟踪 zip 包和 `output/Windows/readme.md`。

重新生成 Windows zip 包使用：

```powershell
.\scripts\package_windows.ps1
```

## 文档

- 开发者平台：https://developer.itigerup.com/
