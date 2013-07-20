#include "organizer_test.h"
#include "organizer.h"
#include "args.h"
#include "fileman.h"
#include "parser.h"
#include <dirent.h>
#include <cassert>
#include <iostream>
#include <string>
using std::string;
using std::cout;
using std::endl;
OrganizerTest::OrganizerTest() : Test("Organizer") {
    const char* infiles[] = {
        "Chuck.S05E01.HDTV.XviD-LOL.avi",
        "Community.S04E02.HDTV.x264-LOL.mp4",
        "subdir/Chuck.S05E02.HDTV.XviD-LOL.avi",
        "The.Prestige.2006.720p.Bluray.x264.anoXmous.mp4",
    };
    const int infilec = sizeof(infiles) / sizeof(*infiles);
    const char* outfiles[] = {
        "Chuck/S05/E01.avi",
        "Community/S04/E02.mp4",
        "Chuck/S05/E02.avi",
        "The Prestige/CD01.mp4",
    };
    const int outfilec = sizeof(outfiles) / sizeof(*outfiles);
    const string indir = getdir() + "input/";
    const string outdir = getdir() + "output/";
    const char* actions[] = { "--move", "--copy", "--link"};
    const int actionc = sizeof(actions) / sizeof(*actions);
    for (int i = 0; i < actionc; i++) {
        // Normal run
        { // Prepare workspace
            Args args;
            FileMan fileman(args);
            fileman.remove_all(getdir());
            for (int j = 0; j < infilec; j++)
                fileman.touch(indir + infiles[j]);
            fileman.dig(outdir);
        }
        { // Let it do its thing
        	char* argv[] = {
	            (char*) "video-organizer",
	            (char*) indir.c_str(), // dir will be omitted
	            (char*) "-v",
	            (char*) "-1",
	            (char*) "-o",
	            (char*) outdir.c_str(),
	            (char*) "-r",
	            (char*) actions[i],
            };
            int argc = sizeof(argv) / sizeof(*argv);
            Args args(argc, argv);
            Organizer organizer(args);
        }
        { // Check results
            Args args;
            FileMan fileman(args);
            for (int j = 0; j < outfilec; j++) {
                string outfile = outdir + outfiles[j];
                string outfiledir = Parser::directory(outfile);
                string filelist = outfiledir + "filelist";
                EQ(fileman.exists(outfile), true);
                EQ(fileman.exists(filelist), true);
            }
        }
        // Simulated run
        { // Prepare workspace
            Args args;
            FileMan fileman(args);
            fileman.remove_all(getdir());
            for (int j = 0; j < infilec; j++)
                fileman.touch(indir + infiles[j]);
            fileman.dig(outdir);
        }
        { // Let it do its thing
        	char* argv[] = {
	            (char*) "video-organizer",
	            (char*) indir.c_str(), // dir will be omitted
	            (char*) "-v",
	            (char*) "-1",
	            (char*) "-o",
	            (char*) outdir.c_str(),
	            (char*) "-r",
	            (char*) actions[i],
	            (char*) "-s",
            };
            int argc = sizeof(argv) / sizeof(*argv);
            Args args(argc, argv);
            Organizer organizer(args);
        }
        { // Check results
            Args args;
            FileMan fileman(args);
            EQ(fileman.isEmpty(outdir), true);
        }
	}
}









