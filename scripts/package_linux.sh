#!/bin/bash
# Linux SDK 打包脚本
# 功能：构建 Debug + Release，安装到 output/Linux/sdk，创建 zip 发行包
# 依赖：若 Boost 1.86 不存在则自动从源码下载编译安装

set -e

PROJECT_ROOT=$(pwd)
JOBS=${JOBS:-$(nproc)}

BOOST_VERSION="${BOOST_VERSION:-1_86_0}"
BOOST_DOTTED="${BOOST_VERSION//_/.}"
BOOST_TARBALL="boost_${BOOST_VERSION}.tar.bz2"
BOOST_URL="https://archives.boost.io/release/${BOOST_DOTTED}/source/${BOOST_TARBALL}"
BOOST_1_86_ROOT="${BOOST_ROOT:-/usr/local/boost_${BOOST_VERSION}}"
CACHE_DIR="${PROJECT_ROOT}/.cache"

echo "=========================================="
echo "  Tiger OpenAPI C++ SDK - Linux Package"
echo "=========================================="

# Preflight: ensure Boost 1.86 is present; auto-build from source if not.
ensure_boost() {
    if [ -d "${BOOST_1_86_ROOT}/include/boost" ]; then
        echo "    [boost] Found at ${BOOST_1_86_ROOT}"
        return
    fi
    echo ""
    echo "==> Boost ${BOOST_DOTTED} not found at ${BOOST_1_86_ROOT}, building from source..."
    mkdir -p "${CACHE_DIR}"
    local src_dir="${CACHE_DIR}/boost_${BOOST_VERSION}"
    if [ ! -d "${src_dir}" ]; then
        if [ ! -f "${CACHE_DIR}/${BOOST_TARBALL}" ]; then
            echo "    Downloading ${BOOST_URL} ..."
            wget -q --show-progress -O "${CACHE_DIR}/${BOOST_TARBALL}" "${BOOST_URL}"
        fi
        echo "    Extracting..."
        tar -C "${CACHE_DIR}" --bzip2 -xf "${CACHE_DIR}/${BOOST_TARBALL}"
    fi
    pushd "${src_dir}" >/dev/null
    ./bootstrap.sh --prefix="${BOOST_1_86_ROOT}" \
        --with-libraries=thread,log,program_options,chrono,filesystem,atomic,regex
    ./b2 headers
    ./b2 -j"${JOBS}" install
    popd >/dev/null
    echo "    [boost] Installed to ${BOOST_1_86_ROOT}"
}

ensure_boost

# 1. 创建构建目录（清理旧 cmake cache 确保路径生效）
echo ""
echo "==> Step 1/5: Creating build directories..."
rm -rf build-debug build-release
mkdir -p build-debug build-release

# Shared cmake configure + build function for Debug and Release variants.
build_variant() {
    local BUILD_TYPE="$1"
    local BUILD_DIR="$2"
    echo ""
    echo "==> Step 2/5: Building ${BUILD_TYPE}..."
    cmake -S . -B "${BUILD_DIR}" \
        -DCMAKE_BUILD_TYPE="${BUILD_TYPE}" \
        -DCMAKE_PREFIX_PATH="/usr/local/opt/cpprestsdk;/usr/local/opt/protobuf-v25.1" \
        -DBOOST_ROOT="${BOOST_1_86_ROOT}" \
        -DBoost_INCLUDE_DIR="${BOOST_1_86_ROOT}/include" \
        -DBoost_LIBRARY_DIR="${BOOST_1_86_ROOT}/lib" \
        -DBoost_NO_BOOST_CMAKE=ON \
        -DBoost_NO_SYSTEM_PATHS=ON
    cmake --build "${BUILD_DIR}" -j"${JOBS}"
}

# 2. 配置并构建
build_variant Debug build-debug
build_variant Release build-release

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
