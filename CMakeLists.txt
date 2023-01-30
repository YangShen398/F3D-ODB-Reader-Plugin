cmake_minimum_required(VERSION 3.2)

project(ODBReader-plugin)

include(GNUInstallDirs)

find_package(f3d REQUIRED)

# Check Plugin SDK is available
if(NOT (DEFINED f3d_SDK_PLUGIN AND ${f3d_SDK_PLUGIN}))
  message(FATAL_ERROR "f3d_SDK_PLUGIN is not enabled, can't configure plugin")
endif()
message(STATUS "f3d_SDK_PLUGIN is enabled")

# find Abaqus SDK
list(INSERT CMAKE_MODULE_PATH 0
${CMAKE_CURRENT_SOURCE_DIR}/cmake)

f3d_plugin_init()

f3d_plugin_declare_reader(
  NAME ODBReader
  EXTENSIONS odb
  VTK_READER vtkF3DODBReader
  DESCRIPTION "Abaqus ODB reader"
)

f3d_plugin_build(
  NAME ODBReader
  VERSION 1.0
  DESCRIPTION "Abaqus ODB reader plugin"
  VTK_MODULES CommonCore CommonExecutionModel
)

option(BUILD_TESTING "Build the tests" OFF)
if(BUILD_TESTING)
  enable_testing()
  add_subdirectory(tests)
endif()