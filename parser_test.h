#pragma once
#include "test.h"
#include "parser.h"
class ParserTest : public Test {
	void film(FilmParser* parser, int index);
	void series(SeriesParser* parser, int index);
public:
	ParserTest();
};
