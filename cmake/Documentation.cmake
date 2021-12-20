# USAGE: To copy handbooks and build API documentation with doxygen, the single CMake
# option of `BUILD_DOCUMENTATION` needs to be set to 'ON' (default is On if doxygen is found), either by GUI, ccmake, or
# on the command line (-DBUILD_DOCUMENTATION=ON).
#
# Prerequisites: doxygen and graphviz need to be installed and findable in the PATH
# 	The doxygen template file is expected as Doxyfile.in in the root of the
#   source tree or in the doc subdirectory
#   
# To enable this module set the module path in the main CMakeLists.txt file and
# include this module. This will create a compiler flag list in the variable
# WARNING_FLAGLIST set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
# "${CMAKE_SOURCE_DIR}/cmake/") include(Documentation)
find_package(Doxygen)

option(BUILD_DOCUMENTATION "Build API documentation using Doxygen. (make doc)"
       ${DOXYGEN_FOUND})

if(BUILD_DOCUMENTATION)
	find_file(DOXYFILE_IN "Doxyfile.in"
			PATHS "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/doc"
			NO_DEFAULT_PATH
			DOC "Path to the doxygen configuration template file")
	set(DOXYFILE "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile")
	include(FindPackageHandleStandardArgs)
	find_package_handle_standard_args(DOXYFILE_IN DEFAULT_MSG "DOXYFILE_IN")

	message( STATUS "CMAKE_CURRENT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}")
	message( STATUS "CMAKE_CURRENT_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}")
	
	file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/doc)
	configure_file(${DOXYFILE_IN} ${CMAKE_CURRENT_BINARY_DIR}/doc/Doxyfile @ONLY)

	add_custom_target( doc_doxygen ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/doc/Doxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/doc
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM )	
endif()

