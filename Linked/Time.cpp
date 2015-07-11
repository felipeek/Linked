#include "Time.h"

static std::chrono::system_clock::time_point m_epoch = std::chrono::high_resolution_clock::now();

double Time::getTime()
{
	return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - m_epoch).count() / 1000000000.0;
}
