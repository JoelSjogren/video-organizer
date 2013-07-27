/**********************************
*  test.cpp                       *
**********************************/
#include "test.h"
#include "args.h"
#include "fileman.h"
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
string Test::getdir() {
    string result = "test/" + name + "/";
    Args args;
    FileMan fileman(args);
    fileman.dig(result);
    return result;
}
