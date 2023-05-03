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
    int testFunction([[maybe_unused]] int argument) const {
        UKLOG_ENTER("test class")
        return 42;
    }
};

int main() {
    UKLOG_INFO("test main", LOG_FOLDER)
    TestClass c;
    c.testFunction(42);
    std::filesystem::path logFileName(LOG_FOLDER);
    logFileName /= "TestUKLoggerFileCannotDelete.log";
    std::cout << "Logging to " << logFileName << std::endl;
    uk::log::logger().setLogfileName(logFileName);
    UKLOG_ERROR("test main", "We should never reach this line")
}
