#pragma once

#include <string>
#include <vector>

using namespace std;

class Utils {
public:
    static vector<int> flatten(vector<vector<int>>& vv);
    static vector<bool> flatten(vector<vector<bool>>& vv);
    static void reverse(string& s);
    static string toBinary(int n);
    Utils() = delete; // Disallow creating an instance of this object
};