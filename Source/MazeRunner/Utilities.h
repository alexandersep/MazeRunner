#pragma once

#include <sstream>

//...standard class body
class LStream : public std::stringbuf {
protected:
	int sync();
};