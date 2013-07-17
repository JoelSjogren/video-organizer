/**********************************
*   args_test.h                   *
*                                 *
*   Tests Args.                   *
**********************************/
#pragma once
#include "test.h"
#include "args.h"
#include <string>
class ArgsTest : public Test {
	using Test::eq; // nifty
	void eq(const Args& a, const Args& b, std::string astr,
			std::string bstr, std::string file, int line);
public:
	ArgsTest();
};
