cmake_minimum_required(VERSION 3.0)

project(tigerapi)

# Configure required libraries ...
if(UNIX) # Darwing or Linux
    
    find_package(Boost REQUIRED COMPONENTS system thread log program_options chrono)
    find_package(Threads REQUIRED)

    if(APPLE)
        # Prefer a homebrew version of OpenSSL over the one in /usr/lib
        # file(GLOB OPENSSL_ROOT_DIR /usr/local/Cellar/openssl*/*)
        file(GLOB OPENSSL_ROOT_DIR /usr/local/opt/openssl/*)
        # Prefer the latest (make the latest one first)
	    list(REVERSE OPENSSL_ROOT_DIR)

        find_package(OpenSSL 1.1 REQUIRED)
        set(OPENSSL_VERSION "1.1.1q")
    else()
        find_package(OpenSSL 1.0.1 REQUIRED)
        set(OPENSSL_VERSION "1.0.1")
    endif()

elseif(WIN32) # Windows systems including Win64.
    message(FATAL_ERROR "-- Windows is not supported for now.")
else()
    message(FATAL_ERROR "-- Unsupported platform sorry! :( ")
endif()

# Configure compiler options ...
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")

    message("-- configuring clang options")
    set(CMAKE_CXX_FLAGS "-arch x86_64 -std=c++11 -stdlib=libc++ -DBOOST_LOG_DYN_LINK -Wno-deprecated-declarations")

elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")

    message("-- configuring gcc options")

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11 -DBOOST_LOG_DYN_LINK")

endif()

# Project construction ...

# src files ...
add_executable(${PROJECT_NAME} src/main.cpp src/base64.hpp
                               src/tigerapi.cpp src/tigerapi.h src/algo_hmac.h src/algo_hmac.cpp src/constants.h
         src/utils.h src/utils.cpp)

# headers search paths ...
set(CPPRESTSDK_INCLUDE_DIR "./libs/cpprestsdk/Release/include")
set(HEADER_SEARCH_PATHS ${CPPRESTSDK_INCLUDE_DIR} ${Boost_INCLUDE_DIR} ${OPENSSL_INCLUDE_DIR})

# library search paths ...
if(APPLE)
    set(OPENSSL_LIBS "${OPENSSL_LIBRARIES}")
    set(CPPRESTSDK_LIBRARY "${PROJECT_SOURCE_DIR}/libs/cpprestsdk/build.debug/Binaries/libcpprest.a")
    set(ZIP_LIBRARY "/usr/local/Cellar/zlib/1.2.12/lib/libz.dylib")

    set(LIBRARIES_SEARCH_PATHS ${OPENSSL_LIBS} ${Boost_LIBRARIES} ${CPPRESTSDK_LIBRARY} ${ZIP_LIBRARY})
else()
    set(OPENSSL_LIBS "${OPENSSL_LIBRARIES}")
    set(CPPRESTSDK_LIBRARY "${PROJECT_SOURCE_DIR}/libs/cpprestsdk/build.debug/Binaries/libcpprest.a")

    set(LIBRARIES_SEARCH_PATHS ${CPPRESTSDK_LIBRARY} ${OPENSSL_LIBS} ${Boost_LIBRARIES})
endif()

message(BOOST_LIBS " ${Boost_LIBRARIES}")
message(OPENSSL_LIBS " ${OPENSSL_LIBRARIES}")
message(CPPRESTSDK_LIBRARY " ${CPPRESTSDK_LIBRARY}")
message(ZLIB_LIBRARY " ${ZLIB_LIBRARY}")
message(LIBRARIES_SEARCH_PATHS " ${LIBRARIES_SEARCH_PATHS}")

include_directories(${HEADER_SEARCH_PATHS})
if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework CoreFoundation")
    target_link_libraries(${PROJECT_NAME} "-framework Security")
    target_link_libraries(${PROJECT_NAME} ${LIBRARIES_SEARCH_PATHS})
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-W1, -F/Library/Frameworks ")
else()
    target_link_libraries(${PROJECT_NAME} ${LIBRARIES_SEARCH_PATHS})
endif()
