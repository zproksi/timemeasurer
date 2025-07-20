# [TimeMeasurer] Purpose

C++ class [TimeMeasurer] for profiling of the code.
Timing of an execution will be provided via std::cout (choosen std::ostream) in nanoseconds.
The std::ostream, number of time serifs and the separator for printing can be customized

*code conforms [cppLib] demands* (Comments; Unit Tests; Samples)

# Table of Contents
  * [Main include file](#main-include-file)
  * [Samples of usage](#samples-of-usage)
  * [Unit Tests Execution](#unit-tests-execution)
  * [How to include into a project](#how-to-include-into-a-project)

## Main include file
[timemeasurer.h]

## Samples of usage

|**See [samples.md]**|
|--|

## Unit Tests Execution

* Open CMakeLists.txt in the tests/unit folder as separate project
* build (see *NOTE:* below)
* execute just built `./testtimemeasurer`

***NOTE:*** it is possible to build unit tests locally
  - [rebuild.cmd] for Windows
  - [rebuild.sh] for Linux and Mac OS

## How to include into a project

1. Include in CMakeLists.txt
```cmake
##get timemeasurer
include(FetchContent)
FetchContent_Declare(timemeasurer
        GIT_REPOSITORY https://github.com/zproksi/timemeasurer
        GIT_TAG stableVersion)
FetchContent_MakeAvailable(timemeasurer)
if(NOT timemeasurer_POPULATED)
    FetchContent_Populate(timemeasurer)
    add_subdirectory(${timemeasurer_SOURCE_DIR} ${timemeasurer_BUILD_DIR})
endif()
include_directories(${timemeasurer_SOURCE_DIR})

```
2. Add in CMakeLists.txt

    1. for your executable `timemeasurer`
    ```cmake
    # Link timemeasurer to the executable project
    target_link_libraries(${PROJECT_NAME} timemeasurer)
    ```

    2. for your library `timemeasurer`

    ```cmake
    # Link timemeasurer to MyLibrary
    target_link_libraries(MyLibrary PUBLIC timemeasurer)
    ```
3. In the C++ code - see [Samples of usage][samples.md]


[Home](#timemeasurer-purpose)

[cppLib]:https://github.com/zproksi/cppLibStruct

[TimeMeasurer]:./timemeasurer.h

[samples.md]:./samples.md
[timemeasurer.h]:./timemeasurer.h
[timemeasurer.cpp]:./timemeasurer.cpp
[rebuild.cmd]:./tests/unit/rebuild.cmd
[rebuild.sh]:./tests/unit/rebuild.sh
