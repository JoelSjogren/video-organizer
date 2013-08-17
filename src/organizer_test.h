/**********************************
*  organizer_test.h               *
*                                 *
*  Tests Organizer.               *
**********************************/
#pragma once
// note: this file depends on organizer_test_data.cpp
#include "test.h"
class OrganizerTest : public Test {
    // test data
    static const char* infiles[];
    static const int infilec;
    static const char* outfiles[];
    static const char* outfiles_ABC[]; // --manual-name ABC
    static const int outfilec;
    const std::string indir;
    const std::string outdir;
    static const char* actions[];
    static const int actionc;
    static const bool indirPreserved[];

    void sampleRuns();
    void isSorted();
    void prepareWorkspace();
public:
	OrganizerTest();
};
