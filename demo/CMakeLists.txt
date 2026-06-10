cmake_minimum_required(VERSION 3.15)


project(openapi_cpp_test)


# Configure required libraries ...
if(UNIX) # Darwing or Linux
    
    find_package(Boost REQUIRED COMPONENTS system thread log program_options chrono)
    find_package(Threads REQUIRED)

    # cpprest
    if(NOT CPPREST_INCLUDE_DIR)
        find_path(CPPREST_INCLUDE_DIR
            NAMES cpprest/http_client.h cpprest/http_msg.h
            HINTS
                ${CPPREST_PREFIX}/include
                /usr/local/opt/cpprest/include
                /opt/homebrew/opt/cpprest/include
                /usr/local/include
        )
    endif()
    if(NOT CPPREST_LIBRARY)
        find_library(CPPREST_LIBRARY
            NAMES cpprest
            HINTS
                ${CPPREST_PREFIX}/lib
                /usr/local/opt/cpprest/lib
                /opt/homebrew/opt/cpprest/lib
                /usr/local/lib
        )
    endif()
    if(NOT CPPREST_INCLUDE_DIR OR NOT CPPREST_LIBRARY)
        message(FATAL_ERROR "cpprestsdk headers or library not found. Set CPPREST_INCLUDE_DIR/CPPREST_LIBRARY explicitly.")
    endif()
    message(CPPREST_INCLUDE_DIR " ${CPPREST_INCLUDE_DIR}")
    message(CPPREST_LIBRARY " ${CPPREST_LIBRARY}")

    # tigerapi
    find_path(TIGERAPI_INCLUDE_DIR NAMES tigerapi/easylogging++.h tigerapi/client_config.h )
    message(TIGERAPI_INCLUDE_DIR " ${TIGERAPI_INCLUDE_DIR}")
    find_library(TIGERAPI_LIBRARY NAMES libtigerapi.a)

    # Use system library or sdk output library
    if(NOT Protobuf_INCLUDE_DIR)
        find_path(Protobuf_INCLUDE_DIR
            NAMES google/protobuf/service.h google/protobuf/descriptor.h
            HINTS
                ${PROTOBUF_PREFIX}/include
                /opt/homebrew/opt/protobuf/include
                /usr/local/opt/protobuf/include
                /usr/local/include
        )
    endif()
    if(NOT Protobuf_LIBRARY)
        find_library(Protobuf_LIBRARY
            NAMES protobuf
            HINTS
                ${PROTOBUF_PREFIX}/lib
                /opt/homebrew/opt/protobuf/lib
                /usr/local/opt/protobuf/lib
                /usr/local/lib
        )
    endif()
    if(NOT Protobuf_INCLUDE_DIR OR NOT Protobuf_LIBRARY)
        message(FATAL_ERROR "Protobuf headers or library not found. Set Protobuf_INCLUDE_DIR/Protobuf_LIBRARY explicitly.")
    endif()
    message(STATUS "Protobuf version: ${Protobuf_VERSION}")
    message(STATUS "Protobuf include dir: ${Protobuf_INCLUDE_DIR}")
    message(STATUS "Protobuf libraries: ${Protobuf_LIBRARY}")


    if(APPLE)
#        set(CMAKE_OSX_DEPLOYMENT_TARGET "13.7")
        # file(GLOB OPENSSL_ROOT_DIR /usr/local/Cellar/openssl*/*)
        file(GLOB OPENSSL_ROOT_DIR /usr/local/opt/openssl/* /opt/homebrew/Cellar/openssl@3/*)
        # Prefer the latest (make the latest one first)
        list(REVERSE OPENSSL_ROOT_DIR)

        find_package(OpenSSL REQUIRED)
        message(OPENSSL_ROOT_DIR " ${OPENSSL_ROOT_DIR}")
        message(OPENSSL_LIBRARIES " ${OPENSSL_LIBRARIES}")
        
        # Find Abseil (required by Protobuf v25)
        find_package(absl QUIET)
        if(NOT absl_FOUND)
            set(ABSL_PREFIX "/opt/homebrew/Cellar/abseil/20240722.0")
            if(EXISTS "${ABSL_PREFIX}")
                set(CMAKE_PREFIX_PATH "${ABSL_PREFIX};${CMAKE_PREFIX_PATH}")
                find_package(absl REQUIRED)
            endif()
        endif()
    else()
        find_package(OpenSSL 1.0.1 REQUIRED)
        set(OPENSSL_VERSION "1.0.1")
    endif()

elseif(WIN32) # Windows systems including Win64.
    message(FATAL_ERROR "-- please compile by visual studio")
else()
    message(FATAL_ERROR "-- Unsupported platform sorry! :( ")
endif()

# Configure compiler options ...
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")

    message("-- configuring clang options")
#    set(CMAKE_CXX_FLAGS "-arch x86_64 -std=c++11 -stdlib=libc++ -DBOOST_LOG_DYN_LINK -Wno-deprecated-declarations")
    set(CMAKE_CXX_FLAGS "-arch arm64 -std=c++17 -stdlib=libc++ -DBOOST_LOG_DYN_LINK -Wno-deprecated-declarations")
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")

    message("-- configuring gcc options")

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11 -DBOOST_LOG_DYN_LINK")
endif()

# Project construction ...
# src files ...
add_executable(${PROJECT_NAME} openapi_cpp_test/openapi_cpp_test.cpp)

# headers search paths ...
# Protobuf_INCLUDE_DIR must come before TIGERAPI_INCLUDE_DIR so the system protobuf
# headers (5.x) shadow the vendored include/google/ stubs (4.x) in the SDK tree.
# pb_source/*.pb.h are now generated with protobuf 5.x and require 5.x headers.
set(HEADER_SEARCH_PATHS ${Protobuf_INCLUDE_DIR} ${CPPREST_INCLUDE_DIR} ${Boost_INCLUDE_DIR} ${OPENSSL_INCLUDE_DIR} ${TIGERAPI_INCLUDE_DIR})
message(HEADER_SEARCH_PATHS " ${HEADER_SEARCH_PATHS}")



# library search paths ...
if(APPLE)
    set(OPENSSL_LIBS "${OPENSSL_LIBRARIES}")
#    set(ZIP_LIBRARY "/usr/local/Cellar/zlib/1.2.12/lib/libz.dylib")
    set(LIBRARIES_SEARCH_PATHS ${OPENSSL_LIBS} ${Boost_LIBRARIES} ${ZIP_LIBRARY} ${CPPREST_LIBRARY} ${TIGERAPI_LIBRARY} ${Protobuf_LIBRARY} )
else()
    set(OPENSSL_LIBS "${OPENSSL_LIBRARIES}")
    set(LIBRARIES_SEARCH_PATHS ${OPENSSL_LIBS} ${Boost_LIBRARIES} ${ZIP_LIBRARY} ${CPPREST_LIBRARY} ${TIGERAPI_LIBRARY} ${Protobuf_LIBRARY} )
endif()

message(LIBRARIES_SEARCH_PATHS " ${LIBRARIES_SEARCH_PATHS}")

include_directories(${HEADER_SEARCH_PATHS})
if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework CoreFoundation")
    target_link_libraries(${PROJECT_NAME} "-framework Security")
    target_link_libraries(${PROJECT_NAME} ${LIBRARIES_SEARCH_PATHS})
    if(absl_FOUND)
        target_link_libraries(${PROJECT_NAME} 
            absl::base absl::log_internal_check_op absl::status absl::strings
            absl::synchronization absl::time absl::cord absl::log_internal_message
        )
    endif()
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-W1, -F/Library/Frameworks ")
else()
    target_link_libraries(${PROJECT_NAME} ${LIBRARIES_SEARCH_PATHS})
    target_link_libraries(${PROJECT_NAME} OpenSSL::SSL OpenSSL::Crypto)

endif()
