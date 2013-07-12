/**********************************
*   test.cpp                      *
**********************************/
#include "test.h"
#include <iostream>
#include <string>
#include <cassert>
using std::cout;
using std::string;
Test::Test(string pname) : name(pname), passed(0) {}
Test::~Test() {
	cout << name << " passed " << passed << " tests.\n";
}
void Test::eq(int a, int b, string astr, string bstr,
			  string file, int line) {
	eq<int, int>(a, b, astr, bstr, file, line);
}
void Test::eq(string a, string b, string astr, string bstr,
			  string file, int line) {
	eq<string, string>(a, b, astr, bstr, file, line);
}
