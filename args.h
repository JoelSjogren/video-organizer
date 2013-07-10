/**********************************
*   args.h                        *
*                                 *
*   Parses and stores command     *
*   line arguments.               *
**********************************/
#pragma once
#include <vector>
#include <string>
#include <ostream>
class Args {
	void checkFiles();
public:
	bool undo;
	std::string outdir;
	std::vector<std::string> infiles;
	enum MCL {
		MOVE, COPY, LINK
	} mcl;
	bool verbose;
	Args(); // default; for testing
	Args(int argc, char* argv[]);
	friend std::ostream& operator<<(std::ostream& os, const Args& args);
};

//TODO: rename mcl to action
