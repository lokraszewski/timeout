#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "timeout::timeout" for configuration "Debug"
set_property(TARGET timeout::timeout APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(timeout::timeout PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "CXX"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/libtimeout.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS timeout::timeout )
list(APPEND _IMPORT_CHECK_FILES_FOR_timeout::timeout "${_IMPORT_PREFIX}/lib/libtimeout.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
