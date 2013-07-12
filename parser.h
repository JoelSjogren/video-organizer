#pragma once
#include <string>
#include <boost/regex.hpp>
class Parser {
public:
	static std::string directory(std::string whole);
	// e.g. "somedir/Film.2010.mp4" -> "Film.2010.mp4"
	static std::string filename(std::string whole);
	// e.g. "Film.2010.mp4" -> ".mp4"
	static std::string extension(std::string fnam);
	static std::string dotsToSpaces(std::string str);
	virtual bool matches(std::string fnam) = 0;
	virtual ~Parser() {}
};
class FilmParser : public Parser {
public:
	// e.g. "Film.2.2010.mp4" -> "Film 2"
	virtual std::string name(std::string fnam) = 0;
};
class SeriesParser : public Parser {
public:
	// e.g. "The.Series.S02E01.mp4" -> "The Series"
	virtual std::string name(std::string fnam) = 0;
	// e.g. "The.Series.S02E01.mp4" -> "S02"
	virtual std::string season(std::string fnam) = 0;
	// e.g. "The.Series.S02E01.mp4" -> "E01"
	virtual std::string episode(std::string fnam) = 0;
};
class FilmParser1 : public FilmParser {
	static boost::regex rex;
	static int rexlen;
public:
	virtual bool matches(std::string fnam);
	virtual std::string name(std::string fnam);
};
class SeriesParser1 : public SeriesParser {
	static boost::regex rex;
	static int rexlen;
	// e.g. "The.Series.S02E01" -> 10, i.e. index of ".S"
	int identifierStart(std::string fnam);
public:
	virtual bool matches(std::string fnam);
	virtual std::string name(std::string fnam);
	virtual std::string season(std::string fnam);
	virtual std::string episode(std::string fnam);
};
class SeriesParser2 : public SeriesParser {
	static boost::regex rex;
	static int rexlen;
	// e.g. "The Series - 01x01" -> 10, i.e. index of " -"
	int identifierStart(std::string fnam);
public:
	virtual bool matches(std::string fnam);
	virtual std::string name(std::string fnam);
	virtual std::string season(std::string fnam);
	virtual std::string episode(std::string fnam);
};


/*class SeriesParser2 : public SeriesParser {
	static const boost::regex rex(" - [0-9]{2}x[0-9]{2}",
		boost::regex::extended);
	static const int rexlen = string(" - 01x01").size();
	int identifierStart(std::string fnam);
};*/













