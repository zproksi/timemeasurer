#pragma once
#include <chrono>
#include <ostream>
#include <string>
#include <string_view>
#include <vector>

namespace zproksi
{
namespace profiler
{
/// @brief default separator for formatting output
constexpr char default_separator = ',';

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
    /// @brief Constructs TimeMeasurer
    ///    @note TimeMeasurer do not possess name of the measurement - remember about life time
    /// @param name - name of the time to measure
    /// @param an_out - output stream
    /// @param amountOFExtra - how many intermediate measurements supposed to be added - allocated before time serif
    /// @param aseparator - character. Separator which will be used in output
    TimeMeasurer(const std::string_view name, std::ostream& an_out,
                 size_t amountOfExtra = 0,
                 const char aseparator = default_separator);

    /// @brief Constructs TimeMeasurer
    ///    @note TimeMeasurer do not possess name of the measurement - remember about life time
    ///    @note default output stream is std::cout
    /// @param name - name of the time to measure
    /// @param amountOFExtra - how many intermediate measurements supposed to be added - allocated before time serif
    /// @param aseparator - character. Separator which will be used in output
    TimeMeasurer(const std::string_view name,
                 size_t amountOfExtra = 0,
                 const char aseparator = default_separator);

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
    ///   default separator can be passed as parameter
    /// @note no conversion to seconds, minutes and etc.
    static std::string FormatNanoseconds(const long long nanoseconds,
                                         const char separator = default_separator);

protected:
    std::ostream& out_;      /// holds reference to the output stream
    DataVector timePoints_;  /// holds extra points for this measurement - can be empty
    TimePoint startPoint_;   /// holds start point for this class
    const char separator_;   /// holds separator for time formatting
};

};// namespace profiler
};// namespace zproksi

