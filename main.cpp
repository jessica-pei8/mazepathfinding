#include <iostream>
#include "maze.h"

int main() {
    srand(time(0)); 

    Maze maze;
    maze.resetVisited(); 
    maze.clearScreen();
    //maze.createMazeDFS();     
    maze.renderMaze(); 
    // std::cout << "DFS Solution:" << std::endl;  
    // maze.solveMazeDFS();  

    // maze.resetVisited(); 
    // std::cout << "BFS Solution:" << std::endl;  
    // maze.solveMazeBFS(); 

    return 0;
}
