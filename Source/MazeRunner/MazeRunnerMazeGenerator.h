#pragma once

#include <optional>
#include <vector>
#include <unordered_map>
#include <string>
using namespace std;

// Source: maze.h based on a Ruby Kurskal's algorithm implementation from: https://weblog.jamisbuck.org/2011/1/3/maze-generation-kruskal-s-algorithm

class Tree {
public:
    Tree();
    Tree* root();
    bool connected(Tree* tree);
    void connect(Tree* tree);

private:
    Tree* _parent;
};



constexpr int N = 1;
constexpr int S = 1 << 1;
constexpr int E = 1 << 2;
constexpr int W = 1 << 3;

constexpr int CORNER_LEFT_UP = 1;
constexpr int CORNER_RIGHT_UP = 1 << 1;
constexpr int CORNER_LEFT_DOWN = 1 << 2;
constexpr int CORNER_RIGHT_DOWN = 1 << 3;

class MazeRunnerMazeGenerator {
public:
    static optional<MazeRunnerMazeGenerator> create(int height, int width);
    void kruskals_algorithm();
    void aldous_broder_algorithm();

    pair<pair<int,int>, pair<int,int>> getBeginEnd();
    vector<pair<int,int>> solve(pair<int,int> begin, pair<int,int> end);

    void printMaze(vector<pair<int,int>> solution = {});
    vector<vector<int>> getCornerMap();
    vector<vector<int>> getMazeMap();

private:
    MazeRunnerMazeGenerator(int height, int width);
    vector<pair<int,int>> getNeighbours(pair<int,int> c);
    vector<pair<int,int>> getUnvisitedNeighbours(pair<int,int> c, vector<vector<bool>>& visited);
    void dfs(pair<int,int> begin, pair<int,int> end, vector<pair<int,int>>& stack, vector<vector<bool>>& visited);
    bool hasPassage(vector<vector<int>>& vv, pair<int,int> c, int direction);
    int getWalls(vector<vector<int>>& vv, pair<int,int> c);
    void setMazeMap();

private:
    vector<vector<int>> _edges;
    vector<vector<int>> _grid;
    vector<vector<Tree>> _sets;
    vector<vector<int>> _mazeMap;

    unordered_map<int, int> _dx = { {E, 1}, {W, -1}, {N, 0}, {S, 0} };
    unordered_map<int, int> _dy = { {E, 0}, {W, 0}, {N, -1}, {S, 1} };
    unordered_map<int, int> _opposite = { {E, W}, {W, E}, {N, S}, {S, N} };
    vector<pair<int,int>> _directions = { {0, -1}, {0, 1}, {-1, 0}, {1, 0} };
};
