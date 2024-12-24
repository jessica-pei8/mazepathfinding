#include "maze.h"

using namespace std::this_thread;
using namespace std::chrono;

Maze::Maze() : size(10), start(0), goal(size * size - 1), pos(start) {
    std::fill_n(visited, size * size, false);
    std::fill_n(path, size * size, 0);
    for (int i = 0; i < size * size; ++i) {
        walls[i][0] = walls[i][1] = false;
    }
}

void Maze::resetVisited() {
    std::fill_n(visited, size * size, false);
}

void Maze::clearScreen() {
#ifdef _WIN32
    system("cls"); // For Windows
#else
    system("clear"); // For Unix/Linux
#endif
}

bool Maze::inBounds(int pos) {
    return pos >= 0 && pos < size * size;
}

int Maze::heuristic(int pos) {
    int x1 = pos % size, y1 = pos / size;
    int x2 = goal % size, y2 = goal / size;
    return std::abs(x1 - x2) + std::abs(y1 - y2); // Manhattan distance
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

bool Maze::validMove(int pos, int next, int dir) {
    if (!inBounds(next)) return false;
    if (dir == -size && !walls[pos][1]) return true;      // Up
    if (dir == 1 && (pos + 1) % size != 0 && !walls[pos][0]) return true; // Right
    if (dir == size && !walls[next][1]) return true;      // Down
    if (dir == -1 && pos % size != 0 && !walls[next][0]) return true;    // Left
    return false;
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

void Maze::loopErasedRandomWalk(int startPos) {
    std::unordered_set<int> walk;
    int pos = startPos;
    walk.insert(pos);
    int next = pos + nextCell(pos, size);

    while (walk.find(next) == walk.end()) {
        walk.insert(next);
        connect(pos, next);
        pos = next;
        next = pos + nextCell(pos, size);
    }
    int loopStart = next;
    bool foundLoop = false;

    for (auto it = walk.begin(); it != walk.end(); ++it) {
        if (*it == loopStart) {
            foundLoop = true;
            break;
        }
    }

    if (foundLoop) {
        for (auto it = walk.begin(); it != walk.end(); ++it) {
            visited[*it] = true;
        }
    }
}

void Maze::createMazeWilson() {
    std::fill_n(visited, size * size, false);
    visited[start] = true;

    for (int i = 0; i < size * size; ++i) {
        if (!visited[i]) {
            loopErasedRandomWalk(i);   
        }
    }
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

bool Maze::solveMazeAStar() {
    resetVisited();
    clearScreen();

    struct Node {
        int pos, cost, estimate;
        bool operator<(const Node& other) const {
            return cost + estimate > other.cost + other.estimate;
        }
    };

    std::priority_queue<Node> openSet;
    openSet.push({start, 0, heuristic(start)});
    path[start] = -1;

    int directions[] = {-size, 1, size, -1}; // Up, Right, Down, Left

    while (!openSet.empty()) {
        Node current = openSet.top();
        openSet.pop();
        int pos = current.pos;

        if (pos == goal) {
            while (pos != start) {
                path[pos] = 1;
                pos = path[pos];
            }
            path[start] = 1;
            renderMaze();
            return true;
        }

        visited[pos] = true;

        for (int dir : directions) {
            int next = pos + dir;
            if (inBounds(next) && !visited[next] && validMove(pos, next, dir)) {
                openSet.push({next, current.cost + 1, heuristic(next)});
                path[next] = pos;
            }
        }

        clearScreen();
        renderMaze();
        sleep_for(milliseconds(50));
    }
    return false;
}

bool Maze::solveMazeRightHandRule() {
    resetVisited();
    clearScreen();

    int pos = start;
    int dir = 1;  
    path[pos] = 1;
    int directions[] = {-size, 1, size, -1}; // Up, Right, Down, Left

    while (pos != goal) {
        int nextDir = (dir + 3) % 4; // Turn right
        int next = pos + directions[nextDir];

        if (inBounds(next) && validMove(pos, next, directions[nextDir])) {
            dir = nextDir;
            pos = next;
        } else if (validMove(pos, pos + directions[dir], dir)) {
            pos += directions[dir]; // Move forward
        } else {
            dir = (dir + 1) % 4; // Turn left
        }

        path[pos] = 1;
        clearScreen();
        renderMaze();
        sleep_for(milliseconds(50));
    }
    renderMaze();
    return true;
}

bool Maze::solveMazeBidirectional() {
    resetVisited();
    clearScreen();

    std::queue<int> forwardQueue, backwardQueue;
    forwardQueue.push(start);
    backwardQueue.push(goal);
    visited[start] = true;
    visited[goal] = true;

    std::unordered_map<int, int> forwardPath, backwardPath;
    forwardPath[start] = -1;
    backwardPath[goal] = -1;

    int directions[] = {-size, 1, size, -1};

    while (!forwardQueue.empty() && !backwardQueue.empty()) {
        if (expandBidirectional(forwardQueue, forwardPath, backwardPath, directions)) {
            renderMaze();
            return true;
        }
        if (expandBidirectional(backwardQueue, backwardPath, forwardPath, directions)) {
            renderMaze();
            return true;
        }

        clearScreen();
        renderMaze();
        sleep_for(milliseconds(50));
    }

    return false;
}

bool Maze::expandBidirectional(std::queue<int>& queue, std::unordered_map<int, int>& path, std::unordered_map<int, int>& otherPath, int directions[]) {
    int pos = queue.front();
    queue.pop();

    for (int i = 0; i < 4; i++) {
        int dir = directions[i]; 
        int next = pos + dir;

        if (inBounds(next) && !visited[next] && validMove(pos, next, dir)) {
            visited[next] = true;
            queue.push(next);
            path[next] = pos;

            if (otherPath.find(next) != otherPath.end()) {
                int meetingPoint = next;
                int path1 = meetingPoint;
                int path2 = otherPath[meetingPoint];
                while (path1 != -1) {
                    path[path1] = 1;
                    path1 = path[path1];
                }
                while (path2 != -1) {
                    path[path2] = 1;
                    path2 = path[path2];
                }
                return true;
            }
        }
    }
    return false;
}

bool Maze::isSolvable() {
    resetVisited();
    return solveMazeDFS();
}

void Maze::renderMaze() {
    for (int i = 0; i < size * size; ++i) {
        if (i % size == 0)
            std::cout << std::endl;
        if (path[i] == 1) {
            std::cout << " O";
        } else if (i == start) {
            std::cout << " S";
        } else if (i == goal) {
            std::cout << " G";
        } else {
            std::cout << " .";
        }
    }
}

void Maze::generateMaze() {
    createMazeWilson();
    resetVisited();
}
