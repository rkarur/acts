#----------------------------------------------------------------
# Generated CMake target import file for configuration "RelWithDebInfo".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "ActsPluginTGeo" for configuration "RelWithDebInfo"
set_property(TARGET ActsPluginTGeo APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(ActsPluginTGeo PROPERTIES
  IMPORTED_LOCATION_RELWITHDEBINFO "${_IMPORT_PREFIX}/lib64/libActsPluginTGeo.so"
  IMPORTED_SONAME_RELWITHDEBINFO "libActsPluginTGeo.so"
  )

list(APPEND _IMPORT_CHECK_TARGETS ActsPluginTGeo )
list(APPEND _IMPORT_CHECK_FILES_FOR_ActsPluginTGeo "${_IMPORT_PREFIX}/lib64/libActsPluginTGeo.so" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
