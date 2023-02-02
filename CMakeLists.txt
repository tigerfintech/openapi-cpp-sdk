cmake_minimum_required(VERSION 3.0)


# Find the source file that contains the version information
find_file(VERSION_FILE include/tigerapi/version.h PATHS ${CMAKE_SOURCE_DIR})

message(version file " ${VERSION_FILE}")
# Extract the version information from the source file
file(READ ${VERSION_FILE} VERSION_FILE_CONTENTS)
string(REGEX MATCH "([0-9]\\.[0-9]\\.[0-9])"
        PROJECT_VERSION "${VERSION_FILE_CONTENTS}")
message(PROJECT_VERSION " ${PROJECT_VERSION}")

project(tigerapi VERSION ${PROJECT_VERSION})


# Configure required libraries ...
if(UNIX) # Darwing or Linux

    find_package(Boost REQUIRED COMPONENTS system thread log program_options chrono)
    find_package(Threads REQUIRED)

    # cpprest
    find_path(CPPREST_INCLUDE_DIR NAMES cpprest/http_client.h cpprest/http_msg.h)
    find_library(CPPREST_LIBRARY NAMES cpprest)
    message(CPPREST_INCLUDE_DIR " ${CPPREST_INCLUDE_DIR}")
    message(CPPREST_LIBRARY " ${CPPREST_LIBRARY}")

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
    #set(CMAKE_PREFIX_PATH "D:/projects/vcpkg/packages/")
    SET(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "D:/projects/boost_1_81_0")
    SET(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "D:/projects/boost_1_81_0/lib")
    FIND_PACKAGE(Boost)
    IF (Boost_FOUND)
        INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
        ADD_DEFINITIONS( "-DHAS_BOOST" )
    ENDIF()

    #find_package(Boost REQUIRED COMPONENTS system thread log program_options chrono)
    #find_package(Threads REQUIRED)


    file(GLOB OPENSSL_ROOT_DIR D:/projects/vcpkg/packages/openssl_x86-windows/*)
    # Prefer the latest (make the latest one first)
    list(REVERSE OPENSSL_ROOT_DIR)
    find_package(OpenSSL REQUIRED)

    #file(GLOB cpprestsdk_DIR D:/projects/vcpkg/packages/cpprestsdk_x86-windows/share/*)
    #list(REVERSE cpprestsdk_DIR)
    # find_package(cpprestsdk CONFIG REQUIRED)
    # cpprest
    #find_path(CPPREST_INCLUDE_DIR NAMES cpprest/http_client.h cpprest/http_msg.h)
    #find_library(CPPREST_LIBRARY NAMES cpprest)



    #file(GLOB ZLIB_ROOT D:/projects/vcpkg/packages/zlib_x86-windows/*)
    #(REVERSE ZLIB_ROOT)
    #find_package(ZLIB REQUIRED)

    #set(CMAKE_PREFIX_PATH "D:/projects/vcpkg/packages/brotli_x86-windows/share/unofficial-brotli")
    #file(GLOB unofficial-brotli_DIR D:/projects/vcpkg/packages/brotli_x86-windows/*)
    #list(REVERSE unofficial-brotli_DIR)
    #find_package(unofficial-brotli CONFIG REQUIRED)
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

elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    add_compile_options(/D BOOST_LOG_DYN_LINK /MT)
endif()

# Project construction ...
# src files ...
add_executable(${PROJECT_NAME} main.cpp src/common/base64.h
        src/tiger_client.cpp include/tigerapi/tiger_client.h include/tigerapi/constants.h
        include/tigerapi/utils.h src/utils.cpp include/tigerapi/service_types.h include/tigerapi/quote_client.h
        src/quote_client.cpp src/trade_client.cpp include/tigerapi/model.h src/order_util.cpp
        include/tigerapi/order_util.h src/contract_util.cpp include/tigerapi/contract_util.h
        include/tigerapi/client_config.h src/common/sign_util.h
        src/common/easylogging++.h src/common/easylogging++.cc)

# headers search paths ...
#set(HEADER_SEARCH_PATHS ${CPPREST_INCLUDE_DIR} ${Boost_INCLUDE_DIR} ${OPENSSL_INCLUDE_DIR})
set(HEADER_SEARCH_PATHS ${Boost_INCLUDE_DIR} ${OPENSSL_INCLUDE_DIR})

# library search paths ...
if(APPLE)
    set(OPENSSL_LIBS "${OPENSSL_LIBRARIES}")
    set(ZIP_LIBRARY "/usr/local/Cellar/zlib/1.2.12/lib/libz.dylib")

    set(LIBRARIES_SEARCH_PATHS ${OPENSSL_LIBS} ${Boost_LIBRARIES} ${CPPREST_LIBRARY} ${ZIP_LIBRARY})
elseif(WIN32)
    set(ZIP_LIBRARY "D:/projects/vcpkg/packages/zlib_x86-windows/zlib_x86-windows/lib/zlib.lib")
    set(OPENSSL_LIBS "${OPENSSL_LIBRARIES}")
    set(LIBRARIES_SEARCH_PATHS  ${OPENSSL_LIBS} ${Boost_LIBRARIES} ${ZIP_LIBRARY})
else()
    set(OPENSSL_LIBS "${OPENSSL_LIBRARIES}")
    set(LIBRARIES_SEARCH_PATHS ${CPPREST_LIBRARY} ${OPENSSL_LIBS} ${Boost_LIBRARIES} ${ZIP_LIBRARY})
endif()

message(BOOST_LIBS " ${Boost_LIBRARIES}")
message(OPENSSL_LIBS " ${OPENSSL_LIBRARIES}")
#message(CPPRESTSDK_LIBRARY " ${CPPRESTSDK_LIBRARY}")
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
    target_link_libraries(${PROJECT_NAME} OpenSSL::SSL OpenSSL::Crypto)

endif()

#set(CMAKE_INSTALL_PREFIX "./")

install(DIRECTORY include/
        DESTINATION "include"
        FILES_MATCHING PATTERN "*.h"
        )

install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}Targets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
        )
install(EXPORT ${PROJECT_NAME}Targets
        FILE ${PROJECT_NAME}Targets.cmake
        NAMESPACE ${PROJECT_NAME}::
        DESTINATION lib/cmake/${PROJECT_NAME}
        )
