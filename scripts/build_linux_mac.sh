#!/usr/bin/env bash
set -euo pipefail

########################################
# Tiger OpenAPI C++ SDK one-click builder
########################################

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="${PROJECT_ROOT:-$(cd "${SCRIPT_DIR}/.." && pwd)}"
OS_NAME="$(uname -s)"

log() { printf "\033[1;32m[+]\033[0m %s\n" "$*"; }
warn() { printf "\033[1;33m[!]\033[0m %s\n" "$*"; }
err() { printf "\033[1;31m[x]\033[0m %s\n" "$*" 1>&2; }
fail() { err "$*"; exit 1; }

require_cmd() {
  local cmd="$1"
  command -v "$cmd" >/dev/null 2>&1 || fail "Command '$cmd' is required but missing."
}

cpu_cores() {
  if command -v nproc >/dev/null 2>&1; then
    nproc
  elif [[ "$OS_NAME" == "Darwin" ]]; then
    sysctl -n hw.ncpu
  else
    printf "2"
  fi
}

usage() {
  cat <<'EOF'
Usage: scripts/build_linux_mac.sh [options]

Environment variables / options:
  BUILD_TYPE=Debug|Release          (default: Debug)
  INSTALL_PREFIX=<path>             (default: <repo>/output/<OS>)
  LOCAL_OPT_PREFIX=<path>           (default: /usr/local/opt)
  SDK_INCLUDE_PREFIX=<path>         (default: /usr/local/opt/tigerapi-sdk)
  CACHE_DIR=<path>                  (default: <repo>/.cache)
  DEPS_DIR=<path>                   (default: <repo>/.deps)
  BOOST_VERSION=1_86_0              Boost version to build if needed
  PROTOBUF_VERSION=v3.21.12         Protobuf tag to checkout
  NUM_JOBS=<n>                      Parallel jobs (default: detected cores)
  SKIP_DEPS=1                       Skip dependency builds (assumes env vars set)
  SKIP_DEMO=1                       Skip demo build + run

You may also override BOOST_ROOT, CPPREST_PREFIX, PROTOBUF_PREFIX, SDK_INSTALL_PREFIX, OPENSSL_ROOT_DIR.
EOF
}

if [[ "${1:-}" == "-h" || "${1:-}" == "--help" ]]; then
  usage
  exit 0
fi

[[ "$OS_NAME" == "Linux" || "$OS_NAME" == "Darwin" ]] || fail "Only macOS or Linux are supported."

BUILD_TYPE="${BUILD_TYPE:-Debug}"
OUTPUT_OS_DIR="Mac"
[[ "$OS_NAME" == "Linux" ]] && OUTPUT_OS_DIR="Linux"
DEFAULT_INSTALL_PREFIX="${PROJECT_ROOT}/output/${OUTPUT_OS_DIR}"
INSTALL_PREFIX="${INSTALL_PREFIX:-$DEFAULT_INSTALL_PREFIX}"
mkdir -p "$INSTALL_PREFIX"
CACHE_DIR="${CACHE_DIR:-${PROJECT_ROOT}/.cache}"; mkdir -p "$CACHE_DIR"
DEPS_DIR="${DEPS_DIR:-${PROJECT_ROOT}/.deps}"; mkdir -p "$DEPS_DIR"
NUM_JOBS="${NUM_JOBS:-$(cpu_cores)}"
BOOST_VERSION="${BOOST_VERSION:-1_86_0}"
BOOST_DOTTED="${BOOST_VERSION//_/.}"
BOOST_TARBALL="boost_${BOOST_VERSION}.tar.bz2"
BOOST_URL="https://archives.boost.io/release/${BOOST_DOTTED}/source/${BOOST_TARBALL}"
LOCAL_OPT_PREFIX="${LOCAL_OPT_PREFIX:-/usr/local/opt}"
BOOST_ROOT="${BOOST_ROOT:-${LOCAL_OPT_PREFIX}/boost_${BOOST_VERSION}}"
CPPREST_PREFIX="${CPPREST_PREFIX:-${LOCAL_OPT_PREFIX}/cpprestsdk}"
PROTOBUF_VERSION="${PROTOBUF_VERSION:-v3.21.12}"
PROTOBUF_PREFIX="${PROTOBUF_PREFIX:-${LOCAL_OPT_PREFIX}/protobuf-${PROTOBUF_VERSION}}"
SDK_INSTALL_PREFIX="${SDK_INSTALL_PREFIX:-${INSTALL_PREFIX}/tigerapi-sdk}"
SDK_INCLUDE_PREFIX="${SDK_INCLUDE_PREFIX:-/usr/local/opt/tigerapi-sdk}"
export BUILD_TYPE BOOST_ROOT CPPREST_PREFIX PROTOBUF_PREFIX SDK_INSTALL_PREFIX SDK_INCLUDE_PREFIX

ensure_prereqs() {
  local base=(git cmake tar)
  if [[ "$OS_NAME" == "Linux" ]]; then
    base+=(wget bzip2 unzip gcc g++ make libtool automake autoconf pkg-config)
    base+=(libssl-dev zlib1g-dev)
    if ! dpkg -s build-essential >/dev/null 2>&1; then
      base+=(build-essential)
    fi
    local sudo_prefix=(sudo)
    if ! command -v sudo >/dev/null 2>&1; then
      warn "sudo not detected, attempting apt-get without it (requires root shell)."
      sudo_prefix=()
    fi
    log "Installing base packages via apt."
    "${sudo_prefix[@]}" apt-get update
    "${sudo_prefix[@]}" apt-get install -y "${base[@]}"
  else
    if ! command -v brew >/dev/null 2>&1; then
      fail "Homebrew is required on macOS. Install from https://brew.sh first."
    fi
    require_cmd xcode-select
    log "Ensuring core tools via Homebrew."
    export HOMEBREW_NO_AUTO_UPDATE=1
    export HOMEBREW_NO_INSTALL_CLEANUP=1
    export HOMEBREW_NO_ENV_HINTS=1
    export HOMEBREW_NO_BOTTLE_SOURCE_FALLBACK=1
    if ! HOMEBREW_NO_BOTTLE_SOURCE_FALLBACK=1 \
      brew install --formula --force-bottle cmake wget automake libtool pkg-config openssl@3; then
      warn "Homebrew bottle install failed; please install cmake/wget/automake/libtool/pkg-config/openssl@3 manually to avoid source tests."
    fi
  fi
}

detect_openssl() {
  if [[ -n "${OPENSSL_ROOT_DIR:-}" ]]; then
    warn "Using provided OPENSSL_ROOT_DIR=$OPENSSL_ROOT_DIR"
    return
  fi
  if [[ "$OS_NAME" == "Darwin" ]]; then
    if command -v brew >/dev/null 2>&1 && brew --prefix openssl@3 >/dev/null 2>&1; then
      OPENSSL_ROOT_DIR="$(brew --prefix openssl@3)"
    elif [[ -d /opt/homebrew/opt/openssl@3 ]]; then
      OPENSSL_ROOT_DIR="/opt/homebrew/opt/openssl@3"
    else
      OPENSSL_ROOT_DIR="/usr/local/opt/openssl@3"
    fi
  else
    OPENSSL_ROOT_DIR="/usr"
  fi
  local lib_ext="so"
  [[ "$OS_NAME" == "Darwin" ]] && lib_ext="dylib"
  export OPENSSL_ROOT_DIR
  export OPENSSL_INCLUDE_DIR="${OPENSSL_ROOT_DIR}/include"
  export OPENSSL_CRYPTO_LIBRARY="${OPENSSL_ROOT_DIR}/lib/libcrypto.${lib_ext}"
  export OPENSSL_SSL_LIBRARY="${OPENSSL_ROOT_DIR}/lib/libssl.${lib_ext}"
  log "OpenSSL detected at ${OPENSSL_ROOT_DIR}"
}

build_boost() {
  if [[ -d "${BOOST_ROOT}/include/boost" ]]; then
    log "Boost already available at ${BOOST_ROOT}"
    return
  fi
  if [[ "$OS_NAME" == "Darwin" ]]; then
    if command -v brew >/dev/null 2>&1 && brew list boost >/dev/null 2>&1; then
      BOOST_ROOT="$(brew --prefix boost)"
      export BOOST_ROOT
      log "Using Homebrew boost at ${BOOST_ROOT}"
      return
    fi
  fi
  log "Building Boost ${BOOST_VERSION} from source..."
  local src_dir="${CACHE_DIR}/boost_${BOOST_VERSION}"
  if [[ ! -d "$src_dir" ]]; then
    [[ -f "${CACHE_DIR}/${BOOST_TARBALL}" ]] || wget -O "${CACHE_DIR}/${BOOST_TARBALL}" "$BOOST_URL"
    tar -C "$CACHE_DIR" --bzip2 -xf "${CACHE_DIR}/${BOOST_TARBALL}"
  fi
  pushd "$src_dir" >/dev/null
  if ! ./bootstrap.sh --prefix "$BOOST_ROOT"; then
    warn "Boost bootstrap.sh does not accept --prefix, falling back to default staging."
    ./bootstrap.sh
  fi
  ./b2 headers
  ./b2 -j "$NUM_JOBS" --prefix="$BOOST_ROOT" install
  popd >/dev/null
}

build_cpprestsdk() {
  if [[ -f "${CPPREST_PREFIX}/lib/libcpprest.so" || -f "${CPPREST_PREFIX}/lib/libcpprest.dylib" ]]; then
    log "cpprestsdk already installed at ${CPPREST_PREFIX}"
    return
  fi
  local repo="${DEPS_DIR}/cpprestsdk"
  if [[ ! -d "$repo/.git" ]]; then
    git clone https://github.com/microsoft/cpprestsdk.git "$repo"
  fi
  pushd "$repo" >/dev/null
  git submodule update --init
  cmake -S . -B build \
    -G "Unix Makefiles" \
    -DCMAKE_BUILD_TYPE="$BUILD_TYPE" \
    -DCMAKE_INSTALL_PREFIX="$CPPREST_PREFIX" \
    -DBUILD_SHARED_LIBS=ON \
    -DBUILD_SAMPLES=OFF \
    -DBUILD_TESTS=OFF \
    -DBOOST_ROOT="$BOOST_ROOT" \
    -DCMAKE_CXX_FLAGS="-Wno-error=null-pointer-subtraction -w" \
    ${OPENSSL_ROOT_DIR:+-DOPENSSL_ROOT_DIR="$OPENSSL_ROOT_DIR"} \
    ${OPENSSL_INCLUDE_DIR:+-DOPENSSL_INCLUDE_DIR="$OPENSSL_INCLUDE_DIR"} \
    ${OPENSSL_SSL_LIBRARY:+-DOPENSSL_SSL_LIBRARY="$OPENSSL_SSL_LIBRARY"} \
    ${OPENSSL_CRYPTO_LIBRARY:+-DOPENSSL_CRYPTO_LIBRARY="$OPENSSL_CRYPTO_LIBRARY"}
  cmake --build build -- -j "$NUM_JOBS"
  cmake --install build
  popd >/dev/null
}

build_protobuf() {
  if [[ -f "${PROTOBUF_PREFIX}/lib/libprotobuf.so" || -f "${PROTOBUF_PREFIX}/lib/libprotobuf.dylib" ]]; then
    log "Protobuf already installed at ${PROTOBUF_PREFIX}"
    return
  fi
  local repo="${DEPS_DIR}/protobuf"
  if [[ ! -d "$repo/.git" ]]; then
    git clone https://github.com/protocolbuffers/protobuf "$repo"
  fi
  pushd "$repo" >/dev/null
  git fetch --tags
  git checkout "$PROTOBUF_VERSION"
  mkdir -p cmake_build
  cmake -S . -B cmake_build \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX="$PROTOBUF_PREFIX" \
    -Dprotobuf_BUILD_TESTS=OFF \
    -Dprotobuf_BUILD_SHARED_LIBS=ON
  cmake --build cmake_build -- -j "$NUM_JOBS"
  cmake --install cmake_build
  popd >/dev/null
}

regenerate_protos() {
  if [[ "${SKIP_PROTO_REGEN:-0}" == "1" ]]; then
    warn "Skipping protobuf regeneration (SKIP_PROTO_REGEN=1)."
    return
  fi

  local proto_src="${PROJECT_ROOT}/include/openapi_pb/pb_file"
  local proto_out="${PROJECT_ROOT}/include/openapi_pb/pb_source"
  local protoc_bin="${PROTOC_BIN:-${PROTOBUF_PREFIX}/bin/protoc}"
  if [[ ! -x "$protoc_bin" ]]; then
    protoc_bin="$(command -v protoc || true)"
  fi
  [[ -x "$protoc_bin" ]] || fail "Unable to locate protoc (looked for ${PROTOBUF_PREFIX}/bin/protoc)."

  if [[ ! -d "$proto_src" ]]; then
    warn "Protobuf source directory ${proto_src} not found; skipping regeneration."
    return
  fi

  log "Regenerating protobuf sources via ${protoc_bin}."
  rm -rf "$proto_out"
  mkdir -p "$proto_out"
  pushd "$proto_src" >/dev/null
  local proto_files=()
  while IFS= read -r proto; do
    proto_files+=("$proto")
  done < <(find . -type f -name '*.proto' ! -path './google/*' -print | sort)
  if [[ ${#proto_files[@]} -eq 0 ]]; then
    popd >/dev/null
    fail "No .proto files discovered in ${proto_src}."
  fi
  local proto_args=(--proto_path=.)
  if [[ -d "${PROTOBUF_PREFIX}/include" ]]; then
    proto_args+=("--proto_path=${PROTOBUF_PREFIX}/include")
  fi
  "$protoc_bin" \
    "${proto_args[@]}" \
    --cpp_out="$proto_out" \
    "${proto_files[@]}"
  popd >/dev/null
}

build_sdk() {
  local build_dir="${PROJECT_ROOT}/build"
  cmake -S "$PROJECT_ROOT" -B "$build_dir" \
    -DCMAKE_BUILD_TYPE="$BUILD_TYPE" \
    -DCMAKE_INSTALL_PREFIX="$SDK_INSTALL_PREFIX" \
    -DBOOST_ROOT="$BOOST_ROOT" \
    -DProtobuf_DIR="${PROTOBUF_PREFIX}/lib/cmake/protobuf" \
    -DCMAKE_PREFIX_PATH="${CPPREST_PREFIX};${PROTOBUF_PREFIX};${BOOST_ROOT}" \
    ${OPENSSL_ROOT_DIR:+-DOPENSSL_ROOT_DIR="$OPENSSL_ROOT_DIR"}
  cmake --build "$build_dir" -- -j "$NUM_JOBS"
  cmake --install "$build_dir"
}

build_demo() {
  local demo_dir="${PROJECT_ROOT}/demo"
  local build_dir="${demo_dir}/build"
  cmake -S "$demo_dir" -B "$build_dir" \
    -DCMAKE_BUILD_TYPE="$BUILD_TYPE" \
    -DBOOST_ROOT="$BOOST_ROOT" \
    -DCMAKE_PREFIX_PATH="${SDK_INSTALL_PREFIX};${CPPREST_PREFIX};${PROTOBUF_PREFIX};${BOOST_ROOT}" \
    -DTIGERAPI_INCLUDE_DIR="${SDK_INCLUDE_PREFIX%/}/include" \
    -DTIGERAPI_LIBRARY="${SDK_INSTALL_PREFIX}/lib/libtigerapi.a" \
    ${OPENSSL_ROOT_DIR:+-DOPENSSL_ROOT_DIR="$OPENSSL_ROOT_DIR"}
  cmake --build "$build_dir" -- -j "$NUM_JOBS"
  local runtime_var="LD_LIBRARY_PATH"
  local runtime_value="${LD_LIBRARY_PATH:-}"
  if [[ "$OS_NAME" == "Darwin" ]]; then
    runtime_var="DYLD_LIBRARY_PATH"
    runtime_value="${DYLD_LIBRARY_PATH:-}"
  fi
  local runtime_paths=()
  for candidate in "${SDK_INSTALL_PREFIX}/lib" "${CPPREST_PREFIX}/lib" "${PROTOBUF_PREFIX}/lib"; do
    [[ -d "$candidate" ]] && runtime_paths+=("$candidate")
  done
  if [[ ${#runtime_paths[@]} -gt 0 ]]; then
    local joined
    joined="$(IFS=:; echo "${runtime_paths[*]}")"
    if [[ -n "$runtime_value" ]]; then
      runtime_value="${joined}:${runtime_value}"
    else
      runtime_value="$joined"
    fi
  fi
  env "${runtime_var}=${runtime_value}" "${build_dir}/openapi_cpp_test" || warn "Demo run failed; check configuration."
}

relocate_headers() {
  local src_dir="${SDK_INSTALL_PREFIX}/include"
  [[ -d "$src_dir" ]] || return

  local dest_root="${SDK_INCLUDE_PREFIX%/}"
  local dest_dir="${dest_root}/include"

  if mkdir -p "$dest_dir" 2>/dev/null; then
    if [[ -z "$dest_dir" || "$dest_dir" == "/" ]]; then
      warn "Refusing to remove invalid destination path ($dest_dir); leaving headers in ${src_dir}."
      return
    fi
    rm -rf "$dest_dir"
    mkdir -p "$dest_dir"
    cp -R "$src_dir/." "$dest_dir/"
    rm -rf "$src_dir"
    log "Headers installed to ${dest_dir}"
  else
    warn "Cannot write headers to ${dest_dir}; leaving copies in ${src_dir}. Try re-running with sudo or set SDK_INCLUDE_PREFIX."
  fi
}

main() {
  log "Tiger OpenAPI SDK builder started (OS=$OS_NAME, type=$BUILD_TYPE, jobs=$NUM_JOBS)"
  if [[ "${SKIP_DEPS:-0}" != "1" ]]; then
    ensure_prereqs
    detect_openssl
    build_boost
    build_cpprestsdk
    build_protobuf
  else
    warn "Skipping dependency build because SKIP_DEPS=1. Ensure BOOST_ROOT, CPPREST_PREFIX, PROTOBUF_PREFIX are set."
  fi
  regenerate_protos
  build_sdk
  relocate_headers
  if [[ "${SKIP_DEMO:-0}" != "1" ]]; then
    build_demo
  else
    warn "Demo build disabled (SKIP_DEMO=1)."
  fi
  local header_dir="${SDK_INCLUDE_PREFIX%/}/include"
  cat <<EOF
============================================
Tiger OpenAPI SDK build finished.
  SDK installed to: $SDK_INSTALL_PREFIX
  Headers copied to: $header_dir
  Boost root      : $BOOST_ROOT
  cpprestsdk      : $CPPREST_PREFIX
  Protobuf        : $PROTOBUF_PREFIX

To use the SDK, add the following to your environment:
  export CPATH="$header_dir:$BOOST_ROOT/include:$CPPREST_PREFIX/include:$PROTOBUF_PREFIX/include:\$CPATH"
  export LIBRARY_PATH="$SDK_INSTALL_PREFIX/lib:$CPPREST_PREFIX/lib:$PROTOBUF_PREFIX/lib:\$LIBRARY_PATH"
  export LD_LIBRARY_PATH="$SDK_INSTALL_PREFIX/lib:$CPPREST_PREFIX/lib:$PROTOBUF_PREFIX/lib:\$LD_LIBRARY_PATH"
(For macOS, replace LD_LIBRARY_PATH with DYLD_LIBRARY_PATH.)
============================================
EOF
}

main "$@"
