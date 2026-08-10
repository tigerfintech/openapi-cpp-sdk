cmake_minimum_required(VERSION 3.15)

# ── Coverage ──────────────────────────────────────────────────────────────────
option(ENABLE_COVERAGE "Enable code coverage instrumentation (gcov)" OFF)
if(ENABLE_COVERAGE)
    # The parent CMakeLists.txt overwrites CMAKE_CXX_FLAGS for Clang, so we
    # must add coverage flags to the *target* rather than to the global flags.
    # --coverage = -fprofile-arcs -ftest-coverage (GCC-compatible, works with Clang)
    set(COVERAGE_CXX_FLAGS "--coverage;-fprofile-arcs;-ftest-coverage")
    set(COVERAGE_LINK_FLAGS "--coverage")
endif()

# ── GoogleTest ────────────────────────────────────────────────────────────────
find_package(GTest CONFIG QUIET)
if(NOT GTest_FOUND)
    find_package(GTest REQUIRED)   # system-installed fallback (e.g. libgtest-dev)
endif()

# common_test_setup: apply include / link / compile-def settings to a test target.
# Re-uses variables already computed in the parent CMakeLists.txt.
function(common_test_setup target)
    target_include_directories(${target} PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/tests
        ${ABSL_OWN_INCLUDE_DIR}
        ${Protobuf_INCLUDE_DIRS}
        ${Boost_INCLUDE_DIRS}
        ${Boost_INCLUDE_DIR}
    )
    target_include_directories(${target} SYSTEM PRIVATE
        ${HOMEBREW_INCLUDE_DIR}
        ${CPPREST_INCLUDE_DIR}
        ${OPENSSL_INCLUDE_DIR}
        ${OPENSSL_INCLUDE_DIRS}
    )
    target_compile_definitions(${target} PUBLIC ELPP_THREAD_SAFE)
    if(ENABLE_COVERAGE)
        target_compile_options(${target} PRIVATE ${COVERAGE_CXX_FLAGS})
        target_link_options(${target} PRIVATE ${COVERAGE_LINK_FLAGS})
        # Also instrument the main library so coverage data is generated for src/ files
        target_compile_options(${PROJECT_NAME} PRIVATE ${COVERAGE_CXX_FLAGS})
        target_link_options(${PROJECT_NAME} PRIVATE ${COVERAGE_LINK_FLAGS})
    endif()
    if(APPLE)
        target_link_libraries(${target} PRIVATE
            ${PROJECT_NAME}
            GTest::gtest
            GTest::gtest_main
            "-framework CoreFoundation"
            "-framework Security"
        )
    else()
        target_link_libraries(${target} PRIVATE
            ${PROJECT_NAME}
            GTest::gtest
            GTest::gtest_main
        )
    endif()
endfunction()

# ── Unit tests ────────────────────────────────────────────────────────────────
add_executable(unit_tests
    unit/test_order_util.cpp
    unit/test_contract_util.cpp
    unit/test_utils.cpp
    unit/test_client_config.cpp
    unit/test_client_config_props.cpp
    unit/test_enums.cpp
    unit/test_tick_util.cpp
    unit/test_properties.cpp
    unit/test_model.cpp
    unit/test_tiger_client.cpp
    unit/test_sign_util.cpp
    unit/test_trade_client_serialize.cpp
)
common_test_setup(unit_tests)
# test_client_config_props.cpp uses std::filesystem (C++17)
set_target_properties(unit_tests PROPERTIES CXX_STANDARD 17)
add_test(NAME UnitTests COMMAND unit_tests)

# ── Integration tests (gated by TIGER_RUN_INTEG=true) ────────────────────────
add_executable(integ_tests
    integ/test_quote_client.cpp
    integ/test_trade_client.cpp
)
common_test_setup(integ_tests)
# C++17 guaranteed copy elision avoids ClientConfig move (deleted move constructor)
set_target_properties(integ_tests PROPERTIES CXX_STANDARD 17)
add_test(NAME IntegTests COMMAND integ_tests)
