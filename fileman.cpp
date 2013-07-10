/**********************************
*   fileman.cpp                   *
**********************************/
#include "fileman.h"
#include "filelist.h"
#include "filename.h"
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
	if (args.verbose)
		cout << "Moving: " << from << " -> " << to << endl;
	if (exists(to)) {
		cerr << "E: " << strerror(EEXIST) << ": " << to << endl;
		return;
	}
	dig(to);
	int ret = rename(from.c_str(), to.c_str());
	if (ret) {
		cerr << "E: " << strerror(errno) << ": " << to << endl;
		return;
	}
	if (reg) registerAction(from, to);
}
void FileMan::copy(string from, string to, int reg) {
	if (args.verbose)
		cout << "Copying: " << from << " -> " << to << endl;
	if (exists(to)) {
		cerr << "E: " << strerror(EEXIST) << ": " << to << endl;
		return;
	}
	dig(to);
	try {
		ifstream src(from.c_str());
		ofstream dst(to.c_str());
		dst << src;
	} catch (ios_base::failure e) {
		cerr << "E: " << e.what() << endl;
		return;
	}
	if (reg) registerAction(from, to);
}
void FileMan::link(string from, string to, int reg) {
	if (args.verbose)
		cout << "Linking: " << from << " -> " << to << endl;
	if (exists(to)) {
		cerr << "E: " << strerror(EEXIST) << ": " << to << endl;
		return;
	}
	dig(to);
	int ret = ::link(from.c_str(), to.c_str());
	if (ret) {
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
	FileList list(extractDirectory(to), args);
	list.add(extractFilename(from), extractFilename(to));
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
	int ret = ::remove(file.c_str());
	if (ret) {
		cerr << "E: " << strerror(errno) << ": " << file << endl;
		return;
	}
}
