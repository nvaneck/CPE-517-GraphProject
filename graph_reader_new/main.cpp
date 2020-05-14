#include <iostream>
#include <omp.h>
#include "graph.h"

char* MultiStatusArrBFS(graph<long, long, int, long, long, char>* ginst,int source, int thread_count){
    double tm = wtime();
    int ptr;
    int j;

    omp_set_num_threads(thread_count);

    char* statusArray = new char[ginst->vert_count];
    //for (int i = 0; i < ginst->vert_count; i++)
    //  statusArray[i] = -1; //-1 means unvisited;

    //Initializing table in parallell(MultiThreads)
    #pragma omp parallel for 
    for(int i = 0; i < ginst->vert_count; ++i)
    {
        statusArray[i] = -1;
    }
    
    statusArray[source] = 0;
    int myFrontierCount = 0;
    int currLevel = 0; 

    #pragma omp parallel num_threads(thread_count) 
        while (true){ 
            int mymyFrontierCount = 0;
            int mycurrLevel = 0; 
            
                //change while to for look for multithreading 
                #pragma omp parallel for             
                for(ptr = 0; ptr < ginst->vert_count; ptr++){
                    if (statusArray[ptr] == mycurrLevel) {
                        int beg = ginst->beg_pos[ptr];
                        int end = ginst->beg_pos[ptr + 1];

                        for (j = beg; j < end; j++) {
                            if (statusArray[ginst->csr[j]] == -1) {
                                statusArray[ginst->csr[j]] = mycurrLevel+1;
                            }
                        }
                    }

                    else if (statusArray[ptr] != mycurrLevel) { 
                        mymyFrontierCount++;
                    }
                }    
                mycurrLevel++;

                if (mymyFrontierCount == ginst->vert_count) {
                    std::cout<<"Runtime: "<<wtime()-tm<<" second(s) for " <<thread_count<<" threads.\n";
                    return statusArray;
                }
                mymyFrontierCount = 0;
        }
}


char* MultiFrontierQueueBFS(graph<long, long, int, long, long, char>* ginst, int source, int thread_count){
    double tm = wtime();
    int j,frontier = 0;
    omp_set_num_threads(thread_count);
 
    char* statusArray = new char[ginst->vert_count];
    //Marks all verts as unvisited
    // for(int i = 0; i < ginst->vert_count; i++){
    //   statusArray[i] = -1;
    //}

    //Initializing table in parallell(MultiThreads)
    #pragma omp parallel for 
    for(int i = 0; i < ginst->vert_count; ++i)
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

    #pragma omp parallel num_threads(thread_count)

        while(true){
            int myj,myfrontier = 0;
            int* mycurrFrontierQueue = new int[ginst->vert_count];
            int* mynextFrontierQueue = new int[ginst->vert_count];
            int mycurrFrontierSize = 0;
            int mynextFrontierSize = 0;
            mycurrFrontierQueue[mycurrFrontierSize] = source;
            mycurrFrontierSize++;
            mystatusArray[source] = 0;
            int mymyFrontierIndex = 0;
            int mycurrLevel = 1;
            
            #pragma omp parallel for 
            for(mymyFrontierIndex = 0; mymyFrontierIndex < mycurrFrontierSize; mymyFrontierIndex++) {
                myfrontier = mycurrFrontierQueue[mymyFrontierIndex];
                long int beg = ginst->beg_pos[myfrontier];
                long int end = ginst->beg_pos[myfrontier+1];

                for(myj = beg; myj < end; myj++){
                    if(mystatusArray[ginst->csr[myj]] == -1){
                        mystatusArray[ginst->csr[myj]] = mycurrLevel;
                        mynextFrontierQueue[mynextFrontierSize] = ginst->csr[myj];
                        mynextFrontierSize++;
                    }
                }
                
            }

            if(mynextFrontierSize == 0){
                    std::cout<<"Runtime: "<<wtime()-tm<<" second(s) for " <<thread_count<<" threads.\n";
                        return mystatusArray;
            }

            //Swap current and next frontier queue
            mycurrFrontierSize = mynextFrontierSize;
            mymyFrontierIndex = 0;
            mynextFrontierSize = 0;

            int*temp = mycurrFrontierQueue;
            mycurrFrontierQueue = mynextFrontierQueue;
            mynextFrontierQueue = temp;
            myurrLevel++;
        }
}

int main(int args, char **argv)
{
    std::cout<<"Input: ./exe beg csr weight\n";
    if(args!=4){std::cout<<"Wrong input\n"; return -1;}

    const char *beg_file=argv[1];
    const char *csr_file=argv[2];
    const char *weight_file=argv[3];

    graph<long, long, int, long, long, char>
    *ginst = new graph
    <long, long, int, long, long, char>
    (beg_file,csr_file,weight_file);

    char*statusArray = MultiStatusArrBFS(ginst,0, 1);
    char*statusArray = MultiStatusArrBFS(ginst,0, 2);
    char*statusArray = MultiStatusArrBFS(ginst,0, 4);
    char*statusArray = MultiStatusArrBFS(ginst,0, 8);
    char*statusArray = MultiStatusArrBFS(ginst,0, 16);

    return 0;
}