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
option(MORE_COMPILER_WARNINGS
       "Builds targets with additional compiler warnings." ON)

# OK, lets go for paranoia mode and try to switch on as many compiler warnings
# as possible
include(CheckCXXCompilerFlag)
function(check_and_add_cxx_compiler_flag LIST FLAG)
  check_cxx_compiler_flag("${FLAG}" COMPILERFLAG_${FLAG})
  if(COMPILERFLAG_${FLAG})
    set(${LIST} ${${LIST}} "${FLAG}" PARENT_SCOPE)
  endif()
endfunction()

function(add_warning_compile_flags_to_target TARGET)
  if(MORE_COMPILER_WARNINGS)
    foreach(FLAG ${WARNING_FLAGLIST})
      target_compile_options(${TARGET} PRIVATE ${FLAG})
    endforeach()
  endif()
endfunction()

# Try to make this compiler version independent by actually checking for the
# compiler option first
if(MORE_COMPILER_WARNINGS)
  if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
    # using Clang
    check_and_add_cxx_compiler_flag(WARNING_FLAGLIST -Wall)
    check_and_add_cxx_compiler_flag(WARNING_FLAGLIST -Wextra)
    check_and_add_cxx_compiler_flag(WARNING_FLAGLIST -pedantic)
    check_and_add_cxx_compiler_flag(WARNING_FLAGLIST -Weverything)
  elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    # using GCC
    message(STATUS "GCC: full blown paranoia mode")
    check_and_add_cxx_compiler_flag(WARNING_FLAGLIST -Wall)
    check_and_add_cxx_compiler_flag(WARNING_FLAGLIST -Wextra)
    check_and_add_cxx_compiler_flag(WARNING_FLAGLIST -pedantic)
    check_and_add_cxx_compiler_flag(WARNING_FLAGLIST -Wcast-align)
    check_and_add_cxx_compiler_flag(WARNING_FLAGLIST -Wcast-qual)
    check_and_add_cxx_compiler_flag(WARNING_FLAGLIST -Wconversion)
    check_and_add_cxx_compiler_flag(WARNING_FLAGLIST -Wctor-dtor-privacy)
    check_and_add_cxx_compiler_flag(WARNING_FLAGLIST -Wduplicated-cond)
    check_and_add_cxx_compiler_flag(WARNING_FLAGLIST -Wduplicated-branches)
    check_and_add_cxx_compiler_flag(WARNING_FLAGLIST -Weffc++)
    check_and_add_cxx_compiler_flag(WARNING_FLAGLIST -Wfloat-equal)
    check_and_add_cxx_compiler_flag(WARNING_FLAGLIST -Wformat=2)
    check_and_add_cxx_compiler_flag(WARNING_FLAGLIST -Wframe-larger-than=256000)
    check_and_add_cxx_compiler_flag(WARNING_FLAGLIST -Wlogical-op)
    check_and_add_cxx_compiler_flag(WARNING_FLAGLIST -Wmissing-declarations)
    check_and_add_cxx_compiler_flag(WARNING_FLAGLIST -Wmisleading-indentation)
    check_and_add_cxx_compiler_flag(WARNING_FLAGLIST -Wno-free-nonheap-object)
    check_and_add_cxx_compiler_flag(WARNING_FLAGLIST -Wnull-dereference)
    check_and_add_cxx_compiler_flag(WARNING_FLAGLIST -Wold-style-cast)
    check_and_add_cxx_compiler_flag(WARNING_FLAGLIST -Woverloaded-virtual)
    check_and_add_cxx_compiler_flag(WARNING_FLAGLIST -Wsign-conversion)
    check_and_add_cxx_compiler_flag(WARNING_FLAGLIST -Wsign-promo)
    check_and_add_cxx_compiler_flag(WARNING_FLAGLIST -Wshadow)
    check_and_add_cxx_compiler_flag(WARNING_FLAGLIST -Wstrict-null-sentinel)
    check_and_add_cxx_compiler_flag(WARNING_FLAGLIST -Wstrict-overflow=5)
    check_and_add_cxx_compiler_flag(WARNING_FLAGLIST -Wswitch-default)
    check_and_add_cxx_compiler_flag(WARNING_FLAGLIST -Wswitch-enum)
    check_and_add_cxx_compiler_flag(WARNING_FLAGLIST -Wundef)
    check_and_add_cxx_compiler_flag(WARNING_FLAGLIST -Wunused)
    check_and_add_cxx_compiler_flag(WARNING_FLAGLIST -Wuseless-cast)
    check_and_add_cxx_compiler_flag(WARNING_FLAGLIST -Wlogical-op)
    check_and_add_cxx_compiler_flag(WARNING_FLAGLIST -Winline)
    check_and_add_cxx_compiler_flag(WARNING_FLAGLIST
                                    -Wzero-as-null-pointer-constant)
  elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
    # using Intel C++
  elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    check_and_add_cxx_compiler_flag(WARNING_FLAGLIST "/W4")
  endif()
  message(STATUS "WARNING_FLAGLIST: ${WARNING_FLAGLIST}")
endif()
