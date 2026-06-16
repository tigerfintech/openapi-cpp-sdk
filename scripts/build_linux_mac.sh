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
  BUILD_TYPE=Debug|Release          (default: Debug; demo/test build)
  BUILD_TYPES="Debug Release"       Space-separated SDK configs to build (default: "Debug Release")
  SDK_LIBRARY_TYPE=static|shared    (default: static)
  SKIP_INCLUDE_COPY=1               Skip copying headers into SDK_INCLUDE_PREFIX (default: 1)
  INSTALL_PREFIX=<path>             (default: <repo>/output/<OS>)
  LOCAL_OPT_PREFIX=<path>           (default: /usr/local/opt)
  SDK_INCLUDE_PREFIX=<path>         (default: /usr/local/opt/tigerapi)
  CACHE_DIR=<path>                  (default: <repo>/.cache)
  DEPS_DIR=<path>                   (default: <repo>/.deps)
  BOOST_VERSION=1_86_0              Boost version to build if needed
  PROTOBUF_VERSION=v3.21.12         Protobuf tag to checkout
  NUM_JOBS=<n>                      Parallel jobs (default: detected cores)
  SKIP_DEPS=1                       Skip dependency builds (assumes env vars set)
  SKIP_DEMO=1                       Skip demo build + run
  DEMO_ONLY=1                       Skip SDK build and only build/run demo (also via --demo-only)

You may also override BOOST_ROOT, CPPREST_PREFIX, PROTOBUF_PREFIX, SDK_INSTALL_PREFIX, OPENSSL_ROOT_DIR.
EOF
}

DEMO_ONLY="${DEMO_ONLY:-0}"
if [[ $# -gt 0 ]]; then
  for arg in "$@"; do
    case "$arg" in
      -h|--help)
        usage
        exit 0
        ;;
      demo|--demo-only)
        DEMO_ONLY="1"
        ;;
      *)
        fail "Unknown argument: $arg"
        ;;
    esac
  done
fi
[[ "$DEMO_ONLY" == "1" ]] && SKIP_DEMO="0"

[[ "$OS_NAME" == "Linux" || "$OS_NAME" == "Darwin" ]] || fail "Only macOS or Linux are supported."

BUILD_TYPE="${BUILD_TYPE:-Debug}"
BUILD_TYPES="${BUILD_TYPES:-Debug Release}"
SDK_LIBRARY_TYPE="${SDK_LIBRARY_TYPE:-static}"
SKIP_INCLUDE_COPY="${SKIP_INCLUDE_COPY:-1}"
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
BOOST_TARBALL="boost-${BOOST_DOTTED}-b2-nodocs.tar.gz"
BOOST_URL="https://github.com/boostorg/boost/releases/download/boost-${BOOST_DOTTED}/${BOOST_TARBALL}"
LOCAL_OPT_PREFIX="${LOCAL_OPT_PREFIX:-/usr/local/opt}"
BOOST_ROOT="${BOOST_ROOT:-${LOCAL_OPT_PREFIX}/boost_${BOOST_VERSION}}"
CPPREST_PREFIX="${CPPREST_PREFIX:-${LOCAL_OPT_PREFIX}/cpprestsdk}"
# Protobuf: use Homebrew on macOS (5.x), source-built on Linux
if [[ "$OS_NAME" == "Darwin" ]] && command -v brew >/dev/null 2>&1 && [[ -d "$(brew --prefix protobuf 2>/dev/null)/lib/cmake" ]]; then
  PROTOBUF_PREFIX="${PROTOBUF_PREFIX:-$(brew --prefix protobuf)}"
else
  PROTOBUF_VERSION="${PROTOBUF_VERSION:-v3.21.12}"
  PROTOBUF_PREFIX="${PROTOBUF_PREFIX:-${LOCAL_OPT_PREFIX}/protobuf-${PROTOBUF_VERSION}}"
fi
SDK_INSTALL_PREFIX="${SDK_INSTALL_PREFIX:-${INSTALL_PREFIX}}"
SDK_OUTPUT_PREFIX="${INSTALL_PREFIX}"
SDK_INCLUDE_PREFIX="${SDK_INCLUDE_PREFIX:-${SDK_INSTALL_PREFIX}}"

# SUDO_INSTALL will be set to 'sudo' when SDK_INSTALL_PREFIX is not writable
SUDO_INSTALL=""

ensure_prefix_writable() {
  local prefix="$1"
  if [[ -d "$prefix" ]]; then
    if [[ -w "$prefix" ]]; then
      return 0
    fi
    if command -v sudo >/dev/null 2>&1; then
      if sudo mkdir -p "$prefix" 2>/dev/null; then
        SUDO_INSTALL="sudo"
        return 0
      fi
    fi
    warn "No write permission for $prefix and sudo unavailable/failed."
    return 1
  else
    local parent
    parent="$(dirname "$prefix")"
    if [[ -w "$parent" ]]; then
      mkdir -p "$prefix"
      return 0
    fi
    if command -v sudo >/dev/null 2>&1; then
      if sudo mkdir -p "$prefix" 2>/dev/null; then
        SUDO_INSTALL="sudo"
        return 0
      fi
    fi
    warn "Cannot create $prefix (no permission). Try running script with sudo or set SDK_INSTALL_PREFIX to a writable location."
    return 1
  fi
}

ensure_prefix_writable "$SDK_INSTALL_PREFIX" || fail "Unable to prepare SDK install prefix: $SDK_INSTALL_PREFIX"

normalize_library_type() {
  printf '%s' "$1" | tr '[:upper:]' '[:lower:]'
}

SDK_LIBRARY_TYPE="$(normalize_library_type "$SDK_LIBRARY_TYPE")"
case "$SDK_LIBRARY_TYPE" in
  static)
    SDK_BUILD_SHARED="OFF"
    ;;
  shared)
    SDK_BUILD_SHARED="ON"
    ;;
  *)
    fail "SDK_LIBRARY_TYPE must be 'static' or 'shared' (got '$SDK_LIBRARY_TYPE')."
    ;;
esac

export BUILD_TYPE BOOST_ROOT CPPREST_PREFIX PROTOBUF_PREFIX SDK_INSTALL_PREFIX SDK_INCLUDE_PREFIX

find_tiger_library() {
  local sdk_root="$1"
  local lib_dir="${sdk_root%/}/lib"
  [[ -d "$lib_dir" ]] || return 1
  local candidates=(
    "$lib_dir/libtigerapi.a"
    "$lib_dir/libtigerapi.so"
    "$lib_dir/libtigerapi.dylib"
  )
  for candidate in "${candidates[@]}"; do
    if [[ -f "$candidate" ]]; then
      printf '%s' "$candidate"
      return 0
    fi
  done
  return 1
}

ensure_prereqs() {
  local base=(git cmake tar)
  if [[ "$OS_NAME" == "Linux" ]]; then
    local sudo_prefix=(sudo)
    if ! command -v sudo >/dev/null 2>&1; then
      warn "sudo not detected, attempting package manager without it (requires root shell)."
      sudo_prefix=()
    fi
    if command -v apt-get >/dev/null 2>&1; then
      base+=(wget bzip2 unzip gcc g++ make libtool automake autoconf pkg-config)
      base+=(libssl-dev zlib1g-dev)
      if ! dpkg -s build-essential >/dev/null 2>&1; then
        base+=(build-essential)
      fi
      log "Installing base packages via apt."
      "${sudo_prefix[@]}" apt-get update
      "${sudo_prefix[@]}" apt-get install -y "${base[@]}"
    elif command -v dnf >/dev/null 2>&1; then
      base+=(wget bzip2 unzip gcc gcc-c++ make libtool automake autoconf pkgconfig)
      base+=(openssl-devel zlib-devel)
      log "Installing base packages via dnf."
      "${sudo_prefix[@]}" dnf install -y "${base[@]}"
    elif command -v yum >/dev/null 2>&1; then
      base+=(wget bzip2 unzip gcc gcc-c++ make libtool automake autoconf pkgconfig)
      base+=(openssl-devel zlib-devel)
      log "Installing base packages via yum."
      "${sudo_prefix[@]}" yum install -y "${base[@]}"
    else
      fail "No supported package manager found (apt-get / dnf / yum). Install dependencies manually."
    fi
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

## Check if a system/package-manager Boost meets our version window (>= 1.86, < 1.90).
## Returns 0 and sets BOOST_ROOT if acceptable, 1 otherwise.
_check_system_boost() {
  local include_dir="$1"
  [[ -f "${include_dir}/boost/version.hpp" ]] || return 1
  local ver_num
  ver_num=$(grep -m1 '#define BOOST_VERSION ' "${include_dir}/boost/version.hpp" | awk '{print $3}')
  [[ -n "$ver_num" ]] || return 1
  local major=$(( ver_num / 100000 ))
  local minor=$(( (ver_num / 100) % 1000 ))
  # Require >= 1.86. cpprestsdk is always built from source against the same BOOST_ROOT,
  # so any version >= 1.86 is ABI-consistent end-to-end.
  if (( major > 1 || (major == 1 && minor >= 86) )); then
    log "Found compatible system Boost ${major}.${minor} at ${include_dir%/include}"
    return 0
  fi
  warn "System Boost ${major}.${minor} < 1.86; will build 1.86 from source."
  return 1
}

build_boost() {
  if [[ -d "${BOOST_ROOT}/include/boost" ]]; then
    log "Boost already available at ${BOOST_ROOT}"
    return
  fi

  # Probe system/package-manager Boost first (fast path).
  local candidate_roots=()
  if [[ "$OS_NAME" == "Darwin" ]] && command -v brew >/dev/null 2>&1; then
    local brew_boost
    brew_boost="$(brew --prefix boost 2>/dev/null || true)"
    [[ -n "$brew_boost" ]] && candidate_roots+=("$brew_boost")
  fi
  # Common system paths on Linux
  candidate_roots+=("/usr" "/usr/local")

  for root in "${candidate_roots[@]}"; do
    if _check_system_boost "${root}/include"; then
      BOOST_ROOT="$root"
      export BOOST_ROOT
      return
    fi
  done

  log "Building Boost ${BOOST_VERSION} from source..."
  # b2-nodocs tarball extracts to boost-<dotted> (e.g. boost-1.86.0), not boost_1_86_0.
  local src_dir="${CACHE_DIR}/boost-${BOOST_DOTTED}"
  if [[ ! -d "$src_dir" ]]; then
    if [[ ! -f "${CACHE_DIR}/${BOOST_TARBALL}" ]]; then
      wget -O "${CACHE_DIR}/${BOOST_TARBALL}" "$BOOST_URL" || {
        rm -f "${CACHE_DIR}/${BOOST_TARBALL}"
        fail "Failed to download Boost from ${BOOST_URL}"
      }
    fi
    tar -C "$CACHE_DIR" -xzf "${CACHE_DIR}/${BOOST_TARBALL}"
    # Verify the expected directory was produced by the tarball.
    if [[ ! -d "$src_dir" ]]; then
      # Tarball may have extracted to a differently named directory; detect it.
      local actual_dir
      actual_dir="$(tar -tzf "${CACHE_DIR}/${BOOST_TARBALL}" 2>/dev/null | head -1 | cut -d/ -f1)"
      if [[ -n "$actual_dir" && -d "${CACHE_DIR}/${actual_dir}" ]]; then
        mv "${CACHE_DIR}/${actual_dir}" "$src_dir"
      else
        fail "Boost source directory not found after extraction (expected ${src_dir})"
      fi
    fi
  fi
  if [[ ! -f "${src_dir}/bootstrap.sh" ]]; then
    fail "bootstrap.sh not found in ${src_dir} — tarball may be corrupt. Delete ${CACHE_DIR}/${BOOST_TARBALL} and retry."
  fi

  # Check write permission on BOOST_ROOT parent before b2 install attempts mkdir.
  local boost_parent
  boost_parent="$(dirname "$BOOST_ROOT")"
  if [[ ! -w "$boost_parent" ]]; then
    fail "No write permission for ${BOOST_ROOT}.
  Option 1: run with sudo:  sudo BOOST_ROOT=${BOOST_ROOT} $0
  Option 2: writable path:  BOOST_ROOT=\$HOME/boost_${BOOST_VERSION} $0"
  fi

  pushd "$src_dir" >/dev/null
  ./bootstrap.sh
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
  local cmake_cxx_flags="-w"
  [[ "$OS_NAME" == "Darwin" ]] && cmake_cxx_flags="-Wno-error=null-pointer-subtraction -w"
  cmake -S . -B build \
    -G "Unix Makefiles" \
    -DCMAKE_BUILD_TYPE="$BUILD_TYPE" \
    -DCMAKE_INSTALL_PREFIX="$CPPREST_PREFIX" \
    -DBUILD_SHARED_LIBS=ON \
    -DBUILD_SAMPLES=OFF \
    -DBUILD_TESTS=OFF \
    -DBOOST_ROOT="$BOOST_ROOT" \
    -DCMAKE_CXX_FLAGS="$cmake_cxx_flags" \
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
  [[ -n "${PROTOBUF_VERSION:-}" ]] || fail "PROTOBUF_VERSION is not set; cannot build protobuf from source."
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
  local configs=()
  for cfg in $BUILD_TYPES; do
    configs+=("$cfg")
  done
  if [[ ${#configs[@]} -eq 0 ]]; then
    configs=("$BUILD_TYPE")
  fi

  for cfg in "${configs[@]}"; do
    local build_dir="${PROJECT_ROOT}/build/${cfg}"
    local install_dir="${SDK_INSTALL_PREFIX}/${cfg}"
    local output_dir="${SDK_OUTPUT_PREFIX}/${cfg}"
    # Remove stale CMakeCache.txt to prevent cached Protobuf_ROOT/Protobuf_DIR
    # from a previous build overriding the values we pass here.
    rm -f "${build_dir}/CMakeCache.txt"
    # Find the correct cmake config directory for protobuf (layout differs between
    # source-built installs and Homebrew).
    local protobuf_cmake_dir="${PROTOBUF_PREFIX}/lib/cmake/protobuf"
    if [[ ! -f "${protobuf_cmake_dir}/protobuf-config.cmake" ]]; then
      # Homebrew 5.x places cmake configs directly under lib/cmake/protobuf
      # but the lib dir itself may be lib64 on some systems; try share/ as fallback.
      for candidate in \
        "${PROTOBUF_PREFIX}/lib/cmake/protobuf" \
        "${PROTOBUF_PREFIX}/lib64/cmake/protobuf" \
        "${PROTOBUF_PREFIX}/share/cmake/protobuf"; do
        if [[ -f "${candidate}/protobuf-config.cmake" ]]; then
          protobuf_cmake_dir="$candidate"
          break
        fi
      done
    fi
    cmake -S "$PROJECT_ROOT" -B "$build_dir" \
      -DCMAKE_BUILD_TYPE="$cfg" \
      -DCMAKE_INSTALL_PREFIX="$install_dir" \
      -DBOOST_ROOT="$BOOST_ROOT" \
      -DProtobuf_DIR="${protobuf_cmake_dir}" \
      -DCMAKE_PREFIX_PATH="${PROTOBUF_PREFIX};${CPPREST_PREFIX};${BOOST_ROOT}" \
      -DBUILD_SHARED_LIBS="$SDK_BUILD_SHARED" \
      ${OPENSSL_ROOT_DIR:+-DOPENSSL_ROOT_DIR="$OPENSSL_ROOT_DIR"}
    cmake --build "$build_dir" -- -j "$NUM_JOBS"
    ${SUDO_INSTALL:+$SUDO_INSTALL} cmake --install "$build_dir"
    log "SDK (${cfg}) installed to ${install_dir}"
  done
}

build_demo() {
  local demo_dir="${PROJECT_ROOT}/demo"
  local build_dir="${demo_dir}/build"
  local active_cfg="$BUILD_TYPE"
  local demo_sdk_root="${SDK_INSTALL_PREFIX}/${active_cfg}"
  if [[ ! -d "$demo_sdk_root" ]]; then
    warn "Demo requested configuration '$active_cfg' not built (missing ${demo_sdk_root})."
    warn "Ensure BUILD_TYPES includes $active_cfg or rerun with BUILD_TYPES=\"$active_cfg\"."
    return
  fi
  local tigerapi_library
  tigerapi_library="$(find_tiger_library "$demo_sdk_root" || true)"
  if [[ -z "$tigerapi_library" ]]; then
    warn "Unable to locate tigerapi library under ${demo_sdk_root}. Skipping demo build."
    return
  fi
  local cpprest_include="${CPPREST_PREFIX}/include"
  local cpprest_library=""
  for candidate in \
    "${CPPREST_PREFIX}/lib/libcpprest.so" \
    "${CPPREST_PREFIX}/lib/libcpprest.dylib"; do
    if [[ -f "$candidate" ]]; then
      cpprest_library="$candidate"
      break
    fi
  done
  local protobuf_include="${PROTOBUF_PREFIX}/include"
  local protobuf_library=""
  for candidate in \
    "${PROTOBUF_PREFIX}/lib/libprotobuf.so" \
    "${PROTOBUF_PREFIX}/lib/libprotobuf.dylib"; do
    if [[ -f "$candidate" ]]; then
      protobuf_library="$candidate"
      break
    fi
  done

  local cmake_args=(
    -S "$demo_dir"
    -B "$build_dir"
    -DCMAKE_BUILD_TYPE="$active_cfg"
    -DBOOST_ROOT="$BOOST_ROOT"
    -DCMAKE_PREFIX_PATH="${demo_sdk_root};${CPPREST_PREFIX};${PROTOBUF_PREFIX};${BOOST_ROOT}"
    -DTIGERAPI_INCLUDE_DIR="${demo_sdk_root}/include"
    -DTIGERAPI_LIBRARY="$tigerapi_library"
  )
  [[ -d "$cpprest_include" ]] && cmake_args+=(-DCPPREST_INCLUDE_DIR="$cpprest_include")
  [[ -n "$cpprest_library" ]] && cmake_args+=(-DCPPREST_LIBRARY="$cpprest_library")
  [[ -d "$protobuf_include" ]] && cmake_args+=(-DProtobuf_INCLUDE_DIR="$protobuf_include")
  [[ -n "$protobuf_library" ]] && cmake_args+=(-DProtobuf_LIBRARY="$protobuf_library")
  [[ -n "${OPENSSL_ROOT_DIR:-}" ]] && cmake_args+=(-DOPENSSL_ROOT_DIR="$OPENSSL_ROOT_DIR")

  cmake "${cmake_args[@]}"
  cmake --build "$build_dir" -- -j "$NUM_JOBS"
  local runtime_var="LD_LIBRARY_PATH"
  local runtime_value="${LD_LIBRARY_PATH:-}"
  if [[ "$OS_NAME" == "Darwin" ]]; then
    runtime_var="DYLD_LIBRARY_PATH"
    runtime_value="${DYLD_LIBRARY_PATH:-}"
  fi
  local runtime_paths=()
  for candidate in "${demo_sdk_root}/lib" "${CPPREST_PREFIX}/lib" "${PROTOBUF_PREFIX}/lib"; do
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
  if [[ "${SKIP_INCLUDE_COPY}" == "1" ]]; then
    warn "Header relocation skipped (SKIP_INCLUDE_COPY=1)."
    return
  fi
  local configs=()
  for cfg in $BUILD_TYPES; do
    configs+=("$cfg")
  done
  local src_dir=""
  for cfg in "${configs[@]}"; do
    local candidate="${SDK_INSTALL_PREFIX}/${cfg}/include"
    if [[ -d "$candidate" ]]; then
      src_dir="$candidate"
      break
    fi
  done
  [[ -n "$src_dir" ]] || return

  local dest_root="${SDK_INCLUDE_PREFIX%/}"
  local dest_dir="${dest_root}/include"

  if ! mkdir -p "$dest_dir" 2>/dev/null; then
    if [[ -n "$SUDO_INSTALL" ]]; then
      $SUDO_INSTALL mkdir -p "$dest_dir"
    else
      warn "Cannot write headers to ${dest_dir}; leaving copies in ${src_dir}. Try re-running with sudo or set SDK_INCLUDE_PREFIX."
      return
    fi
  fi
  if [[ -z "$dest_dir" || "$dest_dir" == "/" ]]; then
    warn "Refusing to remove invalid destination path ($dest_dir); leaving headers in ${src_dir}."
    return
  fi
  if [[ -n "$SUDO_INSTALL" ]]; then
    $SUDO_INSTALL rm -rf "$dest_dir"
    $SUDO_INSTALL mkdir -p "$dest_dir"
    $SUDO_INSTALL cp -R "$src_dir/." "$dest_dir/"
  else
    rm -rf "$dest_dir"
    mkdir -p "$dest_dir"
    cp -R "$src_dir/." "$dest_dir/"
  fi
  log "Headers installed to ${dest_dir}"
}

main() {
  log "Tiger OpenAPI SDK builder started (OS=$OS_NAME, type=$BUILD_TYPE, jobs=$NUM_JOBS)"
  if [[ "$DEMO_ONLY" == "1" ]]; then
    warn "Demo-only mode enabled: skipping dependency and SDK build steps."
  else
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
  fi
  if [[ "$DEMO_ONLY" == "1" || "${SKIP_DEMO:-0}" != "1" ]]; then
    build_demo
  else
    warn "Demo build disabled (SKIP_DEMO=1)."
  fi
  local header_dir="${SDK_INCLUDE_PREFIX%/}/include"
  local preferred_cfg="$BUILD_TYPE"
  local sample_lib_dir="${SDK_OUTPUT_PREFIX}/${preferred_cfg}/lib"
  if [[ ! -d "$sample_lib_dir" ]]; then
    local cfgs=()
    for candidate_cfg in $BUILD_TYPES; do
      cfgs+=("$candidate_cfg")
    done
    for candidate in "${cfgs[@]}"; do
      if [[ -d "${SDK_OUTPUT_PREFIX}/${candidate}/lib" ]]; then
        preferred_cfg="$candidate"
        sample_lib_dir="${SDK_OUTPUT_PREFIX}/${candidate}/lib"
        break
      fi
    done
  fi
  [[ -d "$sample_lib_dir" ]] || sample_lib_dir="${SDK_OUTPUT_PREFIX}"
  local demo_mode_status="disabled"
  [[ "$DEMO_ONLY" == "1" ]] && demo_mode_status="enabled"
  cat <<EOF
============================================
Tiger OpenAPI SDK build finished.
  SDK configs built: $BUILD_TYPES
  Install root      : $SDK_INSTALL_PREFIX (per-config subfolders)
  Output libs       : $SDK_OUTPUT_PREFIX (per-config subfolders)
  Library type      : $SDK_LIBRARY_TYPE
  Demo-only mode    : $demo_mode_status
  Sample libs       : $sample_lib_dir
  Headers copied to: $header_dir (skip copy: $SKIP_INCLUDE_COPY)
  Boost root      : $BOOST_ROOT
  cpprestsdk      : $CPPREST_PREFIX
  Protobuf        : $PROTOBUF_PREFIX

To use the SDK, add the following to your environment:
  export CPATH="$header_dir:$BOOST_ROOT/include:$CPPREST_PREFIX/include:$PROTOBUF_PREFIX/include:\$CPATH"
  export LIBRARY_PATH="$sample_lib_dir:$CPPREST_PREFIX/lib:$PROTOBUF_PREFIX/lib:\$LIBRARY_PATH"
  export LD_LIBRARY_PATH="$sample_lib_dir:$CPPREST_PREFIX/lib:$PROTOBUF_PREFIX/lib:\$LD_LIBRARY_PATH"
(For macOS, replace LD_LIBRARY_PATH with DYLD_LIBRARY_PATH.)
============================================
EOF
}

main "$@"
