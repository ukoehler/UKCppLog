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

#include "UKLogger.hpp"
#include <algorithm>
#include <chrono>
#include <cstdio>   // remove
#include <cstdlib>  // exit, EXIT_FAILURE
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <thread>

namespace uk::log {
UKLogger::UKLogger() {
    log("INFO", "Startup", static_cast<const char*>(__PRETTY_FUNCTION__),
        std::string("Create logger Version ") + getVersion(), __LINE__);
}

UKLogger::~UKLogger() { moveToTerminal(); }

void UKLogger::log(const std::string& severity, const std::string& kind, const std::string& func,
                   const std::string& message, int line) {
    // get current time
    auto now = std::chrono::system_clock::now();

    // get number of milliseconds for the current second
    // (remainder after division into seconds)
    int millisToSeconds = 1000;
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % millisToSeconds;

    // convert to std::time_t in order to convert to std::tm (broken time)
    auto timer = std::chrono::system_clock::to_time_t(now);

    std::tm tmStruct = {0};
#if (defined(_MSC_VER))
    localtime_s(&tmStruct, &timer);
#else
    localtime_r(&timer, &tmStruct);  // POSIX
#endif
    std::ostringstream stream;
    stream << std::put_time(&tmStruct, "%Y.%m.%d %H:%M:%S")
           << '.' << std::setfill('0') << std::setw(3) << milliseconds.count();
    std::string timeStr  = stream.str();
    auto        threadID = std::this_thread::get_id();
    std::string function = func;
    // remove leading const
    std::size_t lenOfConst = std::string("const ").length();
    if (0 == function.find("const ")) {
        function.erase(0, lenOfConst);
    }
    // remove trailing const
    if (function.size() - 6 == function.rfind(" const")) {
        function.erase(function.size() - lenOfConst, lenOfConst);
    }
    // remove leading virtual
    std::size_t lenOfVirtual = std::string("virtual ").length();
    if (0 == function.find("virtual ")) {
        function.erase(0, lenOfVirtual);
    }
    // remove leading static
    std::size_t lenOfStatic = std::string("static ").length();
    if (0 == function.find("static ")) {
        function.erase(0, lenOfStatic);
    }
    // remove function return declaration for non-constructors
    size_t openingBraket = function.find('(');
    size_t space         = function.find(' ');
    if (space < openingBraket) {
        function.erase(0, space + 1);
    }
    // remove function argument declaration
    openingBraket         = function.find('(');
    size_t closeingBraket = function.rfind(')');
    if (openingBraket < closeingBraket && std::string::npos != openingBraket) {
        function.erase(openingBraket + 1, closeingBraket - openingBraket - 1);
        function.insert(openingBraket + 1, "...");
    }
    // handle namespaces in function
    std::size_t functionNameLength = 47;
    if (functionNameLength < function.size()) {
        unsigned int numberOfDoubleColons  = 0;
        size_t       nPos                  = 0;
        size_t       lastDoubleColon       = 0;
        size_t       secondLastDoubleColon = 0;
        while ((nPos = function.find("::", nPos)) != std::string::npos) {
            numberOfDoubleColons++;
            secondLastDoubleColon = lastDoubleColon;
            lastDoubleColon       = nPos;
            nPos += 2;
        }
        if (functionNameLength > (function.size() - secondLastDoubleColon + 1)) {
            function.erase(0, secondLastDoubleColon + 2);
            function = "..." + function;
        } else {
            if (functionNameLength > (function.size() - lastDoubleColon + 1)) {
                function.erase(0, lastDoubleColon + 2);
                function = "..." + function;
            }
        }
        //         size_t doubleColon = function.find("::");
        //         while (doubleColon < lastDoubleColon) {
        //             function.erase(0, doubleColon + 2);
        //             doubleColon = function.find("::");
        //         }
    }
    stream.clear();
    stream.str("");
    int severityLength = 8;
    int threadIDLength = 6;
    int kindLength = 20;
    int lineLength = 6;
    stream << std::setfill(' ') << timeStr << " " << std::left << std::setw(severityLength) << severity << " "
      << "[" << std::right << std::setw(threadIDLength) << threadID << "] "
      << "(" << std::left << std::setw(kindLength) << kind.substr(0, static_cast<size_t>(kindLength)) << ") "
      << std::right << std::setw(static_cast<int>(functionNameLength))
      << function.substr(0, functionNameLength) << " " << std::right << std::setw(lineLength) << line << ": "
      << message << std::endl;
    std::lock_guard<std::mutex> lockGuard(mMutex);
    if (mFileOpen) {
        mFileStream << stream.str() << std::flush;
    } else {
        mInitialBuffer += stream.str();
    }
}

void UKLogger::setLogfileName(const std::string& name) {
    mFileStream.open(name, std::ios::out);
    if (mFileStream.good()) {
        mFileStream.close();
        UKLOG_WARN("Startup", "Logfile " + name + " exists. Deleting.");
        if (0 != remove(name.c_str())) {
            UKLOG_FATAL("Startup", "Could not delete existing logfile " + name + ". Quitting.");
            // std::cout << mInitialBuffer << std::flush;
            // std::exit will call the destrector and moveToTerminal()
            std::exit(EXIT_FAILURE);
        }
    }
    mFileStream.open(name, std::ios::out);
    if (!mFileStream.is_open()) {
        UKLOG_FATAL("Startup", "Could not open logfile " + name + ". Quitting.");
        // std::cout << "UKLogger::setLogfileName should exit" << std::endl << std::flush;
        // std::cout << mInitialBuffer << std::flush;
        // std::exit will call the destructor and moveToTerminal()
        std::exit(EXIT_FAILURE);
    }
    std::lock_guard<std::mutex> lockGuard(mMutex);
    mFileOpen = true;
    mFileStream << mInitialBuffer;
}

void UKLogger::setLogfileName(const std::filesystem::path& name) {
    setLogfileName(name.string());
}

std::string UKLogger::getVersion() { return VERSION_STRING; }

void UKLogger::moveToTerminal() {
    if (!mFileOpen) {
        std::cout << mInitialBuffer << std::flush;
    }
}

// public access to instance
UKLogger& logger() {
    // Do not use new here, as the destructor will not be called than
    static UKLogger inst;
    return inst;
}
}  // namespace uk::log
