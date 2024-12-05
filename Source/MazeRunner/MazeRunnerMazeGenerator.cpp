#include "MazeRunnerMazeGenerator.h"

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

optional<MazeRunnerMazeGenerator> MazeRunnerMazeGenerator::create(int height, int width) {
    if (height <= 0 || width <= 0) {
        return nullopt;
    }
    return MazeRunnerMazeGenerator(height, width);
}

MazeRunnerMazeGenerator::MazeRunnerMazeGenerator(int height, int width) {
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

pair<pair<int,int>, pair<int,int>> MazeRunnerMazeGenerator::getBeginEnd() {
    int height = _grid.size();
    int width = _grid[0].size();

    random_device rd;  // Seed for randomness
    mt19937 gen(rd()); // Mersenne Twister RNG

    uniform_int_distribution<> gx(0, width - 1);
    uniform_int_distribution<> gy(0, height - 1);

    int maximum_distance = sqrt((width * width) + (height * height));
    int distance;
    pair<int,int> begin;
    pair<int,int> end;
    const int MAX_ITERATIONS = 10;
    int i = 0;
    do {
        begin = { gx(gen), gy(gen) };
        end = { gx(gen), gy(gen) };
        distance = sqrt(pow(begin.first - end.first, 2) + pow(begin.second - end.second, 2));
        i++;
    } while ((maximum_distance >> 1) >= distance && i < MAX_ITERATIONS);

    return { begin, end };
}

void MazeRunnerMazeGenerator::kruskals_algorithm() {
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
    setMazeMap();
}

void MazeRunnerMazeGenerator::aldous_broder_algorithm() {
    int height = _grid.size();
    int width = _grid[0].size();

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
    setMazeMap();
}

vector<vector<int>> MazeRunnerMazeGenerator::getMazeMap() {
    return _mazeMap;
}

// Function to display the maze
void MazeRunnerMazeGenerator::printMaze(vector<pair<int,int>> solution) {
    set<pair<int,int>> st;
    for (pair<int,int> c : solution) {
        st.insert(c);
    }

    cout << "\x1b[H";
    int height = _grid.size();
    int width = _grid[0].size();
    cout << " " << string(width * 2 - 1, '_') << endl;

    for (int y = 0; y < height; y++) {
        cout << "|"; // Left wall

        for (int x = 0; x < width; x++) {
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

void MazeRunnerMazeGenerator::setMazeMap() {
    int height = _grid.size();
    int width = _grid[0].size();

    vector<vector<int>> mazeMap(height, vector<int>(width, 0));
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int cell = _grid[y][x];
            int walls = 0;
            if (!(cell & N)) walls |= N; // North
            if (!(cell & E)) walls |= E; // East
            if (!(cell & S)) walls |= S; // South
            if (!(cell & W)) walls |= W; // West
            mazeMap[y][x] = walls;
        }
    }
    _mazeMap = mazeMap;
}

int MazeRunnerMazeGenerator::getWalls(vector<vector<int>>& vv, pair<int,int> c) {
    int x = c.first;
    int y = c.second;
    return vv[y][x];
}

bool MazeRunnerMazeGenerator::hasPassage(vector<vector<int>>& vv, pair<int,int> c, int direction) {
    int x = c.first;
    int y = c.second;
    return !(getWalls(vv, c) & direction);
}

vector<vector<int>> MazeRunnerMazeGenerator::getCornerMap() {
    int height = _grid.size();
    int width = _grid[0].size();

    vector<vector<int>> cornerMap(height, vector<int>(width, 0));
    vector<int> cornerMask = { CORNER_LEFT_UP, CORNER_RIGHT_UP, CORNER_LEFT_DOWN, CORNER_RIGHT_DOWN };
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            pair<int,int> c = { x, y };
            int corner = 0;

            vector<int> verticalIndices = { y - 1, y + 1 };
            vector<bool> verticalCondition = { y - 1 >= 0, y + 1 < height };
            vector<int> upDown = { N, S };
            for (int i = 0; i < verticalCondition.size(); i++) {
                if (verticalCondition[i] && hasPassage(_mazeMap, c, upDown[i])) {
                    int verticalIndex = verticalIndices[i];
                    int backwardIndex = upDown.size() - 1 - i;

                    vector<int> horitonzalIndices = { x - 1, x + 1 };
                    vector<bool> horizontalCondition = { x - 1 >= 0, x + 1 < height };
                    vector<int> leftRight = { W, E };
                    for (int j = 0; j < horitonzalIndices.size(); j++) {
                        int currentCell = _mazeMap[verticalIndex][x];
                        int dirCell = _mazeMap[verticalIndex][horitonzalIndices[j]];

                        int dir = currentCell & leftRight[j];
                        int cellDir = 0;
                        if (horizontalCondition[j] && hasPassage(_mazeMap, c, leftRight[j])) {
                            cellDir = dirCell & upDown[backwardIndex];
                        }
                        if (cellDir && dir) {
                            if (upDown[i] == N) {
                                corner |= cornerMask[i + j];
                            }
                            else {
                                corner |= cornerMask[i + j + 1];
                            }
                        }
                    }
                }
            }
            cornerMap[y][x] = corner;
        }
    }
    return cornerMap;
}

pair<int, int> MazeRunnerMazeGenerator::getRandomPair(int xmin, int ymin, int xmax, int ymax) {
    random_device random;
    mt19937 gen(random());

    uniform_int_distribution<int> x(xmin, xmax);
    uniform_int_distribution<int> y(ymin, ymax);

    return { x(gen), y(gen) };
}

pair<int, int> MazeRunnerMazeGenerator::getRandomPair(pair<int, int> min, pair<int, int> max) {
    return getRandomPair(min.first, min.second, max.first, max.second);
}

/*
return: int, 
0 -> empty
1 -> key
2 -> begin
3 -> end
*/
vector<vector<int>> MazeRunnerMazeGenerator::getTileContentsMap(pair<int, int> begin, pair<int, int> end) {
    int height = _grid.size();
    int width = _grid[0].size();
    if (height <= 1 || width <= 1) {
        return {};
    }
    vector<vector<int>> tileContents(height, vector<int>(width, 0)); // no tile contents
    tileContents[begin.second][begin.first] = 2;
    tileContents[end.second][end.first] = 3;
    height--; // to get index range
    width--;

    int divHeight = height / 2;
    int divWidth = width / 2;

    vector<pair<pair<int, int>, pair<int, int>>> gridLocations = {
        {{0, 0}, {divWidth - 1, divHeight - 1}},
        {{divWidth, 0}, {width, divHeight - 1}},
        {{0, divHeight}, {divWidth - 1, height}},
        {{divWidth, divHeight}, {width, height}}
    };

    vector<pair<int, int>> keyLocations;

    for (const pair<pair<int, int>, pair<int, int>> & grid : gridLocations) {
        pair<int, int> keyLocation;
        do {
            keyLocation = getRandomPair(grid.first, grid.second);
        } while (keyLocation == begin || keyLocation == end || find(keyLocations.begin(), keyLocations.end(), keyLocation) != keyLocations.end());
        keyLocations.push_back(keyLocation);
        tileContents[keyLocation.second][keyLocation.first] = 1;
    }

    return tileContents;
}

vector<pair<int,int>> MazeRunnerMazeGenerator::solve(pair<int,int> begin, pair<int,int> end) {
    vector<pair<int,int>> stack;
    int height = _grid.size();
    int width = _grid[0].size();
    vector<vector<bool>> visited(_grid.size(), vector<bool>(_grid[0].size(), false));
    dfs(begin, end, stack, visited);

    return stack;
}

void MazeRunnerMazeGenerator::dfs(pair<int,int> begin, pair<int,int> end, vector<pair<int,int>>& stack, vector<vector<bool>>& visited) {
    if (begin == end) {
        stack.push_back(begin);  // Mark the final cell in the stack
        return;
    }
    if (visited[begin.second][begin.first]) {
        return;
    }

    visited[begin.second][begin.first] = true;
    stack.push_back(begin);

    vector<pair<int,int>> neighbours = getUnvisitedNeighbours(begin, visited);
    for (pair<int,int> neighbour : neighbours) {
        // Recursive depth first search call for each neighbor
        dfs(neighbour, end, stack, visited);

        // If we reached the end, stop trying other neighbors
        if (!stack.empty() && stack.back() == end) {
            return;
        }
    }

    stack.pop_back();
}

vector<pair<int,int>> MazeRunnerMazeGenerator::getNeighbours(pair<int,int> c) {
    vector<pair<int,int>> neighbours;

    int height = _grid.size();
    int width = _grid[0].size();

    int x = c.first;
    int y = c.second;

    if (y - 1 >= 0) {
        neighbours.push_back({ x, y - 1 });
    }
    if (y + 1 < height) {
        neighbours.push_back({ x, y + 1 });
    }
    if (x - 1 >= 0) {
        neighbours.push_back({ x - 1, y });
    }
    if (x + 1 < width) {
        neighbours.push_back({ x + 1, y });
    }

    return neighbours;
}

vector<pair<int,int>> MazeRunnerMazeGenerator::getUnvisitedNeighbours(pair<int,int> c, vector<vector<bool>>& visited) {
    vector<pair<int,int>> unvisitedNeighbours;
    vector<pair<int,int>> neighbours = getNeighbours(c);

    int height = _grid.size();
    int width = _grid[0].size();

    int x = c.first;
    int y = c.second;
    vector<int> wallDirections = { N, S, W, E }; // Directions corresponding to each move

    for (int i = 0; i < _directions.size(); i++) {
        int nx = x + _directions[i].first;
        int ny = y + _directions[i].second;

        // Check if the neighbor is within bounds
        if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
            // Check if there's no wall between current cell and neighbor
            if ((_grid[y][x] & wallDirections[i]) && !visited[ny][nx]) {
                unvisitedNeighbours.push_back({ nx, ny });
            }
        }
    }
    return unvisitedNeighbours;
}
