/**********************************
*  organizer.cpp                  *
**********************************/
#include "organizer.h"
#include <boost/regex.hpp>
using boost::regex_match;
using boost::regex;
using std::string;
Organizer::Organizer(const Args& pargs)
	: args(pargs), fileman(args), console(args.verbosity) {
	initParsers();
	if (args.undo) undo();
	else sort();
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
void Organizer::undo() {
	for (int i = 0; i < args.infiles.size(); i++) {
		// e.g. "somedir/CD01.mp4"
		const string full = args.infiles[i];
		const string fnam = Parser::filename(full);
		const string dir = Parser::directory(full);
	    console.d("Undoing: %s", full.c_str());
		console.d("  fnam: %s", fnam.c_str());
		console.d("  dir: %s", dir.c_str());
		if (!fileman.exists(dir + "filelist")) {
		    console.e("Could not find matching filelist for: %s",
		              full.c_str());
		    continue;
		}
		FileList list(dir, args);
		if (!list.find(fnam)) {
			console.e("File unknown to filelist: %s", full.c_str());
			continue;
	    }
		const Record rec = list.get(fnam);
		fileman.undo(dir, rec);
	}
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
void Organizer::sort() {
	for (int i = 0; i < args.infiles.size(); i++) {
		// e.g. "somedir/Film.2010.mp4"
		const string full = args.infiles[i];
		const string file = Parser::filename(full);
		const string ext = Parser::extension(file);
		if (isSorted(full)) {
		    console.w("Omitting file: %s", full.c_str());
		    continue;
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
}




