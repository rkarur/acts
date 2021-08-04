#----------------------------------------------------------------
# Generated CMake target import file for configuration "RelWithDebInfo".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "ActsPluginJson" for configuration "RelWithDebInfo"
set_property(TARGET ActsPluginJson APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(ActsPluginJson PROPERTIES
  IMPORTED_LOCATION_RELWITHDEBINFO "${_IMPORT_PREFIX}/lib64/libActsPluginJson.so"
  IMPORTED_SONAME_RELWITHDEBINFO "libActsPluginJson.so"
  )

list(APPEND _IMPORT_CHECK_TARGETS ActsPluginJson )
list(APPEND _IMPORT_CHECK_FILES_FOR_ActsPluginJson "${_IMPORT_PREFIX}/lib64/libActsPluginJson.so" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
