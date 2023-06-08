/*
 * <Basic C++ logger library.>
 * Copyright 2020  Dr. Uwe Köhler <U.Koehler@gmx.de>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*! \mainpage Basic C++ logger.
 *
 * \section intro_sec Introduction
 * 
 * This class allows for easy logging from C++ (via macros).
 * Logging is available before entering
 * the main() function into a std::string buffer and can be send to a
 * log file or will be dumped after process execution to the command-line.
 *
 * \section install_sec Installation and Usage
 * While this library can be installed (see \ref compile), it is meant to be 
 * included in the CMakeLists.txt as an external reference.
 *
 * \subsection cmake CMake
 * Add the following statements to your CMakeLists.txt:
 * \code{.txt}
 * FetchContent_Declare(
 * ukcpplog
 * GIT_REPOSITORY https://github.com/ukoehler/UKCppLog.git
 * GIT_TAG        c2434e32d6cee677038c46c3cc965c4a9029515a # tag 0.7
 * )
 * FetchContent_MakeAvailable(ukcpplog)
 * \endcode
 * (choose the latest tag) and then add the library to your targets. For example:
 * \code{.txt}
 * target_link_libraries(target ... PRIVATE ukcpplog ...)
 * \endcode
 * 
 * \subsection compile Compile and Install
 * All instructions are given for linux.
 * 
 * Clone the repository from GitHub.com
 * \code{.txt}
 * git clone https://github.com/ukoehler/UKCppLog.git
 * cd UKCppLog
 * \endcode
 * 
 * When configuring the project, the following definitions are important:
 *  - -DCMAKE_BUILD_TYPE=Debug or Release
 *  - -DBUILD_DOCUMENTATION=ON or OFF: Build this documentation or not
 *  - -DCODE_CHECKS=ON or OFF: Enable static code checkers or not
 *  - -DCODE_COVERAGE=ON or OFF: Enable or disable code coverage
 *  - -DCMAKE_INSTALL_PREFIX=/usr/local: Set installation folder (optional)
 * 
 * Configure for a simple debug build with documentation would be 
 * \code{.txt}
 * mkdir build
 * cd build 
 * cmake -DCMAKE_BUILD_TYPE=Debug -DBUILD_DOCUMENTATION=ON -DCODE_CHECKS=OFF -DCODE_COVERAGE=OFF ..
 * \endcode
 * 
 * Build and run tests with the following commands
 * \code{.txt}
 * make or cmake --build .
 * cd test
 * ./UKLoggerLibTest or ctest -C debug
 * \endcode
 * The documentation can be found in \code{.txt} build/doc \endcode
 * 
 * To install use 
 * \code{.txt}
 * make install or cmake --install . --prefix /my/install/prefix
 * \endcode
 * 
 * When build with coverage support use the following to create a
 * report (requires lcov):
 * \code{.txt}
 * cmake --build . --target ccov-UKLoggerLibTest
 * \endcode
 * The report will be in \code{.txt}ccov/UKLoggerLibTest/\endcode
 * 
 * \subsection usage Usage
 * \code{.cpp}
 * #include "UKLogger.hpp"
 * 
 *  int main() {
        // Anything logged before setting the logfile name will
        // be stored and added to the file later
        UKLOG_INFO("test main", LOG_FOLDER)

        // The following line should be the only line printed directly to terminal.
        std::cout << "Logging to " << logFileName << std::endl;
        uk::log::logger().setLogfileName(logFileName);

        UKLOG_DEBUG("test main", "Debug")
        UKLOG_INFO("test main", "Info")
        UKLOG_WARN("test main", "Warn")
        UKLOG_ERROR("test main", "Error")
        UKLOG_FATAL("test main", "Fatal")
    }
 * \endcode
 * Logging is thread save.
 * The following macros can be used for logging:
 * \code{.cpp}
 * UKLOG_ENTER(kind)
 * \endcode
 * used to trace entering a function. Kind can be anything like class name.

 * \code{.cpp}
 * UKLOG_LEAVE(kind)
 * \endcode
 * used to trace exiting a function without return value. 
 * Kind can be anything like class name.

 * \code{.cpp}
 * UKLOG_LEAVE(kind, retval)
 * \endcode
 * used to trace exiting a function with return value. 
 * Kind can be anything like class name. Retval is the returned value.

 * \code{.cpp}
 * UKLOG_DEBUG(kind, message)
 * \endcode
 * used to trace entering a function. Kind can be anything like class name.
 * Message is any string to log. Replace DEBUG with one of the levels:
 * DEBUG, INFO, WARN, ERROR, FATAL with increasing severity.

 * \subsection output Sample Output
 * \code{.txt}
2023.06.06 21:05:34.897 INFO     [140447341827008] (Startup             )                uk::log::UKLogger::UKLogger(...)     36: Create logger Version 0.7
2023.06.06 21:05:34.897 INFO     [140447341827008] (test main           )                                       main(...)     53: /var/data1/home/ukoehler/devel/C++/UKCppLog/test/../testdata/output
2023.06.06 21:05:34.897 DEBUG    [140447341827008] (test main           )                                       main(...)     54: Debug
2023.06.06 21:05:34.897 INFO     [140447341827008] (test main           )                                       main(...)     55: Info
2023.06.06 21:05:34.897 WARN     [140447341827008] (test main           )                                       main(...)     56: Warn
2023.06.06 21:05:34.897 ERROR    [140447341827008] (test main           )                                       main(...)     57: Error
2023.06.06 21:05:34.897 FATAL    [140447341827008] (test main           )                                       main(...)     58: Fatal
2023.06.06 21:05:34.897 TRACE    [140447341827008] (test class          )                    TestClass::testFunction(...)     36: Enter const int TestClass::testFunction(int) const
2023.06.06 21:05:34.898 WARN     [140447341827008] (Startup             )          uk::log::UKLogger::setLogfileName(...)    139: Logfile /var/data1/home/ukoehler/devel/C++/UKCppLog/test/../testdata/output/TestUKLoggerFile.log exists. Deleting.
2023.06.06 21:05:34.898 INFO     [140447326398016] (test main           )                                logTenTimes(...)     48: Logging number 0 from a thread.
 * \endcode
 * Given is the date and time, the severity, thread id, kind, function name, line number and the message.
 */

#ifndef SRC_UKLOGGER_HPP_
#define SRC_UKLOGGER_HPP_

#include <fstream>
#include <memory>
#include <mutex>
#include <sstream>
#include <filesystem>
#include <string>

namespace uk::log {

/**
 * @brief Basic C++ logger class.
 * This class allows for easy logging from C++ (via macros).
 * Logging is available before entering
 * the main() function into a std::string buffer and can be send to a
 * log file or will be dumped after process execution to the command-line.
 *
 *
 * \remark This class is implemented as a singleton.
 * \remark In C++11 this class is fully thread save.
 */
class UKLogger {
 public:
    /**
     * @brief Constructor.
     */
    UKLogger();

    /**
     * @brief Destructor.
     * Dump logging to the command line if no file name has been set.
     */
    virtual ~UKLogger();

    /**
     * @brief Log an entry.
     * This member is thread save.
     *
     * @param [in] severity A string denoting the severity of the entry,
     * eg "INFO" or "ERROR".
     * @param [in] kind A string denoting the kind of the entry, normally
     * the class name.
     * @param [in] func The name of the function the entry is is logged
     * in.
     * @param [in] message The logging message.
     * @param [in] line The line in the source code this entry is logged
     * from.
     * @pre None.
     * @post Log line added.
     */
    void log(const std::string& severity, const std::string& kind, const std::string& func, const std::string& message,
             int line = 0);

    /**
     * @brief Set name of the logfile.
     * If the logfile does exist, it will be deleted first. Should that
     * fail, the application will fail immediately. This is also the case if
     * the log file cannot be opened.
     * @param [in] name Name of the logfile.
     * @pre None.
     * @post All subsequential logging is added to file.
     */
    void setLogfileName(const std::string& name);

    /**
     * @brief Set name of the logfile.
     * If the logfile does exist, it will be deleted first. Should that
     * fail, the application will fail immediately. This is also the case if
     * the log file cannot be opened.
     * @param [in] name Name of the logfile as a std::path.
     * @pre None.
     * @post All subsequential logging is added to file.
     */
    void setLogfileName(const std::filesystem::path& name);

    /**
     * @brief Get the version of the library.
     * 
     * @return std::string: The version of the library.
     * @pre None.
     * @post None.
    */
    static std::string getVersion();

    // No copy
    UKLogger(const UKLogger& other) = delete;

    // No assignment
    UKLogger& operator=(const UKLogger& other) = delete;

    // No move copy
    UKLogger(const UKLogger&& other) = delete;

    // No move assignment
    UKLogger& operator=(const UKLogger&& other) = delete;

 private:
    void          moveToTerminal();
    std::mutex    mMutex;
    std::string   mInitialBuffer;
    std::ofstream mFileStream;
    bool          mFileOpen{false};

    // Q_DISABLE_COPY(UKLogger)
};

/**
 * @brief Return the instance of the singleton UKLogger.
 * A new instance is created if none is available.
 * @return Return the instance of the singleton UKLogger
 */
UKLogger& logger();

}  // namespace uk::log

// Since __PRETTY_FUNCTION__ is not official C++
// Provide a macro for VisualStudio. GCC nd Clang should be fine
#if !defined(__PRETTY_FUNCTION__) && !defined(__GNUC__)
#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif

// future version of clang-tidy might not require the static_cast<const char*>(__PRETTY_FUNCTION__)
// macros are no bueno, but __PRETTY_FUNCTION__ requires them here
/**
 * @brief Convenience macro to log entering a function
 * @param [in] kind A string denoting the kind of the entry, normally
 * the class name
 */
#define UKLOG_ENTER(kind)                                                               \
    uk::log::logger().log("TRACE", kind, static_cast<const char*>(__PRETTY_FUNCTION__), \
                          std::string("Enter ") + static_cast<const char*>(__PRETTY_FUNCTION__), __LINE__);

/**
 * @brief Convenience macro to log exiting a function
 * @param [in] kind A string denoting the kind of the entry, normally
 * the class name
 */
#define UKLOG_LEAVE(kind)                                                               \
    uk::log::logger().log("TRACE", kind, static_cast<const char*>(__PRETTY_FUNCTION__), \
                          std::string("Leave ") + static_cast<const char*>(__PRETTY_FUNCTION__), __LINE__);

/**
 * @brief Convenience macro to log exiting a function with a return value
 * @param [in] kind A string denoting the kind of the entry, normally
 * the class name
 * @param [in] retval The return value
 */
#define UKLOG_LEAVE_VALUE(kind, retval)                                                                      \
    uk::log::logger().log(                                                                                   \
        "TRACE", kind, static_cast<const char*>(__PRETTY_FUNCTION__),                                        \
        std::string("Leave %1 returning %2").arg(static_cast<const char*>(__PRETTY_FUNCTION__)).arg(retval), \
        __LINE__);

/**
 * @brief Convenience macro to log a debug message
 * @param [in] kind A string denoting the kind of the entry, normally
 * the class name
 * @param [in] message The logging message
 */
#define UKLOG_DEBUG(kind, message) \
    uk::log::logger().log("DEBUG", kind, static_cast<const char*>(__PRETTY_FUNCTION__), message, __LINE__);

/**
 * @brief Convenience macro to log a info message
 * @param [in] kind A string denoting the kind of the entry, normally
 * the class name
 * @param [in] message The logging message
 */
#define UKLOG_INFO(kind, message) \
    uk::log::logger().log("INFO", kind, static_cast<const char*>(__PRETTY_FUNCTION__), message, __LINE__);

/**
 * @brief Convenience macro to log a warning message
 * @param [in] kind A string denoting the kind of the entry, normally
 * the class name
 * @param [in] message The logging message
 */
#define UKLOG_WARN(kind, message) \
    uk::log::logger().log("WARN", kind, static_cast<const char*>(__PRETTY_FUNCTION__), message, __LINE__);

/**
 * @brief Convenience macro to log an error message
 * @param [in] kind A string denoting the kind of the entry, normally
 * the class name
 * @param [in] message The logging message
 */
#define UKLOG_ERROR(kind, message) \
    uk::log::logger().log("ERROR", kind, static_cast<const char*>(__PRETTY_FUNCTION__), message, __LINE__);

/**
 * @brief Convenience macro to log a fatal message
 * @param [in] kind A string denoting the kind of the entry, normally
 * the class name
 * @param [in] message The logging message
 */
#define UKLOG_FATAL(kind, message) \
    uk::log::logger().log("FATAL", kind, static_cast<const char*>(__PRETTY_FUNCTION__), message, __LINE__);

#endif  // SRC_UKLOGGER_HPP_
