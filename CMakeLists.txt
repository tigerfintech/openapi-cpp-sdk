cmake_minimum_required(VERSION 3.15)


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
if(NOT (UNIX OR WIN32))
    message(FATAL_ERROR "-- Unsupported platform sorry! :( ")
endif()

find_package(Boost 1.86 REQUIRED COMPONENTS thread log program_options chrono filesystem)
if(WIN32)
    # Prevent FindThreads from insisting on pthreads when building with MSVC
    set(CMAKE_USE_WIN32_THREADS_INIT ON)
    set(CMAKE_THREAD_PREFER_PTHREAD OFF)
    set(THREADS_PREFER_PTHREAD_FLAG OFF)
endif()
find_package(Threads REQUIRED)
find_package(Protobuf REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(cpprestsdk CONFIG QUIET)

set(HAVE_CPPREST_TARGET OFF)
if(TARGET cpprestsdk::cpprest)
    set(HAVE_CPPREST_TARGET ON)
endif()

set(VCPKG_TRIPLET_HINT $ENV{VCPKG_DEFAULT_TRIPLET})
if(NOT VCPKG_TRIPLET_HINT AND WIN32)
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(VCPKG_TRIPLET_HINT "x64-windows")
    else()
        set(VCPKG_TRIPLET_HINT "x86-windows")
    endif()
endif()

set(_vcpkg_include_hint "")
set(_vcpkg_lib_hint "")
if(DEFINED ENV{VCPKG_ROOT} AND VCPKG_TRIPLET_HINT)
    set(_vcpkg_include_hint "$ENV{VCPKG_ROOT}/installed/${VCPKG_TRIPLET_HINT}/include")
    set(_vcpkg_lib_hint "$ENV{VCPKG_ROOT}/installed/${VCPKG_TRIPLET_HINT}/lib")
endif()

if(NOT HAVE_CPPREST_TARGET)
    if(NOT CPPREST_INCLUDE_DIR)
        find_path(CPPREST_INCLUDE_DIR
            NAMES cpprest/http_client.h cpprest/http_msg.h
            HINTS
                ${CPPREST_PREFIX}/include
                /usr/local/opt/cpprest/include
                /opt/homebrew/opt/cpprest/include
                /usr/local/include
                ${_vcpkg_include_hint}
        )
    endif()
    if(NOT CPPREST_LIBRARY)
        find_library(CPPREST_LIBRARY
            NAMES cpprestsdk cpprest
            HINTS
                ${CPPREST_PREFIX}/lib
                /usr/local/opt/cpprest/lib
                /opt/homebrew/opt/cpprest/lib
                /usr/local/lib
                ${_vcpkg_lib_hint}
        )
    endif()
    if(NOT CPPREST_INCLUDE_DIR OR NOT CPPREST_LIBRARY)
        message(FATAL_ERROR "cpprestsdk headers or library not found. Provide CPPREST_PREFIX or CPPREST_INCLUDE_DIR/CPPREST_LIBRARY.")
    endif()
endif()

message(CPPREST_INCLUDE_DIR " ${CPPREST_INCLUDE_DIR}")
message(CPPREST_LIBRARY " ${CPPREST_LIBRARY}")
message(STATUS "Protobuf version: ${Protobuf_VERSION}")
message(STATUS "Protobuf include dirs: ${Protobuf_INCLUDE_DIRS}")
message(STATUS "Protobuf libraries: ${Protobuf_LIBRARIES}")

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
file(GLOB MY_SOURCE_FILES "include/cpprest/*.h" "include/cpprest/details/*.h" "include/openapi_pb/pb_source/*.*" 
"include/tigerapi/push_socket/*.h" "include/tigerapi/*.h" "src/*.cpp" "src/push_socket/*.cpp")
add_library(${PROJECT_NAME} cpprest/ ${MY_SOURCE_FILES})
message(MY_SOURCE_FILES " ${MY_SOURCE_FILES}")

set_target_properties(${PROJECT_NAME} PROPERTIES
    SOVERSION ${PROJECT_VERSION}
    VERSION ${PROJECT_VERSION}
)

# headers search paths ...
# Protobuf_INCLUDE_DIRS must come before CMAKE_SOURCE_DIR/include so that the
# system protobuf headers (5.x) shadow the vendored include/google/ stubs (4.x).
# The vendored include/google/ directory is kept for Windows/user convenience but
# must not be resolved first on macOS where pb_source files are generated with 5.x.
if(DEFINED ENV{HOMEBREW_PREFIX})
    set(HOMEBREW_INCLUDE_DIR "$ENV{HOMEBREW_PREFIX}/include")
else()
    set(HOMEBREW_INCLUDE_DIR "/opt/homebrew/include")
endif()
# Protobuf first, then project headers, so system protobuf shadows include/google/.
set(PRIVATE_INCLUDE_DIRS
    ${Protobuf_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/include
    ${Boost_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIR}
)
# System/third-party headers: marked SYSTEM so the compiler searches them AFTER
# PRIVATE_INCLUDE_DIRS, preventing absl/protobuf INTERFACE_INCLUDE_DIRECTORIES
# from injecting /opt/homebrew/include before our pinned Boost 1.86 headers.
set(SYSTEM_INCLUDE_DIRS
    ${HOMEBREW_INCLUDE_DIR}
    ${CPPREST_INCLUDE_DIR}
    ${OPENSSL_INCLUDE_DIR}
    ${OPENSSL_INCLUDE_DIRS}
)
foreach(_dir IN LISTS PRIVATE_INCLUDE_DIRS SYSTEM_INCLUDE_DIRS)
    if(_dir STREQUAL "")
        list(REMOVE_ITEM PRIVATE_INCLUDE_DIRS "${_dir}")
        list(REMOVE_ITEM SYSTEM_INCLUDE_DIRS "${_dir}")
    endif()
endforeach()
list(REMOVE_DUPLICATES PRIVATE_INCLUDE_DIRS)
list(REMOVE_DUPLICATES SYSTEM_INCLUDE_DIRS)

set(OPENSSL_LIBS "${OPENSSL_LIBRARIES}")
set(LIBRARIES_SEARCH_PATHS
    ${OPENSSL_LIBS}
    ${Boost_LIBRARIES}
    ${Protobuf_LIBRARIES}
    OpenSSL::SSL
    OpenSSL::Crypto
)
if(HAVE_CPPREST_TARGET)
    list(APPEND LIBRARIES_SEARCH_PATHS cpprestsdk::cpprest)
elseif(CPPREST_LIBRARY)
    list(APPEND LIBRARIES_SEARCH_PATHS ${CPPREST_LIBRARY})
endif()

message(BOOST_LIBS " ${Boost_LIBRARIES}")
message(OPENSSL_LIBS " ${OPENSSL_LIBRARIES}")
message(LIBRARIES_SEARCH_PATHS " ${LIBRARIES_SEARCH_PATHS}")

target_include_directories(${PROJECT_NAME} PRIVATE ${PRIVATE_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE ${SYSTEM_INCLUDE_DIRS})
if (APPLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        "-framework CoreFoundation"
        "-framework Security"
        ${LIBRARIES_SEARCH_PATHS}
        Threads::Threads
    )
    set_target_properties(${PROJECT_NAME} PROPERTIES 
        LINK_FLAGS "-Wl,-F/Library/Frameworks"
    )
else()
    target_link_libraries(${PROJECT_NAME} PRIVATE
        ${LIBRARIES_SEARCH_PATHS}
        Threads::Threads
    )
endif()




install(DIRECTORY include/
        DESTINATION "include"
        FILES_MATCHING 
        PATTERN "*.h"
        # PATTERN "*.cc"
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
