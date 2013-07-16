/**********************************
*   fileman.h                     *
*                                 *
*   Handles basic file            *
*   operations.                   *
**********************************/
#include "args.h"
#include "filelist.h"
#include <string>
class FileMan {
	const Args& args;
	Console console;
	static std::string ectractDir(std::string filepath);
	// register action in a filelist
	void registerAction(std::string from, std::string to);
	void unregisterAction(std::string dir, const Record& rec);
	// create directory structure so that *to* may be created
	void dig(std::string to);
public:
	FileMan(const Args& pargs);
	// move, copy or link, as set in args
	void action(std::string from, std::string to);
	void undo(std::string dir, const Record& rec);
	// reg - whether to call registerAction on success
	void move(std::string from, std::string to, int reg=true);
	void copy(std::string from, std::string to, int reg=true);
	// create hard link
	void link(std::string from, std::string to, int reg=true);
	void remove(std::string file);
	static bool exists(std::string file);
};


