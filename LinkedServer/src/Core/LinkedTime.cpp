#include "LinkedTime.h"
#include <iostream>
#include <time.h>

static double g_freq;
static bool g_timerInitialized = false;

double LinkedTime::getTime()
{
	clockid_t clockid;
	struct timespec t_spec;
	int start = clock_gettime(CLOCK_MONOTONIC_RAW, &t_spec);
	unsigned long long res = t_spec.tv_nsec + 1000000000 * t_spec.tv_sec;
	return (double)res / 1000000000.0;
	/*if (!g_timerInitialized)
	{
		LARGE_INTEGER li;
		if (!QueryPerformanceFrequency(&li))
			std::cerr << "QueryPerformanceFrequency failed in timer initialization" << std::endl;

		g_freq = double(li.QuadPart);
		g_timerInitialized = true;
	}

	LARGE_INTEGER li;
	if (!QueryPerformanceCounter(&li))
		std::cerr << "QueryPerformanceCounter failed in get time!" << std::endl;

	return double(li.QuadPart) / g_freq;
*/
}