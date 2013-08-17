/**********************************
*  organizer.h                    *
*                                 *
*  The core of all organization.  *
**********************************/
/****************************************************
*  When the console arguments have been parsed, an  *
*  Organizer is constructed. Then the following     *
*  functions call each other:                       *
*                                                   *
*  1. run()                                         *
*  2. iterate()                                     *
*  3. launch() for each file                        *
*  4. argument action() -> sort() or undo()         *
****************************************************/
#pragma once
#include "args.h"
#include "fileman.h"
#include "parser.h"
#include <string>
#include <vector>
#include <boost/function.hpp> // passing functions as arguments
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
	// a directory is valuable if it contains parsable files
	bool isValuable(std::string directory);
	// full - e.g. "somedir/Film.2010.mp4"
	void undo(std::string full);
	void sort(std::string full);
	// known by filename, not filelist
	static bool isSorted(std::string full);
	bool shouldClean(std::string directory);
	friend class OrganizerTest;
public:
	Organizer(const Args& pargs);
	void run();
	~Organizer();
};



