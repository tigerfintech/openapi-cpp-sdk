#!/bin/bash
# macOS SDK 打包脚本
# 功能：构建 Debug + Release，安装到 output/Mac/sdk，创建 zip 发行包

set -e

PROJECT_ROOT=$(pwd)
JOBS=${JOBS:-8}

# Homebrew's boost (1.90+) has an incompatible ABI with the cpprestsdk we link against,
# causing SIGSEGV at runtime (do_use_service 1-arg vs 2-arg). We pin boost 1.86 built
# separately at /usr/local/boost_1_86_0. Do NOT replace this with $(brew --prefix)/include.
BOOST_1_86_ROOT="${BOOST_1_86_ROOT:-/usr/local/boost_1_86_0}"
HOMEBREW_PREFIX="${HOMEBREW_PREFIX:-$(brew --prefix)}"

echo "=========================================="
echo "  Tiger OpenAPI C++ SDK - macOS Package"
echo "=========================================="

# Preflight: verify pinned boost 1.86 exists before handing off to cmake.
# Without this check cmake silently falls back to Homebrew boost 1.90 (ABI incompatible).
if [ ! -d "$BOOST_1_86_ROOT" ]; then
    echo "ERROR: Boost 1.86 not found at $BOOST_1_86_ROOT"
    echo "  Install with: cd /usr/local && wget .../boost_1_86_0.tar.gz && tar xf ... && cd boost_1_86_0 && ./bootstrap.sh && ./b2"
    echo "  Or override: BOOST_1_86_ROOT=/path/to/boost_1_86_0 $0"
    exit 1
fi

# 1. 创建构建目录（清理旧 cmake cache 确保路径生效）
echo ""
echo "==> Step 1/5: Creating build directories..."
rm -rf build-debug build-release
mkdir -p build-debug build-release

# Helper: fix boost include order in flags.make after cmake configure.
# cmake injects /opt/homebrew/include (boost 1.90) before /usr/local/boost_1_86_0
# via absl INTERFACE_INCLUDE_DIRECTORIES, causing boost ABI mismatch (SIGSEGV).
# This function patches the generated flags.make to put boost 1.86 first.
# NOTE: With CMakeLists.txt now using target_include_directories(SYSTEM) for protobuf,
# this patch may become unnecessary — kept as a safety net for now.
fix_boost_include_order() {
    local flags_file="$1/CMakeFiles/tigerapi.dir/flags.make"
    if [ ! -f "$flags_file" ]; then return; fi
    # Only patch if the bad ordering is present (idempotent)
    if ! grep -q "\-I/opt/homebrew/include.*\-I${BOOST_1_86_ROOT}" "$flags_file" 2>/dev/null; then
        return
    fi
    ABSL_VER=$(ls "${HOMEBREW_PREFIX}/Cellar/abseil/" 2>/dev/null | head -1)
    if [ -z "$ABSL_VER" ]; then ABSL_VER="20240722.0"; fi
    sed -i '' \
        "s|-I/opt/homebrew/include \(.*\)-I${BOOST_1_86_ROOT}|-I${BOOST_1_86_ROOT} \1-I${HOMEBREW_PREFIX}/Cellar/abseil/${ABSL_VER}/include|" \
        "$flags_file"
    echo "    [boost ABI fix applied: $(grep 'CXX_INCLUDES' "$flags_file" | cut -c1-120)...)"
}

# Shared cmake configure + build function for Debug and Release variants.
build_variant() {
    local BUILD_TYPE="$1"
    local BUILD_DIR="$2"
    echo ""
    echo "==> Step 2/5: Building ${BUILD_TYPE}..."
    cmake -S . -B "${BUILD_DIR}" \
        -DCMAKE_BUILD_TYPE="${BUILD_TYPE}" \
        -DCMAKE_PREFIX_PATH="${HOMEBREW_PREFIX}/opt/cpprestsdk;${HOMEBREW_PREFIX}/opt/protobuf" \
        -DProtobuf_ROOT="${HOMEBREW_PREFIX}/opt/protobuf" \
        -DBOOST_ROOT="${BOOST_1_86_ROOT}" \
        -DBoost_INCLUDE_DIR="${BOOST_1_86_ROOT}" \
        -DBoost_LIBRARY_DIR="${BOOST_1_86_ROOT}/stage/lib" \
        -DBoost_NO_BOOST_CMAKE=ON \
        -DBoost_NO_SYSTEM_PATHS=ON \
        -DCMAKE_CXX_FLAGS="-arch arm64 -std=c++17 -stdlib=libc++ -DBOOST_LOG_DYN_LINK -Wno-deprecated-declarations"
    fix_boost_include_order "${BUILD_DIR}"
    cmake --build "${BUILD_DIR}" -j${JOBS}
}

# 2. 配置并构建
build_variant Debug build-debug
build_variant Release build-release

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
