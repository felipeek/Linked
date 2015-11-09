#pragma once
#define DEBUG 1
#define GAMESPEED 60.0
#define FRAMECAP 60.0

#if DEBUG
#include <iostream>

#define LOG(x) std::cout << x << std::endl;
#else
#define LOG(x)
#endif