#!/bin/bash
# Linux SDK 打包脚本
# 功能：构建 Debug + Release，安装到 output/Linux/sdk，创建 zip 发行包
# 依赖：若 Boost 1.86 不存在则自动从源码下载编译安装

set -e

PROJECT_ROOT=$(pwd)
JOBS=${JOBS:-$(nproc)}

BOOST_VERSION="${BOOST_VERSION:-1_86_0}"
BOOST_DOTTED="${BOOST_VERSION//_/.}"
BOOST_TARBALL="boost-${BOOST_DOTTED}-b2-nodocs.tar.gz"
BOOST_URL="https://github.com/boostorg/boost/releases/download/boost-${BOOST_DOTTED}/${BOOST_TARBALL}"
BOOST_1_86_ROOT="${BOOST_ROOT:-/usr/local/boost_${BOOST_VERSION}}"
CACHE_DIR="${PROJECT_ROOT}/.cache"

echo "=========================================="
echo "  Tiger OpenAPI C++ SDK - Linux Package"
echo "=========================================="

# Returns 0 and sets BOOST_1_86_ROOT if a system Boost in [1.86, 1.90) is found at $1/include.
_check_system_boost() {
    local include_dir="$1"
    [ -f "${include_dir}/boost/version.hpp" ] || return 1
    local ver_num
    ver_num=$(grep -m1 '#define BOOST_VERSION ' "${include_dir}/boost/version.hpp" | awk '{print $3}')
    [ -n "$ver_num" ] || return 1
    local major=$(( ver_num / 100000 ))
    local minor=$(( (ver_num / 100) % 1000 ))
    if [ "$major" -eq 1 ] && [ "$minor" -ge 86 ] && [ "$minor" -lt 90 ]; then
        echo "    [boost] Found compatible system Boost ${major}.${minor} at ${include_dir%/include}"
        return 0
    fi
    echo "    [boost] System Boost ${major}.${minor} outside [1.86, 1.90); will build from source."
    return 1
}

# Preflight: prefer system Boost in [1.86,1.90); auto-build 1.86 from source if not found.
ensure_boost() {
    if [ -d "${BOOST_1_86_ROOT}/include/boost" ]; then
        echo "    [boost] Found at ${BOOST_1_86_ROOT}"
        return
    fi

    # Fast path: check common system locations first.
    for root in /usr /usr/local; do
        if _check_system_boost "${root}/include"; then
            BOOST_1_86_ROOT="$root"
            return
        fi
    done

    echo ""
    echo "==> Boost ${BOOST_DOTTED} not found at ${BOOST_1_86_ROOT}, building from source..."

    # Check write permission on the parent directory before attempting install.
    local boost_parent
    boost_parent="$(dirname "${BOOST_1_86_ROOT}")"
    if [ ! -w "${boost_parent}" ]; then
        echo "ERROR: ${BOOST_1_86_ROOT} requires write permission on ${boost_parent}."
        echo "  Option 1: run with sudo: sudo BOOST_ROOT=${BOOST_1_86_ROOT} $0"
        echo "  Option 2: point to a writable path: BOOST_ROOT=\$HOME/boost_1_86_0 $0"
        exit 1
    fi

    mkdir -p "${CACHE_DIR}"
    # b2-nodocs tarball extracts to boost-<dotted> (e.g. boost-1.86.0)
    local src_dir="${CACHE_DIR}/boost-${BOOST_DOTTED}"
    if [ ! -d "${src_dir}" ]; then
        if [ ! -f "${CACHE_DIR}/${BOOST_TARBALL}" ]; then
            echo "    Downloading ${BOOST_URL} ..."
            if ! wget -q --show-progress -O "${CACHE_DIR}/${BOOST_TARBALL}" "${BOOST_URL}"; then
                rm -f "${CACHE_DIR}/${BOOST_TARBALL}"
                echo "ERROR: Failed to download Boost from ${BOOST_URL}"
                exit 1
            fi
        fi
        echo "    Extracting..."
        tar -C "${CACHE_DIR}" -xzf "${CACHE_DIR}/${BOOST_TARBALL}"
    fi
    if [ ! -f "${src_dir}/bootstrap.sh" ]; then
        echo "ERROR: bootstrap.sh not found in ${src_dir} — delete ${CACHE_DIR}/${BOOST_TARBALL} and retry."
        exit 1
    fi
    pushd "${src_dir}" >/dev/null
    ./bootstrap.sh
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
