#include "gtest/gtest.h"
#include <frdProfile.hpp>
#include <chrono>
#include <thread>

using namespace std;
using namespace chrono;
using namespace chrono_literals;

void wait(milliseconds t) {
    auto now = steady_clock::now();
    while ((steady_clock::now() - now) < t)
        ;
}

TEST(ProfileTime, wait) {
    frd::Profiler p;
    constexpr auto t = 200ms;
    constexpr auto sleep{"Sleep(t) Time"};

    p.addNewTimer(sleep);
    p.startTimer(sleep);

    wait(200ms);

    p.addPartialTime(sleep);
    p.printTimeSummary();

    EXPECT_TRUE(t < p.getTotalTime(sleep));
    EXPECT_TRUE((t + 1ms) > p.getTotalTime(sleep));
}

TEST(ProfileNested, function_nest) {
    frd::Profiler p;
    p.addNewTimer("Outer");
    p.startTimer("Outer");
    p.addNewTimer("Midle");
    p.addNewTimer("Inner1");
    p.addNewTimer("Inner2");
    for (int i = 0; i < 5; ++i) {
        p.startTimer("Midle");
        for (int j = 0; j < 3; ++j) {
            p.startTimer("Inner1");
            wait(10ms);
            p.addPartialTime("Inner1");

            p.startTimer("Inner2");  // Inner2
            wait(25ms);
            p.addPartialTime("Inner2");  // Inner2
        }
        p.addPartialTime("Midle");  // Middle
    }
    wait(100ms);
    p.addPartialTime("Outer");  // Outer

    wait(50ms);

    p.printTimeSummary();

    auto inner1 = 5 * 3 * (10ms);
    EXPECT_TRUE(inner1 < p.getTotalTime("Inner1"));
    EXPECT_TRUE((inner1 + 1ms) > p.getTotalTime("Inner1"));

    auto inner2 = 5 * 3 * (25ms);
    EXPECT_TRUE(inner2 < p.getTotalTime("Inner2"));
    EXPECT_TRUE((inner2 + 1ms) > p.getTotalTime("Inner2"));

    auto middle = inner1 + inner2;
    EXPECT_TRUE(middle < p.getTotalTime("Midle"));
    EXPECT_TRUE((middle + 1ms) > p.getTotalTime("Midle"));

    auto outer = middle + 100ms;
    EXPECT_TRUE(outer < p.getTotalTime("Outer"));
    EXPECT_TRUE((outer + 1ms) > p.getTotalTime("Outer"));

    auto total = outer + 50ms;
    EXPECT_TRUE(total < p.getTotalTime("_Total"));
    EXPECT_TRUE((total + 1ms) > p.getTotalTime("_Total"));
}