# CMake config for the Acts package
#
# Defines CMake targets for requested and available components.  All additional
# information, e.g. include directories and dependencies, are defined as
# target-specific properties and are automatically propagated when linking to
# the target.
#
# Defines the following additional variables:
#
#   - Acts_COMPONENTS
#   - Acts_COMMIT_HASH
#   - Acts_COMMIT_HASH_SHORT


####### Expanded from @PACKAGE_INIT@ by configure_package_config_file() #######
####### Any changes to this file will be overwritten by the next CMake run ####
####### The input file was ActsConfig.cmake.in                            ########

get_filename_component(PACKAGE_PREFIX_DIR "${CMAKE_CURRENT_LIST_DIR}/../../../" ABSOLUTE)

macro(set_and_check _var _file)
  set(${_var} "${_file}")
  if(NOT EXISTS "${_file}")
    message(FATAL_ERROR "File or directory ${_file} referenced by variable ${_var} does not exist !")
  endif()
endmacro()

macro(check_required_components _NAME)
  foreach(comp ${${_NAME}_FIND_COMPONENTS})
    if(NOT ${_NAME}_${comp}_FOUND)
      if(${_NAME}_FIND_REQUIRED_${comp})
        set(${_NAME}_FOUND FALSE)
      endif()
    endif()
  endforeach()
endmacro()

####################################################################################

set(Acts_COMPONENTS Core;PluginDigitization;PluginIdentification;PluginJson;PluginTGeo;PluginDD4hep;Fatras)
set(Acts_COMMIT_HASH "56f63b24071d36bc55d003233bc5cf93144b27ec-dirty")
set(Acts_COMMIT_HASH_SHORT "56f63b240-dirty")

# print version and components information
if(NOT Acts_FIND_QUIETLY)
  message(STATUS "found Acts version ${Acts_VERSION} commit ${Acts_COMMIT_HASH_SHORT}")
endif()

# check that requested components are available
foreach(_component ${Acts_FIND_COMPONENTS})
  # check if this component is available
  if(NOT _component IN_LIST Acts_COMPONENTS)
    if (${Acts_FIND_REQUIRED_${_component}})
      # not supported, but required -> fail
      set(Acts_FOUND False)
      set(Acts_NOT_FOUND_MESSAGE "required component '${_component}' not found")
    else()
      # not supported and optional -> skip
      list(REMOVE_ITEM Acts_FIND_COMPONENTS ${_component})
      if(NOT Acts_FIND_QUIETLY)
        message(STATUS "optional component '${_component}' not found")
      endif()
    endif()
  endif()
endforeach()

# add this to the current CMAKE_MODULE_PATH to find third party modules 
# that not provide a XXXConfig.cmake or XXX-config.cmake file
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/Modules)

# find external dependencies that are needed to link with Acts. since the
# exported Acts targets only store the linked external target names they need
# to be found again. this avoids hard-coded paths and makes the installed
# config/library relocatable. use exact version match where possible to ensure
# the same versions are found that were used at build time.
# `find_dependency` is a wrapper around `find_package` that automatically
# handles QUIET and REQUIRED parameters.
include(CMakeFindDependencyMacro)
find_dependency(Boost 1.73.0 CONFIG EXACT)
find_dependency(Eigen3 3.3.7 CONFIG EXACT)
if(PluginAutodiff IN_LIST Acts_COMPONENTS)
  find_dependency(autodiff  CONFIG EXACT)
endif()
if(PluginDD4hep IN_LIST Acts_COMPONENTS)
  find_dependency(DD4hep 1.13 CONFIG EXACT)
endif()
if(PluginJson IN_LIST Acts_COMPONENTS)
  find_dependency(nlohmann_json 3.9.1 CONFIG EXACT)
endif()
if(PluginTGeo IN_LIST Acts_COMPONENTS)
  find_dependency(ROOT 6.22.00 CONFIG EXACT)
endif()

# dependencies which cannot be searched in CONFIG mode
if(PluginOnnx IN_LIST Acts_COMPONENTS)
  find_package(OnnxRuntime REQUIRED)
endif()
if(PluginSycl IN_LIST Acts_COMPONENTS)
  find_package(SYCL REQUIRED)
endif()

# load **all** available components. we can not just include the requested
# components since there can be interdependencies between them.
if(NOT Acts_FIND_QUIETLY)
  message(STATUS "loading components:")
endif()
foreach(_component ${Acts_COMPONENTS})
  if(NOT Acts_FIND_QUIETLY)
    message(STATUS "  ${_component}")
  endif()
  # include the targets file to create the imported targets for the user
  include(${CMAKE_CURRENT_LIST_DIR}/Acts${_component}Targets.cmake)
endforeach()
