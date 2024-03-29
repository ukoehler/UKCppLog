#Add a test executables
find_package(Threads REQUIRED)

add_definitions(-D TEST_FOLDER=\\"${CMAKE_CURRENT_SOURCE_DIR}/../testdata/input\\" -D LOG_FOLDER=\\"${CMAKE_CURRENT_SOURCE_DIR}/../testdata/output\\")

add_executable(testukcpplogscreen TestUKLoggerScreen.cpp)

target_link_libraries(testukcpplogscreen PRIVATE ukcpplog Threads::Threads)

add_warning_compile_flags_to_target(testukcpplogscreen)

add_static_code_checks_to_target(testukcpplogscreen)

target_code_coverage(testukcpplogscreen EXCLUDE */test/*)

add_executable(testukcpplogfile TestUKLoggerFile.cpp)

target_link_libraries(testukcpplogfile PRIVATE ukcpplog Threads::Threads)

add_warning_compile_flags_to_target(testukcpplogfile)

add_static_code_checks_to_target(testukcpplogfile)

target_code_coverage(testukcpplogfile EXCLUDE */test/*)

add_executable(testukcpplogfilefailopen TestUKLoggerFileFailOpen.cpp)

target_link_libraries(testukcpplogfilefailopen PRIVATE ukcpplog Threads::Threads)

add_warning_compile_flags_to_target(testukcpplogfilefailopen)

add_static_code_checks_to_target(testukcpplogfilefailopen)

target_code_coverage(testukcpplogfilefailopen EXCLUDE */test/*)

add_executable(testukcpplogfilecannotdelete TestUKLoggerFileCannotDelete.cpp)

target_link_libraries(testukcpplogfilecannotdelete PRIVATE ukcpplog Threads::Threads)

add_warning_compile_flags_to_target(testukcpplogfilecannotdelete)

add_static_code_checks_to_target(testukcpplogfilecannotdelete)

target_code_coverage(testukcpplogfilecannotdelete EXCLUDE */test/*)


enable_testing()

add_executable(UKLoggerLibTest UKLoggerLibTest.cpp)

add_warning_compile_flags_to_target(UKLoggerLibTest)

add_static_code_checks_to_target(UKLoggerLibTest)

target_code_coverage( UKLoggerLibTest EXCLUDE */3rd_party/* */_deps/* */test/*)

target_link_libraries(UKLoggerLibTest PRIVATE ukcpplog gtest)

include(GoogleTest)
gtest_discover_tests(UKLoggerLibTest)
