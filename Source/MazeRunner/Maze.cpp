#include "maze.h"

#include <unordered_map>
#include <algorithm>
#include <iostream>
#include <string>
#include <random>
#include <set>

// Source: maze.cpp based on a Ruby Kurskal's algorithm implementation from: https://weblog.jamisbuck.org/2011/1/3/maze-generation-kruskal-s-algorithm

Tree::Tree() : _parent(nullptr) {
}

// Method to find the root of the tree recursively
Tree* Tree::root() {
    return _parent ? _parent->root() : this;
}

// Method to check if two trees are connected
bool Tree::connected(Tree* tree) {
    return root() == tree->root();
}

// Method to connect two trees
void Tree::connect(Tree* tree) {
    tree->root()->_parent = this;
}

Maze::Maze(int width, int height) {
    _grid = vector<vector<int>>(height, vector<int>(width, 0));
    _sets = vector<vector<Tree>>(height, vector<Tree>(width, Tree()));
    _edges = vector<vector<int>>();

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (y > 0) {
                _edges.push_back({ x, y, N });
            }
            if (x > 0) {
                _edges.push_back({ x, y, W });
            }
        }
    }

    random_device rd;
    mt19937 g(rd());
    shuffle(_edges.begin(), _edges.end(), g);
}

pair<Cell, Cell> Maze::getBeginEnd() {
    int width = _grid[0].size();
    int height = _grid.size();

    random_device rd;  // Seed for randomness
    mt19937 gen(rd()); // Mersenne Twister RNG

    uniform_int_distribution<> gx(0, width - 1);
    uniform_int_distribution<> gy(0, height - 1);

    int maximum_distance = sqrt((width * width) + (height * height));
    int distance;
    Cell begin;
    Cell end;
    do {
        begin = { gx(gen), gy(gen) };
        end = { gx(gen), gy(gen) };
        distance = sqrt(pow(begin.first - end.first, 2) + pow(begin.second - end.second, 2));
    } while ((maximum_distance >> 1) >= distance);

    return { begin, end };
}

void Maze::kruskals_algorithm() {
    while (!_edges.empty()) {
        int x = _edges.back()[0];
        int y = _edges.back()[1];
        int direction = _edges.back()[2];
        _edges.pop_back();

        int nx = x + _dx[direction];
        int ny = y + _dy[direction];

        Tree& set1 = _sets[y][x];
        Tree& set2 = _sets[ny][nx];

        if (!set1.connected(&set2)) {
            set1.connect(&set2);
            _grid[y][x] |= direction;
            _grid[ny][nx] |= _opposite[direction];
        }
    }
}

void Maze::aldous_broder_algorithm() {
    int height = _grid.size();
    int width = _grid[0].size();

    // 1. Pick a random cell as the current cell and mark it as visited
    int x = rand() % width;
    int y = rand() % height;

    int remaining = width * height;

    vector<int> directions = { N, S, E, W };
    random_device rd;
    while (remaining > 1) {
        mt19937 g(rd());
        shuffle(directions.begin(), directions.end(), g);

        for (int dir : directions) {
            int nx = x + _dx[dir];
            int ny = y + _dy[dir];

            if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
                if (_grid[ny][nx] == 0) {
                    _grid[y][x] |= dir;
                    _grid[ny][nx] |= _opposite[dir];
                    remaining--;
                }
                x = nx;
                y = ny;
                break;
            }
        }
    }
}

// Function to display the maze
void Maze::printMaze(vector<Cell> solution) {
    set<Cell> st;
    for (Cell c : solution) {
        st.insert(c);
    }

    cout << "\x1b[H";
    int width = _grid[0].size();
    cout << " " << string(width * 2 - 1, '_') << endl;

    for (size_t y = 0; y < _grid.size(); ++y) {
        cout << "|"; // Left wall

        for (size_t x = 0; x < _grid[y].size(); ++x) {
            int cell = _grid[y][x];
            bool inSet = false;
            if (st.find({ x, y }) != st.end()) {
                cout << "\x1b[41m";
                inSet = true;
            }
            cout << ((cell & S) ? " " : "_");

            // Check if there's a passage to the East
            if (cell & E) {
                cout << (((cell | _grid[y][x + 1]) & S) ? ' ' : '_');
            }
            else {
                cout << "|";
            }
            if (inSet) {
                cout << "\x1b[0m";
            }
        }
        cout << endl;
    }
}

vector<Cell> Maze::solve(Cell begin, Cell end) {
    vector<Cell> stack;
    vector<vector<bool>> visited(_grid.size(), vector<bool>(_grid[0].size(), false));
    dfs(begin, end, stack, visited);

    return stack;
}

void Maze::dfs(Cell begin, Cell end, vector<Cell>& stack, vector<vector<bool>>& visited) {
    if (begin == end) {
        stack.push_back(begin);  // Mark the final cell in the stack
        return;
    }
    if (visited[begin.second][begin.first]) {
        return;
    }

    visited[begin.second][begin.first] = true;
    stack.push_back(begin);

    vector<Cell> neighbours = getUnvisitedNeighbours(begin, visited);
    for (Cell neighbour : neighbours) {
        // Recursive depth first search call for each neighbor
        dfs(neighbour, end, stack, visited);

        // If we reached the end, stop trying other neighbors
        if (!stack.empty() && stack.back() == end) {
            return;
        }
    }

    stack.pop_back();
}

vector<Cell> Maze::getNeighbours(Cell c) {
    vector<Cell> neighbours;

    int height = _grid.size();
    int width = _grid[0].size();

    int x = c.first;
    int y = c.second;

    if (y - 1 > 0) {
        neighbours.push_back({ x, y - 1 });
    }
    if (y + 1 < height) {
        neighbours.push_back({ x, y + 1 });
    }
    if (x - 1 > 0) {
        neighbours.push_back({ x - 1, y });
    }
    if (x + 1 < width) {
        neighbours.push_back({ x + 1, y });
    }

    return neighbours;
}

vector<Cell> Maze::getUnvisitedNeighbours(Cell c, vector<vector<bool>>& visited) {
    vector<Cell> unvisitedNeighbours;
    vector<Cell> neighbours = getNeighbours(c);

    int x = c.first;
    int y = c.second;
    vector<int> wallDirections = { N, S, W, E }; // Directions corresponding to each move

    for (int i = 0; i < _directions.size(); i++) {
        int nx = x + _directions[i].first;
        int ny = y + _directions[i].second;

        // Check if the neighbor is within bounds
        if (nx >= 0 && nx < _grid[0].size() && ny >= 0 && ny < _grid.size()) {
            // Check if there's no wall between current cell and neighbor
            if ((_grid[y][x] & wallDirections[i]) && !visited[ny][nx]) {
                unvisitedNeighbours.push_back({ nx, ny });
            }
        }
    }
    return unvisitedNeighbours;
}