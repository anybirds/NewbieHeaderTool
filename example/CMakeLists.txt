cmake_minimum_required(VERSION 3.8)

if (NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE PATH "toolchain file")
endif()

project(cppast_practice)

file(GLOB PRACTICE_SOURCE "example.cpp")

add_executable(Practice ${PRACTICE_SOURCE})

target_include_directories(Practice
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
)
set_target_properties(
  Practice PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
)

# set(CMAKE_EXPORT_COMPILE_COMMANDS ON)