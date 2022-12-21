cmake_minimum_required(VERSION 3.0)

project(tigerapi-demo)

# Configure compiler options ...
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")

    message("-- configuring clang options")
    set(CMAKE_CXX_FLAGS "-arch x86_64 -std=c++11 -stdlib=libc++ -DBOOST_LOG_DYN_LINK -Wno-deprecated-declarations")

elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")

    message("-- configuring gcc options")

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11 -DBOOST_LOG_DYN_LINK")

endif()


# src files ...
add_executable(${PROJECT_NAME} src/quote_client_demo.cpp)

# headers search paths ...
set(CPPRESTSDK_INCLUDE_DIR "../libs/cpprestsdk/Release/include")
set(HEADER_SEARCH_PATHS ${TIGERAPI_INCLUDE_DIR} ${CPPRESTSDK_INCLUDE_DIR})

include_directories(${HEADER_SEARCH_PATHS})
message(HEADER_SEARCH_PATHS  "${HEADER_SEARCH_PATHS}")



