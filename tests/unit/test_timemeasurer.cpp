#include <gtest/gtest.h>
#include <chrono>
#include <sstream>
#include <iostream>

#include "timemeasurer.h"

/// @brief RAII for cout replace with ostringsstream
class CoutReplacer final {
    /// @brief  no copy, no move
    CoutReplacer(const CoutReplacer&) = delete;
    CoutReplacer(CoutReplacer&&) = delete;
    CoutReplacer& operator =(const CoutReplacer&) = delete;
    CoutReplacer&& operator =(CoutReplacer&&) = delete;

public:
    CoutReplacer(std::ostringstream& toreplaceWith)
    : originalBuffer_(std::cout.rdbuf()) {
        std::cout.rdbuf(toreplaceWith.rdbuf());
    }

    ~CoutReplacer() {
        std::cout.rdbuf(originalBuffer_);
    }
protected:
    std::streambuf* originalBuffer_;
};


/// <summary>
///  test formatting: // "title: X,XXX,XXX ns."
///  title & ns.
///  every 4th position from the end should be ','
///  digits on its positions
/// </summary>
TEST(TimeMeasurer, CheckFormatting) {
    // Create a stringstream
    std::ostringstream sstream;

    std::string_view title = "measure 2 milliseconds";
    {
        // Save the original buffer of std::cout
        CoutReplacer  replacer(sstream);

        using namespace zproksi::profiler;
        TimeMeasurer mt(title);
        std::this_thread::sleep_for(std::chrono::milliseconds(2));
    }

    std::string resultData = sstream.str();
    const size_t sz = resultData.length();
    EXPECT_GT(sz, 13); // "title: X,XXX,XXX ns."

    size_t posTitle = resultData.find(title); // part title
    EXPECT_NE(posTitle, std::string::npos);

    size_t posNS = resultData.find(" ns."); // part " ns."
    EXPECT_NE(posNS, std::string::npos);
    EXPECT_GT(posNS, posTitle);

    // part "X,XXX,XXX"
    if (sz > 12) {
        EXPECT_TRUE(resultData.at(posNS - 4) == ',');
        EXPECT_TRUE(resultData.at(posNS - 8) == ',');
        for (size_t k : {1,2,3,5,6,7,9})
        {
            const char c = resultData.at(posNS - k);
            EXPECT_TRUE(c >= '0' && c <= '9');
        }
    }

}

/// <summary>
///  test output fo serie of measurement
///  Full measure should be last, smalest in the scope must be first
/// </summary>
TEST(TimeMeasurer, CheckSequential) {
    // Create a stringstream
    std::ostringstream sstream;

    std::string_view title1 = "full measure";
    std::string_view title2 = "interim measure";
    std::string_view title3 = "fastest one";
    {
        // Save the original buffer of std::cout
        CoutReplacer  replacer(sstream);

        using namespace zproksi::profiler;
        TimeMeasurer mt(title1, 2); // 2 extra titles supposed
        std::this_thread::sleep_for(std::chrono::milliseconds(2));
        mt.RegisterTimePoint(title2);
        std::this_thread::sleep_for(std::chrono::milliseconds(3));
        mt.RegisterTimePoint(title3);
        std::this_thread::sleep_for(std::chrono::milliseconds(4));
    }

    std::string resultData = sstream.str();

    size_t posTitle3 = resultData.find(title3); // title should be output first   smallest time (end time - moment)
    size_t posTitle2 = resultData.find(title2); // title should be output second  time in the middle
    size_t posTitle1 = resultData.find(title1); // title should be output last    biggest time  (full time to measure)

    EXPECT_GT(posTitle2, posTitle3);
    EXPECT_GT(posTitle1, posTitle3);
    EXPECT_GT(posTitle1, posTitle2);
}

