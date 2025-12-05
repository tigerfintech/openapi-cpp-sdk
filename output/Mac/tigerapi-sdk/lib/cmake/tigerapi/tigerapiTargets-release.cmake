#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "tigerapi::tigerapi" for configuration "Release"
set_property(TARGET tigerapi::tigerapi APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(tigerapi::tigerapi PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libtigerapi.a"
  )

list(APPEND _cmake_import_check_targets tigerapi::tigerapi )
list(APPEND _cmake_import_check_files_for_tigerapi::tigerapi "${_IMPORT_PREFIX}/lib/libtigerapi.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
