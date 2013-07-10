/**********************************
*   organizer.cpp                 *
**********************************/
#include "organizer.h"
#include "filename.h"
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
static const regex series_regex("[.]S[0-9]{2}E[0-9]{2}", regex::extended);
static const size_t series_regex_length = string(".S01E01").size();
static const regex film_regex("[.][0-9]{4}", regex::extended);
static const size_t film_regex_length = string(".2010").size();
Organizer::Organizer(const Args& pargs)
	: args(pargs), fileman(args) {
	if (args.undo) {
		for (int i = 0; i < args.infiles.size(); i++) {
			const string full = args.infiles[i];
			const string fnam = extractFilename(full);
			const string dir = extractDirectory(full);
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
			const string file = extractFilename(full);
			string ext = extractExtension(file);
			if (isFilm(file)) {
				const string film = extractFilmName(file);
				fileman.action(full, args.outdir + "/" + film
							   + "/" + film + ext);
			} else if (isSeries(file)) {
				const string series = extractSeriesName(file);
				const string se = extractSeriesSeason(file);
				const string ep = extractSeriesEpisode(file);
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
string Organizer::extractFilmName(string fnam) {
	for (string::iterator i = fnam.begin(); i < fnam.end(); i++)
		if (regex_match(i, i + film_regex_length, film_regex))
			return dotsToSpaces(string(fnam.begin(), i));
	throw logic_error("extractFilmName failed. fnam: " + fnam);
}
int Organizer::seriesIdentifierStart(string fnam) {
	for (string::iterator i = fnam.begin(); i < fnam.end(); i++)
		if (regex_match(i, i + series_regex_length, series_regex))
			return i - fnam.begin();
	throw logic_error("seriesIdentifierStart failed. fnam: " + fnam);
}
string Organizer::extractSeriesName(string fnam) {
	int i = seriesIdentifierStart(fnam);
	return dotsToSpaces(string(fnam.begin(), fnam.begin() + i));
}
string Organizer::extractSeriesSeason(string fnam) {
	int i = seriesIdentifierStart(fnam);
	static const int off = string(".").size();
	string::iterator start = fnam.begin() + i + off;
	return string(start, start + 3);
}
string Organizer::extractSeriesEpisode(string fnam) {
	int i = seriesIdentifierStart(fnam);
	static const int off = string(".S01").size();
	string::iterator start = fnam.begin() + i + off;
	return string(start, start + 3);
}
string Organizer::extractExtension(string fnam) {
	int i = fnam.rfind(".");
	if (i == string::npos) return "";
	return fnam.substr(i, string::npos);
}
bool Organizer::isSeries(string fnam) {
	// search for .SddEdd. where d are digits
	return regex_search(fnam, series_regex);		
}
bool Organizer::isFilm(string fnam) {
	if (isSeries(fnam)) return false;
	// search for .(dddd). where d are digits
	return regex_search(fnam, film_regex);
}
string Organizer::dotsToSpaces(string str) {
	int lookHere = 0;
	int foundHere;
	while ((foundHere = str.find(".", lookHere)) != string::npos) {
		str.replace(foundHere, 1, " ");
		lookHere = foundHere + 1;
	}
	return str;
}