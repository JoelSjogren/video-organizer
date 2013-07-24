/**********************************
*  args.h                         *
*                                 *
*  Parses and stores command      *
*  line arguments.                *
**********************************/
#pragma once
#include "console.h"
#include <vector>
#include <string>
#include <ostream>
class Args {
	Console console;
	void checkFiles();
	void expandDirectories();
	int parseInt(std::string str);
    bool isDirectory(std::string path);
    void markDirectories();
public:
	bool undo;
	std::string outdir;
	std::vector<std::string> infiles;
	enum Action {
		MOVE, COPY, LINK
	} action;
	int verbosity;
//	bool ls;
	bool recursive;
	bool simulate;
	bool include_part;
	std::string action_pretty() const;
	Args(); // default; for testing
	Args(int argc, char* const* argv);
};





