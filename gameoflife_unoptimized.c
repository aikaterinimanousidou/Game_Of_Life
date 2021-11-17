#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void print_world(int N, int **world);
void count_neighbors(int N, int **world, int **neighbors);
void next_step(int N, int **world, int **one_step, int **neighbors);
void update(int N, int **world, int **one_step);
int compare(int N, int **world, int **one_step, int **two_steps, int **old, int status);


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

    int N = atoi(argv[1]);
    int pattern = atoi(argv[2]);
    int output = atoi(argv[3]);

    // Create necessary matrices
    int **world = (int **)malloc(N * sizeof(int *));
    int **neighbors = (int **)malloc(N * sizeof(int *));
    int **neighbors_2 = (int **)malloc(N * sizeof(int *));
    int **one_step = (int **)malloc(N * sizeof(int *));
    int **old = (int **)malloc(N * sizeof(int *));
    int **two_steps = (int **)malloc(N * sizeof(int *));
    for (int i = 0; i < N; i++)
    {
        world[i] = (int *)malloc(N * sizeof(int));
        neighbors[i] = (int *)malloc(N * sizeof(int));
        neighbors_2[i] = (int *)malloc(N * sizeof(int));
        one_step[i] = (int *)malloc(N * sizeof(int));
        old[i] = (int *)malloc(N * sizeof(int));
        two_steps[i] = (int *)malloc(N * sizeof(int));
    }

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
        
        // Update world with one step
        update(N, world, one_step);

        
        for(int i = 0; i < N; i++)
        {
            for(int j = 0; j < N; j++)
            {
                neighbors[i][j] = 0;
                neighbors_2[i][j] = 0;
            }
        }

        if((output == 2) && (status == 1)){
            printf("Step %d:\n", t);
            print_world(N, world);
        }
        
        // Save previous step
        update(N, old, world);
        t +=1;
    }

    printf("It took %d steps to reach the final state\n", t-2);
    if(output==1 || output ==2){
        printf("Final state:\n");
        print_world(N, world);
    }    
    
    for (int i = 0; i < N; i++)
    {
        free(world[i]);
        free(neighbors[i]);
        free(neighbors_2[i]);
        free(one_step[i]);
        free(two_steps[i]);
        free(old[i]);
    }
    free(world);
    free(neighbors);
    free(neighbors_2);
    free(one_step);
    free(two_steps);
    free(old);
}

void print_world(int N, int **world)
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
            printf("%d ", world[i][j]);
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
            if(i == 0){
                // Point (0,0)
                if(j == 0){
                    if (world[i][j+1] == 1)
                        neighbors[i][j] +=1;
                    if(world[i+1][j+1] == 1)
                        neighbors[i][j] +=1;
                    if(world[i+1][j] == 1)
                        neighbors[i][j] +=1;
                }
                // Point (0,N-1)
                else if (j == N-1){
                    if (world[i][j-1] == 1)
                        neighbors[i][j] +=1;
                    if(world[i+1][j-1] == 1)
                        neighbors[i][j] +=1;
                    if(world[i+1][j] == 1)
                        neighbors[i][j] +=1;
                }
                // Points between (0,0)-(0,N-1)
                else if ((j > 0) && (j < N-1)){
                    if (world[i][j-1] == 1)
                        neighbors[i][j] +=1;
                    if(world[i+1][j-1] == 1)
                        neighbors[i][j] +=1;
                    if(world[i+1][j] == 1)
                        neighbors[i][j] +=1;
                    if(world[i+1][j+1] == 1)
                        neighbors[i][j] +=1;
                    if(world[i][j+1] == 1)
                        neighbors[i][j] +=1;
                }
            }
            if(i == N-1){
                // Point (N-1,0)
                if(j == 0){
                    if (world[i-1][j] == 1)
                        neighbors[i][j] +=1;
                    if(world[i-1][j+1] == 1)
                        neighbors[i][j] +=1;
                    if(world[i][j+1] == 1)
                        neighbors[i][j] +=1;
                }
                // Point (N-1,N-1)
                else if (j == N-1){
                    if (world[i-1][j-1] == 1)
                        neighbors[i][j] +=1;
                    if(world[i-1][j] == 1)
                        neighbors[i][j] +=1;
                    if(world[i][j-1] == 1)
                        neighbors[i][j] +=1;
                }
                // Points between (N-1,0)-(N-1,N-1)
                else if ((j > 0) && (j < N-1)){
                    if (world[i][j-1] == 1)
                        neighbors[i][j] +=1;
                    if(world[i-1][j-1] == 1)
                        neighbors[i][j] +=1;
                    if(world[i-1][j] == 1)
                        neighbors[i][j] +=1;
                    if(world[i-1][j+1] == 1)
                        neighbors[i][j] +=1;
                    if(world[i][j+1] == 1)
                        neighbors[i][j] +=1;
                }
            }
            // Left side
            if(j == 0){
                if ((i != 0) && (i!=N-1)){
                    if (world[i-1][j] == 1)
                        neighbors[i][j] +=1;
                    if(world[i-1][j+1] == 1)
                        neighbors[i][j] +=1;
                    if(world[i][j+1] == 1)
                        neighbors[i][j] +=1;
                    if(world[i+1][j+1] == 1)
                        neighbors[i][j] +=1;
                    if(world[i+1][j] == 1)
                        neighbors[i][j] +=1;
                }
            }
            // Right side
            if(j == N-1){
                if((i > 0) && (i < N-1)){
                    if (world[i-1][j] == 1)
                        neighbors[i][j] +=1;
                    if(world[i-1][j-1] == 1)
                        neighbors[i][j] +=1;
                    if(world[i][j-1] == 1)
                        neighbors[i][j] +=1;
                    if(world[i+1][j-1] == 1)
                        neighbors[i][j] +=1;
                    if(world[i+1][j] == 1)
                        neighbors[i][j] +=1;
                }
            }
            // Interior points
            if((i > 0) && (i < N-1) && (j > 0) && (j < N-1)){
                if (world[i-1][j-1] == 1)
                    neighbors[i][j] +=1;
                if(world[i-1][j] == 1)
                    neighbors[i][j] +=1;
                if(world[i-1][j+1] == 1)
                    neighbors[i][j] +=1;
                if(world[i][j+1] == 1)
                    neighbors[i][j] +=1;
                if(world[i+1][j+1] == 1)
                    neighbors[i][j] +=1;
                if(world[i+1][j] == 1)
                    neighbors[i][j] +=1;
                if(world[i+1][j-1] == 1)
                    neighbors[i][j] +=1;
                if(world[i][j-1] == 1)
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
            world[i][j] = one_step[i][j];
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