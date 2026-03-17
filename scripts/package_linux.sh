#!/bin/bash
# Linux SDK 打包脚本
# 功能：构建 Debug + Release，安装到 output/Linux/sdk，创建 zip 发行包

set -e

PROJECT_ROOT=$(pwd)
JOBS=${JOBS:-$(nproc)}

echo "=========================================="
echo "  Tiger OpenAPI C++ SDK - Linux Package"
echo "=========================================="

# 1. 创建构建目录
echo ""
echo "==> Step 1/5: Creating build directories..."
mkdir -p build-debug build-release

# 2. 配置并构建 Debug
echo ""
echo "==> Step 2/5: Building Debug..."
cmake -S . -B build-debug \
    -DCMAKE_BUILD_TYPE=Debug \
    -DCMAKE_PREFIX_PATH="/usr/local/opt/cpprestsdk;/usr/local/opt/protobuf-v25.1" \
    -DBOOST_ROOT=/usr/local/boost_1_86_0 \
    -DBoost_NO_BOOST_CMAKE=ON
cmake --build build-debug -j${JOBS}

echo ""
echo "==> Step 2/5: Building Release..."
cmake -S . -B build-release \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_PREFIX_PATH="/usr/local/opt/cpprestsdk;/usr/local/opt/protobuf-v25.1" \
    -DBOOST_ROOT=/usr/local/boost_1_86_0 \
    -DBoost_NO_BOOST_CMAKE=ON
cmake --build build-release -j${JOBS}

# 3. 安装
echo ""
echo "==> Step 3/5: Installing to output/Linux/sdk..."
cmake --install build-debug --prefix output/Linux/sdk/Debug
cmake --install build-release --prefix output/Linux/sdk/Release

# 4. 复制库到独立目录
echo ""
echo "==> Step 4/5: Copying libs to standalone directories..."
mkdir -p output/Linux/Debug/lib output/Linux/Release/lib
cp build-debug/libtigerapi.a output/Linux/Debug/lib/libtigerapi.a
cp build-release/libtigerapi.a output/Linux/Release/lib/libtigerapi.a

# 5. 打包
echo ""
echo "==> Step 5/5: Creating zip packages..."
cd output/Linux
rm -f Debug.zip Release.zip
echo "    - Creating Debug.zip..."
zip -r Debug.zip sdk/Debug/ Debug/ > /dev/null
echo "    - Creating Release.zip..."
zip -r Release.zip sdk/Release/ Release/ > /dev/null
cd ${PROJECT_ROOT}

echo ""
echo "=========================================="
echo "  Package Complete!"
echo "=========================================="
echo ""
ls -lh output/Linux/*.zip
echo ""
