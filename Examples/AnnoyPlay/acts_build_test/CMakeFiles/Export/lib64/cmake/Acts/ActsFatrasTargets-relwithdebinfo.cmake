#----------------------------------------------------------------
# Generated CMake target import file for configuration "RelWithDebInfo".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "ActsFatras" for configuration "RelWithDebInfo"
set_property(TARGET ActsFatras APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(ActsFatras PROPERTIES
  IMPORTED_LOCATION_RELWITHDEBINFO "${_IMPORT_PREFIX}/lib64/libActsFatras.so"
  IMPORTED_SONAME_RELWITHDEBINFO "libActsFatras.so"
  )

list(APPEND _IMPORT_CHECK_TARGETS ActsFatras )
list(APPEND _IMPORT_CHECK_FILES_FOR_ActsFatras "${_IMPORT_PREFIX}/lib64/libActsFatras.so" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
