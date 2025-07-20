# Samples of usage [TimeMeasurer]
  * [Single measurement std::cout](#single-measurement-stdcout)
  * [Single measurement std::ostream](#single-measurement-stdostream)
  * [Some measurements with help of one instance of TimeMeasurer class](#some-measurements-with-help-of-one-instance-of-timemeasurer-class)
  * [Set custom output seprator](#set-custom-output-seprator)
  * [Full customization sample](#full-customization-sample)

 [to README][README.md]

## Single measurement std::cout
```cpp
#include "timemeasurer.h"

{ // scope where the measurement needs to be done

    using namespace zproksi::profiler;
    TimeMeasurer time_measure("test measurement");

    //
    // a code whose execution time needs to be measured
    //
} // destructor of the time_measure
```
```cpp
// The potential otput in the std::cout
test measurement: 2,425,211 ns.
```

## Single measurement std::ostream
```cpp
#include "timemeasurer.h"

{ // scope where the measurement needs to be done

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
// The potential data in the sstream variable after the call of the time_measure destructor
test measurement: 1,125,144 ns.
```

## Some measurements with help of one instance of [TimeMeasurer] class
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
// The potential otput in the std::cout
Fastest Time: 1,425,211 ns.
Shorter Time: 4,140,549 ns.
Some logic passed: 7,245,905 ns.
Full Measurement: 9,382,101 ns.
```

## Set custom output seprator
```cpp
#include "timemeasurer.h"

{ // scope where the measurement needs to be done

    using namespace zproksi::profiler;
    TimeMeasurer time_measure("test measurement", 0, ':');

    //
    // a code whose execution time needs to be measured
    //
}
```
```cpp
// The potential otput in the std::cout
test measurement: 3:195:680 ns.
```

## Full customization sample
```cpp
#include "timemeasurer.h"

{ // scope where the measurement needs to be done
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
// The potential data in the sstream variable after the call to the time_measure destructor
Second serif: 59#014 ns.
Full measurement: 1#897#361 ns.
```

[to README][README.md]

[Home](#samples-of-usage-timemeasurer)


[TimeMeasurer]:./timemeasurer.h
[samples.md]:./samples.md
[README.md]:./README.md