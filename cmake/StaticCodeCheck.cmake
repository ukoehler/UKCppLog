# USAGE: To enable any static code checks, the single CMake
# option of `CODE_CHECKS` needs to be set to 'ON', either by GUI, ccmake, or
# on the command line (-DCODE_CHECKS=ON).
#
# To enable this module set the module path in the main CMakeLists.txt file and
# include this module. 
# set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/") 
# include(StaticCodeCheck)

# Add the static code checks to all the targets that you want the additional
# warnings for. It might not be wanted for test code, for example.
# add_library(liba src/A.cpp src/B.cpp ) 
# add_static_code_checks_to_target(liba )

# Options
option(
  CODE_CHECKS
  "Builds targets with static code checks. (Requires clang-tidy, cppcheck and/or cpplint)."
  OFF)
  
# Find the required executables and print the paths
find_program(CLANG-TIDY-EXE clang-tidy)
find_program(CPPCHECK-EXE cppcheck)
find_program(CPPLINT-EXE cpplint)
message(STATUS "CLANG-TIDY-EXE ${CLANG-TIDY-EXE}")
message(STATUS "CPPCHECK-EXE ${CPPCHECK-EXE}")
message(STATUS "CPPLINT-EXE ${CPPLINT-EXE}")

function(add_static_code_checks_to_target TARGET)
    if(CODE_CHECKS)
        if(CLANG-TIDY-EXE)
            # header-filter: Check all, headers in this (or any?) src directory, ignore others (like cairo)
            # -checks=*: activate all checks
            # -modernize-use-trailing-return-type: there are conflicting rules for trailing-return-type
            #   I prefer the old style for now
            # -llvmlibc-*: I only use CLANG for checking. These checks are useless to me
            # -llvm-header-guard: that header guard style is way too complex
            # -fuchsia-default-arguments-calls: cannot really be avoided when using STL
            # -fuchsia-default-arguments-declarations: I disagree
            # -altera-unroll-loops: keeps things understandable
            # -abseil-string-find-startswith: not using any library
            # -llvm-include-order: not the order I want and conflicts with cpplint
            set_target_properties(${TARGET_LIB} PROPERTIES CXX_CLANG_TIDY "${CLANG-TIDY-EXE};-header-filter=UKCppLog;-checks=*,-modernize-use-trailing-return-type,-llvmlibc-*,-llvm-header-guard,-fuchsia-default-arguments-calls,-fuchsia-default-arguments-declaration,-altera-unroll-loops,-abseil-string-find-startswith,-llvm-include-order")
        endif()
        if(CPPCHECK-EXE)
            set_target_properties(${TARGET} PROPERTIES CXX_CPPCHECK "cppcheck;--enable=all;--std=c++17;")
        endif()
        if(CPPLINT-EXE)
            #set_target_properties(${TARGET} PROPERTIES CXX_CPPLINT "cpplint;")
            get_target_property(SRC ${TARGET} SOURCES)
            list(REMOVE_DUPLICATES SRC)
            list(SORT SRC)
            add_custom_command(TARGET ${TARGET}
                POST_BUILD
                COMMAND "${CMAKE_COMMAND}" -E chdir
                        "${CMAKE_CURRENT_SOURCE_DIR}"
                        "${CPPLINT-EXE}"
                        "--linelength=120"
                        "--filter=-build/include_subdir,-build/include_order,-build/c++11"
                        ${SRC}
                DEPENDS ${SRC}
                COMMENT "Linting ${TARGET}"
                VERBATIM
            )
        endif()
    endif()
endfunction()

  
