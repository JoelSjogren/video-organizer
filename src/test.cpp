/**********************************
*   test.cpp                      *
**********************************/
#include "test.h"
#include <iostream>
#include <string>
#include <cassert>
#include <vector>
using std::cout;
using std::string;
using std::vector;
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
void Test::eq(std::vector<string> a, std::vector<string> b,
		std::string astr, std::string bstr,
		std::string file, int line) {
	eq<vector<string>, vector<string> >(a, b, astr, bstr, file, line);
}
void Test::eq(bool a, bool b, string astr, string bstr,
			  string file, int line) {
	eq<bool, bool>(a, b, astr, bstr, file, line);
}
string Test::getdir() {
    return "test/" + name + "/";
}
