# Game_Of_Life
Game Of Life - Individual Project (High-Performance Programming Uppsala University)

## Introduction
This report focuses on the implementation of the Game of Life. The Game of Life was created by John Horton Conway in 1970. This game does not require any players and it describes the evolution of a world of cells. The cells can either be live or dead and the evolution of the game depends on its initial state.

## Rules
Each cell has eight neighbors. The evolution of the world could be divided into steps. The number of live neighbors along with the state of the cell, in other words if the cell is live or dead, determine the status the cell for the next step. For each step and each cell the following rules apply: 
- If a live cell has fewer than 2 neighbors it dies by under-population.
- If a live cell has 2 or 3 neighbors it remains live.
- If a live cell has more than 3 neighbors it dies by over-population.
- If a dead cell has exactly 3 neighbors it becomes live by reproduction.

These rules can then be simplified to the following set of rules: 
- Any live cell with 2 or 3 neighbors remains live.
- Any dead cell with 3 neighbors become live.
- All other live cells die and all other dead cells remain dead.

Each initial state can result in one of the following final states:
- **Stable state:** A group of cells has reached a stable pattern, see figure \ref{fig:stable}, where no matter how many steps are performed the specific group of live cells will not evolve further.
- **Alternating state:** A group of cells has reached an alternating pattern, see figure \ref{fig:alternating}, where for each step the group of cells will alternate between two patterns.
- **Zero state:** All the cells are dead.

## Purpose
The purpose of this project is to implement a program which plays the Game of Life. The program should create a world of cells and calculate the evolution of the world with each step by following the rules of the Game of Life. The game should continue until the world stabilizes either by reaching a stable, alternating or zero state. The program should take three integers as inputs, the size of the square world, *N*, the type of initial state and the type of output the user would like to have. Here are the various options for the initial state and the output:

- **Initial state = 0:** Random pattern.
- **Initial state = 1:** Chessboard pattern, see figure \ref{fig:chessboard}.
- **Output = 0:** Prints only the number of steps needed for the world to stabilize.
- **Output = 1:** Prints the initial and the final states and the number of steps needed for the world to stabilize.
- **Output = 2:** Prints the states of all steps, including the initial and the final steps, and the number of steps needed for the world to stabilize.
