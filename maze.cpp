#include "maze.h"
#include <cstdlib>
#include <ctime>
#include <stack>
#include <queue>
#include <thread>
#include <chrono>
#include <iostream>
#include <algorithm>
#include <random>

using namespace std::this_thread;
using namespace std::chrono;

Maze::Maze() : size(10), start(0), goal(size * size - 1), pos(start) {
    std::fill_n(visited, size * size, false);
    std::fill_n(path, size * size, 0);
    for (int i = 0; i < size * size; ++i) {
        walls[i][0] = walls[i][1] = false;
    }
}

void Maze::clearScreen() {
#ifdef _WIN32
    system("cls"); // For Windows
#else
    system("clear"); // For Unix/Linux
#endif
}

int Maze::nextCell(int pos, int size) {
    int options[4];
    int optionCount = 0;

    if (pos >= size && !visited[pos - size]) { // Top
        options[optionCount++] = -size;
    }
    if ((pos + 1) % size != 0 && !visited[pos + 1]) { // Right
        options[optionCount++] = 1;
    }
    if (pos < size * (size - 1) && !visited[pos + size]) { // Bottom
        options[optionCount++] = size;
    }
    if (pos % size != 0 && !visited[pos - 1]) { // Left
        options[optionCount++] = -1;
    }

    if (optionCount == 0)
        return 0;

    return options[rand() % optionCount];
}

void Maze::connect(int pos1, int pos2) {
    int x1 = pos1 % size;
    int y1 = pos1 / size;
    int x2 = pos2 % size;
    int y2 = pos2 / size;

    // If pos1 and pos2 are horizontally adjacent (same row)
    if (y1 == y2 && abs(x1 - x2) == 1) {
        // Remove the vertical wall between pos1 and pos2
        walls[pos1][0] = true;
        walls[pos2][0] = true;
    }
    // If pos1 and pos2 are vertically adjacent (same column)
    else if (x1 == x2 && abs(y1 - y2) == 1) {
        // Remove the horizontal wall between pos1 and pos2
        walls[pos1][1] = true;
        walls[pos2][1] = true;
    }
}

void Maze::randomDFS(int pos, int size) {
    visited[pos] = 1;
    int next = pos + nextCell(pos, size);

    while (next != pos) {
        sleep_for(nanoseconds(10)); 
        connect(pos, next);
        pos = next;
        randomDFS(pos, size);
        next = pos + nextCell(pos, size);
    }
}

void Maze::createMazeDFS() {
    int pos = 0;
    for (int i = 0; i < size * size; i++) {
        visited[i] = false;
    }

    randomDFS(pos, size);
}

bool Maze::inBounds(int pos) {
    return pos >= 0 && pos < size * size;
}

void Maze::renderMaze() {
    for (int j = 0; j < size; j++) {
        for (int i = 0; i < size; i++) {
            std::cout << "+";
            if (walls[i + j * size][1]) {
                std::cout << "   ";
            } else {
                std::cout << "---";
            }
        }
        std::cout << "+" << std::endl;

        for (int i = 0; i < size; i++) {
            if (walls[i + j * size][0]) {
                std::cout << " ";
            } else {
                std::cout << "|";
            }

            if (i + j * size == start) {
                std::cout << " S ";
            } else if (i + j * size == goal) {
                std::cout << " G ";
            } else if (path[i + j * size]) {
                std::cout << " * ";
            } else {
                std::cout << "   ";
            }
        }
        std::cout << "|" << std::endl;
    }

    for (int i = 0; i < size; i++) {
        std::cout << "+---";
    }
    std::cout << "+" << std::endl;
}

bool Maze::solveMazeDFS() {
    clearScreen();
    resetVisited(); 
    std::stack<int> stack;
    stack.push(start);
    visited[start] = true;

    int directions[] = {-size, 1, size, -1};  

    while (!stack.empty()) {
        int pos = stack.top();
        stack.pop();

        if (pos == goal) {
            path[pos] = 1; 
            break;
        }

        for (int dir : directions) {
            int next = pos + dir;

            if (inBounds(next) && !visited[next]) {
                // Check for valid movements based on wall connections
                if (dir == -size && !walls[pos][1]) {  // Moving up, check the bottom wall of the current cell
                    stack.push(next);
                    visited[next] = true;
                } else if (dir == 1 && (pos + 1) % size != 0 && !walls[pos + 1][0]) {  // Moving right, check the left wall of the right cell
                    stack.push(next);
                    visited[next] = true;
                } else if (dir == size && pos < size * (size - 1) && !walls[pos + size][1]) {  // Moving down, check the top wall of the cell below
                    stack.push(next);
                    visited[next] = true;
                } else if (dir == -1 && pos % size != 0 && !walls[pos][0]) {  // Moving left, check the right wall of the left cell
                    stack.push(next);
                    visited[next] = true;
                }
            }
        }
        path[pos] = 1;
        clearScreen();
        renderMaze();
        sleep_for(milliseconds(50));
    }
    renderMaze();
    return visited[goal];  
}


bool Maze::solveMazeBFS() {
    clearScreen(); 
    std::queue<int> q;
    q.push(start);
    visited[start] = true;
    path[start] = 1;

    int directions[] = {-size, 1, size, -1};
    std::vector<int> parent(size * size, -1);
    std::fill_n(parent.begin(), size * size, -1);

    while (!q.empty()) {
        int pos = q.front();
        q.pop();

        if (pos == goal) {
            break;
        }

        for (int dir : directions) {
            int next = pos + dir;

            if (inBounds(next) && !visited[next]) {
                if (dir == -size && !walls[pos][1]) {
                    q.push(next);
                    visited[next] = true;
                    path[next] = 1;
                    parent[next] = pos;
                } else if (dir == 1 && (pos + 1) % size != 0 && !walls[pos + 1][0]) {
                    q.push(next);
                    visited[next] = true;
                    path[next] = 1;
                    parent[next] = pos;
                } else if (dir == size && pos < size * (size - 1) && !walls[pos + size][1]) {
                    q.push(next);
                    visited[next] = true;
                    path[next] = 1;
                    parent[next] = pos;
                } else if (dir == -1 && pos % size != 0 && !walls[pos][0]) {
                    q.push(next);
                    visited[next] = true;
                    path[next] = 1;
                    parent[next] = pos;
                }
            }
        }

        renderMaze();  
        sleep_for(milliseconds(50));
    }

    int current = goal;
    while (current != start) {
        path[current] = 1;
        current = parent[current];
    }
    path[start] = 1;

    renderMaze();  
    return pos==goal; 
}


void Maze::resetVisited() {
    std::fill_n(visited, size * size, false);
}

bool Maze::isSolvable() {
    resetVisited(); 
    return solveMazeDFS();  
}

void Maze::generateMaze() {
    std::fill_n(visited, size * size, false); 
    randomDFS(start, size);  
}
