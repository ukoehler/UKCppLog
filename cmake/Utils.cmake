# Utils.cmake
# -----------
#
# A collection of helper functions

# When using target_link_libraries the header should be included as SYSTEM,
# aren't. Here is a solution found here:
# https://stackoverflow.com/questions/52135983/cmake-target-link-libraries-include-as-system-to-suppress-compiler-warnings
#
function(target_link_libraries_system target)
  set(options PRIVATE PUBLIC INTERFACE)
  cmake_parse_arguments(TLLS "${options}" "" "" ${ARGN})
  foreach(op ${options})
    if(TLLS_${op})
      set(scope ${op})
    endif()
  endforeach(op)
  set(libs ${TLLS_UNPARSED_ARGUMENTS})

  foreach(lib ${libs})
    get_target_property(lib_include_dirs ${lib} INTERFACE_INCLUDE_DIRECTORIES)
    if(lib_include_dirs)
      if(scope)
        target_include_directories(${target} SYSTEM ${scope} ${lib_include_dirs})
      else()
        target_include_directories(${target} SYSTEM PRIVATE ${lib_include_dirs})
      endif()
    else()
      message("Warning: ${lib} doesn't set INTERFACE_INCLUDE_DIRECTORIES. No include_directories set.")
    endif()
    if(scope)
      target_link_libraries(${target} ${scope} ${lib})
    else()
      target_link_libraries(${target} ${lib})
    endif()
  endforeach()
endfunction(target_link_libraries_system)

if(DEFINED ENV{GITHUB_ACTIONS})
  add_custom_target(show_version
    COMMAND ${CMAKE_COMMAND} -E echo ""
    COMMAND ${CMAKE_COMMAND} -E echo "CMAKE_VERSION ${CMAKE_VERSION}"
    COMMAND ${CMAKE_COMMAND} -E echo "CMAKE_CXX_COMPILER_ID ${CMAKE_CXX_COMPILER_ID}"
    COMMAND ${CMAKE_COMMAND} -E echo "CMAKE_CXX_COMPILER_VERSION ${CMAKE_CXX_COMPILER_VERSION}"
    COMMAND ${CMAKE_COMMAND} -E echo "Running in GitHub Actions"
    COMMAND ${CMAKE_COMMAND} -E echo ""
    VERBATIM
  )
else()
  add_custom_target(show_version
    COMMAND ${CMAKE_COMMAND} -E echo ""
    COMMAND ${CMAKE_COMMAND} -E echo "CMAKE_VERSION ${CMAKE_VERSION}"
    COMMAND ${CMAKE_COMMAND} -E echo "CMAKE_CXX_COMPILER_ID ${CMAKE_CXX_COMPILER_ID}"
    COMMAND ${CMAKE_COMMAND} -E echo "CMAKE_CXX_COMPILER_VERSION ${CMAKE_CXX_COMPILER_VERSION}"
    COMMAND ${CMAKE_COMMAND} -E echo "Running locally"
    COMMAND ${CMAKE_COMMAND} -E echo ""
    VERBATIM
  )
endif()