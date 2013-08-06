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
#include <boost/function.hpp>
class Organizer {
	/* configuration */
	const Args args;
	
	/* file operations */
	FileMan fileman;
	
	/* filename parsing */
	std::vector<FilmParser*> filmParsers;
	std::vector<SeriesParser*> seriesParsers;
	// find a parser that is capable of parsing *fnam*
	// returns index
	int findFilmParser(std::string fnam);
	int findSeriesParser(std::string fnam);
	
	/* console output*/
	Console console;
	
	void initParsers();
	// iterate over input files
	void iterate(boost::function<void (Organizer*,
	             std::string)> action);
	void launch(boost::function<void (Organizer*,
	            std::string)> action, std::string full);
//	void undo();
	// e.g. "somedir/CD01.mp4"
	// a directory is valuable if it contains parsable files
	bool isValuable(std::string directory);
	void undo(std::string full);
//	void sort();
	// full - e.g. "somedir/Film.2010.mp4"
	void sort(std::string full);
	// known by filename, not filelist
	static bool isSorted(std::string full);
	friend class OrganizerTest;
public:
	Organizer(const Args& pargs);
	void run();
	~Organizer();
};



