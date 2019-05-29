#pragma once
#define DEBUG 0

#if DEBUG
#include <iostream>

#define LOG(x) std::cout << x << std::endl;
#else
#define LOG(x)
#endif