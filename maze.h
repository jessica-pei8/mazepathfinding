#ifndef MAZE_H
#define MAZE_H

class Maze {
public:
    const int size;
    int start;
    Maze();
    void clearScreen();
    void randomDFS(int pos, int size);
    void renderMaze();
    void solveMazeDFS();
    void solveMazeBFS();
    void resetVisited();

private:
    int goal;
    int pos;
    bool visited[25];
    bool walls[25][2]; // 0: Left wall, 1: Top wall
    int path[25];

    int nextCell(int pos, int size);
    void connect(int pos1, int pos2);
    bool inBounds(int pos);
};

#endif 
