# Newbie Header Tool
Generates serialization codes by parsing headers. Heavily relies on clang and LLVM. 

## Install
* Build [llvm-project]("https://github.com/llvm/llvm-project")
* Build [cppast]("https://github.com/foonathan/cppast")
* Run `cmake -G "Visual Studio 15 2017" -A x64 -B build -DLLVM_CONFIG_BINARY="C:/llvm-project/build/Release/bin/llvm-config.exe" -DLIBCLANG_BINARY="C:/llvm-project/build/Release/lib/libclang.lib" -DLIBCLANG_INCLUDE_DIR="C:/llvm-project/clang/include" -DCLANG_BINARY="C:/llvm-project/build/Release/bin/clang++.exe"`
* Run `cmake --build build --config Release`

## Usage
* Run `./build/Release/HeaderTool compile_commands_dir_path module_hpp_dir_path module_name > generated_file_name`