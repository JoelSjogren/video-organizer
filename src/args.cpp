/**********************************
*  args.cpp                       *
**********************************/
#include "args.h"
#include "help.h"
#include "build_no.h"
#include "parser.h"
#include "ostream_overloads.h"
#include "fileman.h"
#include "console.h"
#include <unistd.h>     // getopt - argument parsing
#include <getopt.h>     // also getopt
#include <string>
#include <vector>
#include <ostream>
#include <iostream>
#include <cstring>
#include <cerrno>
#include <set>
#include <sstream>
#include <cctype>
#include <cassert>
using std::ostream;
using std::cerr;
using std::cout;
using std::endl;
using std::string;
using std::set;
using std::vector;
using std::istringstream;
using std::ostringstream;
using std::ios_base;
void help() {
	for (int i = 0; i < ___HELP_md_len; i++)
		cout << ___HELP_md[i];
	cout << endl;
}
void build_no() {
    for (int i = 0; i < build_no_txt_len; i++)
        cout << build_no_txt[i];
    cout << endl;
}
string Args::action_pretty() const {
    Console console(*this);
	switch (action) {
	case MOVE:
	    return "move";
	case COPY:
	    return "copy";
	case LINK:
	    return "link";
	default:
	    console.f("Invalid Args::Action: %d", action);
	    exit(1);
	}
}
void Args::markDirectories() {
    Console console(*this);
    FileMan fileman(*this);
    for (int i = 0; i < infiles.size(); i++) {
        string& infile = infiles[i];
        bool isdir = fileman.isDirectory(infile);
	    console.d("directory?: %s - %s", infile.c_str(),
                  isdir?"true":"false");
	    if (isdir && *infile.rbegin() != '/')
	        infile += "/";
    }
}
int Args::parseInt(string str) {
	int result;
	istringstream iss(optarg);
    iss >> result;
    if (iss.fail()) {
        Console console(*this);
		console.f("Unable to interpret integer: %s", str.c_str());
		exit(1);
	}
	return result;
}
void Args::parseSize_err(string str) {
    Console console(*this);
    console.f("Unable to interpret size: %s", str.c_str());
    exit(1);
}
long long Args::parseSize(const string orig) {
    string str(orig);
    if (str.size() == 0 || !isdigit(str[0])) parseSize_err(orig);
    string::iterator last = str.end() - 1;
    long long suffix = 1;
    if (isdigit(*last)) {}
    else if (*last == 'K') suffix = (long long) 1024;
    else if (*last == 'M') suffix = (long long) 1024 * 1024;
    else if (*last == 'G') suffix = (long long) 1024 * 1024 * 1024;
    else parseSize_err(orig);
    if (!isdigit(*last)) str.erase(last);
    for (int i = 0; i < str.size(); i++)
        if (!isdigit(str[i]))
            parseSize_err(orig);
    errno = 0;
    long long result = atol(str.c_str());
    if (errno != 0) parseSize_err(orig);
    result *= suffix;
    return result;
}
std::ostream& operator<<(std::ostream& os, const Args& args) {
	os << "Arguments:\n";
	#define ARGS_D(X) os << "  " #X ": " << args.X << endl
    os << std::boolalpha; // prettier output
	ARGS_D(undo);
	ARGS_D(outdir);
	ARGS_D(infiles);
	cout << "  action: " << args.action_pretty() << endl;
	ARGS_D(verbosity);
	ARGS_D(recursive);
	ARGS_D(include_part);
	ARGS_D(clean);
	ARGS_D(ask_clean);
	ARGS_D(shallow_output);
	ARGS_D(manual_name);
	os << std::noboolalpha;
	#undef ARGS_D
	return os;
}
Args::Args()
	: undo(false), outdir("."), action(MOVE), verbosity(0),
	  recursive(false), include_part(false),
	  clean(0), ask_clean(false), shallow_output(false) {}
Args::Args(int argc, char* const* argv)
	: undo(false), outdir("."), action(MOVE), verbosity(0),
	  recursive(false), include_part(false),
	  clean(0), ask_clean(false), shallow_output(false) {
	Console console(*this);
	static const struct option longopts[] = {
		{ "undo",		    no_argument,		NULL, 'u' },
		{ "outdir",	 	    required_argument,	NULL, 'o' },
		{ "move",	 	    no_argument,		NULL, 'm' },
		{ "copy",	 	    no_argument,		NULL, 'c' },
		{ "link",	 	    no_argument,		NULL, 'l' },
		{ "verbosity", 	    required_argument,	NULL, 'v' },
		{ "help",	 	    no_argument,		NULL, 'h' },
		{ "recursive", 	    no_argument,		NULL, 'r' },
		{ "part",   	    no_argument,		NULL, 'p' },
		{ "build-no",  	    no_argument,		NULL, 'b' },
		{ "clean",  	    required_argument,	NULL,  0  },
		{ "ask-clean",      no_argument,    	NULL,  0  },
		{ "shallow-output", no_argument,    	NULL,  0  },
		{ "manual-name",    required_argument,  NULL,  0  },
		{ NULL,		    	0,					NULL,  0  },
	};
	static const char* shortopts = ":uo:mclv:hrspb";
	bool done = false;
	optind = 1;
	while (!done) {
		int index = -1;
		char c = getopt_long(argc, argv, shortopts,
							 longopts, &index);
		switch (c) {
		case 0:
		    if (strcmp(longopts[index].name, "clean") == 0)
		        clean = parseSize(optarg);
		    if (strcmp(longopts[index].name, "ask-clean") == 0)
		        ask_clean = true;
            if (strcmp(longopts[index].name, "shallow-output") == 0)
                shallow_output = true;
            if (strcmp(longopts[index].name, "manual-name") == 0)
                manual_name = optarg;
		    break;
		case 'u':
			undo = true;
			break;
		case 'o':
			outdir = optarg;
			break;
		case 'm':
			action = Args::MOVE;
			break;
		case 'c':
			action = Args::COPY;
			break;
		case 'l':
			action = Args::LINK;
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
		case 'p': 
		    include_part = true;
		    break;
		case 'b':
		    build_no();
		    exit(0);
		case -1:
			done = true;
			break;
		case '?':
			console.f("Got unexpected option: %s\n"
			          "Try `%s --help' for more information.",
			          argv[optind - 1], argv[0]); // FIXME safe?
			exit(1);
		case ':':
		    console.f("Missing option argument for: %s\n"
		              "Try `%s --help' for more information.",
		              argv[optind - 1], argv[0]);
		    exit(1);
		default:
			console.f("Failed to interpret command line "
			          "arguments. getopt returned %d", c);
			exit(1);
		}
	}
//	console.setVerbosity(verbosity);
	// parse input filenames
	for (/*optind is set*/; optind < argc; optind++) {
	    string infile = argv[optind];
		infiles.push_back(infile);
    }
	if (infiles.size() == 0) {
		console.f("Missing file operand.\n"
		          "Try `%s --help' for more information.",
		          argv[0]);
		exit(1);
	}
	if (console.show_d()) cout << *this;
	markDirectories(); // append '/'
	checkFiles();
}
void Args::checkFiles() {
    Console console(*this);
	console.d("Checking files");
	{ // check output dir: must be a dir!
	    FileMan fileman(*this);
		if (!fileman.isDirectory(outdir)) {
			console.f("%s: %s", strerror(ENOTDIR), outdir.c_str());
			exit(1);	
		}
		if (*outdir.rbegin() != '/')
		    outdir += "/";
		console.d("  Accepted outdir: %s", outdir.c_str());
	}
}
string Args::human_readable_size(clean_t size) const {
    const string k("K"), m("M"), g("G");
    ostringstream oss;
    if (size < 1024L) oss << size;
    else if (size < 1024L * 1024) oss << size / 1024 << k;
    else if (size < 1024L * 1024) oss << size / 1024 / 1024 << m;
    else oss << size / 1024 / 1024 / 1024 << g;
    return oss.str();
}


