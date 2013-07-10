/**********************************
*   args.cpp                      *
**********************************/
#include "args.h"
#include "help.h"
#include <sys/stat.h>
#include <cstdlib>
#include <unistd.h> // getopt
#include <getopt.h> // also getopt
#include <string>
#include <vector>
#include <ostream>
#include <iostream>
#include <stdexcept>
#include <cstring>
#include <cerrno>
using std::ostream;
using std::cerr;
using std::cout;
using std::endl;
using std::string;
using std::runtime_error;

void help() {
	for (int i = 0; i < README_md_len; i++)
		cout << README_md[i];
	cout << endl;
}

std::ostream&
operator<<(std::ostream& os, const std::vector<std::string>& vec) {
	os << "[";
	int i;
	for (i = 0; i < vec.size() - 1; i++)
		os << vec[i] << ", ";
	os << vec[i] << "]";
	return os;
}

std::ostream& operator<<(std::ostream& os, const Args& args) {
	os << "Arguments:\n";
	#define ARGS_D(X) os << "  " #X ": " << args.X << std::endl
	ARGS_D(undo);
	ARGS_D(outdir);
	ARGS_D(infiles);
	ARGS_D(mcl);
	ARGS_D(verbose);
	#undef ARGS_D
	return os;
}

Args::Args() : undo(false), outdir("."), mcl(MOVE), verbose(false){}

Args::Args(int argc, char* argv[])
	: undo(false), outdir("."), mcl(MOVE), verbose(false) {
	const struct option longopts[] = {
		{ "undo",	 no_argument,		NULL, 'u' },
		{ "outdir",	 required_argument, NULL, 'o' },
		{ "move",	 no_argument,		NULL, 'm' },
		{ "copy",	 no_argument,		NULL, 'c' },
		{ "link",	 no_argument,		NULL, 'l' },
		{ "verbose", no_argument,		NULL, 'v' }
	};
	bool done = false;
	while (!done) {
		char c = getopt_long(argc, argv, ":uo:mclv",
							 longopts, NULL);
		switch (c) {
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
			verbose = true;
			break;
		case -1:
			done = true;
			break;
		case '?':
			help();
			cerr << "Error: Got unexpected option\n";
			exit(1);
		default:
			cerr << "Failed to interpret command line arguments. "
				 << "Please file a bug report.\n";
			exit(1);
		}
	}
	for (/*optind is set*/; optind < argc; optind++)
		infiles.push_back(string(argv[optind]));
	if (infiles.size() == 0) {
		help();
		cerr << "Error: no input files\n";
		exit(1);
	}
	checkFiles();
	if (verbose) cout << *this << endl;
}

void Args::checkFiles() {
	{ // check output dir
		struct stat buf;
		int ret = stat(outdir.c_str(), &buf);
		if (ret)
			throw runtime_error(strerror(errno) + string(": ")
								+ outdir);
		if (buf.st_mode & S_IFDIR == 0)
			throw runtime_error(strerror(ENOTDIR) + string(": ")
								+ outdir);
	}
	{ // check input files
		for (int i = 0; i < infiles.size(); i++) {
			struct stat buf;
			int ret = stat(infiles[i].c_str(), &buf);
			if (ret)
				throw runtime_error(strerror(errno) + string(": ")
									+ infiles[i]);
			if (buf.st_mode & S_IFDIR) {
				cout << "Omitting directory: " << infiles[i] << endl;
				infiles.erase(infiles.begin() + i);
				i--; // to undo the following i++
			}
		}
	}
}



