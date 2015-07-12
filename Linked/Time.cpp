#include "Time.h"
#include <time.h>

#include <Windows.h>
#include <iostream>
static double g_freq;
static bool g_timerInitialized = false;


//static std::chrono::system_clock::time_point m_epoch = std::chrono::high_resolution_clock::now();

double Time::getTime()
{
	//return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - m_epoch).count() / 1000000000.0;

	if (!g_timerInitialized)
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

}
