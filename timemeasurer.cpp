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
    timePoints.emplace_back(name, std::chrono::high_resolution_clock::now());
}

TimeMeasurer::TIME_POINT_TYPE TimeMeasurer::ExecutionTimePoint() const {
    return startPoint.elapsed;
}

std::string TimeMeasurer::FormatNanoseconds(const long long nanoseconds, char sep) {
    std::string s = std::to_string(nanoseconds);

    if (s.size() <= 3) {
        // Early return when formating is not needed
        return s;
    }

    int readPos = s.size() - 1;

    // Resize string to have free space at the end
    s.resize(s.size() + (s.size() - 1) / 3);
    int writePos = s.size() - 1;

    // Shift elements to right placing a separator between 3 digits
    for (int digitCounter = 0; readPos >= 0 && writePos >= 0;) {
        if (digitCounter == 3) {
            s[writePos--] = sep;
            digitCounter = 0;
        }

        s[writePos--] = s[readPos--];
        ++digitCounter;
    }

    return s;
}

};// namespace profiler
};// namespace zproksi
