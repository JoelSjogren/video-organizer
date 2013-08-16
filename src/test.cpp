/**********************************
*  test.cpp                       *
**********************************/
#include "test.h"
#include "args.h"
#include "fileman.h"
#include <iostream>
#include <string>
using std::cout;
using std::endl;
using std::string;
Test::Test(string pname) : name(pname), passed(0) {
    cout << "Testing " << name << "..." << endl;
}
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
