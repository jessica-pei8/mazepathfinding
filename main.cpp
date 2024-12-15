#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>
#include <queue>
#include <stack>

using namespace std::this_thread;
using namespace std::chrono;

const int size = 5; // Maze size (for easier visualization in terminal)
bool visited[size * size];
bool walls[size * size][2] = {0}; // 0 for a wall, 1 for no wall: left -> top

int start = 0;               // Start position
int goal = size * size - 1;  // End position
int path[size * size] = {0}; // Path array
int pos = start;             // Current position

void clearScreen() {
#ifdef _WIN32
    system("cls"); // For Windows
#else
    system("clear"); // For Unix/Linux
#endif
}


// Function to randomly choose the next cell for DFS
int nextCell(int pos, int size) {
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

// Function to remove the wall between two cells
void connect(int pos1, int pos2) {
    if (pos2 > pos1) {
        if (pos2 == pos1 + 1) {
            walls[pos2][0] = 1; // Remove left wall of pos2
        } else {
            walls[pos2][1] = 1; // Remove top wall of pos2
        }
    } else {
        if (pos1 == pos2 + 1) {
            walls[pos1][0] = 1; // Remove left wall of pos1
        } else {
            walls[pos1][1] = 1; // Remove top wall of pos1
        }
    }
}

// Recursive function to create the maze using random DFS
void randomDFS(int pos, int size) {
    visited[pos] = true;
    int next = pos + nextCell(pos, size);

    while (next != pos) {
        connect(pos, next);
        randomDFS(next, size);
        next = pos + nextCell(pos, size);
    }
}


// Function to check if a cell is within bounds
bool inBounds(int pos) {
    return pos >= 0 && pos < size * size;
}


// Function to render the maze in the terminal
void renderMaze() {
    for (int j = 0; j < size; j++) {
        // Render top walls
        for (int i = 0; i < size; i++) {
            std::cout << "+";
            if (walls[i + j * size][1]) {
                std::cout << "   "; // No top wall
            } else {
                std::cout << "---"; // Top wall
            }
        }
        std::cout << "+" << std::endl;

        // Render left walls and cells
        for (int i = 0; i < size; i++) {
            if (walls[i + j * size][0]) {
                std::cout << " "; // No left wall
            } else {
                std::cout << "|"; // Left wall
            }

            if (i + j * size == start) {
                std::cout << " S "; // Start cell
            } else if (i + j * size == goal) {
                std::cout << " G "; // Goal cell
            } else if (path[i + j * size]) {
                std::cout << " * "; // Solution path
            } else {
                std::cout << "   "; // Empty cell
            }
        }
        std::cout << "|" << std::endl;
    }

    // Render bottom wall
    for (int i = 0; i < size; i++) {
        std::cout << "+---";
    }
    std::cout << "+" << std::endl;
}

// Function to solve the maze using depth-first search
void solveMazeDFS() {
    std::stack<int> stack;
    stack.push(start);
    visited[start] = true;
    path[start] = 1;

    int directions[] = {-size, 1, size, -1}; // Top, Right, Bottom, Left

    while (!stack.empty()) {
        int pos = stack.top();
        stack.pop();

        if (pos == goal) {
            break;
        }

        for (int dir : directions) {
            int next = pos + dir;

            // Check if the next cell is within bounds and not visited
            if (inBounds(next) && !visited[next]) {
                if (dir == -size && !walls[pos][1]) { // Moving up (top wall)
                    stack.push(next);
                    visited[next] = true;
                    path[next] = 1;
                } else if (dir == 1 && (pos + 1) % size != 0 && !walls[pos + 1][0]) { // Moving right (left wall)
                    stack.push(next);
                    visited[next] = true;
                    path[next] = 1;
                } else if (dir == size && pos < size * (size - 1) && !walls[pos + size][1]) { // Moving down (bottom wall)
                    stack.push(next);
                    visited[next] = true;
                    path[next] = 1;
                } else if (dir == -1 && pos % size != 0 && !walls[pos][0]) { // Moving left (right wall)
                    stack.push(next);
                    visited[next] = true;
                    path[next] = 1;
                }
            }
        }

        // Delay for visualization
        sleep_for(milliseconds(50));  
        clearScreen();            
        renderMaze();                
}
}


// Function to solve the maze using breadth-first search
void solveMazeBFS() {
    std::queue<int> q;
    q.push(start);
    visited[start] = true;
    path[start] = 1;

    int directions[] = {-size, 1, size, -1}; // Top, Right, Bottom, Left
    int parent[size * size]; // Parent array to track the path
    std::fill_n(parent, size * size, -1); // Initialize parent array with -1

    while (!q.empty()) {
        int pos = q.front();
        q.pop();

        // If goal is reached, break out of the loop
        if (pos == goal) {
            break;
        }

        for (int dir : directions) {
            int next = pos + dir;

            // Check if the next cell is within bounds and not visited
            if (inBounds(next) && !visited[next]) {
                if (dir == -size && !walls[pos][1]) { // Moving up (top wall)
                    q.push(next);
                    visited[next] = true;
                    path[next] = 1;
                    parent[next] = pos;
                } else if (dir == 1 && (pos + 1) % size != 0 && !walls[pos + 1][0]) { // Moving right (left wall)
                    q.push(next);
                    visited[next] = true;
                    path[next] = 1;
                    parent[next] = pos;
                } else if (dir == size && pos < size * (size - 1) && !walls[pos + size][1]) { // Moving down (bottom wall)
                    q.push(next);
                    visited[next] = true;
                    path[next] = 1;
                    parent[next] = pos;
                } else if (dir == -1 && pos % size != 0 && !walls[pos][0]) { // Moving left (right wall)
                    q.push(next);
                    visited[next] = true;
                    path[next] = 1;
                    parent[next] = pos;
                }
            }
        }

        // Delay for visualization
        sleep_for(milliseconds(50)); 
        clearScreen();           
        renderMaze();              
    }

    // Backtrack to find the full path using parent array
    int current = goal;
    while (current != start) {
        path[current] = 1;
        current = parent[current];
    }
    path[start] = 1;

    clearScreen();
    renderMaze();
}

int main() {
    srand(time(0));
    clearScreen();
    randomDFS(start, size);
    std::cout<<"DFS"<<std::endl;
    solveMazeDFS(); 
    std::cout<<"BFS"<<std::endl;
    solveMazeBFS(); 
    return 0;
}
