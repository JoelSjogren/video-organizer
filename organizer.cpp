/**********************************
*   organizer.cpp                 *
**********************************/
#include "organizer.h"
#include <fstream>
#include <cassert>
#include <stdexcept>
#include <iostream>
#include <sstream>
#include <iomanip>
using boost::regex_search;
using boost::regex_match;
using boost::regex;
using std::logic_error;
using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::stringstream;
using std::setfill;
using std::setw;
Organizer::Organizer(const Args& pargs)
	: args(pargs), fileman(args) {
	// init filename Parsers
	filmParsers.push_back(new FilmParser1);
	filmParsers.push_back(new FilmParser2);
	filmParsers.push_back(new FilmParser3);
	seriesParsers.push_back(new SeriesParser1);
	seriesParsers.push_back(new SeriesParser2);
	// iterate over input files
	if (args.undo) {
		for (int i = 0; i < args.infiles.size(); i++) {
			const string full = args.infiles[i];
			const string fnam = Parser::filename(full);
			const string dir = Parser::directory(full);
			if (fnam == "filelist") {
				// undo the whole list
				const FileList list(dir, args);
				for (int j = 0; j < list.size(); j++) {
					const Record rec = list[j];
					fileman.undo(dir, rec);
				}
			} else if (FileMan::exists(dir + "/filelist")) {
				// undo only fnam
				FileList list(dir, args);
				if (list.find(fnam)) {
					const Record rec = list.get(fnam);
					fileman.undo(dir, rec);
				} else {
					cerr << "E: could not find a filelist containing "
							"information on how to undo: "
						 << full << endl;
				}
			}
		}
	} else {
		for (int i = 0; i < args.infiles.size(); i++) {
			// e.g. "somedir/Film.2010.mp4"
			const string full = args.infiles[i];
			const string file = Parser::filename(full);
			string ext = Parser::extension(file);
			int j;
			if (0 <= (j = findFilmParser(file))) {
				const string film = filmParsers[j]->name(file);
				fileman.action(full, args.outdir + "/" + film
							   + "/" + film + ext);
			} else if (0 <= (j = findSeriesParser(file))) {
				const string series = seriesParsers[j]->name(file);
				const string se = seriesParsers[j]->season(file);
				const string ep = seriesParsers[j]->episode(file);
				const string to = args.outdir + "/" + series + "/"
								+ se + "/" + ep + ext;
				fileman.action(full, to);
			} else {
				cerr << "E: Unable to interpret filename: " << file
					 << endl;
			}
		}
	}
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
/*
bool Organizer::isSeries(string fnam) {
	// search for ".SddEdd." where d are digits
	if (regex_search(fnam, series_regex1) return true;
	// search for " - ddxdd" where d are digits
	if (regex_search(fnam, series_regex2) return true;
	return false;
}*/




