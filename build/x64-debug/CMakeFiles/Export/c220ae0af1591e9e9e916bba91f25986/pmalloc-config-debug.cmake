#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "pmalloc::pmalloc" for configuration "Debug"
set_property(TARGET pmalloc::pmalloc APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(pmalloc::pmalloc PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "C"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/pmalloc.lib"
  )

list(APPEND _cmake_import_check_targets pmalloc::pmalloc )
list(APPEND _cmake_import_check_files_for_pmalloc::pmalloc "${_IMPORT_PREFIX}/lib/pmalloc.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
