#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void print_world(int N, int **world);
void count_neighbors(int N, int **world, int **neighbors);
void next_step(int N, int **world, int **one_step, int **neighbors);
void update(int N, int **world, int **one_step);
int compare(int N, int **world, int **one_step, int **two_steps, int **old, int status);

/*  Optimization techniques:
    In main function:
    Buffer for all memory allocation
    Changed int to const int (where possible)
    Loop unrolling when resetting the neighbor matrices
    Update the world with two steps in one iteration

    Simplified count_neighbors function
*/

int main(int argc, const char *argv[])
{
    if (argc != 4)
    {
        printf("Give the following input arguments:\n");
        printf("N: Size of the NxN world (integer)\n");
        printf("Initial state: random (0), chessboard (1)\n");
        printf("Output: Number of steps until final state (0) \n");
        printf("        Number of steps until final state, initial and final states (1) \n");
        printf("        Number of steps until final state and all states states (2) \n");
        exit(0);
    }

    const int N = atoi(argv[1]);
    const int pattern = atoi(argv[2]);
    const int output = atoi(argv[3]);

    // Create necessary matrices
    const int n = N+1;
    int **buffer = (int **)malloc(6 * n * sizeof(int *));
    for(int i = 0; i < (6*n); i++)
    {
        buffer[i] = (int *)malloc(n*sizeof(int));
    }

    int **world = &buffer[0];
    int **neighbors = &buffer[n];
    int **neighbors_2 = &buffer[2*n];
    int **one_step = &buffer[3*n];
    int **two_steps = &buffer[4*n];
    int **old = &buffer[5*n];

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

    if(output==1 || output==2){
        printf("Initial state:\n");
        print_world(N, world);
    }
    
    int status = 1;
    int t = 1;
    update(N, old, world);

    while(status == 1)
    {
        // Predict one step forward
        count_neighbors(N, world, neighbors);
        next_step(N, world, one_step, neighbors);

        // Predict two steps forward
        count_neighbors(N, one_step, neighbors_2);
        next_step(N, one_step, two_steps, neighbors_2);

        // Compare all predicted steps
        status = compare(N, world, one_step, two_steps, old, status);
        
        // Update world with two steps
        update(N, world, two_steps);
        
        for(int i = 0; i < N; i++)
        {
            for(int j = 0; j < N; j+=2)
            {
                neighbors[i][j] = 0;
                neighbors[i][j+1] = 0;
                neighbors_2[i][j] = 0;
                neighbors_2[i][j+1] = 0;
            }
        }

        if((output == 2) && (status == 1)){
            printf("Step %d:\n", t);
            print_world(N, one_step);
            printf("Step %d:\n", t+1);
            print_world(N, two_steps);
        }
        
        // Save previous step
        update(N, old, world);
        t+=2;
    }

    printf("It took %d steps to reach the final state\n", (t-3));
    if(output==1 || output ==2){
        printf("Final state:\n");
        print_world(N, world);
    }    
    
    for (int i = 0; i < (6*n); i++)
    {
        free(buffer[i]);
    }
    free(buffer);
}

void print_world(int N, int **world)
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            printf("%d ", world[i][j]);
        }
        printf("\n");
    }
    printf("\n");   
}

void count_neighbors(int N, int **world, int **neighbors)
{
    int i; //rows
    int j; //col
    for (i = 0; i <= N-1; i++){
        for (j = 0; j <= N-1; j++){     
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
}

void next_step(int N, int **world, int **one_step, int **neighbors)
{
    int i, j;
    for (i = 0; i < N; i++){
        for (j = 0; j < N; j++){
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
}

void update(int N, int **world, int **one_step)
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            world[i][j] = one_step[i][j];               
        }
            
    }
}

int compare(int N, int **world, int **one_step, int **two_steps, int **old, int status)
{
    int counter1=0, counter2=0, counter3=0;
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            if(world[i][j] == one_step[i][j])
                counter1++;
            if(world[i][j] == 0)
                counter2++;
            if(old[i][j] == two_steps[i][j])
                counter3++;
        }
    }
    if (counter1 == (N*N))
        status = 0;
    else if(counter2 == (N*N))
        status = 0;
    else if(counter3 == (N*N))
        status = 0;
    return status;
}