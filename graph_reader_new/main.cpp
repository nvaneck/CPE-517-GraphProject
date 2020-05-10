#include <iostream>
#include "graph.h"

int main(int args, char **argv)
{
	std::cout<<"Input: ./exe beg csr weight\n";
	if(args!=4){std::cout<<"Wrong input\n"; return -1;}
	
	const char *beg_file=argv[1];
	const char *csr_file=argv[2];
	const char *weight_file=argv[3];
	
	//template <file_vertex_t, file_index_t, file_weight_t
	//new_vertex_t, new_index_t, new_weight_t>
	graph<long, long, /*int*/long, long, long,/* char*/long>
	*ginst = new graph
	<long, long, /*int*/long, long, long, /*char*/long>
	(beg_file,csr_file,weight_file);
    
    //**
    //You can implement your single threaded graph algorithm here.
    //like BFS, SSSP, PageRank and etc.
    
    // Simple algorithm that prints neighbors and the associated weigths on those
    // edges
    for(int i = 0; i < ginst->vert_count; i++)
    {
        int beg = ginst->beg_pos[i];
        int end = ginst->beg_pos[i+1];
        std::cout<<i<<"'s neighor list: ";
     //   std::cout<<i<<"'s outgoing money: ";
        for(int j = beg; j < end; j++)
        {
            std::cout<<ginst->csr[j]<<" ";
            std::cout<<"Money:"; std::cout<<ginst->weight[j]<<" ";
        }
        std::cout<<"\n";
    }
     */

	return 0;	
}

// Algorithm 1: Single-threaded BFS Frontier Queue
// Provided in the associated docs
char* BFSTraverse(graph<long, long, int, long, long, char>* ginst, int source){
    int j,frontier = 0;
    char* statusArray = new char[ginst->vert_count];
    //Marks all verts as unvisited
    for(int i = 0; i < ginst->vert_count; i++){
        statusArray[i] = -1;
    }
    int* currFrontierQueue = new int[ginst->vert_count];
    int* nextFrontierQueue = new int[ginst->vert_count];
    int currFrontierSize = 0;
    int nextFrontierSize = 0;
    currFrontierQueue[currFrontierSize] = source;
    currFrontierSize++;
    //Mark Source as visited
    statusArray[source] = 0;
    int myFrontierIndex = 0;
    int currLevel = 1;
    while(true){
        while(myFrontierIndex < currFrontierSize) {
            frontier = currFrontierQueue[myFrontierIndex];
            long int beg = ginst->beg_pos[frontier];
            long int end = ginst->beg_pos[frontier+1];
            
            for(j = beg; j < end; j++){
                if(statusArray[ginst->csr[j]] == -1){
                    statusArray[ginst->csr[j]] = currLevel;
                    nextFrontierQueue[nextFrontierSize] = ginst->csr[j];
                    nextFrontierSize++;
                }
            }
            myFrontierIndex++;
        }
        if(nextFrontierSize == 0){return statusArray;}
        
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
