message(STATUS "Fetching Annoy")

include(ExternalProject)

set(Annoy_TAG "v1.17.0")
set(Annoy_URL https://github.com/spotify/annoy.git)
set(Annoy_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/annoy-prefix/src/annoy/src)

file(MAKE_DIRECTORY ${Annoy_INCLUDE_DIR})

ExternalProject_Add(
  annoy
  GIT_REPOSITORY ${Annoy_URL}
  GIT_TAG ${Annoy_TAG}
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  INSTALL_COMMAND ""
)

add_library(Annoy INTERFACE IMPORTED GLOBAL)
add_dependencies(Annoy annoy)
target_include_directories(Annoy INTERFACE ${Annoy_INCLUDE_DIR})
install(DIRECTORY ${Annoy_INCLUDE_DIR}/annoy DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

