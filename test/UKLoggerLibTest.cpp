/*
* Copyright 2021 Dr. Uwe Köhler <U.Koehler@gmx.de>
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#include <filesystem>
#include <regex>
#include <sstream>
#include <thread>
#include <gtest/gtest.h>
#include "../src/UKLogger.hpp"

namespace {
// See PrettyUnitTestResultPrinter in gtest.cc and EmptyTestEventListener in gtest.h
// Add gtest output to logfile
class LogPrinter : public ::testing::EmptyTestEventListener {
 private:
    // Called before any test activity starts.
    void OnTestProgramStart(const ::testing::UnitTest& /* unit_test */) override {}

    // Fired before each iteration of tests starts.
    void OnTestIterationStart(const ::testing::UnitTest& /*unit_test*/,
                            int /*iteration*/) override {}

    void OnEnvironmentsSetUpStart(const ::testing::UnitTest& /*unit_test*/) override {
        UKLOG_INFO("TestSystem", "Global test environment set-up.");
    }

    void OnEnvironmentsSetUpEnd(const ::testing::UnitTest& /*unit_test*/) override {
        UKLOG_INFO("TestSystem", "Global test environment set-up completed.");
    }

    void OnTestSuiteStart(const testing::TestSuite& test_suite) override {
        std::ostringstream s;
        s << "### Starting TestSuite " << test_suite.name();
        UKLOG_INFO("TestSystem", s.str());
    }

    // Called before a test starts.
    void OnTestStart(const ::testing::TestInfo& test_info) override {
        std::ostringstream s;
        s << "=== Starting Test " << test_info.test_case_name() << "." << test_info.name();
        UKLOG_INFO("TestSystem", s.str());
    }

    // Called after a failed assertion or a SUCCEED() invocation.
    void OnTestPartResult(const ::testing::TestPartResult& test_part_result) override {
        std::ostringstream s;
        if (test_part_result.failed()) {
            s << "*** Failure in " << test_part_result.file_name() << ":" << test_part_result.line_number();
            s << " " << test_part_result.summary();
            UKLOG_ERROR("TestSystem", s.str());
        } else {
            s << "*** Success in " << test_part_result.file_name() << ":" << test_part_result.line_number();
            s << " " << test_part_result.summary();
            UKLOG_INFO("TestSystem", s.str());
        }
    }

    // Called after a test ends.
    void OnTestEnd(const ::testing::TestInfo& test_info) override {
        std::ostringstream s;
        if (test_info.result()->Failed()) {
            s << "=== FAILED Test " << test_info.test_case_name() << "." << test_info.name()
            << " (" << test_info.result()->total_part_count() << ") in "
                << test_info.result()->elapsed_time()/1000.0 << " s";
            UKLOG_ERROR("TestSystem", s.str());
        } else {
            s << "=== PASSED Test " << test_info.test_case_name() << "." << test_info.name()
            << " (" << test_info.result()->total_part_count() << ") in "
                << test_info.result()->elapsed_time()/1000.0 << " s";
            UKLOG_INFO("TestSystem", s.str());
        }
    }

    void OnTestSuiteEnd(const testing::TestSuite& test_suite) override {
        std::ostringstream s;
        if (test_suite.Failed()) {
            s << "### FAILED TestSuite " << test_suite.name() << " (" << test_suite.failed_test_count()
                << " of " << test_suite.total_test_count() << " failed) in "
                << test_suite.elapsed_time()/1000.0 << " s";
            UKLOG_ERROR("TestSystem", s.str());
        } else {
            s << "### PASSED TestSuite " << test_suite.name() << " (" << test_suite.failed_test_count()
                << " of " << test_suite.total_test_count() << " failed) in "
                << test_suite.elapsed_time()/1000.0 << " s";
            UKLOG_INFO("TestSystem", s.str());
        }
    }

    void OnEnvironmentsTearDownStart(const ::testing::UnitTest& /*unit_test*/) override {
        UKLOG_INFO("TestSystem", "Global test environment tear-down.");
    }

    void OnEnvironmentsTearDownEnd(const ::testing::UnitTest& /*unit_test*/) override {
        UKLOG_INFO("TestSystem", "Global test environment tear-down completed.");
    }

    void OnTestIterationEnd(const ::testing::UnitTest& /*unit_test*/,
                        int /*iteration*/) override {
    }

    // Called after all test activities have ended.
    void OnTestProgramEnd(const ::testing::UnitTest& unit_test) override {
        std::ostringstream s;
        if (unit_test.Failed()) {
            s << "###### FAILED (" << unit_test.failed_test_count()
                << " of " << unit_test.total_test_count() << " failed) in "
                << unit_test.elapsed_time()/1000.0 << " s";
            UKLOG_ERROR("TestSystem", s.str());
            std::cout << "\"" << s.str() << "\"" << std::endl;
        } else {
            s << "###### PASSED (" << unit_test.failed_test_count()
                << " of " << unit_test.total_test_count() << " failed) in "
                << unit_test.elapsed_time()/1000.0 << " s";
            UKLOG_INFO("TestSystem", s.str());
        }
    }
};  // LogPrinter

class logInfo {
 public:
    explicit logInfo(std::string line) {
        std::string timeRegexp("(\\d{4}\\.\\d{2}\\.\\d{2} \\d{2}:\\d{2}:\\d{2}\\.\\d{3})");
        std::regex re(timeRegexp + " ([A-Z ]{8}) \\[(.*)\\] \\((.{20})\\) (.{47}) (.{6}): (.*)");
        std::smatch match;
        if (std::regex_search(line, match, re) && match.size() > 7) {
            std::cout << match[1] << "  " << match[2] << "  " << match[3] << "  " << match[4]
                << "  " << match[5] << "  " << match[6]  << "  " << match[7] << std::endl;
            mTime = {};
            std::istringstream ss(match[1]);
            // ss.imbue(std::locale("de_DE.utf-8"));
            ss >> std::get_time(&mTime, "%Y.%m.%d %H:%M:%S");
            if (ss.fail()) {
                // std::cout << "Parse failed\n";
            } else {
                // std::cout << std::put_time(&t, "%c") << '\n';
            }
            mSeverity = match[2];
            mThreadID = match[3];
            mKind = match[4];
            mFunction = match[5];
            mLine = std::stoi(match[6]);
            mMessage = match[7];
        }
    }

    std::tm mTime;
    std::string mSeverity;
    std::string mThreadID;
    std::string mKind;
    std::string mFunction;
    int mLine;
    std::string mMessage;
};

std::string ltrim(const std::string &s) {
    return std::regex_replace(s, std::regex("^\\s+"), std::string(""));
}

std::string rtrim(const std::string &s) {
    return std::regex_replace(s, std::regex("\\s+$"), std::string(""));
}

std::string trim(const std::string &s) {
    return ltrim(rtrim(s));
}

std::string exec(const char* cmd);

std::string exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}
}  // namespace

// Execution of the code is performed in external executables. Output is checked here
TEST(UKLOGGER, LogToScreen) {
    std::string output = exec(PROJECT_BIN_DIR "/test/testukcpplogscreen");
    UKLOG_INFO("TEST", output);
    EXPECT_FALSE(output.empty());
    // split output into lines
    std::stringstream ss(output);
    std::string segment;
    std::vector<std::string> lines;
    while (std::getline(ss, segment, '\n')) {
        lines.push_back(segment);  // Spit string at newline character
        logInfo info(segment);
    }
    EXPECT_EQ(57ul, lines.size()) << "Unexpected number of log messages: " << lines.size();
    // Check first line
    logInfo info1(lines.at(0));
    // get current time
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    auto now = std::chrono::system_clock::now();
    auto timer = std::chrono::system_clock::to_time_t(now);
    std::tm tm = *std::localtime(&timer);
    double seconds = std::difftime(timer, std::mktime(&info1.mTime));
    EXPECT_GE(30, seconds);
    EXPECT_EQ(std::string("Startup"), trim(info1.mKind));
    EXPECT_EQ(std::string("uk::log::UKLogger::UKLogger(...)"), trim(info1.mFunction));
    EXPECT_EQ(37, info1.mLine);
    EXPECT_EQ(std::string("Create logger Version ") + VERSION_STRING, trim(info1.mMessage));
}


int main(int argc, char**argv) {
    std::filesystem::path logFileName(LOG_FOLDER);
    logFileName /= "UKLoggerLibTest.log";
    std::cout << "Logging to " << logFileName << std::endl;
    uk::log::logger().setLogfileName(logFileName);
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::UnitTest::GetInstance()->listeners().Append(new LogPrinter);
    return RUN_ALL_TESTS();
}
