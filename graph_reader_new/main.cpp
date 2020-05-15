#include "stdafx.h"
#include <omp.h>
#include <chrono>
#include "C:\msys64\home\Frank\CPE_517\CPE-517-GraphProject\graph_reader_new\graph.h"
#pragma warning(disable : 4996)


char* MultiStatusArrBFS(graph<long, long, long, long, long, long>* ginst, int source, int thread_count) {
	auto start_time = std::chrono::high_resolution_clock::now();
	int ptr;
	int j;

	omp_set_num_threads(thread_count);

	char* statusArray = new char[ginst->vert_count];
	//for (int i = 0; i < ginst->vert_count; i++)
	//  statusArray[i] = -1; //-1 means unvisited;

	//Initializing table in parallell(MultiThreads)
#pragma omp parallel for 
	for (int i = 0; i < ginst->vert_count; ++i)
	{
		statusArray[i] = -1;
	}

	statusArray[source] = 0;
	int myFrontierCount = 0;
	int currLevel = 0;

#pragma omp parallel num_threads(thread_count)
	while (true) {

		long mymyFrontierCount = 0;
		int mycurrLevel = 0;
		//change while to for look for multithreading 
#pragma omp parallel for      
		for (ptr = 0; ptr < ginst->vert_count; ptr++) {
			if (statusArray[ptr] == mycurrLevel) {
				int beg = ginst->beg_pos[ptr];
				int end = ginst->beg_pos[ptr + 1];

				for (j = beg; j < end; j++) {
					if (statusArray[ginst->csr[j]] == -1) {
						statusArray[ginst->csr[j]] = mycurrLevel + 1;
					}
				}
			}

			else if (statusArray[ptr] != mycurrLevel) {
				mymyFrontierCount++;
			}
		}
		mycurrLevel++;

		if (mymyFrontierCount == ginst->vert_count) {
			auto end_time = std::chrono::high_resolution_clock::now();
			auto time = end_time - start_time;
			std::cout << "Runtime: " << time / std::chrono::milliseconds(1) << " millisecond(s) for " << thread_count << " threads.\n";
			return statusArray;
		}
		mymyFrontierCount = 0;
	}
	
}


char* MultiFrontierQueueBFS(graph<long, long, /*int*/ long, long, long, /*char*/ long>* ginst, int source, int thread_count) {
	int j, frontier = 0;

	char* statusArray = new char[ginst->vert_count];
	//Marks all verts as unvisited
	// for(int i = 0; i < ginst->vert_count; i++){
	//   statusArray[i] = -1;
	//}

	//Initializing table in parallell(MultiThreads)
#pragma omp parallel for 
	for (int i = 0; i < ginst->vert_count; ++i)
	{
		statusArray[i] = -1;
	}

	int* currFrontierQueue = new int[ginst->vert_count];
	int* nextFrontierQueue = new int[ginst->vert_count];
	int currFrontierSize = 0;
	int nextFrontierSize = 0;
	currFrontierQueue[currFrontierSize] = source;
	currFrontierSize++;
	statusArray[source] = 0;
	int myFrontierIndex = 0;
	int currLevel = 1;

	while (true) {
		while (myFrontierIndex < currFrontierSize) {
			frontier = currFrontierQueue[myFrontierIndex];
			long int beg = ginst->beg_pos[frontier];
			long int end = ginst->beg_pos[frontier + 1];

			for (j = beg; j < end; j++) {
				if (statusArray[ginst->csr[j]] == -1) {
					statusArray[ginst->csr[j]] = currLevel;
					nextFrontierQueue[nextFrontierSize] = ginst->csr[j];
					nextFrontierSize++;
				}
			}
			myFrontierIndex++;
		}
		if (nextFrontierSize == 0) {
			std::cout << "Runtime: second(s).\n";
			return statusArray;
		}

		//Swap current and next frontier queue
		currFrontierSize = nextFrontierSize;
		myFrontierIndex = 0;
		nextFrontierSize = 0;

		int*temp = currFrontierQueue;
		currFrontierQueue = nextFrontierQueue;
		nextFrontierQueue = temp;
		currLevel++;
	}
}

int main(int args, char **argv)
{
	std::cout << "Input: ./exe beg csr weight\n";
	if (args != 4) { std::cout << "Wrong input\n"; return -1; }

	const char *beg_file = argv[1];
	const char *csr_file = argv[2];
	const char *weight_file = argv[3];

	graph<long, long, long, long, long, long> *ginst = new graph
	<long, long, long, long, long, long>
		(beg_file, csr_file, weight_file);

	char*statusArray1 = MultiStatusArrBFS(ginst, 0, 1);
	char*statusArray2= MultiStatusArrBFS(ginst, 0, 2);
	char*statusArray3 = MultiStatusArrBFS(ginst, 0, 4);
	char*statusArray4 = MultiStatusArrBFS(ginst, 0, 8);
	char*statusArray5 = MultiStatusArrBFS(ginst, 0, 16);

	return 0;
}