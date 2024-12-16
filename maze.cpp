#include "maze.h"
#include <cstdlib>
#include <ctime>
#include <stack>
#include <queue>
#include <thread>
#include <chrono>
#include <iostream>

using namespace std::this_thread;
using namespace std::chrono;

Maze::Maze() : size(5), start(0), goal(size * size - 1), pos(start) {
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
    if (pos2 > pos1) {
        if (pos2 == pos1 + 1) {
            walls[pos2][0] = true; // Remove left wall of pos2
        } else {
            walls[pos2][1] = true; // Remove top wall of pos2
        }
    } else {
        if (pos1 == pos2 + 1) {
            walls[pos1][0] = true; // Remove left wall of pos1
        } else {
            walls[pos1][1] = true; // Remove top wall of pos1
        }
    }
}

void Maze::randomDFS(int pos, int size) {
    visited[pos] = true;
    int next = pos + nextCell(pos, size);

    while (next != pos) {
        connect(pos, next);
        randomDFS(next, size);
        next = pos + nextCell(pos, size);
    }
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

void Maze::solveMazeDFS() {
    std::stack<int> stack;
    stack.push(start);
    visited[start] = true;
    path[start] = 1;

    int directions[] = {-size, 1, size, -1};

    while (!stack.empty()) {
        int pos = stack.top();
        stack.pop();

        if (pos == goal) {
            break;
        }

        for (int dir : directions) {
            int next = pos + dir;

            if (inBounds(next) && !visited[next]) {
                if (dir == -size && !walls[pos][1]) {
                    stack.push(next);
                    visited[next] = true;
                    path[next] = 1;
                } else if (dir == 1 && (pos + 1) % size != 0 && !walls[pos + 1][0]) {
                    stack.push(next);
                    visited[next] = true;
                    path[next] = 1;
                } else if (dir == size && pos < size * (size - 1) && !walls[pos + size][1]) {
                    stack.push(next);
                    visited[next] = true;
                    path[next] = 1;
                } else if (dir == -1 && pos % size != 0 && !walls[pos][0]) {
                    stack.push(next);
                    visited[next] = true;
                    path[next] = 1;
                }
            }
        }

        sleep_for(milliseconds(50));
        clearScreen();
        renderMaze();
    }
}

void Maze::solveMazeBFS() {
    std::queue<int> q;
    q.push(start);
    visited[start] = true;
    path[start] = 1;

    int directions[] = {-size, 1, size, -1};
    std::vector<int> parent(size * size, -1);
    std::fill_n(parent, size * size, -1);

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

        sleep_for(milliseconds(50));
        clearScreen();
        renderMaze();
    }

    int current = goal;
    while (current != start) {
        path[current] = 1;
        current = parent[current];
    }
    path[start] = 1;

    clearScreen();
    renderMaze();
}

void Maze::resetVisited() {
    std::fill_n(visited, size * size, false);
}
