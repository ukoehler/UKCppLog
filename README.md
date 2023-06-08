# Logging for Modern C++

<!-- PROJECT SHIELDS -->
<table align="center" style="“border: none;”">
    <!-- Linux build and test -->
    <tr>
        <td>
            <img src="https://upload.wikimedia.org/wikipedia/commons/9/9e/UbuntuCoF.svg" width="20" height="20">
            <img src="https://upload.wikimedia.org/wikipedia/commons/a/af/GNU_Compiler_Collection_logo.svg" width="20" height="20">
        </td>
        <td>
            Linux / GCC
        </td>
        <td>
            <img src="https://img.shields.io/endpoint?url=https://gist.githubusercontent.com/ukoehler/6935a607aff6233d996070495bba70b7/raw/ubuntu-latest-build.json" />
        </td>
        <td>
            <img src="https://img.shields.io/endpoint?url=https://gist.githubusercontent.com/ukoehler/6935a607aff6233d996070495bba70b7/raw/ubuntu-latest-build-warnings.json" />
        </td>   
        <td>
            <img src="https://img.shields.io/endpoint?url=https://gist.githubusercontent.com/ukoehler/6935a607aff6233d996070495bba70b7/raw/ubuntu-latest-test.json" />
        </td>    
    </tr>
    <!-- MacOS build and test -->
    <tr>
        <td>
            <img src="https://upload.wikimedia.org/wikipedia/commons/0/00/MacOS_wordmark.svg" width="20" height="20">
            <img src="https://llvm.org/img/DragonSmall.png" width="20" height="20">
        </td>
        <td>
            MacOS / Clang
        </td>
        <td>
            <img src="https://img.shields.io/endpoint?url=https://gist.githubusercontent.com/ukoehler/6935a607aff6233d996070495bba70b7/raw/macos-latest-build.json" />
        </td>
        <td>
            <img src="https://img.shields.io/endpoint?url=https://gist.githubusercontent.com/ukoehler/6935a607aff6233d996070495bba70b7/raw/macos-latest-build-warnings.json" />
        </td>   
        <td>
            <img src="https://img.shields.io/endpoint?url=https://gist.githubusercontent.com/ukoehler/6935a607aff6233d996070495bba70b7/raw/macos-latest-test.json" />
        </td>    
    </tr>
    <!-- Windows build and test -->
    <tr>
        <td>
            <img src="https://simpleicons.org/icons/windows.svg" width="20" height="20">
            <img src="https://upload.wikimedia.org/wikipedia/commons/5/59/Visual_Studio_Icon_2019.svg" width="20" height="20">
        </td>
        <td>
            Windows / VisualStudio
        </td>
        <td>
            <img src="https://img.shields.io/endpoint?url=https://gist.githubusercontent.com/ukoehler/6935a607aff6233d996070495bba70b7/raw/windows-latest-build.json" />
        </td>
        <td>
            <img src="https://img.shields.io/endpoint?url=https://gist.githubusercontent.com/ukoehler/6935a607aff6233d996070495bba70b7/raw/windows-latest-build-warnings.json" />
        </td>   
        <td>
            <img src="https://img.shields.io/endpoint?url=https://gist.githubusercontent.com/ukoehler/6935a607aff6233d996070495bba70b7/raw/windows-latest-test.json" />
        </td>    
    </tr>
    <tr>
        <td></td>
         <td>
            Static code checks
        </td>       
        <td>
            <img src="https://img.shields.io/endpoint?url=https://gist.githubusercontent.com/ukoehler/6935a607aff6233d996070495bba70b7/raw/ubuntu-latest-clang-tidy-warnings.json" />
        </td>    
        <td>
            <img src="https://img.shields.io/endpoint?url=https://gist.githubusercontent.com/ukoehler/6935a607aff6233d996070495bba70b7/raw/ubuntu-latest-cppcheck-warnings.json" />
        </td>
        <td>
            <img src="https://img.shields.io/endpoint?url=https://gist.githubusercontent.com/ukoehler/6935a607aff6233d996070495bba70b7/raw/ubuntu-latest-cpplint-warnings.json" />
         </td>
     </tr>
     <tr>
        <td></td>
         <td>
            Valgrind
        </td>       
        <td>
            <img src="https://img.shields.io/endpoint?url=https://gist.githubusercontent.com/ukoehler/6935a607aff6233d996070495bba70b7/raw/ubuntu-latest-valgrind-warnings.json" />
        </td>    
        <td>
            <img src="https://img.shields.io/endpoint?url=https://gist.githubusercontent.com/ukoehler/6935a607aff6233d996070495bba70b7/raw/ubuntu-latest-helgrind-warnings.json" />
        </td>
        <td>
            <img src="https://img.shields.io/endpoint?url=https://gist.githubusercontent.com/ukoehler/6935a607aff6233d996070495bba70b7/raw/ubuntu-latest-drd-warnings.json" />
         </td>
     </tr>
     <tr>
        <td></td>
         <td>
            GCC Sanitizers
        </td>       
        <td>
            <img src="https://img.shields.io/endpoint?url=https://gist.githubusercontent.com/ukoehler/6935a607aff6233d996070495bba70b7/raw/ubuntu-latest-address.json" />
        </td>    
        <td>
            <img src="https://img.shields.io/endpoint?url=https://gist.githubusercontent.com/ukoehler/6935a607aff6233d996070495bba70b7/raw/ubuntu-latest-thread.json" />
        </td>
        <td>
         </td>
     </tr>
    <tr>
        <td></td>
         <td>
            Coverage / Documentation
        </td>       
        <td>
            <img src="https://img.shields.io/endpoint?url=https://gist.githubusercontent.com/ukoehler/6935a607aff6233d996070495bba70b7/raw/ubuntu-latest-coverage.json" />
        </td>    
        <td>
            <img src="https://img.shields.io/endpoint?url=https://gist.githubusercontent.com/ukoehler/6935a607aff6233d996070495bba70b7/raw/ubuntu-latest-doxygen-warnings.json" />
        </td>
        <td>
         </td>
     </tr>
    <!-- Doxygen Coverage -->
</table>

[Doxygen documentation](https://ukoehler.github.io/UKCppLog/index.html) 

## Table of contents
- [Logging for Modern C++](#logging-for-modern-c)
  - [Table of contents](#table-of-contents)
  - [Motivation](#motivation)
  - [Lessons](#lessons)
  - [Usage](#usage)
    - [Introduction](#introduction)
    - [Installation and Usage](#installation-and-usage)
      - [Cmake](#cmake)
      - [Compile and Install](#compile-and-install)
    - [Usage](#usage-1)
    - [Sample Output](#sample-output)
## Motivation
I needed a logging library for C++ that allowed me to log the information I want and also doesn't leave the user looking for the log-files. At the same time this small projects seemed like the ideal candidate to learn about GitHub, GitHub Actions and Badges. I have always believed in using all available tools to write the safest code possible. Obviously the badges should reflect all those efforts.

As can be seen by the badges above I use three compilers on three platforms with all compilers warnings enabled (and I mean all, even for GCC), unit testing with code coverage, static code checkers, sanitizers (Vagrind and GCC) and use Doxygen to generate the very important code documentation.
## Lessons

Well, is all the testing and checking effort overkill for a small project like this? As it turns out, not at all. The tools did point problems out to me that I wasn't aware of (and found quite a number of rants about the quality of C/C++ for). So, it was well worth the effort. The workflows can easily be used for more complex projects, as well.

Most problems were pointed out by at least one compiler. The static code checkers also pointed them out, as well as Valgrind and the GCC sanitizers. Studies online do show, however, that none of those tools are enough on their own.
## Usage

### Introduction

This class allows for easy logging from C++ (via macros).
Logging is available before entering
the main() function into a std::string buffer and can be send to a
log file or will be dumped after process execution to the command-line.

### Installation and Usage

While this library can be installed (see \ref compile), it is meant to be 
included in the CMakeLists.txt as an external reference.

#### Cmake

Add the following statements to your CMakeLists.txt:
```
FetchContent_Declare(
    ukcpplog
    GIT_REPOSITORY https://github.com/ukoehler/UKCppLog.git
    GIT_TAG        c2434e32d6cee677038c46c3cc965c4a9029515a # tag 0.7
)
FetchContent_MakeAvailable(ukcpplog)
```
(choose the latest tag) and then add the library to your targets. For example:
```
target_link_libraries(target ... PRIVATE ukcpplog ...)
```
#### Compile and Install

All instructions are given for linux.

Clone the repository from GitHub.com
```
git clone https://github.com/ukoehler/UKCppLog.git
cd UKCppLog
```
When configuring the project, the following definitions are important:
 - -DCMAKE_BUILD_TYPE=Debug or Release
 - -DBUILD_DOCUMENTATION=ON or OFF: Build this documentation or not
 - -DCODE_CHECKS=ON or OFF: Enable static code checkers or not
 - -DCODE_COVERAGE=ON or OFF: Enable or disable code coverage
 - -DCMAKE_INSTALL_PREFIX=/usr/local: Set installation folder (optional)

Configure for a simple debug build with documentation would be 
```
mkdir build
cd build 
cmake -DCMAKE_BUILD_TYPE=Debug -DBUILD_DOCUMENTATION=ON -DCODE_CHECKS=OFF -DCODE_COVERAGE=OFF ..
```

Build and run tests with the following commands
```
make or cmake --build .
cd test
./UKLoggerLibTest or ctest -C debug
```
The documentation can be found in \code{.txt} build/doc \endcode

To install use 
```
make install or cmake --install . --prefix /my/install/prefix
```

When build with coverage support use the following to create a
report (requires lcov):
```
cmake --build . --target ccov-UKLoggerLibTest
```
The report will be in ```ccov/UKLoggerLibTest/```

### Usage
```
#include "UKLogger.hpp"

int main() {
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
```
Logging is thread save. The following macros can be used for logging:
```
 UKLOG_ENTER(kind)
```
used to trace entering a function. Kind can be anything like class name.

```
UKLOG_LEAVE(kind)
```
used to trace exiting a function without return value. 
Kind can be anything like class name.

```
UKLOG_LEAVE(kind, retval)
```
used to trace exiting a function with return value. 
Kind can be anything like class name. Retval is the returned value.

```
UKLOG_DEBUG(kind, message)
```
used to trace entering a function. Kind can be anything like class name.
Message is any string to log. Replace DEBUG with one of the levels:
DEBUG, INFO, WARN, ERROR, FATAL with increasing severity.

### Sample Output
```
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
```
Given is the date and time, the severity, thread id, kind, function name, line number and the message.


