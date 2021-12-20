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
#include <fstream>
#include <iostream>
#include <sstream>
#include <thread>
#include <vector>
#include "../src/UKLogger.hpp"

#if defined(WIN32) || defined(_WIN32)
#define PATH_SEPARATOR "\\"
#else
#define PATH_SEPARATOR "/"
#endif

// Just get some interesting __PRETTY_FUNCTION__ output
class TestClass {
 public:
    const int testFunction(__attribute__((unused)) int argument) const {
        UKLOG_ENTER("test class")
        return 42;
    }
};

void logTenTimes() {
    for (int i = 0; i < 10; i++) {
        std::ostringstream s;
        s << "Logging number " << i << " from a thread.";
        UKLOG_INFO("test main", s.str())
    }
}

int dirExists(const char *path) {
    struct stat info;

    if (stat(path, &info) != 0) {
        return 0;
    } else {
        if (info.st_mode & S_IFDIR) {
            return 1;
        } else {
            return 0;
        }
    }
}

int main() {
    UKLOG_INFO("test main", PROJECT_BIN_DIR)
    UKLOG_DEBUG("test main", "Debug")
    UKLOG_INFO("test main", "Info")
    UKLOG_WARN("test main", "Warn")
    UKLOG_ERROR("test main", "Error")
    UKLOG_FATAL("test main", "Fatal")
    TestClass c;
    c.testFunction(42);
    std::cout << "Logging to " << PATH_SEPARATOR << "var" << PATH_SEPARATOR << "log" << PATH_SEPARATOR << "btmp"
              << std::endl;
    uk::log::logger().setLogfileName(PATH_SEPARATOR "var" PATH_SEPARATOR "log" PATH_SEPARATOR "btmp");
    std::vector<std::thread> threads;
    threads.emplace_back(std::thread([&] { logTenTimes(); }));
    threads.emplace_back(std::thread([&] { logTenTimes(); }));
    threads.emplace_back(std::thread([&] { logTenTimes(); }));
    threads.emplace_back(std::thread([&] { logTenTimes(); }));
    threads.emplace_back(std::thread([&] { logTenTimes(); }));
    for (auto & thread : threads) {
        thread.join();
    }
}
