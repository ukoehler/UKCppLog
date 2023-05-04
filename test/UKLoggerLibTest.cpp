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

#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>
#include <map>
#include <array>
#include <regex>
#include <sstream>
#include <thread>
#include "../src/UKLogger.hpp"

#ifdef _WIN32
#define popen _popen
#define pclose _pclose
#define WEXITSTATUS
#endif
namespace {
// See PrettyUnitTestResultPrinter in gtest.cc and EmptyTestEventListener in gtest.h
// Add gtest output to logfile
class LogPrinter : public ::testing::EmptyTestEventListener {
 private:
    // Called before any test activity starts.
    void OnTestProgramStart(const ::testing::UnitTest& /* unit_test */) override {}

    // Fired before each iteration of tests starts.
    void OnTestIterationStart(const ::testing::UnitTest& /*unit_test*/, int /*iteration*/) override {}

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
            s << "=== FAILED Test " << test_info.test_case_name() << "." << test_info.name() << " ("
              << test_info.result()->total_part_count() << ") in " << test_info.result()->elapsed_time() / 1000.0
              << " s";
            UKLOG_ERROR("TestSystem", s.str());
        } else {
            s << "=== PASSED Test " << test_info.test_case_name() << "." << test_info.name() << " ("
              << test_info.result()->total_part_count() << ") in " << test_info.result()->elapsed_time() / 1000.0
              << " s";
            UKLOG_INFO("TestSystem", s.str());
        }
    }

    void OnTestSuiteEnd(const testing::TestSuite& test_suite) override {
        std::ostringstream s;
        if (test_suite.Failed()) {
            s << "### FAILED TestSuite " << test_suite.name() << " (" << test_suite.failed_test_count() << " of "
              << test_suite.total_test_count() << " failed) in " << test_suite.elapsed_time() / 1000.0 << " s";
            UKLOG_ERROR("TestSystem", s.str());
        } else {
            s << "### PASSED TestSuite " << test_suite.name() << " (" << test_suite.failed_test_count() << " of "
              << test_suite.total_test_count() << " failed) in " << test_suite.elapsed_time() / 1000.0 << " s";
            UKLOG_INFO("TestSystem", s.str());
        }
    }

    void OnEnvironmentsTearDownStart(const ::testing::UnitTest& /*unit_test*/) override {
        UKLOG_INFO("TestSystem", "Global test environment tear-down.");
    }

    void OnEnvironmentsTearDownEnd(const ::testing::UnitTest& /*unit_test*/) override {
        UKLOG_INFO("TestSystem", "Global test environment tear-down completed.");
    }

    void OnTestIterationEnd(const ::testing::UnitTest& /*unit_test*/, int /*iteration*/) override {}

    // Called after all test activities have ended.
    void OnTestProgramEnd(const ::testing::UnitTest& unit_test) override {
        std::ostringstream s;
        if (unit_test.Failed()) {
            s << "###### FAILED (" << unit_test.failed_test_count() << " of " << unit_test.total_test_count()
              << " failed) in " << unit_test.elapsed_time() / 1000.0 << " s";
            UKLOG_ERROR("TestSystem", s.str());
            std::cout << "\"" << s.str() << "\"" << std::endl;
        } else {
            s << "###### PASSED (" << unit_test.failed_test_count() << " of " << unit_test.total_test_count()
              << " failed) in " << unit_test.elapsed_time() / 1000.0 << " s";
            UKLOG_INFO("TestSystem", s.str());
        }
    }
};  // LogPrinter

class logInfo {
 public:
    explicit logInfo(std::string line) {
        std::string timeRegexp("(\\d{4}\\.\\d{2}\\.\\d{2} \\d{2}:\\d{2}:\\d{2}\\.\\d{3})");
        std::regex  re(timeRegexp + " ([A-Z ]{8}) \\[(.*)\\] \\((.{20})\\) (.{47}) (.{6}): (.*)");
        std::smatch match;
        if (std::regex_search(line, match, re) && match.size() > 7) {
            // std::cout << match[1] << "  " << match[2] << "  " << match[3] << "  " << match[4] << "  " << match[5]
            //           << "  " << match[6] << "  " << match[7] << std::endl;
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
            mKind     = match[4];
            mFunction = match[5];
            mLine     = std::stoi(match[6]);
            mMessage  = match[7];
        }
    }

    std::tm     mTime;
    std::string mSeverity;
    std::string mThreadID;
    std::string mKind;
    std::string mFunction;
    int         mLine;
    std::string mMessage;
};

std::string ltrim(const std::string& s) { return std::regex_replace(s, std::regex("^\\s+"), std::string("")); }

std::string rtrim(const std::string& s) { return std::regex_replace(s, std::regex("\\s+$"), std::string("")); }

std::string trim(const std::string& s) { return ltrim(rtrim(s)); }

std::string exec(const char* cmd);

std::string exec(const char* cmd) {
    std::array<char, 128>                    buffer;
    std::string                              result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

void checkLogLine(const std::string &lineStr, const time_t &timer,
                  const std::string &severity, const std::string &kind,
                  const std::string &function,
                  int line, const std::string &message) {
    logInfo info(lineStr);
    double  seconds = std::difftime(timer, std::mktime(&info.mTime));
    EXPECT_GE(30, seconds);
    EXPECT_EQ(severity, trim(info.mSeverity));
    EXPECT_EQ(kind, trim(info.mKind));
    EXPECT_EQ(function, trim(info.mFunction));
    EXPECT_EQ(line, info.mLine);
    EXPECT_EQ(message, trim(info.mMessage));
}

void logPermissons(std::filesystem::perms p) {
    std::stringstream ss;
    ss  << ((p & std::filesystem::perms::owner_read) != std::filesystem::perms::none ? "r" : "-")
        << ((p & std::filesystem::perms::owner_write) != std::filesystem::perms::none ? "w" : "-")
        << ((p & std::filesystem::perms::owner_exec) != std::filesystem::perms::none ? "x" : "-")
        << ((p & std::filesystem::perms::group_read) != std::filesystem::perms::none ? "r" : "-")
        << ((p & std::filesystem::perms::group_write) != std::filesystem::perms::none ? "w" : "-")
        << ((p & std::filesystem::perms::group_exec) != std::filesystem::perms::none ? "x" : "-")
        << ((p & std::filesystem::perms::others_read) != std::filesystem::perms::none ? "r" : "-")
        << ((p & std::filesystem::perms::others_write) != std::filesystem::perms::none ? "w" : "-")
        << ((p & std::filesystem::perms::others_exec) != std::filesystem::perms::none ? "x" : "-")
        << std::endl;
    UKLOG_INFO("logPermissons", ss.str());
}

}  // namespace

// Execution of the code is performed in external executables. Output is checked here
// Test logging to screen
TEST(UKLOGGER, LogToScreen) {
    UKLOG_INFO("TEST", PROJECT_BIN_DIR "/test/testukcpplogscreen");
    std::string output = exec("\"" PROJECT_BIN_DIR "/test/testukcpplogscreen" "\"");
    UKLOG_INFO("TEST", output);
    EXPECT_FALSE(output.empty());
    // split output into lines
    std::stringstream        ss(output);
    std::string              segment;
    std::vector<std::string> lines;
    while (std::getline(ss, segment, '\n')) {
        lines.push_back(segment);  // Spit string at newline character
    }
    EXPECT_EQ(61ul, lines.size()) << "Unexpected number of log messages: " << lines.size();
    // get current time
    auto    now     = std::chrono::system_clock::now();
    auto    timer   = std::chrono::system_clock::to_time_t(now);
    // Check line 1
    checkLogLine(lines.at(0), timer, "INFO", "Startup",
                 "uk::log::UKLogger::UKLogger(...)", 36,
                 std::string("Create logger Version ") + VERSION_STRING);


    // Check line 2
    checkLogLine(lines.at(1), timer, "DEBUG", "test main",
                 "main(...)", 73,
                 "Debug");

    // Check line 3
    logInfo info3(lines.at(2));
    // get current time
    double seconds = std::difftime(timer, std::mktime(&info3.mTime));
    EXPECT_GE(30, seconds);
    EXPECT_EQ("INFO", trim(info3.mSeverity));
    EXPECT_EQ(std::string("test main"), trim(info3.mKind));
    EXPECT_EQ(std::string("main(...)"), trim(info3.mFunction));
    EXPECT_EQ(74, info3.mLine);
    EXPECT_EQ("Info", trim(info3.mMessage));

    // Check line 4
    logInfo info4(lines.at(3));
    // get current time
    seconds = std::difftime(timer, std::mktime(&info4.mTime));
    EXPECT_GE(30, seconds);
    EXPECT_EQ("WARN", trim(info4.mSeverity));
    EXPECT_EQ(std::string("test main"), trim(info4.mKind));
    EXPECT_EQ(std::string("main(...)"), trim(info4.mFunction));
    EXPECT_EQ(75, info4.mLine);
    EXPECT_EQ("Warn", trim(info4.mMessage));

    // Check line 5
    logInfo info5(lines.at(4));
    // get current time
    seconds = std::difftime(timer, std::mktime(&info5.mTime));
    EXPECT_GE(30, seconds);
    EXPECT_EQ("ERROR", trim(info5.mSeverity));
    EXPECT_EQ(std::string("test main"), trim(info5.mKind));
    EXPECT_EQ(std::string("main(...)"), trim(info5.mFunction));
    EXPECT_EQ(76, info5.mLine);
    EXPECT_EQ("Error", trim(info5.mMessage));

    // Check line 6
    logInfo info6(lines.at(5));
    // get current time
    seconds = std::difftime(timer, std::mktime(&info6.mTime));
    EXPECT_GE(30, seconds);
    EXPECT_EQ("FATAL", trim(info6.mSeverity));
    EXPECT_EQ(std::string("test main"), trim(info6.mKind));
    EXPECT_EQ(std::string("main(...)"), trim(info6.mFunction));
    EXPECT_EQ(77, info6.mLine);
    EXPECT_EQ("Fatal", trim(info6.mMessage));

    // Check line 7
    logInfo info7(lines.at(6));
    // get current time
    seconds = std::difftime(timer, std::mktime(&info7.mTime));
    EXPECT_GE(30, seconds);
    EXPECT_EQ("INFO", trim(info7.mSeverity));
    EXPECT_EQ(std::string("Test1"), trim(info7.mKind));
    EXPECT_EQ(std::string("...Test1::Log1(...)"), trim(info7.mFunction));
    EXPECT_EQ(32, info7.mLine);
    EXPECT_EQ("Log1", trim(info7.mMessage));

    // Check line 8
    logInfo info8(lines.at(7));
    // get current time
    seconds = std::difftime(timer, std::mktime(&info8.mTime));
    EXPECT_GE(30, seconds);
    EXPECT_EQ("INFO", trim(info8.mSeverity));
    EXPECT_EQ(std::string("Test2"), trim(info8.mKind));
    EXPECT_EQ(std::string("...Test2::Test2(...)"), trim(info8.mFunction));
    EXPECT_EQ(41, info8.mLine);
    EXPECT_EQ("Constructor", trim(info8.mMessage));

    // Check line 9
    logInfo info9(lines.at(8));
    // get current time
    seconds = std::difftime(timer, std::mktime(&info9.mTime));
    EXPECT_GE(30, seconds);
    EXPECT_EQ("INFO", trim(info9.mSeverity));
    EXPECT_EQ(std::string("Test2"), trim(info9.mKind));
    EXPECT_EQ(std::string("...Test2::Log2(...)"), trim(info9.mFunction));
    EXPECT_EQ(47, info9.mLine);
    EXPECT_EQ("Log2", trim(info9.mMessage));

    // Check line 10
    logInfo info10(lines.at(9));
    // get current time
    seconds = std::difftime(timer, std::mktime(&info10.mTime));
    EXPECT_GE(30, seconds);
    EXPECT_EQ("INFO", trim(info10.mSeverity));
    EXPECT_EQ(std::string("Test2"), trim(info10.mKind));
    EXPECT_EQ(std::string("...Test2::~Test2(...)"), trim(info10.mFunction));
    EXPECT_EQ(44, info10.mLine);
    EXPECT_EQ("Destructor", trim(info10.mMessage));

    // Check line 11
    logInfo info11(lines.at(10));
    // get current time
    seconds = std::difftime(timer, std::mktime(&info11.mTime));
    EXPECT_GE(30, seconds);
    EXPECT_EQ("TRACE", trim(info11.mSeverity));
    EXPECT_EQ(std::string("test class"), trim(info11.mKind));
    EXPECT_EQ(std::string("TestClass::testFunction(...)"), trim(info11.mFunction));
    EXPECT_EQ(56, info11.mLine);
    EXPECT_EQ("Enter int TestClass::testFunction(int) const", trim(info11.mMessage));

    // Test thread safety and thread ids
    std::map<std::string, int> threadIDMap;
    for (unsigned int i = 11; i < lines.size(); ++i) {
        logInfo info(lines.at(i));
        double seconds = std::difftime(timer, std::mktime(&info.mTime));
        EXPECT_GE(30, seconds);
        EXPECT_EQ("INFO", trim(info.mSeverity));
        EXPECT_EQ(std::string("test main"), trim(info.mKind));
        EXPECT_EQ(std::string("logTenTimes(...)"), trim(info.mFunction));
        EXPECT_EQ(68, info.mLine);
        if (0 == threadIDMap.count(info.mThreadID)) {
            threadIDMap.insert({info.mThreadID, 1});
        } else {
            threadIDMap[info.mThreadID]++;
        }
    }
    EXPECT_EQ(5ul, threadIDMap.size());
    for (auto iter = threadIDMap.begin(); iter != threadIDMap.end(); ++iter) {
        EXPECT_EQ(10, iter->second);
    }
}

// Execution of the code is performed in external executables. Output is checked here
// Test logging to file
TEST(UKLOGGER, LogToFile) {
    std::string output = exec("\"" PROJECT_BIN_DIR "/test/testukcpplogfile" "\"");
    UKLOG_INFO("TEST", output);
    EXPECT_FALSE(output.empty());
    // split output into lines
    std::stringstream        ss(output);
    std::string              segment;
    std::vector<std::string> lines;
    while (std::getline(ss, segment, '\n')) {
        lines.push_back(segment);  // Spit string at newline character
    }
    EXPECT_EQ(1ul, lines.size()) << "Unexpected number of log messages: " << lines.size();
    std::filesystem::path logFileName(LOG_FOLDER);
    logFileName /= "TestUKLoggerFile.log";
    EXPECT_EQ(std::string("Logging to \"") + logFileName.u8string() + "\"\n", output);
    std::fstream logFile;
    logFile.open(logFileName, std::ios::in);
    EXPECT_TRUE(logFile.is_open());
    lines.clear();
    if (logFile.is_open()) {
        std::string line;
        while (getline(logFile, line)) {
            lines.push_back(line);
            // UKLOG_INFO("TEST", line);
        }
    }
    EXPECT_EQ(59ul, lines.size()) << "Unexpected number of log messages: " << lines.size();
    // get current time
    auto    now     = std::chrono::system_clock::now();
    auto    timer   = std::chrono::system_clock::to_time_t(now);
    // Check line 1
    checkLogLine(lines.at(0), timer, "INFO", "Startup",
                 "uk::log::UKLogger::UKLogger(...)", 36,
                 std::string("Create logger Version ") + VERSION_STRING);

    // Check line 2
    checkLogLine(lines.at(1), timer, "INFO", "test main",
                 "main(...)", 53,
                 LOG_FOLDER);

    // Check line 3
    checkLogLine(lines.at(2), timer, "DEBUG", "test main",
                 "main(...)", 54,
                 "Debug");

    // Check line 4
    checkLogLine(lines.at(3), timer, "INFO", "test main",
                 "main(...)", 55,
                 "Info");

    // Check line 5
    checkLogLine(lines.at(4), timer, "WARN", "test main",
                 "main(...)", 56,
                 "Warn");

    // Check line 6
    checkLogLine(lines.at(5), timer, "ERROR", "test main",
                 "main(...)", 57,
                 "Error");

    // Check line 7
    checkLogLine(lines.at(6), timer, "FATAL", "test main",
                 "main(...)", 58,
                 "Fatal");

    // Check line 8
    checkLogLine(lines.at(7), timer, "TRACE", "test class",
                 "TestClass::testFunction(...)", 36,
                 "Enter const int TestClass::testFunction(int) const");

    // Check line 9
    std::filesystem::path expectedLogFileName(LOG_FOLDER);
    expectedLogFileName /= "TestUKLoggerFile.log";
    checkLogLine(lines.at(8), timer, "WARN", "Startup",
                 "uk::log::UKLogger::setLogfileName(...)", 133,
                 "Logfile " + expectedLogFileName.u8string() + " exists. Deleting.");

    // Test thread safety and thread ids
    std::map<std::string, int> threadIDMap;
    for (unsigned int i = 9; i < lines.size(); ++i) {
        logInfo info(lines.at(i));
        double seconds = std::difftime(timer, std::mktime(&info.mTime));
        EXPECT_GE(30, seconds);
        EXPECT_EQ("INFO", trim(info.mSeverity));
        EXPECT_EQ(std::string("test main"), trim(info.mKind));
        EXPECT_EQ(std::string("logTenTimes(...)"), trim(info.mFunction));
        EXPECT_EQ(48, info.mLine);
        if (0 == threadIDMap.count(info.mThreadID)) {
            threadIDMap.insert({info.mThreadID, 1});
        } else {
            threadIDMap[info.mThreadID]++;
        }
    }
    EXPECT_EQ(5ul, threadIDMap.size());
    for (auto iter = threadIDMap.begin(); iter != threadIDMap.end(); ++iter) {
        EXPECT_EQ(10, iter->second);
    }
}

// Execution of the code is performed in external executables. Output is checked here
// Test logging to file, fail to open the file
TEST(UKLOGGER, LogToFileFailOpen) {
    std::string output = exec("\"" PROJECT_BIN_DIR "/test/testukcpplogfilefailopen" "\"");
    UKLOG_INFO("TEST", output);
    EXPECT_FALSE(output.empty());
    // split output into lines
    std::stringstream        ss(output);
    std::string              segment;
    std::vector<std::string> lines;
    while (std::getline(ss, segment, '\n')) {
        lines.push_back(segment);  // Spit string at newline character
    }
    EXPECT_EQ(5ul, lines.size()) << "Unexpected number of log messages: " << lines.size();

    // Check line 1
    std::filesystem::path logFileName(LOG_FOLDER);
    logFileName /= "Doesnotexist";
    logFileName /= "TestUKLoggerFile.log";
    EXPECT_EQ(std::string("Logging to \"") + logFileName.u8string() +"\"", lines.at(0));

    auto    now     = std::chrono::system_clock::now();
    auto    timer   = std::chrono::system_clock::to_time_t(now);

    checkLogLine(lines.at(1), timer, "INFO", "Startup",
                 "uk::log::UKLogger::UKLogger(...)", 36,
                 std::string("Create logger Version ") + VERSION_STRING);
    checkLogLine(lines.at(2), timer, "INFO", "test main",
                 "main(...)", 42,
                 LOG_FOLDER);
    checkLogLine(lines.at(3), timer, "TRACE", "test class",
                 "TestClass::testFunction(...)", 36,
                 "Enter int TestClass::testFunction(int) const");
    checkLogLine(lines.at(4), timer, "FATAL", "Startup",
                 "uk::log::UKLogger::setLogfileName(...)", 143,
                 std::string("Could not open logfile ") + logFileName.u8string() + ". Quitting.");
}

// Execution of the code is performed in external executables. Output is checked here
// Test logging to file, cannot delete existing file
TEST(UKLOGGER, LogToFileCannotDelete) {
    std::filesystem::path logFileName(LOG_FOLDER);
    logFileName /= "TestUKLoggerFileCannotDelete.log";
    std::ofstream outfile(logFileName);  // create file
    outfile << "my text here!" << std::endl;
    UKLOG_INFO("LogToFileCannotDelete", "Permissions of Lock file");
    logPermissons(std::filesystem::status(logFileName).permissions());
    UKLOG_INFO("LogToFileCannotDelete", "Permissions of Lock file folder before changes");
    logPermissons(std::filesystem::status(LOG_FOLDER).permissions());
    std::filesystem::perms oldPerms = std::filesystem::status(LOG_FOLDER).permissions();
    std::filesystem::permissions(LOG_FOLDER,
                    std::filesystem::perms::owner_write | std::filesystem::perms::group_write |
                    std::filesystem::perms::others_write,
                    std::filesystem::perm_options::remove);
    UKLOG_INFO("LogToFileCannotDelete", "Permissions of Lock file folder after changes");
    logPermissons(std::filesystem::status(LOG_FOLDER).permissions());

    std::string output = exec("\"" PROJECT_BIN_DIR "/test/testukcpplogfilecannotdelete" "\"");
    UKLOG_INFO("TEST", output);
    EXPECT_FALSE(output.empty());
    // split output into lines
    std::stringstream        ss(output);
    std::string              segment;
    std::vector<std::string> lines;
    while (std::getline(ss, segment, '\n')) {
        lines.push_back(segment);  // Spit string at newline character
    }
    EXPECT_EQ(6ul, lines.size()) << "Unexpected number of log messages: " << lines.size();

    EXPECT_EQ(std::string("Logging to \"") + logFileName.u8string() +"\"", lines.at(0));
    auto    now     = std::chrono::system_clock::now();
    auto    timer   = std::chrono::system_clock::to_time_t(now);

    checkLogLine(lines.at(1), timer, "INFO", "Startup",
                 "uk::log::UKLogger::UKLogger(...)", 36,
                 std::string("Create logger Version ") + VERSION_STRING);
    checkLogLine(lines.at(2), timer, "INFO", "test main",
                 "main(...)", 42,
                 LOG_FOLDER);
    checkLogLine(lines.at(3), timer, "TRACE", "test class",
                 "TestClass::testFunction(...)", 36,
                 "Enter int TestClass::testFunction(int) const");
    checkLogLine(lines.at(4), timer, "WARN", "Startup",
                 "uk::log::UKLogger::setLogfileName(...)", 133,
                 std::string("Logfile ") + logFileName.u8string() + " exists. Deleting.");
    checkLogLine(lines.at(5), timer, "FATAL", "Startup",
                 "uk::log::UKLogger::setLogfileName(...)", 135,
                 std::string("Could not delete existing logfile ") + logFileName.u8string() + ". Quitting.");

    std::filesystem::permissions(LOG_FOLDER,
                    oldPerms,
                    std::filesystem::perm_options::replace);
    UKLOG_INFO("LogToFileCannotDelete", "Permissions of Lock file folder restored");
    logPermissons(std::filesystem::status(LOG_FOLDER).permissions());
    outfile.close();
    std::filesystem::remove(logFileName);
}

int main(int argc, char** argv) {
    std::filesystem::path logFileName(LOG_FOLDER);
    logFileName /= "UKLoggerLibTest.log";
    std::cout << "Logging to " << logFileName << std::endl;
    uk::log::logger().setLogfileName(logFileName);
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::UnitTest::GetInstance()->listeners().Append(new LogPrinter);
    return RUN_ALL_TESTS();
}
