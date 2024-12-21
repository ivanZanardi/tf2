cmake_minimum_required(VERSION 3.10)


# Global
# =====================================
project(pycomet
  VERSION 1.0
  LANGUAGES C CXX Fortran
  DESCRIPTION "Physics-informed machine learning for scientific computing and operator discovery"
)


# Include cmake modules
# =====================================
list(INSERT CMAKE_MODULE_PATH 0 "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(GNUInstallDirs)
include(EnsureOutOfSourceBuild)
include(LogMessage)
ensure_out_of_source_build()
setLogsOn()


# Set C++ version
# =====================================
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()
if(CMAKE_CXX_STANDARD LESS 17)
  LogWarning("Incompatible C++ standard ${CMAKE_CXX_STANDARD}. PyCOMET requires C++17")
endif()


# Find external packages
# =====================================
find_package(tensorflow REQUIRED)
find_package(cppflow REQUIRED)
find_package(json REQUIRED)


# Installation path
# =====================================
# Note that it is not CMAKE_INSTALL_PREFIX we are checking here
if(DEFINED CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  LogMessage("CMAKE_INSTALL_PREFIX is not set")
  LogMessage(">> Default value: ${CMAKE_INSTALL_PREFIX}")
  LogMessage(">> Will set it to ${CMAKE_SOURCE_DIR}/install")
  set(CMAKE_INSTALL_PREFIX
    "${CMAKE_SOURCE_DIR}/install"
    CACHE PATH "Where the library will be installed to" FORCE
 )
else()
  LogMessage("CMAKE_INSTALL_PREFIX is already set")
  LogMessage(">> Current value: ${CMAKE_INSTALL_PREFIX}")
endif()
set(CMAKE_Fortran_MODULE_DIRECTORY ${CMAKE_INSTALL_PREFIX}/mod)


# Target files
# =====================================
# Globbing source files for C and Fortran
file(GLOB_RECURSE SOURCE_FILES_C RELATIVE ${CMAKE_SOURCE_DIR} "src/*.c")
file(GLOB_RECURSE SOURCE_FILES_F RELATIVE ${CMAKE_SOURCE_DIR} "src/model.f90" "src/utils.f90")
# Set the language property for source files
set_source_files_properties(${SOURCE_FILES_C} PROPERTIES LANGUAGE CXX)
set_source_files_properties(${SOURCE_FILES_F} PROPERTIES LANGUAGE Fortran)
# Create a shared library
add_library(${PROJECT_NAME} SHARED)
# Add the source files to the target
target_sources(${PROJECT_NAME}
  PRIVATE
    ${SOURCE_FILES_C}
    ${SOURCE_FILES_F}
)


# Include directories
# =====================================
set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include/")
target_include_directories(${PROJECT_NAME}
  PRIVATE
    # Where the library itself will look for its internal headers
    ${CMAKE_CURRENT_SOURCE_DIR}/src
  PUBLIC
    ${tensorflow_INCLUDE_DIR}
    ${cppflow_INCLUDE_DIR}
    ${json_INCLUDE_DIR}
    # Where top-level project will look for the library's public headers
    $<BUILD_INTERFACE:${INCLUDE_DIR}>
    # Where external projects will look for the library's public headers
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)


# Build type
# =====================================
# Two buid types are available:
# - "Debug":     Uses debug compiler options
# - "Release":   Uses optimized compiler options
# The default build type is "Release".
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build, options are: Debug, Release" FORCE)
endif()
LogMessage("Build type: ${CMAKE_BUILD_TYPE}")
# Set flags
include(SetCXXFlags)
include(SetFortranFlags)


# Debug suffix
# =====================================
# It might be a good idea to add d suffix to debug binaries - that way you’ll
# get lib<name>d.so with 'Debug' configuration and lib<name>.so with 'Release'.
# To do that you need to set the DEBUG_POSTFIX property.
set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX "d")


# External libraries
# =====================================
add_dependencies(${PROJECT_NAME}
  cppflow
  json
)
target_link_libraries(${PROJECT_NAME}
  PRIVATE
    ${tensorflow_LIBRARY}
)


# Build examples
# =====================================
option(BUILD_EXAMPLES "Build examples" OFF)
if(BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()


# Install
# =====================================
install(
  DIRECTORY ${INCLUDE_DIR}
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
)
install(TARGETS ${PROJECT_NAME}
  EXPORT "${PROJECT_NAME}Targets"
)
install(EXPORT "${PROJECT_NAME}Targets"
  FILE "${PROJECT_NAME}Targets.cmake"
  NAMESPACE ${namespace}::
  DESTINATION cmake
)


# Configs
# =====================================
include(CMakePackageConfigHelpers)

# Generate the version file for the config file
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
  VERSION "${version}"
  COMPATIBILITY AnyNewerVersion
)
# Create config file
configure_package_config_file(
  "${CMAKE_MODULE_PATH}/${PROJECT_NAME}Config.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
  INSTALL_DESTINATION cmake
)
# Install config files
install(FILES
  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
  DESTINATION cmake
)
# Generate the export targets for the build tree
export(EXPORT "${PROJECT_NAME}Targets"
  FILE "${CMAKE_CURRENT_BINARY_DIR}/cmake/${PROJECT_NAME}Targets.cmake"
  NAMESPACE ${namespace}::
)
