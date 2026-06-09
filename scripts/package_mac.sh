#!/bin/bash
# macOS SDK 打包脚本
# 功能：构建 Debug + Release，安装到 output/Mac/sdk，创建 zip 发行包

set -e

PROJECT_ROOT=$(pwd)
JOBS=${JOBS:-8}

echo "=========================================="
echo "  Tiger OpenAPI C++ SDK - macOS Package"
echo "=========================================="

# 1. 创建构建目录（清理旧 cmake cache 确保路径生效）
echo ""
echo "==> Step 1/5: Creating build directories..."
rm -rf build-debug build-release
mkdir -p build-debug build-release

# 2. 配置并构建 Debug
echo ""
echo "==> Step 2/5: Building Debug..."
cmake -S . -B build-debug \
    -DCMAKE_BUILD_TYPE=Debug \
    -DCMAKE_PREFIX_PATH="/opt/homebrew/opt/cpprestsdk;/opt/homebrew/opt/protobuf" \
    -DProtobuf_ROOT=/opt/homebrew/opt/protobuf \
    -DBOOST_ROOT=/usr/local/boost_1_86_0 \
    -DBoost_INCLUDE_DIR=/usr/local/boost_1_86_0 \
    -DBoost_LIBRARY_DIR=/usr/local/boost_1_86_0/stage/lib \
    -DBoost_NO_BOOST_CMAKE=ON \
    -DBoost_NO_SYSTEM_PATHS=ON \
    -DCMAKE_CXX_FLAGS="-arch arm64 -std=c++17 -stdlib=libc++ -DBOOST_LOG_DYN_LINK -Wno-deprecated-declarations"
cmake --build build-debug -j${JOBS}

echo ""
echo "==> Step 2/5: Building Release..."
cmake -S . -B build-release \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_PREFIX_PATH="/opt/homebrew/opt/cpprestsdk;/opt/homebrew/opt/protobuf" \
    -DProtobuf_ROOT=/opt/homebrew/opt/protobuf \
    -DBOOST_ROOT=/usr/local/boost_1_86_0 \
    -DBoost_INCLUDE_DIR=/usr/local/boost_1_86_0 \
    -DBoost_LIBRARY_DIR=/usr/local/boost_1_86_0/stage/lib \
    -DBoost_NO_BOOST_CMAKE=ON \
    -DBoost_NO_SYSTEM_PATHS=ON \
    -DCMAKE_CXX_FLAGS="-arch arm64 -std=c++17 -stdlib=libc++ -DBOOST_LOG_DYN_LINK -Wno-deprecated-declarations"
cmake --build build-release -j${JOBS}

# 3. 安装
echo ""
echo "==> Step 3/5: Installing to output/Mac/sdk..."
cmake --install build-debug --prefix output/Mac/sdk/Debug
cmake --install build-release --prefix output/Mac/sdk/Release

# 4. 复制库到独立目录
echo ""
echo "==> Step 4/5: Copying libs to standalone directories..."
mkdir -p output/Mac/Debug/lib output/Mac/Release/lib
cp build-debug/libtigerapi.a output/Mac/Debug/lib/libtigerapi.a
cp build-release/libtigerapi.a output/Mac/Release/lib/libtigerapi.a

# 5. 打包
echo ""
echo "==> Step 5/5: Creating zip packages..."
cd output/Mac
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
ls -lh output/Mac/*.zip
echo ""
