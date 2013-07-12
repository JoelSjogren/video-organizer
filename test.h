/**********************************
*   test.h                        *
*                                 *
*   Base test class for others to *
*   inherit from.                 *
**********************************/
#pragma once
#include <cassert>
#include <string>
#include <cstdlib>
#include <iostream>
#define EQ(X, Y) eq((X), (Y), #X, #Y, __FILE__, __LINE__); passed++
class Test {
	std::string name;
protected:
	int passed;
	template <class A, class B>
	void eq(A a, B b, std::string astr, std::string bstr,
			std::string file, int line);
	void eq(int a, int b, std::string astr, std::string bstr,
			std::string file, int line);
	void eq(std::string a, std::string b, std::string astr,
			std::string bstr, std::string file, int line);
public:
	Test(std::string name);
	~Test();
};
template <class A, class B>
void Test::eq(A a, B b, std::string astr, std::string bstr,
			  std::string file, int line) {
	if (a != b) {
		std::cout << "Test failed in " << file << ", line " << line << ": "
				  << "EQ(" << astr << ", " << bstr << ") -> "
				  << "assert(" << a << " == " << b << ")\n";
		exit(1);
	}
}
