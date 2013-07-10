/**********************************
*   test.cpp                      *
**********************************/
#include "test.h"
#include <iostream>
#include <string>
using std::cout;
using std::string;
Test::Test(string pname) : name(pname), passed(0) {}
Test::~Test() {
	cout << name << " passed " << passed << " tests.\n";
}
