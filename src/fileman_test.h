/**********************************
*  fileman_test.h                 *
*                                 *
*  Tests FileMan.                 *
**********************************/
#pragma once
#include "test.h"
#include <string>
class FileManTest : public Test {
    void writeRandom(std::string outfile, size_t bytec);
    void checkRandom(std::string infile, size_t bytec);
public:
	FileManTest();
};
