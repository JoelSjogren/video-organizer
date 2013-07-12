#include "parser.h"
#include <stdexcept>
using boost::regex_search;
using boost::regex_match;
using boost::regex;
using std::string;
using std::logic_error;
bool containsDirectory(string path) {
	return path.find("/") != string::npos;
}
/**********************************************
*  Parser									  *
**********************************************/
string Parser::directory(string path) {
	int lastSlash = path.rfind("/");
	if (lastSlash == string::npos) return ".";
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
*  FilmParser2							  *
**********************************************/
regex FilmParser2::rex("[. ][0-9]{4}", regex::extended);
int FilmParser2::rexlen(string(" 2010").size());
bool FilmParser2::matches(string fnam) {
//	if (isSeries(fnam)) return false; // TODO use this line
	// search for " dddd" where d are digits
	return regex_search(fnam, rex);
}
string FilmParser2::name(string fnam) {
	for (string::iterator i = fnam.begin(); i < fnam.end(); i++)
		if (regex_match(i, i + rexlen, rex))
			return dotsToSpaces(string(fnam.begin(), i));
	throw logic_error("FilmParser2::name failed. fnam: " + fnam);
}
/**********************************************
*  FilmParser3								  *
**********************************************/
regex FilmParser3::rex("[. ][([][0-9]{4}[])]", regex::extended);
int FilmParser3::rexlen(string(" (2010)").size());
bool FilmParser3::matches(string fnam) {
//	if (isSeries(fnam)) return false; // TODO use this line
	// search for " dddd" where d are digits
	return regex_search(fnam, rex);
}
string FilmParser3::name(string fnam) {
	for (string::iterator i = fnam.begin(); i < fnam.end(); i++)
		if (regex_match(i, i + rexlen, rex))
			return dotsToSpaces(string(fnam.begin(), i));
	throw logic_error("FilmParser2::name failed. fnam: " + fnam);
}
/**********************************************
*  SeriesParser1							  *
**********************************************/
regex SeriesParser1::rex("[. ]S[0-9]{2}E[0-9]{2}", regex::extended);
int SeriesParser1::rexlen(string(".S01E01").size());
bool SeriesParser1::matches(string fnam) {
	// search for ".SddEdd" where d are digits
	return regex_search(fnam, rex);
}
string SeriesParser1::name(string fnam) {
	int i = identifierStart(fnam);
	return dotsToSpaces(string(fnam.begin(), fnam.begin() + i));
}
string SeriesParser1::season(string fnam) {
	int i = identifierStart(fnam);
	static const int off = string(".").size();
	string::iterator start = fnam.begin() + i + off;
	return string(start, start + 3);
}
string SeriesParser1::episode(string fnam) {
	int i = identifierStart(fnam);
	static const int off = string(".S01").size();
	string::iterator start = fnam.begin() + i + off;
	return string(start, start + 3);
}
int SeriesParser1::identifierStart(string fnam) {
	for (string::iterator i = fnam.begin(); i < fnam.end(); i++)
		if (regex_match(i, i + rexlen, rex))
			return i - fnam.begin();
	throw logic_error("SeriesParser1::identifierStart failed. "
					  "fnam: " + fnam);
}
/**********************************************
*  SeriesParser2							  *
**********************************************/
regex SeriesParser2::rex(" - [0-9]{2}x[0-9]{2}", regex::extended);
int SeriesParser2::rexlen(string(" - 01x01").size());
bool SeriesParser2::matches(string fnam) {
	// search for ".SddEdd" where d are digits
	return regex_search(fnam, rex);
}
string SeriesParser2::name(string fnam) {
	int i = identifierStart(fnam);
	return dotsToSpaces(string(fnam.begin(), fnam.begin() + i));
}
string SeriesParser2::season(string fnam) {
	int i = identifierStart(fnam);
	static const int off = string(" - ").size();
	string::iterator start = fnam.begin() + i + off;
	return "S" + string(start, start + 2);
}
string SeriesParser2::episode(string fnam) {
	int i = identifierStart(fnam);
	static const int off = string(" - 01x").size();
	string::iterator start = fnam.begin() + i + off;
	return "E" + string(start, start + 2);
}
int SeriesParser2::identifierStart(string fnam) {
	for (string::iterator i = fnam.begin(); i < fnam.end(); i++)
		if (regex_match(i, i + rexlen, rex))
			return i - fnam.begin();
	throw logic_error("SeriesParser2::identifierStart failed. "
					  "fnam: " + fnam);
}

