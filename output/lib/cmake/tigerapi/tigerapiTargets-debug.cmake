#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "tigerapi::tigerapi" for configuration "Debug"
set_property(TARGET tigerapi::tigerapi APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(tigerapi::tigerapi PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "CXX"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/libtigerapi.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS tigerapi::tigerapi )
list(APPEND _IMPORT_CHECK_FILES_FOR_tigerapi::tigerapi "${_IMPORT_PREFIX}/lib/libtigerapi.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
