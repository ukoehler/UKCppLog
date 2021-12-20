# USAGE: To enable any code coverage instrumentation/targets, the single CMake
# option of `CODE_COVERAGE` needs to be set to 'ON', either by GUI, ccmake, or
# on the command line (-DCODE_COVERAGE=ON).
#
# To enable this module set the module path in the main CMakeLists.txt file and
# include this module. This will create a compiler flag list in the variable
# WARNING_FLAGLIST set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
# "${CMAKE_SOURCE_DIR}/cmake/") include(CodeCoverage)

# Add the compiler flags to all the targets that you want the additional
# warnings for. It might not be wanted for test code, for example.
# add_library(liba src/A.cpp src/B.cpp ) 
# add_warning_compile_flags_to_target(liba )


# Options
option(
  CODE_COVERAGE
  "Builds targets with code coverage instrumentation. (Requires GCC or Clang)."
  OFF)

# Programs
find_program(LLVM_COV_PATH llvm-cov)
find_program(LCOV_PATH lcov)
find_program(GENHTML_PATH genhtml)
# Variables
set(CMAKE_COVERAGE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/ccov)

IF(WIN32)
	find_package(Perl)
	set(LCOV_COMMAND ${PERL_EXECUTABLE})
	set(GENHTML_COMMAND ${PERL_EXECUTABLE})
	set(LCOV_COMMAND_ARGS ${LCOV_PATH})
	set(GENHTML_COMMAND_ARGS ${GENHTML_PATH})
ELSE(WIN32)
	set(LCOV_COMMAND ${LCOV_PATH})
	set(GENHTML_COMMAND ${GENHTML_PATH})
ENDIF(WIN32)
message(STATUS "LCOV_COMMAND is ${LCOV_COMMAND} ${LCOV_COMMAND_ARGS}")

# Common initialization/checks
if(CODE_COVERAGE AND NOT CODE_COVERAGE_ADDED)
  set(CODE_COVERAGE_ADDED ON)

  # Common Targets
  add_custom_target(ccov-preprocessing
                    COMMAND ${CMAKE_COMMAND} -E make_directory
                            ${CMAKE_COVERAGE_OUTPUT_DIRECTORY}
                    DEPENDS ccov-clean)

  if("${CMAKE_C_COMPILER_ID}" MATCHES "(Apple)?[Cc]lang"
     OR "${CMAKE_CXX_COMPILER_ID}" MATCHES "(Apple)?[Cc]lang")
    # Messages
    message(STATUS "Building with llvm Code Coverage Tools")

    if(NOT LLVM_COV_PATH)
      message(FATAL_ERROR "llvm-cov not found! Aborting.")
    else()
      # Version number checking for 'EXCLUDE' compatability
      execute_process(COMMAND ${LLVM_COV_PATH} --version
                      OUTPUT_VARIABLE LLVM_COV_VERSION_CALL_OUTPUT)
      string(REGEX MATCH
                   "[0-9]+\\.[0-9]+\\.[0-9]+"
                   LLVM_COV_VERSION
                   ${LLVM_COV_VERSION_CALL_OUTPUT})

      if(LLVM_COV_VERSION VERSION_LESS "7.0.0")
        message(
          WARNING
            "target_code_coverage()/add_code_coverage_all_targets() 'EXCLUDE' option only available on llvm-cov >= 7.0.0"
          )
      endif()
    endif()

    # Targets
    add_custom_target(ccov-clean
                      COMMAND rm -f
                              ${CMAKE_COVERAGE_OUTPUT_DIRECTORY}/binaries.list
                      COMMAND rm -f
                              ${CMAKE_COVERAGE_OUTPUT_DIRECTORY}/profraw.list)

  elseif(CMAKE_COMPILER_IS_GNUCXX)
    # Messages
    message(STATUS "Building with lcov Code Coverage Tools")

    if(CMAKE_BUILD_TYPE)
      string(TOUPPER ${CMAKE_BUILD_TYPE} upper_build_type)
      if(NOT ${upper_build_type} STREQUAL "DEBUG")
        message(
          WARNING
            "Code coverage results with an optimized (non-Debug) build may be misleading"
          )
      endif()
    else()
      message(
        WARNING
          "Code coverage results with an optimized (non-Debug) build may be misleading"
        )
    endif()
    if(NOT LCOV_PATH)
      message(FATAL_ERROR "lcov not found! Aborting...")
    endif()
    if(NOT GENHTML_PATH)
      message(FATAL_ERROR "genhtml not found! Aborting...")
    endif()

    # Targets
    add_custom_target(ccov-clean
                      COMMAND ${LCOV_COMMAND} ${LCOV_COMMAND_ARGS}
                              --directory ${CMAKE_BINARY_DIR}
                              --zerocounters)

  else()
    message(FATAL_ERROR "Code coverage requires Clang or GCC. Aborting.")
  endif()
endif()

# Adds code coverage instrumentation to a library, or instrumentation/targets
# for an executable target.
# ~~~
# EXECUTABLE ADDED TARGETS:
# GCOV/LCOV:
# ccov : Generates HTML code coverage report for every target added with 'AUTO' parameter.
# ccov-${TARGET_NAME} : Generates HTML code coverage report for the associated named target.
# ccov-all : Generates HTML code coverage report, merging every target added with 'ALL' parameter into a single detailed report.
#
# LLVM-COV:
# ccov : Generates HTML code coverage report for every target added with 'AUTO' parameter.
# ccov-report : Generates HTML code coverage report for every target added with 'AUTO' parameter.
# ccov-${TARGET_NAME} : Generates HTML code coverage report.
# ccov-report-${TARGET_NAME} : Prints to command line summary per-file coverage information.
# ccov-show-${TARGET_NAME} : Prints to command line detailed per-line coverage information.
# ccov-all : Generates HTML code coverage report, merging every target added with 'ALL' parameter into a single detailed report.
# ccov-all-report : Prints summary per-file coverage information for every target added with ALL' parameter to the command line.
#
# Required:
# TARGET_NAME - Name of the target to generate code coverage for.
# Optional:
# AUTO - Adds the target to the 'ccov' target so that it can be run in a batch with others easily. Effective on executable targets.
# ALL - Adds the target to the 'ccov-all' and 'ccov-all-report' targets, which merge several executable targets coverage data to a single report. Effective on executable targets.
# EXCLUDE <REGEX_PATTERNS> - Excludes files of the patterns provided from coverage. **These do not copy to the 'all' targets.**
# ~~~
function(target_code_coverage TARGET_NAME)
  message(STATUS "target_code_coverage ${TARGET_NAME}")
  # Argument parsing
  set(options AUTO ALL)
  set(multi_value_keywords EXCLUDE)
  cmake_parse_arguments(target_code_coverage
                        "${options}"
                        ""
                        "${multi_value_keywords}"
                        ${ARGN})

  if(CODE_COVERAGE)
    # Instrumentation
    if("${CMAKE_C_COMPILER_ID}" MATCHES "(Apple)?[Cc]lang"
       OR "${CMAKE_CXX_COMPILER_ID}" MATCHES "(Apple)?[Cc]lang")
      target_compile_options(
        ${TARGET_NAME}
        PRIVATE -fprofile-instr-generate -fcoverage-mapping)
      set_property(TARGET ${TARGET_NAME}
                   APPEND_STRING
                   PROPERTY LINK_FLAGS "-fprofile-instr-generate ")
      set_property(TARGET ${TARGET_NAME}
                   APPEND_STRING
                   PROPERTY LINK_FLAGS "-fcoverage-mapping ")
    elseif(CMAKE_COMPILER_IS_GNUCXX)
      target_compile_options(${TARGET_NAME}
                             PRIVATE -fprofile-arcs -ftest-coverage)
      target_link_libraries(${TARGET_NAME} PRIVATE gcov)
    endif()

    # Targets
    get_target_property(target_type ${TARGET_NAME} TYPE)
    if(target_type STREQUAL "EXECUTABLE")
      if("${CMAKE_C_COMPILER_ID}" MATCHES "(Apple)?[Cc]lang"
         OR "${CMAKE_CXX_COMPILER_ID}" MATCHES "(Apple)?[Cc]lang")
        add_custom_target(
          ccov-run-${TARGET_NAME}
          COMMAND LLVM_PROFILE_FILE=${TARGET_NAME}.profraw
                  $<TARGET_FILE:${TARGET_NAME}>
          COMMAND echo "-object=$<TARGET_FILE:${TARGET_NAME}>" >>
                  ${CMAKE_COVERAGE_OUTPUT_DIRECTORY}/binaries.list
          COMMAND echo "${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}.profraw " >>
                  ${CMAKE_COVERAGE_OUTPUT_DIRECTORY}/profraw.list
          DEPENDS ccov-preprocessing ${TARGET_NAME})

        add_custom_target(ccov-processing-${TARGET_NAME}
                          COMMAND llvm-profdata merge -sparse
                                  ${TARGET_NAME}.profraw -o
                                  ${TARGET_NAME}.profdata
                          DEPENDS ccov-run-${TARGET_NAME})

        if(LLVM_COV_VERSION VERSION_GREATER_EQUAL "7.0.0")
          foreach(EXCLUDE_ITEM ${target_code_coverage_EXCLUDE})
            set(EXCLUDE_REGEX ${EXCLUDE_REGEX}
                -ignore-filename-regex='${EXCLUDE_ITEM}')
          endforeach()
        endif()

        add_custom_target(ccov-show-${TARGET_NAME}
                          COMMAND llvm-cov show $<TARGET_FILE:${TARGET_NAME}>
                                  -instr-profile=${TARGET_NAME}.profdata
                                  -show-line-counts-or-regions ${EXCLUDE_REGEX}
                          DEPENDS ccov-processing-${TARGET_NAME})

        add_custom_target(ccov-report-${TARGET_NAME}
                          COMMAND llvm-cov report $<TARGET_FILE:${TARGET_NAME}>
                                  -instr-profile=${TARGET_NAME}.profdata
                                  ${EXCLUDE_REGEX}
                          DEPENDS ccov-processing-${TARGET_NAME})

        add_custom_target(
          ccov-${TARGET_NAME}
          COMMAND llvm-cov show $<TARGET_FILE:${TARGET_NAME}>
                  -instr-profile=${TARGET_NAME}.profdata
                  -show-line-counts-or-regions
                  -output-dir=${CMAKE_COVERAGE_OUTPUT_DIRECTORY}/${TARGET_NAME}
                  -format="html" ${EXCLUDE_REGEX}
          DEPENDS ccov-processing-${TARGET_NAME})

      elseif(CMAKE_COMPILER_IS_GNUCXX)
        set(COVERAGE_INFO
            "${CMAKE_COVERAGE_OUTPUT_DIRECTORY}/${TARGET_NAME}.info")

        add_custom_target(ccov-run-${TARGET_NAME}
                          COMMAND $<TARGET_FILE:${TARGET_NAME}>
                          DEPENDS ccov-preprocessing ${TARGET_NAME})

        foreach(EXCLUDE_ITEM ${target_code_coverage_EXCLUDE})
          set(EXCLUDE_REGEX
              ${EXCLUDE_REGEX}
              --remove
              ${COVERAGE_INFO}
              '${EXCLUDE_ITEM}')
        endforeach()

        if(EXCLUDE_REGEX)
          set(EXCLUDE_COMMAND
              ${LCOV_COMMAND} ${LCOV_COMMAND_ARGS}
              ${EXCLUDE_REGEX}
              --output-file
              ${COVERAGE_INFO})
        else()
          set(EXCLUDE_COMMAND ;)
        endif()

        add_custom_target(
          ccov-${TARGET_NAME}
          COMMAND ${LCOV_COMMAND} ${LCOV_COMMAND_ARGS} --directory ${CMAKE_BINARY_DIR} --zerocounters --rc lcov_branch_coverage=1
          COMMAND $<TARGET_FILE:${TARGET_NAME}>
          COMMAND echo "CMAKE_CURRENT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}"
          COMMAND echo "CMAKE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}"
          # --directory ${CMAKE_BINARY_DIR}: coverage for CMake modified headers
          # --directory ${CMAKE_CURRENT_SOURCE_DIR}: coverage for any files in the current source
          #     ignore files in 3rd_party
          # --no-external: Do not produce coverage for files outside of --directory or --base-directory parameters
          COMMAND ${LCOV_COMMAND} ${LCOV_COMMAND_ARGS}
                  --directory ${CMAKE_BINARY_DIR}
                  --base-directory ${CMAKE_SOURCE_DIR}
                  --no-external
                  --capture
                  --output-file ${COVERAGE_INFO}
          COMMAND ${EXCLUDE_COMMAND}
          COMMAND ${GENHTML_COMMAND} ${GENHTML_COMMAND_ARGS} -o
                  ${CMAKE_COVERAGE_OUTPUT_DIRECTORY}/${TARGET_NAME}
                  --prefix ${CMAKE_SOURCE_DIR}
                  --demangle-cpp
                  ${COVERAGE_INFO}
          #COMMAND ${CMAKE_COMMAND} -E remove ${COVERAGE_INFO}
          DEPENDS ccov-preprocessing ${TARGET_NAME})
      endif()

      add_custom_command(
        TARGET ccov-${TARGET_NAME} POST_BUILD
        COMMAND ;
        COMMENT
          "Open ${CMAKE_COVERAGE_OUTPUT_DIRECTORY}/${TARGET_NAME}/index.html in your browser to view the coverage report."
        )

      # AUTO
      if(target_code_coverage_AUTO)
        if(NOT TARGET ccov)
          add_custom_target(ccov)
        endif()
        add_dependencies(ccov ccov-${TARGET_NAME})

        if(NOT CMAKE_COMPILER_IS_GNUCXX)
          if(NOT TARGET ccov-report)
            add_custom_target(ccov-report)
          endif()
          add_dependencies(ccov-report ccov-report-${TARGET_NAME})
        endif()
      endif()

      # ALL
      if(target_code_coverage_ALL)
        if(NOT TARGET ccov-all-processing)
          message(
            FATAL_ERROR
              "Calling target_code_coverage with 'ALL' must be after a call to 'add_code_coverage_all_targets'."
            )
        endif()

        add_dependencies(ccov-all-processing ccov-run-${TARGET_NAME})
      endif()
    endif()
  endif()
endfunction()
