# Tiger OpenAPI C++ SDK

[![Version](https://img.shields.io/badge/version-1.2.3-blue.svg)](https://github.com/tigerbrokers/openapi-cpp-sdk)
[![License](https://img.shields.io/badge/license-Apache%202.0-green.svg)](LICENSE)

Tiger OpenAPI C++ SDK 是老虎证券开放平台的官方 C++ 客户端，提供交易、行情、长连接推送等功能。

[English](README_EN.md) | 中文

## 主要特性

- 🚀 完整的交易接口支持（下单、撤单、查询持仓等）
- 📊 实时行情数据获取（K线、报价、逐笔成交等）
- 🔌 WebSocket 长连接推送（持仓、订单、行情实时推送）
- 🌐 跨平台支持（macOS、Linux、Windows）
- 🛡️ 类型安全的 C++ API 设计
- 📦 支持静态库和动态库编译

## 环境要求

| 平台 | 编译器 | CMake | 依赖管理 |
|------|--------|-------|---------|
| macOS | Clang (Xcode) | 3.15+ | Homebrew + 源码 |
| Linux | GCC/G++ | 3.15+ | apt + 源码 |
| Windows | MSVC 2019/2022 | 3.15+ | vcpkg |

### 核心依赖

- **Boost** 1.86.0（system, thread, log, program_options, chrono, filesystem）
- **OpenSSL** 1.0.1+（推荐 3.x）
- **cpprestsdk** 最新版（HTTP 客户端）
- **Protobuf** 5.28.3
- **Abseil** 20240722+（Protobuf 运行时依赖）
- **C++ 标准**: C++17

## 快速开始

### 一键构建（推荐）

#### macOS/Linux

```bash
# 赋予执行权限（仅首次）
chmod +x scripts/build_linux_mac.sh

# 默认构建（Debug 模式，自动安装依赖并运行 demo）
./scripts/build_linux_mac.sh

# Release 模式构建
BUILD_TYPE=Release ./scripts/build_linux_mac.sh

# 仅构建 SDK，跳过 demo
SKIP_DEMO=1 ./scripts/build_linux_mac.sh
```

脚本会自动完成：
1. 安装系统依赖（Homebrew/apt）
2. 编译 Boost、cpprestsdk、Protobuf
3. 构建 SDK（Debug + Release）
4. 构建并运行 demo

#### Windows

```powershell
# 在 Developer PowerShell 中构建 SDK（x64 Release MD）
msbuild openapi-cpp-sdk.vcxproj /t:Rebuild /p:Configuration=Release-MD /p:Platform=x64 /m /nr:false

# 构建 demo
msbuild demo\openapi_cpp_test\openapi_cpp_test.vcxproj /t:Rebuild /p:Configuration=Release-MD /p:Platform=x64 /m /nr:false
```

### 使用预编译库

如果不想从源码编译，可以直接使用 `output/` 目录下的预编译库：

```
output/
├── Mac/
│   ├── Debug.zip      # macOS Debug 静态库 + 头文件
│   └── Release.zip    # macOS Release 静态库 + 头文件
├── Linux/
│   ├── Debug.zip
│   └── Release.zip
└── Windows/
    ├── readme.md
    ├── x64/
    │   ├── Debug-MD.zip
    │   ├── Debug-MT.zip
    │   ├── Release-MD.zip
    │   └── Release-MT.zip
    └── Win32/
        ├── Debug-MD.zip
        ├── Debug-MT.zip
        ├── Release-MD.zip
        └── Release-MT.zip
```

## 手动编译

### macOS

#### 1. 安装基础工具

```bash
brew install cmake wget automake libtool pkg-config openssl@3 abseil
```

#### 2. 安装 Boost

```bash
cd /usr/local
wget https://archives.boost.io/release/1.86.0/source/boost_1_86_0.tar.bz2
tar --bzip2 -xf boost_1_86_0.tar.bz2
cd boost_1_86_0
./bootstrap.sh
./b2 headers
./b2 -j $(sysctl -n hw.ncpu)
```

#### 3. 编译 cpprestsdk

```bash
cd /tmp
git clone https://github.com/microsoft/cpprestsdk.git
cd cpprestsdk
git submodule update --init
cmake -S . -B build \
    -DCMAKE_BUILD_TYPE=Debug \
    -DCMAKE_INSTALL_PREFIX=/usr/local/opt/cpprestsdk \
    -DBUILD_SHARED_LIBS=ON \
    -DBUILD_SAMPLES=OFF \
    -DBUILD_TESTS=OFF \
    -DBOOST_ROOT=/usr/local/boost_1_86_0 \
    -DOPENSSL_ROOT_DIR=$(brew --prefix openssl@3) \
    -DCMAKE_CXX_FLAGS="-Wno-error=null-pointer-subtraction -w"
cmake --build build -- -j $(sysctl -n hw.ncpu)
sudo cmake --install build
```

#### 4. 编译 Protobuf

```bash
cd /tmp
curl -sL https://github.com/protocolbuffers/protobuf/releases/download/v28.3/protobuf-28.3.tar.gz -o protobuf-28.3.tar.gz
tar xzf protobuf-28.3.tar.gz
cd protobuf-28.3
cmake -S . -B build \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX=/usr/local/opt/protobuf-v28.3 \
    -DCMAKE_CXX_STANDARD=17 \
    -Dprotobuf_BUILD_TESTS=OFF \
    -Dprotobuf_ABSL_PROVIDER=package \
    -DCMAKE_PREFIX_PATH=$(brew --prefix)
cmake --build build -- -j $(sysctl -n hw.ncpu)
sudo cmake --install build
```

#### 5. 构建 SDK

```bash
cd <project_root>
export CPATH="$(brew --prefix abseil)/include:$CPATH"

cmake -S . -B build/Debug \
    -DCMAKE_BUILD_TYPE=Debug \
    -DCMAKE_INSTALL_PREFIX=/usr/local/opt/tigerapi/Debug \
    -DBOOST_ROOT=/usr/local/boost_1_86_0 \
    -DCMAKE_PREFIX_PATH="/usr/local/opt/cpprestsdk;/usr/local/opt/protobuf-v28.3" \
    -DOPENSSL_ROOT_DIR=$(brew --prefix openssl@3)
cmake --build build/Debug -- -j $(sysctl -n hw.ncpu)
sudo cmake --install build/Debug
```

#### 6. 构建 Demo

```bash
cd demo
cmake -S . -B build \
    -DCMAKE_BUILD_TYPE=Debug \
    -DBOOST_ROOT=/usr/local/boost_1_86_0 \
    -DTIGERAPI_INCLUDE_DIR=/usr/local/opt/tigerapi/Debug/include \
    -DTIGERAPI_LIBRARY=/usr/local/opt/tigerapi/Debug/lib/libtigerapi.a \
    -DCPPREST_INCLUDE_DIR=/usr/local/opt/cpprestsdk/include \
    -DCPPREST_LIBRARY=/usr/local/opt/cpprestsdk/lib/libcpprest.dylib \
    -DProtobuf_INCLUDE_DIR=/usr/local/opt/protobuf-v28.3/include \
    -DProtobuf_LIBRARY=/usr/local/opt/protobuf-v28.3/lib/libprotobuf.dylib \
    -DOPENSSL_ROOT_DIR=$(brew --prefix openssl@3)
cmake --build build -- -j $(sysctl -n hw.ncpu)

# 运行 demo（需要设置动态库路径）
export DYLD_LIBRARY_PATH="/usr/local/opt/cpprestsdk/lib:/usr/local/opt/protobuf-v28.3/lib:$DYLD_LIBRARY_PATH"
./build/openapi_cpp_test
```

### Linux (Ubuntu)

#### 1. 安装系统依赖

```bash
sudo apt-get update
sudo apt-get install -y git wget bzip2 unzip gcc g++ libtool automake autoconf \
    build-essential cmake libssl-dev libabsl-dev zlib1g-dev pkg-config
```

#### 2-6. 编译步骤

与 macOS 类似，主要区别：
- 去掉 `OPENSSL_ROOT_DIR` 参数（Linux 上 OpenSSL 在系统路径）
- 使用 `LD_LIBRARY_PATH` 代替 `DYLD_LIBRARY_PATH`

```bash
export LD_LIBRARY_PATH="/usr/local/opt/cpprestsdk/lib:/usr/local/opt/protobuf-v28.3/lib:$LD_LIBRARY_PATH"
```

### Windows

Windows 平台提供两种编译方式：

#### 方式一：使用 Visual Studio（推荐）

1. **前置要求**
   - Visual Studio 2019 或 2022
   - 安装"使用 C++ 的桌面开发"工作负载

2. **准备依赖**

   项目使用 vcpkg 管理依赖，支持两种方式：

   **自动安装（推荐）**：
   ```powershell
   # vcpkg 会根据 vcpkg.json 自动安装依赖
   # 首次打开解决方案时自动触发
   ```

   **手动安装**：
   ```powershell
   git clone https://github.com/microsoft/vcpkg .vcpkg
   .\.vcpkg\bootstrap-vcpkg.bat -disableMetrics
    .\.vcpkg\vcpkg install --triplet x64-windows
   ```

3. **打开并编译**
   - 双击打开 `openapi-cpp-sdk.sln`
   - 选择配置（推荐 `Release-MD|x64`）
   - 右键解决方案 → 生成解决方案

4. **配置说明**

   解决方案提供 8 种配置组合：

   | 配置 | 平台 | 运行时库 | 说明 |
   |------|------|---------|------|
   | Debug-MD | x64/Win32 | /MDd | Debug + 动态运行时 |
   | Debug-MT | x64/Win32 | /MTd | Debug + 静态运行时 |
   | Release-MD | x64/Win32 | /MD | Release + 动态运行时（推荐） |
   | Release-MT | x64/Win32 | /MT | Release + 静态运行时 |

5. **编译产物**
   ```
   output/Windows/
   ├── x64/
   │   ├── Release-MD/
   │   │   ├── openapi-cpp-sdk.dll
   │   │   ├── openapi-cpp-sdk.lib
   │   │   └── openapi_cpp_test.exe
   │   └── Release-MT/
   └── Win32/
       └── ...
   ```

6. **依赖路径配置**

   工程文件已配置好依赖路径：
   - Boost: `.deps/boost/boost_1_86_0`（可通过环境变量 `BOOST_ROOT` 覆盖）
   - vcpkg 依赖: `vcpkg_installed/x64-windows/`（自动管理）
   - SDK 头文件: `include/`

#### 方式二：使用 CMake 命令行

推荐使用仓库脚本统一准备依赖并调用 CMake。Windows 下默认使用 vcpkg 安装树中的 Protobuf 5.28.3、Abseil、OpenSSL 和 cpprestsdk，避免与生成代码版本不一致；同时保留 Protobuf 源码安装路径，便于和 macOS/Linux 的 CMake 构建方式保持一致。

```powershell
Set-ExecutionPolicy -Scope Process -ExecutionPolicy Bypass -Force

# 构建 SDK + demo（x64 Release MD）
.\scripts\build_windows.ps1 -Triplet x64-windows -BuildType Release -Runtime MD

# 仅构建 SDK
.\scripts\build_windows.ps1 -Triplet x64-windows -BuildType Release -Runtime MD -SkipDemo

# 使用 Protobuf 源码安装路径（默认是 vcpkg）
.\scripts\build_windows.ps1 -Triplet x64-windows -BuildType Release -Runtime MD -ProtobufProvider Source
```

如果需要手动执行 CMake，可参考以下命令。

1. **安装 vcpkg**

```powershell
git clone https://github.com/microsoft/vcpkg .vcpkg
.\.vcpkg\bootstrap-vcpkg.bat -disableMetrics
```

2. **安装依赖**

```powershell
.\.vcpkg\vcpkg install --triplet x64-windows
```

3. **构建 SDK**

```powershell
$env:VCPKG_ROOT = "$PWD\.vcpkg"
$toolchain = "$env:VCPKG_ROOT\scripts\buildsystems\vcpkg.cmake"

cmake -S . -B build\windows -A x64 `
    -DCMAKE_BUILD_TYPE=Release `
    -DCMAKE_TOOLCHAIN_FILE=$toolchain `
    -DVCPKG_TARGET_TRIPLET=x64-windows `
    -DCMAKE_PREFIX_PATH="$PWD\.deps\install\x64-windows\boost-1.86.0;$PWD\vcpkg_installed\x64-windows" `
    -DBOOST_ROOT="$PWD\.deps\install\x64-windows\boost-1.86.0" `
    -DProtobuf_DIR="$PWD\vcpkg_installed\x64-windows\share\protobuf" `
    -DCMAKE_INSTALL_PREFIX=output\Windows\x64\Release-MD

cmake --build build\windows --config Release
cmake --install build\windows --config Release
```

4. **构建 Demo**

```powershell
cmake -S demo -B demo\build\windows -A x64 `
    -DCMAKE_BUILD_TYPE=Release `
    -DCMAKE_TOOLCHAIN_FILE=$toolchain `
    -DVCPKG_TARGET_TRIPLET=x64-windows `
    -DCMAKE_PREFIX_PATH="$PWD\.deps\install\x64-windows\boost-1.86.0;$PWD\vcpkg_installed\x64-windows;output\Windows\x64\Release-MD" `
    -DTIGERAPI_INCLUDE_DIR=output\Windows\x64\Release-MD\include `
    -DTIGERAPI_LIBRARY=output\Windows\x64\Release-MD\lib\tigerapi.lib `
    -DProtobuf_DIR="$PWD\vcpkg_installed\x64-windows\share\protobuf"

cmake --build demo\build\windows --config Release --target openapi_cpp_test
```

## 使用示例

### HTTP 行情接口

```cpp
#include "tigerapi/quote_client.h"

TigerClient client;
client.init("your_tiger_id", "your_private_key", "your_account");

// 获取 K 线数据
std::vector<std::string> symbols = {"AAPL", "TSLA"};
ResponseModel response = client.grab_quote_kline(symbols, "day", "2024-01-01", "2024-12-31");
```

### WebSocket 长连接推送

```cpp
#include "tigerapi/push_client.h"

// 创建推送客户端
PushClient push_client(tiger_id, private_key, "your_language");

// 订阅持仓推送
push_client.subscribe_position();

// 订阅行情推送
push_client.subscribe_quote({"AAPL", "TSLA"});

// 连接并接收推送
push_client.connect();
```

更多示例请参考 `demo/openapi_cpp_test/openapi_cpp_test.cpp`。

## 项目结构

```
openapi-cpp-sdk/
├── include/                    # 头文件
│   ├── tigerapi/              # SDK 公共头文件
│   ├── cpprest/               # cpprestsdk 头文件
│   ├── google/protobuf/       # Protobuf 5.28.3 头文件
│   └── openapi_pb/            # Protobuf 生成的消息定义
├── src/                       # SDK 源码实现
├── demo/                      # 示例代码
│   └── openapi_cpp_test/
├── scripts/                   # 构建脚本
│   ├── build_linux_mac.sh    # macOS/Linux 一键构建
│   └── build_windows.ps1     # Windows CMake 构建脚本
├── output/                    # 编译产物
│   ├── Mac/
│   ├── Linux/
│   └── Windows/
└── CMakeLists.txt            # CMake 配置
```

## 常见问题

### macOS

**Q: 找不到 OpenSSL**
```bash
# 指定 OpenSSL 路径
-DOPENSSL_ROOT_DIR=$(brew --prefix openssl@3)
```

**Q: 架构不匹配（arm64 vs x86_64）**

修改 `CMakeLists.txt` 和 `demo/CMakeLists.txt` 中的架构参数：
```cmake
# ARM Mac（默认）
set(CMAKE_CXX_FLAGS "-arch arm64 -std=c++17 ...")

# Intel Mac
set(CMAKE_CXX_FLAGS "-arch x86_64 -std=c++17 ...")
```

**Q: 运行时找不到动态库**
```bash
export DYLD_LIBRARY_PATH="/usr/local/opt/cpprestsdk/lib:/usr/local/opt/protobuf-v28.3/lib:$DYLD_LIBRARY_PATH"
```

### Linux

**Q: 运行时找不到 .so 文件**
```bash
export LD_LIBRARY_PATH="/usr/local/opt/cpprestsdk/lib:/usr/local/opt/protobuf-v28.3/lib:$LD_LIBRARY_PATH"
```

### Windows

**Q: vcpkg bootstrap 失败**

确保安装了 Visual Studio "使用 C++ 的桌面开发"工作负载。

**Q: 运行时库不匹配（MT vs MD）**

确保 SDK 和 demo 使用相同的 `-DCMAKE_MSVC_RUNTIME_LIBRARY` 设置。

### 所有平台

**Q: Protobuf 版本冲突**

确保使用 Protobuf 5.28.3，需要 Abseil 和 C++17 支持。Windows 工程已通过 `PROTOBUF_USE_DLLS` 与 vcpkg DLL 版本保持一致。

**Q: 编译警告被当作错误**
```bash
# 添加编译参数关闭警告
-DCMAKE_CXX_FLAGS="-w"
```

## 更新日志

查看 [CHANGELOG.md](CHANGELOG.md) 了解版本更新历史。

## 许可证

Apache License 2.0

## 相关链接

- [老虎证券开放平台](https://developer.itigerup.com/)
- [API 文档](https://docs.itigerup.com/docs/)
- [Python SDK](https://github.com/tigerbrokers/openapi-python-sdk)
- [Java SDK](https://github.com/tigerbrokers/openapi-java-sdk)

## 技术支持

如有问题，请通过以下方式联系：
- 提交 [GitHub Issue](https://github.com/tigerbrokers/openapi-cpp-sdk/issues)
- 发送邮件至：openapi@tigerbrokers.com
