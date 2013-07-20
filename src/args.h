/**********************************
*   args.h                        *
*                                 *
*   Parses and stores command     *
*   line arguments.               *
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
public:
	bool undo;
	std::string outdir;
	std::vector<std::string> infiles;
	enum MCL {
		MOVE, COPY, LINK
	} mcl;
	int verbosity;
//	bool ls;
	bool recursive;
	bool simulate;
	bool include_part;
	Args(); // default; for testing
	Args(int argc, char* const* argv);
};

//TODO: rename mcl to action
