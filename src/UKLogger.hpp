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
 * log file or will be dumped after process execution to the commandline.
 *
 * When using a QObject by mEngine.rootContext()->setContextProperty("ukLogger", &uk::log::logger());
 * the singleton mechanism stops working and an additional instance is
 * created. Move all Signal/Slot mechanisms used in connection with QML
 * to UKLoggerObject.
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
     * Dump logging to the command line if not file name has been set.
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
     * fail, the application will fail immediately.
     * @param [in] name Name of the logfile.
     * @pre None.
     * @post All subsequential logging is added to file.
     */
    void setLogfileName(const std::string& name);

    /**
     * @brief Set name of the logfile.
     * If the logfile does exist, it will be deleted first. Should that
     * fail, the application will fail immediately.
     * @param [in] name Name of the logfile.
     * @pre None.
     * @post All subsequential logging is added to file.
     */
    void setLogfileName(const std::filesystem::path& name);

    /**
     * @brief Get the version of the library.
     * 
     * @return std::string: The version of the library.
     * @pre None.
     * @post Log line added.
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
