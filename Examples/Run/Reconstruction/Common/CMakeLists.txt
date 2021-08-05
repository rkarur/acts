add_library(
  ActsExamplesRecTracksCommon SHARED
  RecInput.cpp 
  RecTruthTracks.cpp
  RecCKFTracks.cpp
  SeedingExample.cpp)
target_include_directories(
  ActsExamplesRecTracksCommon
  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
target_link_libraries(
  ActsExamplesRecTracksCommon
  PUBLIC
    ActsExamplesFramework
    ActsExamplesCommon
    ActsExamplesDigitization
    ActsExamplesTrackFitting
    ActsExamplesTrackFinding
    ActsExamplesMagneticField
    ActsExamplesTruthTracking
    ActsExamplesIoCsv
    ActsExamplesIoPerformance)
if(ACTS_BUILD_PLUGIN_ONNX)
  target_compile_definitions(
    ActsExamplesRecTracksCommon PUBLIC ACTS_PLUGIN_ONNX)
  target_link_libraries(
    ActsExamplesRecTracksCommon PUBLIC ActsPluginOnnx)
endif()

install(
  TARGETS ActsExamplesRecTracksCommon
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})


if(ACTS_BUILD_EXAMPLES_ANN)
  add_library(
    ActsExamplesRecANN SHARED
    RecANN.cpp)
  target_include_directories(
    ActsExamplesRecANN
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
  target_link_libraries(
    ActsExamplesRecANN
    PUBLIC
    ActsExamplesCommon
    ActsExamplesDigitization
    ActsExamplesFramework
    ActsExamplesIoCsv
    ActsExamplesRecTracksCommon
    ActsExamplesTrackFinding
    ActsExamplesANN)
  install(
    TARGETS ActsExamplesRecANN
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
endif()
