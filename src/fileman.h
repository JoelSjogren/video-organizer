/**********************************
*  fileman.h                      *
*                                 *
*  Handles basic file             *
*  operations.                    *
**********************************/
#pragma once
#include "args.h"
#include "filelist.h"
#include <string>
class FileMan {
	const Args& args;
	Console console;
	// register action in a filelist
	void registerAction(std::string from, std::string to);
	void unregisterAction(std::string dir, const Record& rec);
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
	// remove recursively
	void remove_all(std::string file);
	static bool exists(std::string file);
	// create directory structure so that *to* may be created
	// if *to* is a directory (ends with '/'), create it as well
	void dig(std::string to);
	// make sure file exists
	void touch(std::string file);
	bool isEmpty(std::string directory);
};


