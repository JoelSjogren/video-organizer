/**********************************
*  parser_test_data.cpp           *
**********************************/
#include "parser_test.h"
const char* ParserTest::files[] = {
	// film naming convention 2
	"folder//double//The.Prestige.2006.720p.Bluray.x264.anoXmous.mp4",
	"Paranormal.Activity.3.2011.UNRATED.DVDRip.XviD-SPARKS.avi",
	"../../sub/Rise.of.the.Planet.of.the.Apes.2011.TS.XviD-NOVA.avi",
	// series: naming convention 1
	"some/dir/Chuck.S05E01.HDTV.XviD-LOL.avi",
	"/Chuck.S05E02.HDTV.XviD-LOL.avi",
	"Community.S04E02.HDTV.x264-LOL.mp4",
	// series: naming convention 2
	"/The Big Bang Theory - 01x01 - Addic7ed.com.srt",
	"Family Guy - 10x07 - Amish Guy.LOL.English.HI.C.orig.Addic7ed.com.srt",
	"somedir/../Made up Show - 99x99 - this will be ignored.srt",
	// film naming convention 2
	"Man of Steel 2013 CAM XviD MP3 MiLLENiUM.srt",
	// film naming convention 3
	"/../Whip It (2009) DvdRip [Xvid] {1337x}-X.srt",
	"Seven.[1995].DVDRip.Xvid.Blood.avi",
	// multi-disc films
	"Cube.Zero.2004.SWESUB.AC3.DVDRip.XviD-Pride86.CD01.avi",
	"./Cube.Zero.2004.SWESUB.AC3.DVDRip.XviD-Pride86.CD02.avi",
	"../The.Book.Of.Eli.2010.SWESUB.DVDRip.XviD-Pride86.CD01.avi",
	"/The.Book.Of.Eli.2010.SWESUB.DVDRip.XviD-Pride86.CD02.avi",
};
const int ParserTest::filec = sizeof(files) / sizeof(*files);
const char* ParserTest::fileNames[] = {
	"The.Prestige.2006.720p.Bluray.x264.anoXmous.mp4",
	"Paranormal.Activity.3.2011.UNRATED.DVDRip.XviD-SPARKS.avi",
	"Rise.of.the.Planet.of.the.Apes.2011.TS.XviD-NOVA.avi",
	"Chuck.S05E01.HDTV.XviD-LOL.avi",
	"Chuck.S05E02.HDTV.XviD-LOL.avi",
	"Community.S04E02.HDTV.x264-LOL.mp4",
	"The Big Bang Theory - 01x01 - Addic7ed.com.srt",
	"Family Guy - 10x07 - Amish Guy.LOL.English.HI.C.orig.Addic7ed.com.srt",
	"Made up Show - 99x99 - this will be ignored.srt",
	"Man of Steel 2013 CAM XviD MP3 MiLLENiUM.srt",
	"Whip It (2009) DvdRip [Xvid] {1337x}-X.srt",
	"Seven.[1995].DVDRip.Xvid.Blood.avi",
	"Cube.Zero.2004.SWESUB.AC3.DVDRip.XviD-Pride86.CD01.avi",
	"Cube.Zero.2004.SWESUB.AC3.DVDRip.XviD-Pride86.CD02.avi",
	"The.Book.Of.Eli.2010.SWESUB.DVDRip.XviD-Pride86.CD01.avi",
	"The.Book.Of.Eli.2010.SWESUB.DVDRip.XviD-Pride86.CD02.avi",
};
const char* ParserTest::titles[] = {
	"The Prestige",
	"Paranormal Activity 3",
	"Rise of the Planet of the Apes",
	"Chuck",
	"Chuck",
	"Community",
	"The Big Bang Theory",
	"Family Guy",
	"Made up Show",
	"Man of Steel",
	"Whip It",
	"Seven",
	"Cube Zero",
	"Cube Zero",
	"The Book Of Eli",
	"The Book Of Eli",
};
const char* ParserTest::ext[] = {
	".mp4",
	".avi",
	".avi",
	".avi",
	".avi",
	".mp4",
	".srt",
	".srt",
	".srt",
	".srt",
	".srt",
	".avi",
	".avi",
	".avi",
	".avi",
	".avi",
};
const char* ParserTest::season[] = {
	NULL,
	NULL,
	NULL,
	"S05",
	"S05",
	"S04",
	"S01",
	"S10",
	"S99",
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
};
const char* ParserTest::episode[] = {
	NULL,
	NULL,
	NULL,
	"E01",
	"E02",
	"E02",
	"E01",
	"E07",
	"E99",
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
};
const char* ParserTest::filenames_nodots[] = {
	"The Prestige 2006 720p Bluray x264 anoXmous mp4",
	"Paranormal Activity 3 2011 UNRATED DVDRip XviD-SPARKS avi",
	"Rise of the Planet of the Apes 2011 TS XviD-NOVA avi",
	"Chuck S05E01 HDTV XviD-LOL avi",
	"Chuck S05E02 HDTV XviD-LOL avi",
	"Community S04E02 HDTV x264-LOL mp4",
	"The Big Bang Theory - 01x01 - Addic7ed com srt",
	"Family Guy - 10x07 - Amish Guy LOL English HI C orig Addic7ed com srt",
	"Made up Show - 99x99 - this will be ignored srt",
	"Man of Steel 2013 CAM XviD MP3 MiLLENiUM srt",
	"Whip It (2009) DvdRip [Xvid] {1337x}-X srt",
	"Seven [1995] DVDRip Xvid Blood avi",
	"Cube Zero 2004 SWESUB AC3 DVDRip XviD-Pride86 CD01 avi",
	"Cube Zero 2004 SWESUB AC3 DVDRip XviD-Pride86 CD02 avi",
	"The Book Of Eli 2010 SWESUB DVDRip XviD-Pride86 CD01 avi",
	"The Book Of Eli 2010 SWESUB DVDRip XviD-Pride86 CD02 avi",
};
const char* ParserTest::cd[] = {
    "CD01",
    "CD01",
    "CD01",
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    "CD01",
    "CD01",
    "CD01",
    "CD01",
    "CD02",
    "CD01",
    "CD02",
};



