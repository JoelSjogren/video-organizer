/**********************************
*   args.cpp                      *
**********************************/
#include "args.h"
#include "help.h"
#include "ostream_overloads.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h> // getopt
#include <getopt.h> // also getopt
#include <dirent.h>
#include <cstdlib>
#include <string>
#include <vector>
#include <ostream>
#include <iostream>
#include <stdexcept>
#include <cstring>
#include <cerrno>
#include <cstring>
#include <set>
#include <cstdio>
#include <sstream>
using std::ostream;
using std::cerr;
using std::cout;
using std::endl;
using std::string;
using std::runtime_error;
using std::set;
using std::vector;
using std::istringstream;
using std::ios_base;
void help() {
	for (int i = 0; i < ___HELP_md_len; i++)
		cout << ___HELP_md[i];
	cout << endl;
}
int Args::parseInt(string str) {
	int result;
	istringstream iss(optarg);
	iss.exceptions(istringstream::failbit |
				   istringstream::badbit);
	try { iss >> result; }
	catch (ios_base::failure) {
		console.f("Unable to interpret integer: %s", str.c_str());
		exit(1);
	}
	return result;
}
std::ostream& operator<<(std::ostream& os, const Args& args) {
	os << "Arguments:\n";
	#define ARGS_D(X) os << "  " #X ": " << args.X << std::endl
	ARGS_D(undo);
	ARGS_D(outdir);
	ARGS_D(infiles);
	ARGS_D(mcl);
	ARGS_D(verbosity);
//	ARGS_D(ls);
	ARGS_D(recursive);
	#undef ARGS_D
	return os;
}
Args::Args()
	: undo(false), outdir("."), mcl(MOVE), verbosity(0),
	  recursive(false), simulate(false) {}
Args::Args(int argc, char* const* argv)
	: undo(false), outdir("."), mcl(MOVE), verbosity(0),
	  recursive(false), simulate(false) {
	static const struct option longopts[] = {
		{ "undo",		no_argument,		NULL, 'u' },
		{ "outdir",	 	required_argument,	NULL, 'o' },
		{ "move",	 	no_argument,		NULL, 'm' },
		{ "copy",	 	no_argument,		NULL, 'c' },
		{ "link",	 	no_argument,		NULL, 'l' },
		{ "verbosity", 	required_argument,	NULL, 'v' },
//		{ "quiet",		no_argument,		NULL, 'q' },
		{ "help",	 	no_argument,		NULL, 'h' },
//		{ "ls",		 	no_argument,		NULL,  0  },
		{ "recursive", 	no_argument,		NULL, 'r' },
		{ "simulate",	no_argument,		NULL, 's' },
		{ NULL,			0,					NULL,  0  },
	};
	static const char* shortopts = ":uo:mclv:hrs";
	bool done = false;
	optind = 1;
	while (!done) {
		int index = -1;
		char c = getopt_long(argc, argv, shortopts,
							 longopts, &index);
		switch (c) {
//		case 0:
//			if (strcmp("ls", longopts[index].name) == 0)
//				ls = true;
//			break;
		case 'u':
			undo = true;
			break;
		case 'o':
			outdir = optarg;
			break;
		case 'm':
			mcl = Args::MOVE;
			break;
		case 'c':
			mcl = Args::COPY;
			break;
		case 'l':
			mcl = Args::LINK;
			break;
		case 'v':
			verbosity = parseInt(optarg);
			break;
		case 'h':
			help();
			exit(0);
		case 'r':
			recursive = true;
			break;
		case 's':
			simulate = true;
			break;
		case -1:
			done = true;
			break;
		case '?':
			help();
			console.f("Got unexpected option: %s", argv[optind]);
			exit(1);
		default:
			console.f("Failed to interpret command line arguments. "
				 	  "getopt returned %d", c);
			exit(1);
		}
	}
	console.setVerbosity(verbosity);
	for (/*optind is set*/; optind < argc; optind++)
		infiles.push_back(string(argv[optind]));
	if (infiles.size() == 0) {
		console.f("missing file operand.\n"
		          "Try `%s --help' for more information.",
		          argv[0]);
		exit(1);
	}
	if (0 < verbosity) cout << *this << endl;
	if (recursive) expandDirectories();
	checkFiles();
}
void::Args::expandDirectories() {
	set<ino_t> expanded; // infinite looping is avoided by allowing
						 // each inode to be expanded only once
	vector<string>::iterator i = infiles.begin();
	while (i < infiles.end()) {
		struct stat buf;
		if (stat(i->c_str(), &buf) != 0) {
			console.e("%s: %s", strerror(errno), i->c_str());
			infiles.erase(i);
			continue;
		}
		if (S_ISDIR(buf.st_mode)) {
			const string exp = *i;
			console.v("Expanding directory: %s", exp.c_str());
			i = infiles.erase(i); // i will be replaced by its contents
			DIR* dir = opendir(exp.c_str());
			if (dir == NULL) {
				console.e("%s: %s", strerror(errno), exp.c_str());
				continue;
			}
			dirent* dent;
			while (dent = readdir(dir)) {
				{ // ignore . and ..
					if (strcmp(dent->d_name, ".") == 0 ||
						strcmp(dent->d_name, "..") == 0)
						continue;
				}
				{ // avoid infinite looping
					if (expanded.count(dent->d_fileno) == 0) {
						expanded.insert(dent->d_fileno);
						string full(exp + dent->d_name);
						if (dent->d_type == DT_DIR) full += "/";
						i = infiles.insert(i, full);
						console.d("Expanded to: %s", full.c_str());
					}
				}
			}
			closedir(dir);
		} else {
			i++;
		}
	}
}
void Args::checkFiles() {
	console.v("Checking files");
	{ // check output dir
		struct stat buf;
		int ret = stat(outdir.c_str(), &buf);
		if (ret) {
			console.f("%s: %s", strerror(errno), outdir.c_str());
			exit(1);	
		}
		if (buf.st_mode & S_IFDIR == 0) {
			console.f("%s: %s", strerror(ENOTDIR), outdir.c_str());
			exit(1);	
		}
		if (*outdir.rbegin() != '/')
		    outdir += "/";
	}
	{ // check input files
		for (int i = 0; i < infiles.size(); i++) {
			struct stat buf;
			int ret = stat(infiles[i].c_str(), &buf);
			if (ret) {
				console.f("%s: %s", strerror(errno),
						  infiles[i].c_str());
				exit(1);
			}
			if (buf.st_mode & S_IFDIR) {
				console.w("Omitting directory: %s",
						  infiles[i].c_str());
				infiles.erase(infiles.begin() + i);
				i--; // to undo the next i++
			}
		}
	}
}



