#include <iostream>
#include <sstream>
#include "timemeasurer.h"

namespace zproksi
{
namespace profiler
{

TimeMeasurer::TimeMeasurer(const std::string_view name_, std::ostream& an_out,
                           size_t amountOfExtra,
                           const char aseparator)
    : out_(an_out)
    , separator_(aseparator) {
    if (amountOfExtra > 0) {
        timePoints_.reserve(amountOfExtra);
    }
    startPoint_.name = name_;
    startPoint_.elapsed = std::chrono::high_resolution_clock::now();
}



TimeMeasurer::TimeMeasurer(const std::string_view name, size_t amountOfExtra,
                           const char aseparator)
    : TimeMeasurer(name, std::cout, amountOfExtra, aseparator){
}

TimeMeasurer::~TimeMeasurer() {
    const TIME_POINT_TYPE now = std::chrono::high_resolution_clock::now();
    for (DataVector::const_reverse_iterator rit = timePoints_.crbegin(); rit != timePoints_.crend(); ++rit) {
        out_ << rit->name << ": " << FormatNanoseconds(
            std::chrono::duration_cast<std::chrono::nanoseconds>(now - rit->elapsed).count(), separator_
            ) << " ns." << std::endl;
    }
    out_ << startPoint_.name << ": " << FormatNanoseconds(
        std::chrono::duration_cast<std::chrono::nanoseconds>(now - ExecutionTimePoint()).count(), separator_
        ) << " ns." << std::endl;
}

const long long TimeMeasurer::NanosecondsElapsed(const TIME_POINT_TYPE at) const {
    return std::chrono::duration_cast<std::chrono::nanoseconds>(at - ExecutionTimePoint()).count();
}

void TimeMeasurer::RegisterTimePoint(const std::string_view name) {
    timePoints_.emplace_back(TimePoint{name, std::chrono::high_resolution_clock::now()});
}

TimeMeasurer::TIME_POINT_TYPE TimeMeasurer::ExecutionTimePoint() const {
    return startPoint_.elapsed;
}

std::string TimeMeasurer::FormatNanoseconds(const long long nanoseconds, const char separator) {
    std::string s = std::to_string(nanoseconds);
    if (s.size() < 4) {
        return s; // Early return when formating is not needed
    }
    size_t readPos = s.size() - 1; // source of digits
    s.resize(s.size() + (s.size() - 1) / 3); // Resize string to have free space at the end
    size_t writePos = s.size() - 1; // destination for digits
    // Shift elements to right placing a separator after every 3 digits
    for (size_t digitCounter = 0; readPos > 0 && writePos > 0;) {
        s[writePos--] = s[readPos--];
        if (0 == ((++digitCounter) % 3)) {
            s[writePos--] = separator;
        }
    }

    return s;
}

};// namespace profiler
};// namespace zproksi
