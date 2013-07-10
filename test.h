/**********************************
*   test.h                        *
*                                 *
*   Base test class for others to *
*   inherit from.                 *
**********************************/
#pragma once
#include <cassert>
#include <string>
#define EQ(X, Y) assert((X) == (Y)); passed++
class Test {
	std::string name;
protected:
	int passed;
public:
	Test(std::string name);
	~Test();
};
