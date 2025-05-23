#include <iostream>
#include <sstream>
#include "timemeasurer.h"

namespace zproksi
{
namespace profiler
{

TimeMeasurer::TimeMeasurer(const std::string_view name_, size_t amountOfExtra) {
    if (amountOfExtra > 0) {
        timePoints.reserve(amountOfExtra);
    }
    startPoint.name = name_;
    startPoint.elapsed = std::chrono::high_resolution_clock::now();
}

TimeMeasurer::~TimeMeasurer() {
    const TIME_POINT_TYPE now = std::chrono::high_resolution_clock::now();
    for (DataVector::const_reverse_iterator rit = timePoints.crbegin(); rit != timePoints.crend(); ++rit)
    {
        std::cout << rit->name << ": " << FormatNanoseconds(
            std::chrono::duration_cast<std::chrono::nanoseconds>(now - rit->elapsed).count()
            ) << " ns.\n";
    }
    std::cout << startPoint.name << ": " << FormatNanoseconds(
        std::chrono::duration_cast<std::chrono::nanoseconds>(now - ExecutionTimePoint()).count()
        ) << " ns.\n";
}

const long long TimeMeasurer::NanosecondsElapsed(const TIME_POINT_TYPE at) const {
    return std::chrono::duration_cast<std::chrono::nanoseconds>(at - ExecutionTimePoint()).count();
}

void TimeMeasurer::RegisterTimePoint(const std::string_view name) {
    timePoints.emplace_back(TimePoint{name, std::chrono::high_resolution_clock::now()});
}

TimeMeasurer::TIME_POINT_TYPE TimeMeasurer::ExecutionTimePoint() const {
    return startPoint.elapsed;
}

std::string TimeMeasurer::FormatNanoseconds(const long long nanoseconds) {
    std::string s;
    s.reserve(32); // max length of long long in string representation is 21
    s = std::to_string(nanoseconds);
    const size_t len = s.length();
    for (size_t i = 0; len > 3 && i < (len - 1) / 3; ++i)
    {
        s.insert(len - ((i + 1) * 3), 1, ','); // separate number of nanoseconds with ',' at every 3-d position
    }
    return s;
}

};// namespace profiler
};// namespace zproksi
