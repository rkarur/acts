#----------------------------------------------------------------
# Generated CMake target import file for configuration "RelWithDebInfo".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "ActsPluginDigitization" for configuration "RelWithDebInfo"
set_property(TARGET ActsPluginDigitization APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(ActsPluginDigitization PROPERTIES
  IMPORTED_LOCATION_RELWITHDEBINFO "${_IMPORT_PREFIX}/lib64/libActsPluginDigitization.so"
  IMPORTED_SONAME_RELWITHDEBINFO "libActsPluginDigitization.so"
  )

list(APPEND _IMPORT_CHECK_TARGETS ActsPluginDigitization )
list(APPEND _IMPORT_CHECK_FILES_FOR_ActsPluginDigitization "${_IMPORT_PREFIX}/lib64/libActsPluginDigitization.so" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
