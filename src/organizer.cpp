/**********************************
*  organizer.cpp                  *
**********************************/
#include "organizer.h"
#include "ostream_overloads.h"
#include "fileman.h"
#include <boost/regex.hpp>      // regex matching
#include <boost/filesystem.hpp> // filesystem operations
#include <cstdlib>
#include <set>
#include <cassert>
using boost::regex_match;
using boost::regex;
using std::string;
using boost::function;
using std::set;
Organizer::Organizer(const Args& pargs)
	: args(pargs), fileman(args), console(args) {
	initParsers();
}
void Organizer::run() {
    assert(args.infiles.size() != 0);
	function<void (Organizer*, string)> action;
	if (args.undo) action = &Organizer::undo;
	else action = &Organizer::sort;
	iterate(action);
}
Organizer::~Organizer() {
	while (filmParsers.size() != 0) {
		delete *filmParsers.rbegin();
		filmParsers.pop_back();
	}
	while (seriesParsers.size() != 0) {
		delete *seriesParsers.rbegin();
		seriesParsers.pop_back();
	}
}
int Organizer::findFilmParser(string fnam) {
	for (unsigned i = 0; i < filmParsers.size(); i++)
		if (filmParsers[i]->matches(fnam))
			return i;
	return -1;
}
int Organizer::findSeriesParser(string fnam) {
	for (unsigned i = 0; i < seriesParsers.size(); i++)
		if (seriesParsers[i]->matches(fnam))
			return i;
	return -1;
}
void Organizer::initParsers() {
	filmParsers.push_back(new FilmParser2);
	filmParsers.push_back(new FilmParser3);
	seriesParsers.push_back(new SeriesParser1);
	seriesParsers.push_back(new SeriesParser2);
	seriesParsers.push_back(new SeriesParser3);
	seriesParsers.push_back(new SeriesParser4);
}
bool Organizer::isValuable(const string directory) {
    for (FileIterator i(directory, args); i; i++) {
        const string full = *i;
        const string fnam = Parser::filename(full);
        if (!fileman.isDirectory(full)) {
            if(0 <= findFilmParser(fnam) ||
               0 <= findSeriesParser(fnam)) {
                return true;
            }
        }
    }
    return false;
}
void Organizer::iterate(function<void (Organizer*, string)> action) {
    set<string> usedDirs;
	for (unsigned i = 0; i < args.infiles.size(); i++) {
	    const string full = args.infiles[i];
		if (!fileman.exists(full)) {
	        console.f("%s: %s", strerror(ENOENT), full.c_str());
	    }
		if (fileman.isDirectory(full)) {
	        if (args.recursive) {
	            usedDirs.insert(full);
	            for (FileIterator j(full, args); j; j++) {
	                string iFull = *j;
	                if (fileman.isDirectory(iFull)) {
	                    if (*iFull.rbegin() != '/') iFull += "/";
	                    usedDirs.insert(iFull);
	                } else {
        	            launch(action, iFull);
        	        }
	            }
	        } else {
	            console.w("Omitting directory: %s", full.c_str());
	        }
	    }
	    else launch(action, full);
	}
	// clean. time complexity seems to be quadratic. improve?
	if (0 < args.clean) {
    	console.v("== Cleaning ==");
	    set<string>::iterator i = usedDirs.begin();
	    unsigned consecutive = 0;
	    while (consecutive != usedDirs.size()) {
	        console.d("Clean?: %s", i->c_str());
	        if (fileman.exists(*i) && shouldClean(*i)) {
                console.d("  yes");
                fileman.remove_all(*i);
                consecutive = 0;
	        } else {
	            console.d("  no");
	        }
	        i++;
	        if (i == usedDirs.end()) i = usedDirs.begin();
	        consecutive++;
	    }
	}
}
void Organizer::launch(function<void (Organizer*, string)> action,
        const string full) {
	{ // omit .part and filelist files
        bool part = !args.include_part &&
                    Parser::extension(full) == ".part";
        bool filelist = Parser::filename(full) ==
                        "filelist";
        if (part || filelist) {
            console.w("Omitting file: %s", full.c_str());
            return;
        }
    }
    action(this, full);
}
void Organizer::undo(const string full) {
	const string fnam = Parser::filename(full);
	const string dir = Parser::directory(full);
    console.d("Undoing: %s", full.c_str());
	console.d("  fnam: %s", fnam.c_str());
	console.d("  dir: %s", dir.c_str());
	if (!fileman.exists(dir + "filelist")) {
	    console.e("Could not find matching filelist for: %s",
	              full.c_str());
	    return;
	}
	FileList list(dir, args);
	if (!list.find(fnam)) {
		console.e("File unknown to filelist: %s", full.c_str());
		return;
    }
	const Record rec = list.get(fnam);
	fileman.undo(dir, rec);
}
bool Organizer::isSorted(string full) {
	const string name = Parser::filenameNoExt(full);
	const regex sorted[] = {
	    regex("CD[0-9]{2}", regex::extended),
	    regex("E[0-9]{2}", regex::extended),
	};
	const int sortedc = sizeof(sorted) / sizeof(*sorted);
	for (int i = 0; i < sortedc; i++)
	    if (regex_match(name, sorted[i]))
	        return true;
	return false;
}
void Organizer::sort(const string full) {
	const string file = Parser::filename(full);
	const string ext = Parser::extension(file);
	if (isSorted(full)) {
	    console.w("Omitting sorted file: %s", full.c_str());
	    return;
	}
	int j;
	if (0 <= (j = findFilmParser(file))) {
		string name;
		if (args.manual_name.size() == 0)
		    name = filmParsers[j]->name(file);
		else name = args.manual_name;
		const string cd = filmParsers[j]->cd(file);
		string to = args.outdir;
		if (!args.shallow_output) to += name + "/";
		to += cd + ext;
		fileman.action(full, to);
	} else if (0 <= (j = findSeriesParser(file))) {
		string name;
		if (args.manual_name.size() == 0)
		    name = seriesParsers[j]->name(file);
		else name = args.manual_name;
		const string se = seriesParsers[j]->season(file);
		const string ep = seriesParsers[j]->episode(file);
		string to = args.outdir;
		if (!args.shallow_output) to += name + "/" + se + "/";
		to += ep + ext;
		fileman.action(full, to);
	} else {
		console.e("Unable to interpret filename: %s",
				  file.c_str());
	}
}
const char* albo(bool a) { return a ? "true" : "false"; }
bool Organizer::shouldClean(const std::string directory) {
    const long long size = fileman.recursiveSize(directory);
    if (args.clean <= size) return false;
    if (isValuable(directory)) return false;
    if (!args.ask_clean) return true;
    { // Show directory tree
        console.ui("Found directory to clean:\n");
        fileman.tree(directory);
        console.ui("size: %s\n", args.human_readable_size(size)
                                 .c_str());
    }
    return console.Yn("Do you want to clean it [Y/n]? ");
}





