#pragma once
#include <chrono>
#include <vector>
#include <string>
#include <string_view>

namespace zproksi
{
namespace profiler
{

/// @brief TimeMeasurer allows to measure time between creation and destruction of the class
///          supports interim events registration
class TimeMeasurer final
{
private:
    /// @brief  no copy, no move
    TimeMeasurer(const TimeMeasurer&) = delete;
    TimeMeasurer(TimeMeasurer&&) = delete;
    TimeMeasurer& operator =(const TimeMeasurer&) = delete;
    TimeMeasurer&& operator =(TimeMeasurer&&) = delete;
public:
    typedef decltype(std::chrono::high_resolution_clock::now()) TIME_POINT_TYPE;

protected:
    struct TimePoint
    {
        std::string_view name;
        TIME_POINT_TYPE elapsed;
    };
    using DataVector = std::vector<TimePoint>;

public:
    /// @brief Full name of meaure should be passed in constructor
    ///    note: TimeMeasurer do not possess name of the measure - life time of the name should be longer
    /// @param name - name of the time to measure
    /// @param amountOFExtra - how many intermediate measurements supposed to be added - allocated before time serif
    TimeMeasurer(const std::string_view name, size_t amountOfExtra = 0);

    /// @brief all time points from later added to earlier added will be printed into std::cout
    ///   with corresponding names. (Main name provided in constructor will be printed last)
    ~TimeMeasurer();

    /// @brief returns amount of time (nanoseconds) from the class creation till the time point 'at'
    const long long NanosecondsElapsed(const TIME_POINT_TYPE at) const;

    /// @brief save time point in the class
    void RegisterTimePoint(const std::string_view name);

    /// @brief returns initial moment of measurements
    TIME_POINT_TYPE ExecutionTimePoint() const;

    /// @brief returns amount of time (nanoseconds) in format "...,XXX,XXX,XXX,XXX"
    /// @note no conversion to seconds, minutes and etc.
    static std::string FormatNanoseconds(const long long nanoseconds);

protected:
    DataVector timePoints; /// holds extra points for this measurement - can be empty
    TimePoint startPoint; /// holds start point for this class
};

};// namespace profiler
};// namespace zproksi

