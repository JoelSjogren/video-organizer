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
	vector<Parser*> parsers;
	for (int i = 0; i < filmParsers.size(); i++)
		parsers.push_back(filmParsers[i]);
	for (int i = 0; i < seriesParsers.size(); i++)
		parsers.push_back(seriesParsers[i]);
	// iterate over *files* and parse them
	for (int i = 0; i < filec; i++) {
		const char* const file = files[i];
//		cout << "file: " << file << endl;
		{ // check for false positives and false negatives
			int matches = 0;
			for (int j = 0; j < parsers.size(); j++)
				if (parsers[j]->matches(file))
					matches++;
//			cout << "matches: " << matches << endl;
			EQ(matches, 1);
		}
		for (int j = 0; j < filmParsers.size(); j++)
			if (filmParsers[j]->matches(file))
				film(filmParsers[j], i);
		for (int j = 0; j < seriesParsers.size(); j++)
			if (seriesParsers[j]->matches(file))
				series(seriesParsers[j], i);
	}
	
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
//	cout << "index: " << index << endl;
	EQ(fnam, fileNames[index]);
	EQ(parser->name(fnam), titles[index]);
	EQ(parser->season(fnam), season[index]);
	EQ(parser->episode(fnam), episode[index]);
	EQ(Parser::extension(fnam), ext[index]);
	EQ(parser->matches(fnam), true);
	EQ(Parser::dotsToSpaces(fnam), filenames_nodots[index]);
}

