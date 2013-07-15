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
protected:
	virtual boost::regex name_rex() = 0;
	virtual size_t name_rex_len() = 0;
	virtual boost::regex cd_rex();
	virtual size_t cd_rex_len();
public:
	virtual bool matches(std::string fnam);
	// e.g. "Film.2.2010.mp4" -> "Film 2"
	virtual std::string name(std::string fnam);
	// e.g. "Film.2010.DVDRip.CD01.avi" -> "CD01"
	virtual std::string cd(std::string fnam);
};
class SeriesParser : public Parser {
protected:
	virtual boost::regex name_rex() = 0;
	virtual size_t name_rex_len() = 0;
	// e.g. "The.Series.S02E01" -> 10, i.e. index of ".S"
	virtual int identifierStart(std::string fnam);
	virtual int seasonOffset() = 0;
	virtual int episodeOffset() = 0;
public:
	virtual bool matches(std::string fnam);
	// e.g. "The.Series.S02E01.mp4" -> "The Series"
	virtual std::string name(std::string fnam);
	// e.g. "The.Series.S02E01.mp4" -> "S02"
	virtual std::string season(std::string fnam);
	// e.g. "The.Series.S02E01.mp4" -> "E01"
	virtual std::string episode(std::string fnam);
};
// e.g. The Film 2010 XviD.avi or The.Film.2010.XviD.avi
class FilmParser2 : public FilmParser {
protected:
	virtual boost::regex name_rex();
	virtual size_t name_rex_len();
};
// e.g. The Film (2010) XviD.avi or The.Film.[2010].XviD.avi
class FilmParser3 : public FilmParser {
protected:
	virtual boost::regex name_rex();
	virtual size_t name_rex_len();
};
// e.g. The.Series.S02E01.avi or The Series S02E01.avi
class SeriesParser1 : public SeriesParser {
protected:
	virtual boost::regex name_rex();
	virtual size_t name_rex_len();
	virtual int seasonOffset();
	virtual int episodeOffset();
};
// e.g. The Series - 02x01.avi
class SeriesParser2 : public SeriesParser {
protected:
	virtual boost::regex name_rex();
	virtual size_t name_rex_len();
	virtual int seasonOffset();
	virtual int episodeOffset();
public:
	virtual std::string season(std::string fnam); // override
	virtual std::string episode(std::string fnam); // override
};








