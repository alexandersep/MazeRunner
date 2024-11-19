#include "Test.h"

#include <iostream>

using namespace std;

Test::Test(int x) {
	_x = x;
	cout << "Test: " << _x << endl;
}

int Test::getX() {
	return _x;
}