#include "gtest/gtest.h"
#include <frdProfile.hpp>
#include <chrono>
#include <thread>



using namespace std;

TEST(ProfileTime, sleep1000) {

	frd::Profiler p;
	p.addNewTimer("Sleep(1000) Time");
	p.startTimer(1);
	this_thread::sleep_for(chrono::milliseconds(1000));
	p.addPartialTime(1);
	EXPECT_LT(1000000000, p.total_time[1]);
	EXPECT_LT(p.total_time[1], 1001000000);
}