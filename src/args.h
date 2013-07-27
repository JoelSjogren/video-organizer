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
    	// Values, e.g. MOVE = 0, must not change!
		MOVE, COPY, LINK
	} action;
	int verbosity;
	bool recursive;
	bool simulate;
	bool include_part;
	std::string action_pretty() const;
	Args(); // for testing
	Args(int argc, char* const* argv);
};





