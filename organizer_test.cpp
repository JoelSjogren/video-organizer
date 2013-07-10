#include "organizer.h"
#include "organizer_test.h"
#include "filename.h"
#include <string>
#include <iostream>
using std::string;
using std::cout;
using std::endl;
OrganizerTest::OrganizerTest() : Test("Organizer") {
	OrganizerTest::film();
	OrganizerTest::series();
//	cout << "OrganizerTest passed " << passed << " tests\n";
}
void OrganizerTest::film() {
		const char* files[] = {
		"folder//double//The.Prestige.2006.720p.Bluray.x264.anoXmous.mp4",
		"Paranormal.Activity.3.2011.UNRATED.DVDRip.XviD-SPARKS.avi",
		"../../sub/Rise.of.the.Planet.of.the.Apes.2011.TS.XviD-NOVA.avi",
	};
	const int filec = sizeof(files) / sizeof(*files);
	const char* fileNames[] = {
		"The.Prestige.2006.720p.Bluray.x264.anoXmous.mp4",
		"Paranormal.Activity.3.2011.UNRATED.DVDRip.XviD-SPARKS.avi",
		"Rise.of.the.Planet.of.the.Apes.2011.TS.XviD-NOVA.avi",
	};
	const char* titles[] = {
		"The Prestige",
		"Paranormal Activity 3",
		"Rise of the Planet of the Apes",
	};
	const char* ext[] = {
		".mp4",
		".avi",
		".avi",
	};
	const char* fileNames_nodots[] = {
		"The Prestige 2006 720p Bluray x264 anoXmous mp4",
		"Paranormal Activity 3 2011 UNRATED DVDRip XviD-SPARKS avi",
		"Rise of the Planet of the Apes 2011 TS XviD-NOVA avi",
	};

	for (int i = 0; i < filec; i++) {
		const string fnam = extractFilename(files[i]);
		EQ(fnam, fileNames[i]);
		EQ(Organizer::extractFilmName(fnam), titles[i]);
		EQ(Organizer::extractExtension(fnam), ext[i]);
		EQ(Organizer::isFilm(fnam), true);
		EQ(Organizer::isSeries(fnam), false);
		EQ(Organizer::dotsToSpaces(fnam), fileNames_nodots[i]);
	}
}
void OrganizerTest::series() {
	const char* files[] = {
		"some/dir/Chuck.S05E01.HDTV.XviD-LOL.avi",
		"/Chuck.S05E02.HDTV.XviD-LOL.avi",
		"Community.S04E02.HDTV.x264-LOL.mp4",
	};
	const int filec = sizeof(files) / sizeof(*files);
	const char* fileNames[] = {
		"Chuck.S05E01.HDTV.XviD-LOL.avi",
		"Chuck.S05E02.HDTV.XviD-LOL.avi",
		"Community.S04E02.HDTV.x264-LOL.mp4",
	};
	const char* titles[] = {
		"Chuck",
		"Chuck",
		"Community",
	};
	const char* season[] = {
		"S05",
		"S05",
		"S04",
	};
	const char* episode[] = {
		"E01",
		"E02",
		"E02",
	};
	const char* ext[] = {
		".avi",
		".avi",
		".mp4",
	};
	const int idstart[] = {
		5,
		5,
		9,
	};
	const char* filenames_nodots[] = {
		"Chuck S05E01 HDTV XviD-LOL avi",
		"Chuck S05E02 HDTV XviD-LOL avi",
		"Community S04E02 HDTV x264-LOL mp4",
	};
	for (int i = 0; i < filec; i++) {
		const string fnam = extractFilename(files[i]);
		EQ(fnam, fileNames[i]);
		EQ(Organizer::extractSeriesName(fnam), titles[i]);
		EQ(Organizer::extractSeriesSeason(fnam), season[i]);
		EQ(Organizer::extractSeriesEpisode(fnam), episode[i]);
		EQ(Organizer::extractExtension(fnam), ext[i]);
		EQ(Organizer::seriesIdentifierStart(fnam), idstart[i]);
		EQ(Organizer::isFilm(fnam), false);
		EQ(Organizer::isSeries(fnam), true);
		EQ(Organizer::dotsToSpaces(fnam), filenames_nodots[i]);
	}
}
