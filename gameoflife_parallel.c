#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>

/*  Parallel code with PTHREADS */

//Global variables 
int N;                      // Size of the world
int nthreads;               // Number of threads
int **world;                // pointer to the world
int **neighbors;            // pointer to the neighbors matrix
int **neighbors_2;          // pointer to the neighbors matrix
int **old;                  // pointer to the old matrix
int **one_step;             // pointer to the old matrix
int **two_steps;            // pointer to the old matrix       
int done;                   // nr of threads that are done
pthread_barrier_t barrier;  // pthread barrier

typedef struct info_thread
{
    int threadID;       // threadID
    int low;            // lower limit of interval
    int high;           // higher limit of interval
    int status;         // status for when the for loop should stop
    int timesteps;      // maximum amount of time-steps
    int steps;          // nr of performed time-steps
}t_info;

void * thread_func(void *arg);
void print_world(int **world);

int main(int argc, const char *argv[])
{
    if (argc != 5)
    {
        printf("Give the following input arguments:\n");
        printf("N: Size of the NxN world (integer)\n");
        printf("Initial state: random (0), chessboard (1)\n");
        printf("Output: Number of steps until final state (0) \n");
        printf("        Number of steps until final state, initial and final states (1) \n");
        printf("Threads: Number of threads (integer)\n");
        exit(0);
    }

    N = atoi(argv[1]);
    const int pattern = atoi(argv[2]);
    const int output = atoi(argv[3]);
    nthreads = atoi(argv[4]);
    
    // Create necessary matrices
    const int n = N+1;
    int **buffer = (int **)malloc(6 * n * sizeof(int *));
    for(int i = 0; i < (6*n); i++)
    {
        buffer[i] = (int *)malloc(n*sizeof(int));
    }

    world = &buffer[0];
    neighbors = &buffer[n];
    neighbors_2 = &buffer[2*n];
    one_step = &buffer[3*n];
    two_steps = &buffer[4*n];
    old = &buffer[5*n];

    // Setting a random initial pattern
    if(pattern == 0){
        srand(time(0));
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
            {
                int r = rand() % 10;
                if (r > 5)
                    world[i][j] = 1;
                else
                    world[i][j] = 0;
            }
        }
    }
    // Setting a chessboard initial state
    else if(pattern == 1){
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
            {
                if(i%2 == 0){
                    if(j%2 == 0)
                        world[i][j] = 0;
                    else
                        world[i][j] = 1;
                }
                else{
                    if(j%2 == 0)
                        world[i][j] = 1;
                    else
                        world[i][j] = 0;
                }
            }
        }
    }

    if(output==1){
        printf("Initial state:\n");
        print_world(world);
    }
        
    // Create threads and thread info
    pthread_t thread[nthreads];
    t_info threadinfo[nthreads];
    pthread_barrier_init (&barrier, NULL, nthreads);
    const int interval = N/nthreads;

    for (int k=0; k<nthreads; k++)
    {
        threadinfo[k].threadID = k;
        threadinfo[k].low = k*interval;
        threadinfo[k].high = (k+1)*interval-1;
        threadinfo[k].status = 1;
        threadinfo[k].timesteps = 10000;
        threadinfo[k].steps = 0;
    }
    threadinfo[nthreads-1].high = N-1;
    done = 0;
    
    // Run threads
    for (int k=0; k<nthreads; k++)
        pthread_create(&thread[k], NULL, thread_func, (void *)&threadinfo[k]);
    for (int k=0; k<nthreads; k++)
        pthread_join(thread[k],NULL);
    
    if(threadinfo[nthreads-1].steps != threadinfo[nthreads-1].timesteps)
    {
        printf("It took %d steps to reach the final state\n", threadinfo[nthreads-1].steps);
        if(output==1){
            printf("Final state:\n");
            print_world(world);
        }    
    }
    else
    {
        printf("The maximum amount of iterations has been reach.\n");
        printf("The problem couldn't be solved.\n");
    }
        
    // Free allocated memory
    for (int i = 0; i < (6*n); i++)
    {
        free(buffer[i]);
    }
    free(buffer);
}

void * thread_func(void *arg)
{
    t_info *threadinfo = arg;
    int low = threadinfo->low;
    int high = threadinfo->high;

    for (int t=1; t < threadinfo->timesteps; t++)
    {
        pthread_barrier_wait(&barrier);

        for (int i = low; i <= high; i++){
            for (int j = 0; j <= N-1; j++){     
                if (i > 0){
                    if (j > 0){
                        if (world[i-1][j-1] == 1)
                            neighbors[i][j] +=1;
                    }
                    if (j < N-1){
                        if (world[i-1][j+1] == 1)
                            neighbors[i][j] +=1;
                    }
                    if (world[i-1][j] == 1)
                        neighbors[i][j] +=1;
                }
                if (i < N-1){
                    if (j > 0){
                        if (world[i+1][j-1] == 1)
                            neighbors[i][j] +=1;
                    }
                    if (j < N-1){
                        if (world[i+1][j+1] == 1)
                            neighbors[i][j] +=1;
                    }
                    if (world[i+1][j] == 1)
                        neighbors[i][j] +=1;
                }
                if (j > 0){
                    if (world[i][j-1] == 1)
                        neighbors[i][j] +=1;
                }
                if(j < N-1){
                    if (world[i][j+1] == 1)
                        neighbors[i][j] +=1;
                }
            }
        }
    
        for (int i = low; i <= high; i++){
            for (int j = 0; j <= N-1; j++){
                if (world[i][j] == 1)
                {
                    if (neighbors[i][j] == 2 || neighbors[i][j] == 3)
                        one_step[i][j] = 1;
                    else
                        one_step[i][j] = 0;
                }
                else if (world[i][j] == 0)
                {
                    if (neighbors[i][j] == 3)
                        one_step[i][j] = 1;
                    else
                        one_step[i][j] = 0;
                }
            }
        }
    
        pthread_barrier_wait(&barrier);

        for (int i = low; i <= high; i++){
            for (int j = 0; j <= N-1; j++){     
                if (i > 0){
                    if (j > 0){
                        if (one_step[i-1][j-1] == 1)
                            neighbors_2[i][j] +=1;
                    }
                    if (j < N-1){
                        if (one_step[i-1][j+1] == 1)
                            neighbors_2[i][j] +=1;
                    }
                    if (one_step[i-1][j] == 1)
                        neighbors_2[i][j] +=1;
                }
                if (i < N-1){
                    if (j > 0){
                        if (one_step[i+1][j-1] == 1)
                            neighbors_2[i][j] +=1;
                    }
                    if (j < N-1){
                        if (one_step[i+1][j+1] == 1)
                            neighbors_2[i][j] +=1;
                    }
                    if (one_step[i+1][j] == 1)
                        neighbors_2[i][j] +=1;
                }
                if (j > 0){
                    if (one_step[i][j-1] == 1)
                        neighbors_2[i][j] +=1;
                }
                if(j < N-1){
                    if (one_step[i][j+1] == 1)
                        neighbors_2[i][j] +=1;
                }
            }
        }
    
        for (int i = low; i <= high; i++){
            for (int j = 0; j <= N-1; j++){
                if (one_step[i][j] == 1)
                {
                    if (neighbors_2[i][j] == 2 || neighbors_2[i][j] == 3)
                        two_steps[i][j] = 1;
                    else
                        two_steps[i][j] = 0;
                }
                else if (one_step[i][j] == 0)
                {
                    if (neighbors_2[i][j] == 3)
                        two_steps[i][j] = 1;
                    else
                        two_steps[i][j] = 0;
                }
            }
        }

        int counter1=0, counter2=0, counter3=0;
        int diff = high - low +1;
        for (int i = low; i <= high; i++)
        {
            for (int j = 0; j <= N-1; j++)
            {   
                if(world[i][j] == one_step[i][j])
                    counter1++;
                if(world[i][j] == 0)
                    counter2++;
                if(old[i][j] == two_steps[i][j])
                    counter3++;
            }
        }
        
        if (counter1 == (diff*N))
            threadinfo->status = 0;
        else if(counter2 == (diff*N))
            threadinfo->status = 0;
        else if(counter3 == (diff*N))
            threadinfo->status = 0;

        
        if(threadinfo->status == 0)
            done++;
        pthread_barrier_wait(&barrier);
        
        if(done == nthreads)
            break;
        
        for (int i = low; i <= high; i++)
        {
            for (int j = 0; j <= N-1 ; j++)
                world[i][j] = two_steps[i][j];               
        }

        for (int i = low; i <= high; i++)
        {
            for (int j = 0; j <= N-1 ; j++)
                old[i][j] = world[i][j];               
        }
    
        for(int i = low; i <= high; i++)
        {
            for(int j = 0; j <= N-1; j++)
            {
                neighbors[i][j] = 0;
                neighbors_2[i][j] = 0;
            }
        }

        done = 0;
        threadinfo->steps+=2;
    }

    pthread_exit(NULL);
}

void print_world(int **world)
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N ; j+=1)
            printf("%d ", world[i][j]);
        printf("\n");
    }
    printf("\n");   
}