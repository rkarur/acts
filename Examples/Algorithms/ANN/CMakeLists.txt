add_library(
  ActsExamplesANN SHARED
  src/ApproximateNearestNeighborAlgorithm.cpp)
target_include_directories(
  ActsExamplesANN
  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
target_link_libraries(
  ActsExamplesANN
  PUBLIC
    ActsCore
    ActsExamplesFramework
    ActsExamplesIoJson
    ActsExamplesMagneticField
    Boost::program_options
    Annoy)

install(
  TARGETS ActsExamplesANN
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
