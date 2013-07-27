/**********************************
*  test.h                         *
*                                 *
*  Base test class for others to  *
*  inherit from.                  *
**********************************/
#pragma once
#include "ostream_overloads.h"
#include <string>
#include <cstdlib>
#include <iostream>
#define EQ(X, Y) eq((X), (Y), #X, #Y, __FILE__, __LINE__); passed++
#define TR(X) assert((X)); passed++
class Test {
	std::string name;
protected:
	int passed;
	template <class A, class B>
	void eq(A a, B b, std::string astr, std::string bstr,
			std::string file, int line);
	// get directory for data storage
	std::string getdir();
public:
	Test(std::string name);
	~Test();
};
template <class A, class B>
void Test::eq(A a, B b, std::string astr, std::string bstr,
			  std::string file, int line) {
	if (a != b) {
		std::cout << "Test failed in " << file << ", line " << line
		          << ": EQ(" << astr << ", " << bstr << ") -> "
				  << "assert(" << a << " == " << b << ")\n";
		exit(1);
	}
}
