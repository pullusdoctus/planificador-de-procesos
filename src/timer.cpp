#include <timer.h>

Timer::Timer(double newIORequisite) : IORequisite(newIORequisite) {}

void Timer::start() {
	IOStartTime = std::chrono::steady_clock::now();
}

bool Timer::checkTime() {
	std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
	double elapsed = std::chrono::duration<double>(now - IOStartTime).count();
	return IORequisite - elapsed <= 0 ? true : false;
}

std::chrono::steady_clock::time_point Timer::getStartTime() {
	return IOStartTime; 
}