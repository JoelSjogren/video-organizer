/**********************************
*   organizer.h                   *
*                                 *
*   The core of all organizing.   *
**********************************/
#include "args.h"
#include "fileman.h"
#include "parser.h"
#include <string>
#include <stdexcept>
#include <boost/regex.hpp>
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
	
	/* debugging */
	friend class OrganizerTest;
public:
	Organizer(const Args& pargs);
	~Organizer();
};

/*class FileAlreadyExists : public runtime_error {

};*/



