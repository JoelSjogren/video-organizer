/**********************************
*  organizer.h                    *
*                                 *
*  The core of all organization.  *
**********************************/
#pragma once
#include "args.h"
#include "fileman.h"
#include "parser.h"
#include <string>
#include <vector>
class Organizer {
	/* configuration */
	const Args& args;
	
	/* file operations */
	FileMan fileman;
	
	/* filename parsing */
	std::vector<FilmParser*> filmParsers;
	std::vector<SeriesParser*> seriesParsers;
	//  find a parser that is capable of parsing *fnam*
	int findFilmParser(std::string fnam);
	int findSeriesParser(std::string fnam);
	
	/* console output*/
	Console console;
	
	void initParsers();
	// iterate over input files
	void undo();
	void sort();
	// known by filename, not filelist
	static bool isSorted(std::string full);
	friend class OrganizerTest;
public:
	Organizer(const Args& pargs);
	~Organizer();
};



