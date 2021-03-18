# Newbie Header Tool
# Make sure that the llvm-project is installed. 
# Below command should be used to build this project.
# cmake -G "Visual Studio 15 2017" -A x64 -B build -DLLVM_CONFIG_BINARY="C:/llvm-project/build/Release/bin/llvm-config.exe" -DLIBCLANG_BINARY="C:/llvm-project/build/Release/lib/libclang.lib" -DLIBCLANG_INCLUDE_DIR="C:/llvm-project/clang/include" -DCLANG_BINARY="C:/llvm-project/build/Release/bin/clang++.exe"

cmake_minimum_required(VERSION 3.8)

if (NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE PATH "toolchain file")
endif()

project(headertool)

find_package(nlohmann_json CONFIG REQUIRED)
add_subdirectory(cppast)

file(GLOB HEADERTOOL_SOURCE "main.cpp")

add_executable(HeaderTool ${HEADERTOOL_SOURCE})

target_link_libraries(HeaderTool PRIVATE cppast nlohmann_json nlohmann_json::nlohmann_json)
target_include_directories(HeaderTool
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
)
set_target_properties(
  HeaderTool PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
)
