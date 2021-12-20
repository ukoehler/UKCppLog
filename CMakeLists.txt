cmake_minimum_required (VERSION 3.10)

project(UKCppLog
        VERSION 0.5 DESCRIPTION "A simple C++ logging library"
        LANGUAGES CXX)

set(AUTHOR "Uwe Köhler")
set(VERSION_STRING "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}")
# from CMake 3.12
#add_compile_definitions(VERSION_STRING ${VERSION_STRING} )
add_definitions(-D VERSION_STRING=\\"${VERSION_STRING}\\" -D PROJECT_BIN_DIR=\\"${PROJECT_BINARY_DIR}\\")
set(CMAKE_CXX_STANDARD 17)

# lets see what is actually happening under the hood this is helpful when
# running into problems
set(CMAKE_VERBOSE_MAKEFILE ON)
# Some init settings
set(CMAKE_COLOR_MAKEFILE ON)
# Set path to additional CMake modules
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

message(STATUS "CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH}")

include(HandleCompilerWarnings)
include(CodeCoverage)
include(Documentation)
include(StaticCodeCheck)
message(STATUS "CLANG-TIDY-EXE ${CLANG-TIDY-EXE}")
message(STATUS "CPPCHECK-EXE ${CPPCHECK-EXE}")
message(STATUS "CPPLINT-EXE ${CPPLINT-EXE}")

if( CMAKE_BUILD_TYPE MATCHES Debug)
    message(STATUS "Debug mode")
endif()

# add the CMakeFile that defines the application and library
add_subdirectory(src)
add_subdirectory(test)
