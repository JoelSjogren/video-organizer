/**********************************
*  args.h                         *
*                                 *
*  Parses and stores command      *
*  line arguments.                *
**********************************/
#pragma once
#include <vector>
#include <string>
typedef long long clean_t;
class Args {
	void checkFiles();
	void expandDirectories();
	int parseInt(std::string str);
	long long parseSize(std::string str);
	void parseSize_err(std::string str);
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
	clean_t clean; // upper bound for cleaning
	bool ask_clean;
	bool shallow_output;
	std::string action_pretty() const;
	std::string human_readable_size(clean_t size) const;
	Args(); // for testing
	Args(int argc, char* const* argv);
};





