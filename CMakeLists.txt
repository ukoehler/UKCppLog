cmake_minimum_required (VERSION 3.10)

# Set the project name
project(UKCppLog
        VERSION 0.5 DESCRIPTION "A simple C++ logging library"
        LANGUAGES CXX)

set(AUTHOR "Uwe Köhler")
set(VERSION_STRING "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}")
# from CMake 3.12
#add_compile_definitions(VERSION_STRING ${VERSION_STRING} )
# inner "" to get C++ to treat definition as a string, outer "" to handle spaces
add_definitions(-D VERSION_STRING="\\"${VERSION_STRING}\\"" -D PROJECT_BIN_DIR="\\"${PROJECT_BINARY_DIR}\\"")
set(CMAKE_CXX_STANDARD 17)

if(NOT DEFINED ENV{GITHUB_ACTIONS})
    # lets see what is actually happening under the hood this is helpful when
    # running into problems
    set(CMAKE_VERBOSE_MAKEFILE ON)
endif()
# Some init settings
set(CMAKE_COLOR_MAKEFILE ON)
# Set path to additional CMake modules
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

message(STATUS "CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH}")

include(Utils)
include(HandleCompilerWarnings)
include(CodeCoverage)
include(Documentation)
include(StaticCodeCheck)
include(FetchContent)

# the RPATH to be used when installing, but only if it's not a system directory
LIST(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/lib" isSystemDir)
IF("${isSystemDir}" STREQUAL "-1")
   SET(CMAKE_INSTALL_RPATH "$ORIGIN/../lib")
ENDIF("${isSystemDir}" STREQUAL "-1")

set(DEBUG_POSTFIX "_g")

message(STATUS "CLANG-TIDY-EXE ${CLANG-TIDY-EXE}")
message(STATUS "CPPCHECK-EXE ${CPPCHECK-EXE}")
message(STATUS "CPPLINT-EXE ${CPPLINT-EXE}")
if( CMAKE_BUILD_TYPE MATCHES Debug)
    message(STATUS "Debug mode")
endif()

# add the CMakeFile that defines the application and library
add_subdirectory(3rdparty)
add_subdirectory(src)
add_subdirectory(test)

# General packaging support
# Not used at the moment
set(CPACK_PACKAGE_VENDOR ${AUTHOR})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY ${PROJECT_DESCRIPTION})
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
#set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENCE")
#set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")

# Add rule to create a source package
# use make package_source
# create a *.tgz and a *.zip file
set(CPACK_SOURCE_GENERATOR "TGZ;ZIP")
# include all file apart from the following
set(CPACK_SOURCE_IGNORE_FILES
    /.git
    /dist
    /.*build.*
    /\\\\.DS_Store
)
include(CPack)
