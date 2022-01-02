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

#include <iostream>
#include <sstream>
#include <thread>
#include <vector>
#include "../src/UKLogger.hpp"

namespace aa::bb::cc::dd::ee::ff::gg::hh::ii::jj::kk::ll::mm::nn::oo::pp::qq::rr::ss::tt::uu::vv::ww::xx::yy {
class Test1 {
 public:
    static void Log1() {
        UKLOG_INFO("Test1", "Log1");
    }
};
}  // namespace aa::bb::cc::dd::ee::ff::gg::hh::ii::jj::kk::ll::mm::nn::oo::pp::qq::rr::ss::tt::uu::vv::ww::xx::yy

namespace abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz::abcdefghijklmnopqrstuvwxyz {
class Test2 {
 public:
    explicit Test2(__attribute__((unused)) int argument) {
        UKLOG_INFO("Test2", "Constructor");
    }
    virtual ~Test2() {
        UKLOG_INFO("Test2", "Destructor");
    }
    virtual inline void Log2(__attribute__((unused)) int argument) const noexcept{
        UKLOG_INFO("Test2", "Log2");
    }
};
}  // namespace abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz::abcdefghijklmnopqrstuvwxyz

// Just get some interesting __PRETTY_FUNCTION__ output
class TestClass {
 public:
    int testFunction(__attribute__((unused)) int argument) const {
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
    UKLOG_DEBUG("test main", "Debug")
    UKLOG_INFO("test main", "Info")
    UKLOG_WARN("test main", "Warn")
    UKLOG_ERROR("test main", "Error")
    UKLOG_FATAL("test main", "Fatal")
    aa::bb::cc::dd::ee::ff::gg::hh::ii::jj::kk::ll::mm::nn::oo::pp::qq::rr::ss::tt::uu::vv::ww::xx::yy::Test1::Log1();
    {
        abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz::abcdefghijklmnopqrstuvwxyz::Test2 test2(42);
        test2.Log2(1);
    }
    TestClass c;
    c.testFunction(42);
    std::vector<std::thread> threads;
    threads.emplace_back(std::thread([&] { logTenTimes(); }));
    threads.emplace_back(std::thread([&] { logTenTimes(); }));
    threads.emplace_back(std::thread([&] { logTenTimes(); }));
    threads.emplace_back(std::thread([&] { logTenTimes(); }));
    threads.emplace_back(std::thread([&] { logTenTimes(); }));
    for (auto& thread : threads) {
        thread.join();
    }
}
