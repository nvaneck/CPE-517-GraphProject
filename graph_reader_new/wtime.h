#ifndef __H_TIME__
#define __H_TIME__
#include <chrono>


//#include <sys/time.h>
#include <stdlib.h>

inline std::chrono::steady_clock::time_point wtime()
{
	
	auto time = std::chrono::high_resolution_clock::now();

	return time;
}

#endif
