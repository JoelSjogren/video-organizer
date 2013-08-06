/**********************************
*  organizer.cpp                  *
**********************************/
#include "organizer.h"
#include <boost/regex.hpp>
#include <boost/filesystem.hpp>
#include <cstdlib>
#include <set>
#include <cassert>
using boost::regex_match;
using boost::regex;
using std::string;
using boost::filesystem::recursive_directory_iterator;
using boost::filesystem::is_directory;
using boost::filesystem::exists;
using boost::filesystem::file_size;
using boost::function;
using std::set;
Organizer::Organizer(const Args& pargs)
	: args(pargs), fileman(args), console(args.verbosity) {
	initParsers();
	assert(args.infiles.size() != 0);
}
void Organizer::run() {
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
	for (int i = 0; i < filmParsers.size(); i++)
		if (filmParsers[i]->matches(fnam))
			return i;
	return -1;
}
int Organizer::findSeriesParser(string fnam) {
	for (int i = 0; i < seriesParsers.size(); i++)
		if (seriesParsers[i]->matches(fnam))
			return i;
	return -1;
}
void Organizer::initParsers() {
	filmParsers.push_back(new FilmParser2);
	filmParsers.push_back(new FilmParser3);
	seriesParsers.push_back(new SeriesParser1);
	seriesParsers.push_back(new SeriesParser2);
}
bool Organizer::isValuable(const string directory) {
    typedef recursive_directory_iterator RecDirIt;
    for (RecDirIt i(directory); i != RecDirIt(); i++) {
        const string full = i->path().string();
        const string fnam = Parser::filename(full);
        if (!is_directory(*i)) {
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
	for (int i = 0; i < args.infiles.size(); i++) {
	    const string full = args.infiles[i];
		if (!exists(full)) {
	        console.f("%s: %s", strerror(ENOENT), full.c_str());
	        exit(1);
	    }
		if (is_directory(full)) {
	        if (args.recursive) {
	            usedDirs.insert(full);
	            typedef recursive_directory_iterator RecDirIt;
	            for (RecDirIt j(full); j != RecDirIt(); j++) {
	                string iFull = j->path().string();
	                if (is_directory(*j)) {
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
	// clean
	if (0 < usedDirs.size()) {
	    set<string>::iterator i = usedDirs.begin();
	    set<string>::iterator j = i;
	    j++;
	    if (j == usedDirs.end()) j = usedDirs.begin();
	    // Loop until all directories have been considered
	    // consecutively without any being removed.
	    // Maybe this should be replaced by a tree-like approach?
	    while (i != j) {
	        console.d("Remove?: %s", j->c_str());
	        if (exists(*j)) {
	            console.d("  exists");
	            long long size = fileman.recursiveSize(*j);
                console.d("size: %d, clean: %d", size, args.clean);
	            if (size < args.clean) {
	                if (isValuable(*j)) {
	                    console.w("Not removing directory because "
	                              "it contains files that may be "
	                              "sorted: %s", j->c_str());
	                } else {
	                    fileman.remove_all(*j);
	                    i = j;
	                    console.d("  yes");
	                }
	            }
	        }
	        j++;
	        if (j == usedDirs.end()) j = usedDirs.begin();
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
/*void Organizer::undo() {
	for (int i = 0; i < args.infiles.size(); i++) {
	    const string full = args.infiles[i];
		if (!exists(full)) {
	        console.f("%s: %s", strerror(ENOENT), full.c_str());
	        exit(1);
	    }
		if (is_directory(full)) {
	        if (args.recursive) {
	            typedef recursive_directory_iterator RecDirIt;
	            for (RecDirIt j(full); j != RecDirIt(); j++) {
	                if (!is_directory(*j)) {
        	            undo(j->path().string());
        	        }
	            }
	        } else {
	            console.w("Omitting directory: %s", full.c_str());
	        }
	    }
	    else undo(full);
	}
}*/
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
/*void Organizer::sort() {
	for (int i = 0; i < args.infiles.size(); i++) {
	    const string full = args.infiles[i];
	    if (!exists(full)) {
	        console.f("%s: %s", strerror(ENOENT), full.c_str());
	        exit(1);
	    }
	    if (is_directory(full)) {
	        if (args.recursive) {
	            typedef recursive_directory_iterator RecDirIt;
	            for (RecDirIt j(full); j != RecDirIt(); j++) {
	                if (!is_directory(*j)) {
        	            sort(j->path().string());
        	        }
	            }
	        } else {
	            console.w("Omitting directory: %s", full.c_str());
	        }
	    }
	    else sort(full);
	}
}*/
void Organizer::sort(const string full) {
	const string file = Parser::filename(full);
	const string ext = Parser::extension(file);
	if (isSorted(full)) {
	    console.w("Omitting sorted file: %s", full.c_str());
	    return;
	}
	int j;
	if (0 <= (j = findFilmParser(file))) {
		const string film = filmParsers[j]->name(file);
		const string cd = filmParsers[j]->cd(file);
		const string to = args.outdir + film + "/" + cd + ext;
		fileman.action(full, to);
	} else if (0 <= (j = findSeriesParser(file))) {
		const string series = seriesParsers[j]->name(file);
		const string se = seriesParsers[j]->season(file);
		const string ep = seriesParsers[j]->episode(file);
		const string to = args.outdir + series + "/"
					      + se + "/" + ep + ext;
		fileman.action(full, to);
	} else {
		console.e("Unable to interpret filename: %s",
				  file.c_str());
	}
}

