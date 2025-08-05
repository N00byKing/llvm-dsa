cmake_minimum_required(VERSION 3.15)
project(dsa-cover)

find_package(LLVM REQUIRED CONFIG)
list(APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}")
include(AddLLVM)
add_definitions(${LLVM_DEFINITIONS})
include_directories(${LLVM_INCLUDE_DIRS})
link_directories(${LLVM_LIBRARY_DIR})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

install(DIRECTORY include/
  DESTINATION include
  FILES_MATCHING
  PATTERN "*.h"
  PATTERN "*.TXT"  
  PATTERN "CMakeFiles" EXCLUDE  
  PATTERN ".svn" EXCLUDE)

add_subdirectory (lib/DSA)
