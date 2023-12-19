#include "TimerUtils.h"

float currentMs = TimerUtils::getTime();
float TimerUtils::getTime() {
	std::chrono::high_resolution_clock m_clock;
	return std::chrono::duration_cast<std::chrono::nanoseconds>(m_clock.now().time_since_epoch()).count() / 1000000.f;
}

unsigned __int64 TimerUtils::getCurrentMs() {
	FILETIME f;
	GetSystemTimeAsFileTime(&f);
	(long long)f.dwHighDateTime;
	unsigned __int64 nano = ((__int64)f.dwHighDateTime << 32LL) + (__int64)f.dwLowDateTime;
	return (nano - 116444736000000000LL) / 10000;
}

void TimerUtils::reset() {
	currentMs = getTime();
}

bool TimerUtils::hasTimeElapsed(float sec) {
	if (getTime() - currentMs >= (sec)) {
		reset();
		return true;
	}
	return false;
}