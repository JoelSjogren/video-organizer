/**********************************
*  testsuite.cpp                  *
*                                 *
*  Runs all tests and crashes if  *
*  one fails.                     * 
**********************************/
#include "parser_test.h"
#include "filelist_test.h"
#include "args_test.h"
#include "organizer_test.h"
#include "console.h"
int main() {
	ParserTest();
	FileListTest();
	ArgsTest();
	OrganizerTest();
}
