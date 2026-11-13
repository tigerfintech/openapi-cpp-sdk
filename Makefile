# Makefile — convenience targets for TigerOpen C++ SDK tests
#
# Usage:
#   make test                       # build + run unit tests
#   make integ                      # build + run integration tests (needs real creds)
#   make build-tests                # cmake configure + compile only
#   make clean-tests                # remove test build directory
#
# Integration tests require:
#   export TIGER_TEST_ID=...
#   export TIGER_TEST_PRIVATE_KEY=...
#   export TIGER_TEST_ACCOUNT=...

BUILD_DIR ?= build_tests
BUILD_TYPE ?= Debug
NPROC ?= $(shell nproc 2>/dev/null || sysctl -n hw.logicalcpu 2>/dev/null || echo 4)

.PHONY: test integ build-tests clean-tests help

help:
	@echo "Targets:"
	@echo "  test         Build and run unit tests (no credentials)"
	@echo "  integ        Build and run integration tests (requires TIGER_TEST_* env vars)"
	@echo "  build-tests  CMake configure + compile"
	@echo "  clean-tests  Remove build directory"

build-tests:
	cmake -S . -B $(BUILD_DIR) \
		-DCMAKE_BUILD_TYPE=$(BUILD_TYPE) \
		-DBUILD_TESTS=ON \
		$(if $(BOOST_ROOT),-DBOOST_ROOT=$(BOOST_ROOT),) \
		$(if $(PROTOBUF_PREFIX),-DPROTOBUF_PREFIX=$(PROTOBUF_PREFIX),) \
		$(if $(ABSL_PREFIX),-DABSL_PREFIX=$(ABSL_PREFIX),)
	cmake --build $(BUILD_DIR) --target unit_tests integ_tests -j$(NPROC)

test: build-tests
	ctest --test-dir $(BUILD_DIR) -R UnitTests -V --output-on-failure

integ: build-tests
	TIGER_RUN_INTEG=true ctest --test-dir $(BUILD_DIR) -R IntegTests -V --output-on-failure

clean-tests:
	rm -rf $(BUILD_DIR)
