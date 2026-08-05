cmake_minimum_required(VERSION 3.15)

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
)
common_test_setup(unit_tests)
add_test(NAME UnitTests COMMAND unit_tests)

# ── Integration tests (gated by TIGER_RUN_INTEG=true) ────────────────────────
add_executable(integ_tests
    integ/test_quote_client.cpp
    integ/test_trade_client.cpp
)
common_test_setup(integ_tests)
add_test(NAME IntegTests COMMAND integ_tests)
