/**********************************
*  parser_test.cpp                *
**********************************/
#include "parser.h"
#include "parser_test.h"
#include <string>
#include <vector>
#include <iostream>
using std::string;
using std::vector;
using std::cout;
using std::endl;
ParserTest::ParserTest() : Test("Parser") {
	// init parsers
	vector<FilmParser*> filmParsers;
	vector<SeriesParser*> seriesParsers;
	filmParsers.push_back(new FilmParser2);
	filmParsers.push_back(new FilmParser3);
	seriesParsers.push_back(new SeriesParser1);
	seriesParsers.push_back(new SeriesParser2);
	seriesParsers.push_back(new SeriesParser3);
	seriesParsers.push_back(new SeriesParser4);
	vector<Parser*> parsers;
	for (unsigned i = 0; i < filmParsers.size(); i++)
		parsers.push_back(filmParsers[i]);
	for (unsigned i = 0; i < seriesParsers.size(); i++)
		parsers.push_back(seriesParsers[i]);
	// iterate over *files* and parse them
	for (unsigned i = 0; i < filec; i++) {
		const char* const file = files[i];
		{ // check for false positives and false negatives
			int matches = 0;
			for (unsigned j = 0; j < parsers.size(); j++)
				if (parsers[j]->matches(file))
					matches++;
			EQ(matches, 1);
		}
		for (unsigned j = 0; j < filmParsers.size(); j++)
			if (filmParsers[j]->matches(file))
				film(filmParsers[j], i);
		for (unsigned j = 0; j < seriesParsers.size(); j++)
			if (seriesParsers[j]->matches(file))
				series(seriesParsers[j], i);
	}
	filenameEtc();
}
void ParserTest::film(FilmParser* parser, int index) {
	const string fnam = Parser::filename(files[index]);
	EQ(fnam, fileNames[index]);
	EQ(parser->name(fnam), titles[index]);
	EQ(Parser::extension(fnam), ext[index]);
	EQ(parser->matches(fnam), true);
	EQ(Parser::dotsToSpaces(fnam), filenames_nodots[index]);
	EQ(parser->cd(fnam), cd[index]);
}
void ParserTest::series(SeriesParser* parser, int index) {
	const string fnam = Parser::filename(files[index]);
	EQ(fnam, fileNames[index]);
	EQ(parser->name(fnam), titles[index]);
	EQ(parser->season(fnam), season[index]);
	EQ(parser->episode(fnam), episode[index]);
	EQ(Parser::extension(fnam), ext[index]);
	EQ(parser->matches(fnam), true);
	EQ(Parser::dotsToSpaces(fnam), filenames_nodots[index]);
}
void ParserTest::filenameEtc() {
    EQ(Parser::directory("somedir/Film.2010.mp4"), "somedir/");
    EQ(Parser::filename("somedir/Film.2010.mp4"), "Film.2010.mp4");
    EQ(Parser::extension("somedir/Film.2010.mp4"), ".mp4");
    EQ(Parser::extension("noext"), "");
    EQ(Parser::filenameNoExt("somedir/Film.2010.mp4"), "Film.2010");
    EQ(Parser::filenameNoExt("some.dir/noext"), "noext");
    EQ(Parser::dotsToSpaces("Film.2010.mp4"), "Film 2010 mp4");
}



