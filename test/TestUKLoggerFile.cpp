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

#include <sys/stat.h>
#include <sys/types.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <thread>
#include <vector>
#include "../src/UKLogger.hpp"

// Just get some interesting __PRETTY_FUNCTION__ output
class TestClass {
 public:
    const int testFunction([[maybe_unused]] int argument) const {
        UKLOG_ENTER("test class")
        return 42;
    }
};

// Keep code checker happy with declaring the function first
void logTenTimes();

void logTenTimes() {
    for (int i = 0; i < 10; i++) {
        std::ostringstream s;
        s << "Logging number " << i << " from a thread.";
        UKLOG_INFO("test main", s.str())
    }
}

int main() {
    UKLOG_INFO("test main", LOG_FOLDER)
    UKLOG_DEBUG("test main", "Debug")
    UKLOG_INFO("test main", "Info")
    UKLOG_WARN("test main", "Warn")
    UKLOG_ERROR("test main", "Error")
    UKLOG_FATAL("test main", "Fatal")
    TestClass c;
    c.testFunction(42);
    std::filesystem::path logFileName(LOG_FOLDER);
    logFileName /= "TestUKLoggerFile.log";
    std::cout << "Logging to " << logFileName << std::endl;
    uk::log::logger().setLogfileName(logFileName);
    std::vector<std::thread> threads;
    threads.emplace_back(std::thread([&] { logTenTimes(); }));
    threads.emplace_back(std::thread([&] { logTenTimes(); }));
    threads.emplace_back(std::thread([&] { logTenTimes(); }));
    threads.emplace_back(std::thread([&] { logTenTimes(); }));
    threads.emplace_back(std::thread([&] { logTenTimes(); }));
    for (auto &thread : threads) {
        thread.join();
    }
}
