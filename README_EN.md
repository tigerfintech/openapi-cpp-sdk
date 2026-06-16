# Tiger OpenAPI C++ SDK

[![Version](https://img.shields.io/badge/version-1.2.0-blue.svg)](https://github.com/tigerbrokers/openapi-cpp-sdk)
[![License](https://img.shields.io/badge/license-Apache%202.0-green.svg)](LICENSE)

Tiger OpenAPI C++ SDK is the official C++ client for Tiger Brokers Open Platform, providing trading, market data, and WebSocket push functionalities.

English | [中文](README.md)

## Features

- 🚀 Complete trading API support (place orders, cancel orders, query positions, etc.)
- 📊 Real-time market data (K-line, quotes, tick-by-tick trades, etc.)
- 🔌 WebSocket push notifications (positions, orders, real-time quotes)
- 🌐 Cross-platform support (macOS, Linux, Windows)
- 🛡️ Type-safe C++ API design
- 📦 Static and dynamic library compilation support

## Requirements

| Platform | Compiler | CMake | Package Manager |
|----------|----------|-------|-----------------|
| macOS | Clang (Xcode) | 3.15+ | Homebrew + Source |
| Linux | GCC/G++ | 3.15+ | apt + Source |
| Windows | MSVC 2019/2022 | 3.15+ | vcpkg |

### Core Dependencies

- **Boost** 1.86.0 (system, thread, log, program_options, chrono, filesystem)
- **OpenSSL** 1.0.1+ (3.x recommended)
- **cpprestsdk** Latest (HTTP client)
- **Protobuf** v25.1 (libprotobuf 4.25.1)
- **Abseil** 20240722+ (Protobuf v25 runtime dependency)
- **C++ Standard**: C++17

## Quick Start

### One-Click Build (Recommended)

#### macOS/Linux

```bash
# Grant execute permission (first time only)
chmod +x scripts/build_linux_mac.sh

# Default build (Debug mode, auto-install dependencies and run demo)
./scripts/build_linux_mac.sh

# Release mode build
BUILD_TYPE=Release ./scripts/build_linux_mac.sh

# Build SDK only, skip demo
SKIP_DEMO=1 ./scripts/build_linux_mac.sh
```

The script will automatically:
1. Install system dependencies (Homebrew/apt)
2. Compile Boost, cpprestsdk, Protobuf
3. Build SDK (Debug + Release)
4. Build and run demo

#### Windows

```powershell
# Build SDK + demo (x64 Release MD)
powershell -ExecutionPolicy Bypass -File scripts\build_windows.ps1 `
    -Triplet x64-windows -BuildType Release -Runtime MD

# Rebuild all 8 Windows precompiled zip packages
powershell -ExecutionPolicy Bypass -File scripts\package_windows.ps1
```

### Using Pre-compiled Libraries

If you don't want to build from source, use the pre-compiled libraries in the `output/` directory:

```
output/
├── Mac/
│   ├── Debug.zip      # macOS Debug static library + headers
│   └── Release.zip    # macOS Release static library + headers
├── Linux/
│   ├── Debug/
│   └── Release/
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

## Manual Build

### macOS

#### 1. Install Basic Tools

```bash
brew install cmake wget automake libtool pkg-config openssl@3 abseil
```

#### 2. Install Boost

```bash
cd /usr/local
wget https://archives.boost.io/release/1.86.0/source/boost_1_86_0.tar.bz2
tar --bzip2 -xf boost_1_86_0.tar.bz2
cd boost_1_86_0
./bootstrap.sh
./b2 headers
./b2 -j $(sysctl -n hw.ncpu)
```

#### 3. Build cpprestsdk

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

#### 4. Build Protobuf

```bash
cd /tmp
curl -sL https://github.com/protocolbuffers/protobuf/releases/download/v25.1/protobuf-25.1.tar.gz -o protobuf-25.1.tar.gz
tar xzf protobuf-25.1.tar.gz
cd protobuf-25.1
cmake -S . -B build \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX=/usr/local/opt/protobuf-v25.1 \
    -DCMAKE_CXX_STANDARD=17 \
    -Dprotobuf_BUILD_TESTS=OFF \
    -Dprotobuf_ABSL_PROVIDER=package \
    -DCMAKE_PREFIX_PATH=$(brew --prefix)
cmake --build build -- -j $(sysctl -n hw.ncpu)
sudo cmake --install build
```

#### 5. Build SDK

```bash
cd <project_root>
export CPATH="$(brew --prefix abseil)/include:$CPATH"

cmake -S . -B build/Debug \
    -DCMAKE_BUILD_TYPE=Debug \
    -DCMAKE_INSTALL_PREFIX=/usr/local/opt/tigerapi/Debug \
    -DBOOST_ROOT=/usr/local/boost_1_86_0 \
    -DCMAKE_PREFIX_PATH="/usr/local/opt/cpprestsdk;/usr/local/opt/protobuf-v25.1" \
    -DOPENSSL_ROOT_DIR=$(brew --prefix openssl@3)
cmake --build build/Debug -- -j $(sysctl -n hw.ncpu)
sudo cmake --install build/Debug
```

#### 6. Build Demo

```bash
cd demo
cmake -S . -B build \
    -DCMAKE_BUILD_TYPE=Debug \
    -DBOOST_ROOT=/usr/local/boost_1_86_0 \
    -DTIGERAPI_INCLUDE_DIR=/usr/local/opt/tigerapi/Debug/include \
    -DTIGERAPI_LIBRARY=/usr/local/opt/tigerapi/Debug/lib/libtigerapi.a \
    -DCPPREST_INCLUDE_DIR=/usr/local/opt/cpprestsdk/include \
    -DCPPREST_LIBRARY=/usr/local/opt/cpprestsdk/lib/libcpprest.dylib \
    -DProtobuf_INCLUDE_DIR=/usr/local/opt/protobuf-v25.1/include \
    -DProtobuf_LIBRARY=/usr/local/opt/protobuf-v25.1/lib/libprotobuf.dylib \
    -DOPENSSL_ROOT_DIR=$(brew --prefix openssl@3)
cmake --build build -- -j $(sysctl -n hw.ncpu)

# Run demo (set dynamic library path)
export DYLD_LIBRARY_PATH="/usr/local/opt/cpprestsdk/lib:/usr/local/opt/protobuf-v25.1/lib:$DYLD_LIBRARY_PATH"
./build/openapi_cpp_test
```

### Linux (Ubuntu)

#### 1. Install System Dependencies

```bash
sudo apt-get update
sudo apt-get install -y git wget bzip2 unzip gcc g++ libtool automake autoconf \
    build-essential cmake libssl-dev libabsl-dev zlib1g-dev pkg-config
```

#### 2-6. Build Steps

Similar to macOS, main differences:
- Remove `OPENSSL_ROOT_DIR` parameter (OpenSSL is in system path on Linux)
- Use `LD_LIBRARY_PATH` instead of `DYLD_LIBRARY_PATH`

```bash
export LD_LIBRARY_PATH="/usr/local/opt/cpprestsdk/lib:/usr/local/opt/protobuf-v25.1/lib:$LD_LIBRARY_PATH"
```

### Windows

Windows platform provides two build methods:

#### Method 1: Using Visual Studio (Recommended)

1. **Prerequisites**
   - Visual Studio 2019 or 2022
   - Install "Desktop development with C++" workload

2. **Prepare Dependencies**

   The project uses vcpkg for dependency management, supporting two approaches:

   **Automatic Installation (Recommended)**:
   ```powershell
   # vcpkg will automatically install dependencies based on vcpkg.json
   # Triggered automatically when opening the solution for the first time
   ```

   **Manual Installation**:
   ```powershell
   git clone https://github.com/microsoft/vcpkg .vcpkg
   .\.vcpkg\bootstrap-vcpkg.bat -disableMetrics
   .\.vcpkg\vcpkg install boost:x64-windows openssl:x64-windows cpprestsdk:x64-windows protobuf:x64-windows
   ```

3. **Open and Build**
   - Double-click to open `openapi-cpp-sdk.sln`
   - Select configuration (recommended: `Release-MD|x64`)
   - Right-click solution → Build Solution

4. **Configuration Options**

   The solution provides 8 configuration combinations:

   | Configuration | Platform | Runtime Library | Description |
   |--------------|----------|-----------------|-------------|
   | Debug-MD | x64/Win32 | /MDd | Debug + Dynamic runtime |
   | Debug-MT | x64/Win32 | /MTd | Debug + Static runtime |
   | Release-MD | x64/Win32 | /MD | Release + Dynamic runtime (Recommended) |
   | Release-MT | x64/Win32 | /MT | Release + Static runtime |

5. **Build Artifacts**
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

6. **Dependency Path Configuration**

   Project files are pre-configured with dependency paths:
   - Boost: `.deps/boost/boost_1_86_0` (can be overridden by `BOOST_ROOT` environment variable)
   - vcpkg dependencies: `vcpkg_installed/x64-windows/` (automatically managed)
   - SDK headers: `include/`

7. **Package all Windows zips**

    ```powershell
    .\scripts\package_windows.bat

    # Re-compress existing output directories without rebuilding
    .\scripts\package_windows.bat -SkipBuild
    ```

#### Method 2: Using CMake Command Line

1. **Install vcpkg**

```powershell
git clone https://github.com/microsoft/vcpkg .vcpkg
.\.vcpkg\bootstrap-vcpkg.bat -disableMetrics
```

2. **Install Dependencies**

```powershell
.\.vcpkg\vcpkg install boost:x64-windows openssl:x64-windows cpprestsdk:x64-windows protobuf:x64-windows
```

3. **Build SDK**

```powershell
$env:VCPKG_ROOT = "$PWD\.vcpkg"
$toolchain = "$env:VCPKG_ROOT\scripts\buildsystems\vcpkg.cmake"

cmake -S . -B build\windows -A x64 `
    -DCMAKE_BUILD_TYPE=Release `
    -DCMAKE_TOOLCHAIN_FILE=$toolchain `
    -DVCPKG_TARGET_TRIPLET=x64-windows `
    -DCMAKE_INSTALL_PREFIX=output\Windows\x64\Release-MD

cmake --build build\windows --config Release
cmake --install build\windows --config Release
```

4. **Build Demo**

```powershell
cmake -S demo -B demo\build\windows -A x64 `
    -DCMAKE_BUILD_TYPE=Release `
    -DCMAKE_TOOLCHAIN_FILE=$toolchain `
    -DVCPKG_TARGET_TRIPLET=x64-windows `
    -DTIGERAPI_INCLUDE_DIR=output\Windows\x64\Release-MD\include `
    -DTIGERAPI_LIBRARY=output\Windows\x64\Release-MD\lib\tigerapi.lib

cmake --build demo\build\windows --config Release --target openapi_cpp_test
```

## Usage Examples

### HTTP Market Data API

```cpp
#include "tigerapi/quote_client.h"

TigerClient client;
client.init("your_tiger_id", "your_private_key", "your_account");

// Get K-line data
std::vector<std::string> symbols = {"AAPL", "TSLA"};
ResponseModel response = client.grab_quote_kline(symbols, "day", "2024-01-01", "2024-12-31");
```

### WebSocket Push Notifications

```cpp
#include "tigerapi/push_client.h"

// Create push client
PushClient push_client(tiger_id, private_key, "your_language");

// Subscribe to position updates
push_client.subscribe_position();

// Subscribe to quote updates
push_client.subscribe_quote({"AAPL", "TSLA"});

// Connect and receive push notifications
push_client.connect();
```

For more examples, see `demo/openapi_cpp_test/openapi_cpp_test.cpp`.

## Project Structure

```
openapi-cpp-sdk/
├── include/                    # Header files
│   ├── tigerapi/              # SDK public headers
│   ├── cpprest/               # cpprestsdk headers
│   ├── google/protobuf/       # Protobuf 5.28.3 headers
│   └── openapi_pb/            # Protobuf generated message definitions
├── src/                       # SDK source implementation
├── demo/                      # Example code
│   └── openapi_cpp_test/
├── scripts/                   # Build scripts
│   ├── build_linux_mac.sh    # macOS/Linux one-click build
│   ├── build_windows.ps1     # Windows CMake build
│   ├── package_windows.ps1   # Windows zip packaging
│   └── package_windows.bat   # Execution-policy-safe Windows packaging wrapper
├── output/                    # Build artifacts
│   ├── Mac/
│   ├── Linux/
│   └── Windows/
└── CMakeLists.txt            # CMake configuration
```

## Troubleshooting

### macOS

**Q: OpenSSL not found**
```bash
# Specify OpenSSL path
-DOPENSSL_ROOT_DIR=$(brew --prefix openssl@3)
```

**Q: Architecture mismatch (arm64 vs x86_64)**

Modify architecture parameter in `CMakeLists.txt` and `demo/CMakeLists.txt`:
```cmake
# ARM Mac (default)
set(CMAKE_CXX_FLAGS "-arch arm64 -std=c++17 ...")

# Intel Mac
set(CMAKE_CXX_FLAGS "-arch x86_64 -std=c++17 ...")
```

**Q: Dynamic library not found at runtime**
```bash
export DYLD_LIBRARY_PATH="/usr/local/opt/cpprestsdk/lib:/usr/local/opt/protobuf-v25.1/lib:$DYLD_LIBRARY_PATH"
```

### Linux

**Q: .so file not found at runtime**
```bash
export LD_LIBRARY_PATH="/usr/local/opt/cpprestsdk/lib:/usr/local/opt/protobuf-v25.1/lib:$LD_LIBRARY_PATH"
```

### Windows

**Q: vcpkg bootstrap failed**

Ensure Visual Studio "Desktop development with C++" workload is installed.

**Q: Runtime library mismatch (MT vs MD)**

Ensure SDK and demo use the same `-DCMAKE_MSVC_RUNTIME_LIBRARY` setting.

### All Platforms

**Q: Protobuf version conflict**

Ensure using Protobuf v25.1 (libprotobuf 4.25.1), requires Abseil and C++17 support.

**Q: Warnings treated as errors**
```bash
# Add compile flag to disable warnings
-DCMAKE_CXX_FLAGS="-w"
```

## Changelog

See [CHANGELOG.md](CHANGELOG.md) for version history.

## License

Apache License 2.0

## Related Links

- [Tiger Brokers Open Platform](https://developer.itigerup.com/)
- [API Documentation](https://docs.itigerup.com)
- [Python SDK](https://github.com/tigerbrokers/openapi-python-sdk)
- [Java SDK](https://github.com/tigerbrokers/openapi-java-sdk)

## Support

For questions or issues:
- Submit a [GitHub Issue](https://github.com/tigerbrokers/openapi-cpp-sdk/issues)
- Email: openapi@tigerbrokers.com
