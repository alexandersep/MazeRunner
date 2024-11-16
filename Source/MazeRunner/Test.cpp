#include "Test.h"

using namespace std;

Test::Test(int x) {
	_x = x;
	UE_LOG(LogTemp, Warning, TEXT("Test: %d"), _x);
}

int Test::getX() {
	return _x;
}