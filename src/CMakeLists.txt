#Add a library

add_library(ukcpplog UKLogger.cpp)

install(TARGETS ukcpplog 
    EXPORT ukcpplog
    LIBRARY DESTINATION lib 
    ARCHIVE DESTINATION lib 
    INCLUDES DESTINATION include)

install(FILES UKLogger.hpp 
    DESTINATION include)

install(EXPORT ukcpplog 
    FILE UKCppLogConfig.cmake 
    NAMESPACE UKCppLog::
    DESTINATION lib/cmake/UKCppLog)

add_warning_compile_flags_to_target(ukcpplog)

add_static_code_checks_to_target(ukcpplog)

target_code_coverage(ukcpplog)
