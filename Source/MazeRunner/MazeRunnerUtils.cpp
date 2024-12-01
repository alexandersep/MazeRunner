#include "MazeRunnerUtils.h"

vector<int> Utils::flatten(vector<vector<int>>& vv) {
    int height = vv.size();
    int width = vv[0].size();
    if (height <= 0 || width <= 0) {
        return {};
    }
    vector<int> res(height * width);
    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {
            res[(y * width) + x] = vv[y][x];
        }
    }
    return res;
}

void Utils::reverse(string& s) {
    int l = 0;
    int r = s.size() - 1;
    while (l < r) {
        swap(s[l], s[r]);
        l++;
        r--;
    }
}

string Utils::toBinary(int n) {
    if (n == 0) {
        return { '0' };
    }
    string res;
    while (n) {
        int remainder = n % 2;
        res.push_back(remainder + '0');
        n /= 2;
    }
    reverse(res);
    return res;
}