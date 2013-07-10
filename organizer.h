/**********************************
*   organizer.h                   *
*                                 *
*   This is where the sorting     *
*   logic resides.                *
**********************************/
#include "args.h"
#include "fileman.h"
#include <string>
#include <stdexcept>
#include <boost/regex.hpp>
class Organizer {
	// configuration
	const Args& args;
	
	// file operations
	FileMan fileman;
	
	// logic
	// e.g. "Film.2.2010.mp4" -> "Film 2"
	static std::string extractFilmName(std::string fnam);
	// e.g. "The.Series.S02E01" -> "The Series"
	static std::string extractSeriesName(std::string fnam);
	// e.g. "The.Series.S02E01" -> "S02"
	static std::string extractSeriesSeason(std::string fnam);
	// e.g. "The.Series.S02E01" -> "E01"
	static std::string extractSeriesEpisode(std::string fnam);
	// e.g. "Film.2010.mp4" -> ".mp4"
	static std::string extractExtension(std::string fnam);
	// e.g. "The.Series.S02E01" -> 10, i.e. index of .S
	static int seriesIdentifierStart(std::string fnam);
	static bool isFilm(std::string fnam);
	static bool isSeries(std::string fnam);
	static std::string dotsToSpaces(std::string str);
	
	// debugging
	friend class OrganizerTest;
public:
	Organizer(const Args& pargs);
};

/*class FileAlreadyExists : public runtime_error {

};*/
