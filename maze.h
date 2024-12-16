#ifndef MAZE_H
#define MAZE_H

class Maze {
public:
    Maze();
    void clearScreen();
    void renderMaze();
    bool solveMazeDFS();
    bool solveMazeBFS();
    void resetVisited();
    void generateMaze(); 
    void createMazeDFS(); 

private:
    const int size;
    int start;
    int goal;
    int pos;
    bool visited[10];
    bool walls[10][2]; // 0: Left wall, 1: Top wall
    int path[10];

    int nextCell(int pos, int size);
    void randomDFS(int pos, int size);
    void connect(int pos1, int pos2);
    bool inBounds(int pos);
    bool isSolvable(); 
};

#endif 
