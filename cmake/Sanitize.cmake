# USAGE: To enable extra compiler warnings, the single CMake option of
# `MORE_COMPILER_WARNINGS` needs to be set to 'ON' (default), either by GUI,
# ccmake, or on the command line (-DMORE_COMPILER_WARNINGS=ON).
#
# To enable this module set the module path in the main CMakeLists.txt file and
# include this module. This will create a compiler flag list in the variable
# WARNING_FLAGLIST set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
# "${CMAKE_SOURCE_DIR}/cmake/") include(HandleCompilerWarnings)

# Add the compiler flags to all the targets that you want the additional
# warnings for. It might not be wanted for test code, for example.
# add_library(liba src/A.cpp src/B.cpp ) 
# add_warning_compile_flags_to_target( liba )

# Options
option(AddressSanitize
       "Builds targets with address sanitizer for gcc." OFF)

# Options
option(ThreadSanitize
       "Builds targets with thread sanitizer for gcc." OFF)


       # Try to make this compiler version independent by actually checking for the
# compiler option first
if(AddressSanitize)
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
  # using Clang
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  # using GCC
  add_compile_options(-fsanitize=address -fsanitize=pointer-compare -fsanitize=pointer-subtract
  -fsanitize=leak -fsanitize-recover=all -fsanitize-address-use-after-scope -fsanitize=undefined -fsanitize=float-divide-by-zero -fsanitize=float-cast-overflow)
  add_link_options(-fsanitize=address -fsanitize=pointer-compare -fsanitize=pointer-subtract
  -fsanitize=leak -fsanitize-recover=all -fsanitize-address-use-after-scope -fsanitize=undefined -fsanitize=float-divide-by-zero -fsanitize=float-cast-overflow)
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
  # using Intel C++
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
endif()
endif()

if(ThreadSanitize)
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
  # using Clang
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  # using GCC
  add_compile_options(-fsanitize=thread)
  add_link_options(-fsanitize=thread)
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
  # using Intel C++
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
endif()
endif()
