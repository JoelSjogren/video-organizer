/**********************************
*   fileman.cpp                   *
**********************************/
#include "fileman.h"
#include "filelist.h"
#include "parser.h"
#include <unistd.h>
#include <sys/stat.h>
#include <iostream>
#include <string>
#include <cassert>
#include <stdexcept>
#include <cerrno>
#include <cstring>
#include <cstdio>
#include <fstream>
using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::runtime_error;
using std::ifstream;
using std::ofstream;
using std::ios_base;
FileMan::FileMan(const Args& pargs) : args(pargs) {}
void FileMan::move(string from, string to, int reg) {
	if (0 < args.verbosity)
		cout << "Moving: " << from << " -> " << to << endl;
	if (args.simulate) return;
	if (exists(to)) {
		if (-2 < args.verbosity)
			cerr << "E: " << strerror(EEXIST) << ": " << to << endl;
		return;
	}
	dig(to);
	int ret = rename(from.c_str(), to.c_str());
	if (ret) {
		if (-2 < args.verbosity)
			cerr << "E: " << strerror(errno) << ": " << to << endl;
		return;
	}
	if (reg) registerAction(from, to);
}
void FileMan::copy(string from, string to, int reg) {
	if (0 < args.verbosity)
		cout << "Copying: " << from << " -> " << to << endl;
	if (args.simulate) return;
	if (exists(to)) {
		if (-2 < args.verbosity)
			cerr << "E: " << strerror(EEXIST) << ": " << to << endl;
		return;
	}
	dig(to);
	try {
		ifstream src(from.c_str());
		ofstream dst(to.c_str());
		dst << src;
	} catch (ios_base::failure e) {
		if (-2 < args.verbosity)
			cerr << "E: " << e.what() << endl;
		return;
	}
	if (reg) registerAction(from, to);
}
void FileMan::link(string from, string to, int reg) {
	if (0 < args.verbosity)
		cout << "Linking: " << from << " -> " << to << endl;
	if (args.simulate) return;
	if (exists(to)) {
		if (-2 < args.verbosity)
			cerr << "E: " << strerror(EEXIST) << ": " << to << endl;
		return;
	}
	dig(to);
	int ret = ::link(from.c_str(), to.c_str());
	if (ret) {
		if (-2 < args.verbosity)
			cerr << "E: " << strerror(errno) << ": " << to << endl;
		return;
	}
	if (reg) registerAction(from, to);
}
void FileMan::action(string from, string to) {
	switch (args.mcl) {
	case Args::MOVE:
		move(from, to);
		break;
	case Args::COPY:
		copy(from, to);
		break;
	case Args::LINK:
		link(from, to);
		break;
	default:
		assert(false);
	}
}
void FileMan::registerAction(string from, string to) {
	FileList list(Parser::directory(to), args);
	list.add(Parser::filename(from), Parser::filename(to));
	list.write();
}
void FileMan::unregisterAction(string dir, const Record& rec) {
	FileList list(dir, args);
	if (list.find(rec.to)) list.remove(rec.to);
	list.write();
}
void FileMan::dig(string to) {
	int lookhere = 0;
	int foundhere;
	while ((foundhere = to.find("/", lookhere)) != string::npos) {
		string part = to.substr(0, foundhere + 1);
		int ret = mkdir(part.c_str(), S_IRWXU | S_IRWXG |
						S_IROTH | S_IXOTH);
		if (ret && errno != EEXIST)
			throw runtime_error("Unable to create directory: " + part);
		lookhere = foundhere + 1;
	}
}
bool FileMan::exists(string file) {
	struct stat buf;
	int ret = stat(file.c_str(), &buf);
	if (ret && errno == ENOENT) return false;
	return true;
}
void FileMan::undo(string dir, const Record& rec) {
	const string full = dir + "/" + rec.to;
	switch (rec.action) {
	case Args::MOVE:
		move(full, args.outdir + "/" + rec.from, false);
		break;
	case Args::COPY:
		remove(full);
		break;
	case Args::LINK:
		remove(full);
		break;
	default:
		assert(false);
	}
	unregisterAction(dir, rec);
}
void FileMan::remove(string file) {
	if (0 < args.verbosity)
		cout << "Removing: " << file << endl;
	if (args.simulate) return;
	int ret = ::remove(file.c_str());
	if (ret) {
		if (-2 < args.verbosity)
			cerr << "E: " << strerror(errno) << ": " << file << endl;
		return;
	}
}
