# timemeasurer Purpose

C++ class [TimeMeasurer] for profiling of the code.
Timing of an execution will be provided via std::cout (choosen std::ostream) in nanoseconds.
The std::ostream, number of time serifs and the separator for printing can be customized.

*code conforms [cppLib] demands* (Comments; Unit Tests; Samples)

# Table of Contents

  * [Samples of usage](#samples-of-usage)
    * [Single measurement std::cout](#single-measurement-stdcout)
    * [Single measurement std::ostream](#single-measurement-stdostream)
    * [Some measurements with help of one instance of TimeMeasurer class](#some-measurements-with-help-of-one-instance-of-timemeasurer-class)
    * [Set custom output seprator](#set-custom-output-seprator)
    * [Full customization sample](#full-customization-sample)
  * [Unit Tests Execution](#unit-tests-execution)
  * [How to include into a project](#how-to-include-into-a-project)

## Samples of usage

### Single measurement std::cout
```cpp
#include "timemeasurer.h"

{ // scope where measurement need to done

    using namespace zproksi::profiler;
    TimeMeasurer time_measure("test measurement");

    //
    // a code whose execution time needs to be measured
    //
} // destructor of the time_measure
```
```cpp
// possible otput in the std::cout
test measurement: 2,425,211 ns.
```

### Single measurement std::ostream
```cpp
#include "timemeasurer.h"

{ // scope where measurement need to done

    std::ostringstream sstream;
    {
        using namespace zproksi::profiler;
        TimeMeasurer time_measure("test measurement", sstream);

        //
        // a code whose execution time needs to be measured
        //
    } // destructor of the time_measure
}
```
```cpp
// possible data in the sstream variable after call of the time_measure destructor
test measurement: 1,125,144 ns.
```

### Some measurements with help of one instance of [TimeMeasurer] class
```cpp
#include "timemeasurer.h"

{ // scope where measurement need to be done
    using namespace zproksi::profiler;
    /// supposed: 1 main and 3 extra points of measurements
    TimeMeasurer time_measure("Full Measurement", 3);
    // a code point + all below
    //

    time_measure.RegisterTimePoint("Some logic passed");
    // a code point 1 and all below
    //

    time_measure.RegisterTimePoint("Shorter Time");
    // a code point 2 and all below
    //

    time_measure.RegisterTimePoint("Fastest Time");
    // a code point 3 and all below
    //
} // destructor of the time_measure
```
```cpp
// possible otput in the std::cout
Fastest Time: 1,425,211 ns.
Shorter Time: 4,140,549 ns.
Some logic passed: 7,245,905 ns.
Full Measurement: 9,382,101 ns.
```

### Set custom output seprator
```cpp
#include "timemeasurer.h"

{ // scope where measurement need to done

    using namespace zproksi::profiler;
    TimeMeasurer time_measure("test measurement", 0, ':');

    //
    // a code whose execution time needs to be measured
    //
}
```
```cpp
// possible otput in the std::cout
test measurement: 3:195:680 ns.
```

### Full customization sample
```cpp
#include "timemeasurer.h"

{ // scope where measurement need to done
    std::ostringstream sstream;
    {
        using namespace zproksi::profiler;
        TimeMeasurer time_measure("Full measurement", sstream, 1, '#');

        //
        // a code whose execution time needs to be measured
        //
        time_measure.RegisterTimePoint("Second serif");
        //
        // more code whose execution time needs to be measured
        //
    } // destructor of the time_measure

}
```
```cpp
// possible otput in the sstream variable
Second serif: 2#559#014 ns.
Full measurement: 5#197#361 ns.
```

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
3. In the C++ code - see [Samples of usage](#samples-of-usage)


[Home](#timemeasurer-purpose)

[cppLib]:https://github.com/zproksi/cppLibStruct

[TimeMeasurer]:./timemeasurer.h

[timemeasurer.h]:./timemeasurer.h
[timemeasurer.cpp]:./timemeasurer.cpp
[rebuild.cmd]:./tests/unit/rebuild.cmd
[rebuild.sh]:./tests/unit/rebuild.sh
