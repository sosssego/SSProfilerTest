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
	p.printTimeSummary();
	EXPECT_LT(1.0, p.total_time[1].count());
	EXPECT_LT(p.total_time[1].count(), 1.001);
	
}

void innerFunction1() {
	this_thread::sleep_for(10ms);
}

void innerFunction2() {
	this_thread::sleep_for(25ms);
}

TEST(ProfileNested, Total1000ms) {
	
	frd::secondsDouble total;

	frd::Profiler p;
	p.addNewTimer("Outer");
	p.startTimer(1);
	p.addNewTimer("Midle");
	p.addNewTimer("Inner1");
	p.addNewTimer("Inner2");
	for (int i = 0; i < 5; ++i) {
		p.startTimer(2); // Midle
		for (int j = 0; j < 10; ++j) {			
			p.startTimer(3); //Inner1
			auto start = std::chrono::steady_clock::now();
			innerFunction1();
			p.addPartialTime(3); //Inner1
			total += std::chrono::steady_clock::now() - start;
			p.startTimer(4); //Inner2
			innerFunction2();
			p.addPartialTime(4); //Inner2
			this_thread::sleep_for(5ms);
		}
		p.addPartialTime(2); // Middle
		this_thread::sleep_for(50ms);
	}
	this_thread::sleep_for(100ms);
	p.addPartialTime(1); // Outer
	p.printTimeSummary();

	cout << total.count();

	EXPECT_LT(2.35, p.total_time[0].count());
	EXPECT_LT(p.total_time[0].count(), 2.35 + 0.1);		// up to 1ms error per sleep

	EXPECT_LT(2.25, p.total_time[1].count());
	EXPECT_LT(p.total_time[1].count(), 2.35 + 0.1);		// up to 1ms error per sleep

	EXPECT_LT(2.0, p.total_time[2].count());
	EXPECT_LT(p.total_time[2].count(), 2.0 + 0.1);		// up to 1ms error per sleep

	EXPECT_LT(0.5, p.total_time[3].count());
	EXPECT_LT(p.total_time[3].count(), 0.5 + 0.1);		// up to 1ms error per iteration

	EXPECT_LT(1.25, p.total_time[4].count());
	EXPECT_LT(p.total_time[4].count(), 1.25 + (0.001 * 50));		// up to 1ms error per iteration
}