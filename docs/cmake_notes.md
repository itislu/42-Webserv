

## CMAKE

### CMAKE_EXPORT_COMPILE_COMMANDS :
  - creates a 'compile_commands.json' file in the build folder.
  - clangd, clang-tidy, use this file to know how each file is compiled,
    so linting, code completion, and static analysis work correctly.

### CXX_EXTENSIONS :
  - controls whether the compiler is allowed to use 
    compiler-specific C++ language extensions (gnu)
  - turned off because we need to use `-std=c++98` and not `-std=gnu++98`
