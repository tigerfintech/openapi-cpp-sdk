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
        file(GLOB OPENSSL_ROOT_DIR /usr/local/opt/openssl/* /opt/homebrew/Cellar/openssl@3/*)
        # Prefer the latest (make the latest one first)
        list(REVERSE OPENSSL_ROOT_DIR)

        find_package(OpenSSL REQUIRED)
#        set(OPENSSL_VERSION "1.1.1q")

        set(ZLIB_LIBRARY  /opt/homebrew/Cellar/zlib/1.2.13/lib/libz.dylib )
        message(ZLIB_LIBRARY " ${ZLIB_LIBRARY}")

    else()
        find_package(OpenSSL 1.0.1 REQUIRED)
        set(OPENSSL_VERSION "1.0.1")
    endif()

elseif(WIN32)
    message(FATAL_ERROR "-- please compile via Visual Studio ")
else()
    message(FATAL_ERROR "-- Unsupported platform sorry! :( ")
endif()

# Configure compiler options ...
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")

    message("-- configuring clang options")
#    set(CMAKE_CXX_FLAGS "-arch x86_64 -std=c++11 -stdlib=libc++ -DBOOST_LOG_DYN_LINK -Wno-deprecated-declarations")
    set(CMAKE_CXX_FLAGS "-arch arm64 -std=c++11 -stdlib=libc++ -DBOOST_LOG_DYN_LINK -Wno-deprecated-declarations")

elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")

    message("-- configuring gcc options")

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11 -DBOOST_LOG_DYN_LINK")

endif()

# Project construction ...
# src files ...
file(GLOB MY_SOURCE_FILES "include/cpprest/*.h" "include/cpprest/details/*.h" "include/tigerapi/push_socket/*.h" "include/tigerapi/*.h" "src/*.cpp" "src/push_socket/*.cpp")
add_library(${PROJECT_NAME} cpprest/  ${MY_SOURCE_FILES})

# headers search paths ...
#set(HEADER_SEARCH_PATHS ${CPPREST_INCLUDE_DIR} ${Boost_INCLUDE_DIR} ${OPENSSL_INCLUDE_DIR})
set(HEADER_SEARCH_PATHS ${Boost_INCLUDE_DIR} ${OPENSSL_INCLUDE_DIR})

# library search paths ...
if(APPLE)
    set(OPENSSL_LIBS "${OPENSSL_LIBRARIES}")
#    set(ZIP_LIBRARY ZLIB::ZLIB)

    set(LIBRARIES_SEARCH_PATHS ${OPENSSL_LIBS} ${Boost_LIBRARIES} ${CPPREST_LIBRARY} ${ZIP_LIBRARY})
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
