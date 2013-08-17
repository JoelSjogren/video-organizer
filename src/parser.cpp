/**********************************
*  parser.cpp                     *
**********************************/
#include "parser.h"
#include <stdexcept>
using boost::regex_search;
using boost::regex_match;
using boost::regex;
using boost::regex_constants::syntax_option_type;
using std::string;
using std::logic_error;
static const syntax_option_type regex_syntax = regex::icase |
                                               regex::extended;
bool containsDirectory(string path) {
	return path.find("/") != string::npos;
}
/**********************************************
*  Parser									  *
**********************************************/
string Parser::directory(string path) {
	int lastSlash = path.rfind("/");
	if (lastSlash == string::npos) return "./";
	return path.substr(0, lastSlash + 1);
}
string Parser::filename(string path) {
	int lastSlash = path.rfind('/');
	if (lastSlash == string::npos) return path;
	return path.substr(lastSlash + 1, string::npos);
}
string Parser::extension(string path) {
	string fnam = filename(path);
	int i = fnam.rfind(".");
	if (i == string::npos) return "";
	return fnam.substr(i, string::npos);
}
string Parser::filenameNoExt(string path) {
	string fnam = filename(path);
	int i = fnam.rfind(".");
	if (i == string::npos) return fnam;
	return fnam.substr(0, i);
}
string Parser::dotsToSpaces(string str) {
	int lookHere = 0;
	int foundHere;
	while ((foundHere = str.find(".", lookHere)) != string::npos) {
		str.replace(foundHere, 1, " ");
		lookHere = foundHere + 1;
	}
	return str;
}
/**********************************************
*  FilmParser                                 *
**********************************************/
bool FilmParser::matches(string file) {
    const string fnam = filename(file);
    return regex_search(fnam, name_rex());
}
std::string FilmParser::name(std::string file) {
    const string fnam = filename(file);
	for (string::const_iterator i = fnam.begin(); i < fnam.end(); i++)
		if (regex_match(i, i + name_rex_len(), name_rex()))
			return dotsToSpaces(string(fnam.begin(), i));
	throw logic_error("FilmParser::name failed. fnam: " + fnam);    
}
std::string FilmParser::cd(std::string file) {
    const string fnam = filename(file);
	for (string::const_iterator i = fnam.begin(); i < fnam.end(); i++)
		if (regex_match(i, i + cd_rex_len(), cd_rex()))
			return dotsToSpaces(string(i + 1, i + 5));
	return "CD01";
}
boost::regex FilmParser::cd_rex() {
    return regex("[. ]CD[0-9]{2}", regex_syntax);
}
size_t FilmParser::cd_rex_len() {
    return string(".CD01").size();
}
/**********************************************
*  FilmParser2                                *
**********************************************/
boost::regex FilmParser2::name_rex() {
    return regex("[. ][0-9]{4}", regex_syntax);
}
size_t FilmParser2::name_rex_len() {
    return string(".2010").size();
}
/**********************************************
*  FilmParser3								  *
**********************************************/
boost::regex FilmParser3::name_rex() {
    return regex("[. ][([][0-9]{4}[])]", regex_syntax);
}
size_t FilmParser3::name_rex_len() {
    return string(".(2010)").size();
}
/**********************************************
*  SeriesParser							      *
**********************************************/
bool SeriesParser::matches(string file) {
    const string fnam = filename(file);
	// search for ".SddEdd" where d are digits
	return regex_search(fnam, name_rex());
}
std::string SeriesParser::name(std::string file) {
    const string fnam = filename(file);
	int i = identifierStart(fnam);
	return dotsToSpaces(string(fnam.begin(), fnam.begin() + i));
}
std::string SeriesParser::season(std::string file) {
    const string fnam = filename(file);
	int i = identifierStart(fnam);
	string::const_iterator start = fnam.begin() + i + seasonOffset();
	return string(start, start + 3).replace(0, 1, "S");
}
std::string SeriesParser::episode(std::string file) {
    const string fnam = filename(file);
	int i = identifierStart(fnam);
	string::const_iterator start = fnam.begin() + i + episodeOffset();
	return string(start, start + 3).replace(0, 1, "E");
}
int SeriesParser::identifierStart(string file) {
    const string fnam = filename(file);
	for (string::const_iterator i = fnam.begin(); i < fnam.end(); i++)
		if (regex_match(i, i + name_rex_len(), name_rex()))
			return i - fnam.begin();
	throw logic_error("SeriesParser::identifierStart failed. "
					  "fnam: " + fnam);
}
/**********************************************
*  SeriesParser1							  *
**********************************************/
boost::regex SeriesParser1::name_rex() {
    return regex("[. ]S[0-9]{2}E[0-9]{2}", regex_syntax);
}
size_t SeriesParser1::name_rex_len() {
    return string(".S01E01").size();
}
int SeriesParser1::seasonOffset() {
    return string(".").size();
}
int SeriesParser1::episodeOffset() {
    return string(".S01").size();
}
/**********************************************
*  SeriesParser2							  *
**********************************************/
boost::regex SeriesParser2::name_rex() {
    return regex(" - [0-9]{2}x[0-9]{2}", regex_syntax);
}
size_t SeriesParser2::name_rex_len() {
    return string(" - 01x01").size();
}
int SeriesParser2::seasonOffset() {
    return string(" -").size();
}
int SeriesParser2::episodeOffset() {
    return string(" - 01").size();
}


