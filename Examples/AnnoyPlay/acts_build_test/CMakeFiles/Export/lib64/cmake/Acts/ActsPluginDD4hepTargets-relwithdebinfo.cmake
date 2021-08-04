#----------------------------------------------------------------
# Generated CMake target import file for configuration "RelWithDebInfo".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "ActsPluginDD4hep" for configuration "RelWithDebInfo"
set_property(TARGET ActsPluginDD4hep APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(ActsPluginDD4hep PROPERTIES
  IMPORTED_LOCATION_RELWITHDEBINFO "${_IMPORT_PREFIX}/lib64/libActsPluginDD4hep.so"
  IMPORTED_SONAME_RELWITHDEBINFO "libActsPluginDD4hep.so"
  )

list(APPEND _IMPORT_CHECK_TARGETS ActsPluginDD4hep )
list(APPEND _IMPORT_CHECK_FILES_FOR_ActsPluginDD4hep "${_IMPORT_PREFIX}/lib64/libActsPluginDD4hep.so" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
