/**********************************
*  fileman.cpp                    *
**********************************/
#include "fileman.h"
#include "filelist.h"
#include "parser.h"
#include <boost/filesystem.hpp> // filesystem operations
#include <dirent.h>             // directory browsing
#include <sys/stat.h>           // info about files
#include <string>
#include <cassert>
#include <stdexcept>
#include <cerrno>
#include <cstring>
#include <cstdio>
#include <fstream>
using std::string;
using std::runtime_error;
using std::ifstream;
using std::ofstream;
using std::ios_base;
using boost::filesystem::recursive_directory_iterator;
using boost::filesystem::file_size;
FileMan::FileMan(const Args& pargs)
    : args(pargs), console(args.verbosity) {}
void FileMan::move(string from, string to, int reg) {
	console.v("Moving: %s -> %s", from.c_str(), to.c_str());
	if (args.simulate) return;
	if (exists(to)) {
		console.e("%s: %s", strerror(EEXIST), to.c_str());
		return;
	}
	dig(to);
	int ret = rename(from.c_str(), to.c_str());
	if (ret) {
		console.e("%s: %s", strerror(errno), to.c_str());
		return;
	}
	if (reg) registerAction(from, to);
}
void FileMan::copy(string from, string to, int reg) {
	console.v("Copying: %s -> %s", from.c_str(), to.c_str());
	if (args.simulate) return;
	if (exists(to)) {
		console.e("%s: %s", strerror(EEXIST), to.c_str());
		return;
	}
	dig(to);
	try {
		ifstream src(from.c_str());
		ofstream dst(to.c_str());
		dst << src;
	} catch (ios_base::failure e) {
		console.e("%s", e.what());
		return;
	}
	if (reg) registerAction(from, to);
}
void FileMan::link(string from, string to, int reg) {
	console.v("Linking: %s -> %s", from.c_str(), to.c_str());
	if (args.simulate) return;
	if (exists(to)) {
		console.e("%s: %s", strerror(EEXIST), to.c_str());
		return;
	}
	dig(to);
	int ret = ::link(from.c_str(), to.c_str());
	if (ret) {
		console.e("%s: %s", strerror(errno), to.c_str());
		return;
	}
	if (reg) registerAction(from, to);
}
void FileMan::action(string from, string to) {
	switch (args.action) {
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
			throw runtime_error("Unable to create directory: " +
			                    part);
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
	const string full = dir + rec.to;
    console.d("FileMan::undo: %s", full.c_str());
	switch (rec.action) {
	case Args::MOVE:
		move(full, args.outdir + rec.from, false);
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
	console.v("Removing: %s", file.c_str());
	if (args.simulate) return;
	int ret = ::remove(file.c_str());
	if (ret) {
		console.e("%s: %s", strerror(errno), file.c_str());
		return;
	}
}
void FileMan::remove_all(string file) {
    console.v("Removing recursively: %s", file.c_str());
    if (args.simulate) return;
    boost::filesystem::remove_all(file);
}
void FileMan::touch(std::string file) {
    console.v("Touching: %s", file.c_str());
    if (args.simulate) return;
    if (!exists(file)) {
        dig(file);
        ofstream of(file.c_str());
    }
}
int FileMan::fileCount(std::string directory) {
    DIR* dir = opendir(directory.c_str());
    if (dir == NULL)
        throw runtime_error(strerror(errno) + (": " + directory));
    dirent* dent;
    int count = 0;
    errno = 0; // no error
    while (dent = readdir(dir)) count++;
    if (errno != 0)
        throw runtime_error(strerror(errno) + (": " + directory));
    if (closedir(dir))
        throw runtime_error(strerror(errno) + (": " + directory));
    return count;
}
bool FileMan::isEmpty(std::string directory) {
    return fileCount(directory) == 2;
}
bool FileMan::isDirectory(std::string file) {
    using boost::filesystem::is_directory;
    return is_directory(file);
}
/* Alternative implementation
bool FileMan::isDirectory(string path) {
	struct stat buf;
	if (stat(path.c_str(), &buf) != 0) {
		console.f("%s: %s", strerror(errno), path.c_str());
		exit(1);
	}
    return S_ISDIR(buf.st_mode);
}*/
long long FileMan::recursiveSize(string directory) {
    long long result = 0;
    for (recursive_directory_iterator i(directory);
         i != recursive_directory_iterator(); i++) {
        if (!is_directory(*i))
            result += file_size(*i);
    }
    return result;
}

