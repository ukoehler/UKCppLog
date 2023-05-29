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
## Motivation
I needed a logging library for C++ that allowed me to log the information I want and also doesn't leave the user looking for the log-files. At the same time this small projects seemed like the ideal candidate to learn about GitHub, GitHub Actions and Badges. I have always believed in using all available tools to write the safest code possible. Obviously the badges should reflect all those efforts.

As can be seen by the badges above I use three compilers on three platforms with all compilers warnings enabled (and I mean all, even for GCC), unit testing with code coverage, static code checkers, sanitizers (Vagrind and GCC) and use Doxygen to generate the very important code documentation.
## Lessons
Well, is all the testing and checking effort overkill for a small project like this? As it turns out, not at all. The tools did point problems out to me that I wasn't aware of (and found quite a number of rants about the quality of C/C++ for). So, it was well worth the effort. The workflows can easily be used for more complex projects, as well.

Most problems were pointed out by at least one compiler. The static code checkers also pointed them out, as well as Valgrind and the GCC sanitizers. Studies online do show, however, that none of those tools are enough on their own.
## Usage

