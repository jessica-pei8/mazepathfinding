#ifndef MAZE_H
#define MAZE_H
#include <cstdlib>
#include <ctime>
#include <stack>
#include <queue>
#include <thread>
#include <chrono>
#include <iostream>
#include <algorithm>
#include <random>
#include <unordered_set>
#include <set>
#include <cmath>
#include <unordered_map>

class Maze {
public:
    Maze();
    void clearScreen();
    void renderMaze();
    bool solveMazeDFS();
    bool solveMazeBFS();
    bool solveMazeAStar(); 
    bool solveMazeRightHandRule(); 
    bool solveMazeBidirectional(); 
    void resetVisited();
    void generateMaze(); 

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
    int heuristic(int pos);
    bool validMove(int pos, int next, int dir); 
    bool expandBidirectional(std::queue<int>& queue, std::unordered_map<int, int>& path, std::unordered_map<int, int>& otherPath, int directions[]); 
    void createMazeDFS(); 
    void createMazeWilson();
    void loopErasedRandomWalk(int startPos); 
};

#endif 
