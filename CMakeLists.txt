cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_VERBOSE_MAKEFILE ON)

project(LLVM_mpiattrs)

find_package(LLVM 17 REQUIRED CONFIG)

include_directories(${LLVM_INCLUDE_DIRS})
include_directories(${LLVM_DEFINITIONS})

list(APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}")

include(AddLLVM)
add_llvm_pass_plugin(MPIAttrs
  ./src/MPIAttributesPass.cpp
  ./src/MPIAnnotationFunctions.cpp

  PLUGIN_TOOL
  opt
)
if (DEFINED "USE_OPENMPI")
  add_compile_definitions(USE_OPENMPI)
endif()
