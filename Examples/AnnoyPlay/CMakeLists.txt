add_executable(ANNTracking src/annoy_reco.cpp)
target_link_libraries(
  ANNTracking
  PUBLIC ActsCore ActsExamplesFramework
  PRIVATE ROOT::Core ROOT::Hist ROOT::Tree ROOT::Gpad)
install(TARGETS ANNTracking RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
