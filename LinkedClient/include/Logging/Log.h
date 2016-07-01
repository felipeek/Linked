#pragma once
#include <iostream>

template<typename T>
static void fatal(T msg)
{
	std::cout << msg << std::endl;
	exit(EXIT_FAILURE);
}

template<typename T, typename... Args>
static void fatal(T msg, Args&&... args)
{
	std::cout << msg;
	fatal(args...);
}

template<typename T>
static void print(T msg)
{
	std::cout << msg << std::endl;
}

template<typename T, typename... Args>
static void print(T msg, Args&&... args)
{
	std::cout << msg;
	print(args...);
}