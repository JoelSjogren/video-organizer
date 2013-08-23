/**********************************
*  parser_test.h                  *
*                                 *
*  Tests Parser.                  *
**********************************/
#pragma once
// note: this file depends on parser_test_data.cpp
#include "test.h"
#include "parser.h"
class ParserTest : public Test {
    // test data
    static const char* files[];
    static const unsigned filec;
    static const char* fileNames[];
    static const char* titles[];
    static const char* ext[];
    static const char* season[];
    static const char* episode[];
    static const char* filenames_nodots[];
    static const char* cd[];
    
	void film(FilmParser* parser, int index);
	void series(SeriesParser* parser, int index);
	void filenameEtc();
public:
	ParserTest();
};
