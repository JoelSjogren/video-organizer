/**********************************
*  args_test.cpp                  *
**********************************/
#include "args_test.h"
#include "args.h"
#include "fileman.h"
#include <string>
using std::string;
ArgsTest::ArgsTest() : Test("Args") {
    const char* files[] = {
        "Chuck.S05E01.HDTV.XviD-LOL.avi",
        "Community.S04E02.HDTV.x264-LOL.mp4",
        "subdir/Chuck.S05E02.HDTV.XviD-LOL.avi",
        "The.Prestige.2006.720p.Bluray.x264.anoXmous.mp4",
    };
    const int filec = sizeof(files) / sizeof(*files);
    const string indir = getdir() + "input/";
    const string outdir = getdir() + "output/";
    { // Prepare workspace
        Args args;
        FileMan fileman(args);
        fileman.remove_all(getdir());
        for (int i = 0; i < filec; i++)
            fileman.touch(indir + files[i]);
        fileman.dig(outdir);
    }
	{ // Test 1: minimal
		char* argv[] = {
			(char*) "video-organizer",
			(char*) indir.c_str(),
			(char*) "--verbosity=-1", // suppress warnings
		};
		int argc = sizeof(argv) / sizeof(*argv);
		Args args(argc, argv);
		EQ(args.undo, false);
		EQ(args.outdir, "./");
		EQ(args.infiles.size(), 1);
		EQ(args.action, Args::MOVE);
		EQ(args.verbosity,  -1);
		EQ(args.recursive,  false);
		EQ(args.simulate,  false);
		EQ(args.include_part, false);
		EQ(args.clean, 0);
	}
	{ // Test 2: short options
		char* argv[] = {
			(char*) "video-organizer",
			(char*) indir.c_str(),
			(char*) "-v",
			(char*) "-1",
			(char*) "-o",
			(char*) outdir.c_str(),
			(char*) "-c",
			(char*) "-r",
			(char*) "-s",
			(char*) "-p",
		};
		int argc = sizeof(argv) / sizeof(*argv);
		Args args(argc, argv);
		EQ(args.undo,  false);
		EQ(args.outdir,  outdir);
		EQ(args.infiles.size(), 1);
		EQ(args.action,  Args::COPY);
		EQ(args.verbosity,  -1);
		EQ(args.recursive,  true);
		EQ(args.simulate,  true);
		EQ(args.include_part, true);
		EQ(args.clean, 0);
	}
	{ // Test 3: long options
		string outdirarg = "--outdir=" + outdir; // put on stack
		char* argv[] = {
			(char*) "video-organizer",
			(char*) "--link",
			(char*) indir.c_str(),
			(char*) "--verbosity=-1",
			(char*) outdirarg.c_str(),
			(char*) "--recursive",
			(char*) "--simulate",
			(char*) "--part",
			(char*) "--clean=3M",
		};
		int argc = sizeof(argv) / sizeof(*argv);
		Args args(argc, argv);
		EQ(args.undo,  false);
		EQ(args.outdir,  outdir);
		EQ(args.infiles.size(), 1);
		EQ(args.action,  Args::LINK);
		EQ(args.verbosity,  -1);
		EQ(args.recursive,  true);
		EQ(args.simulate,  true);
		EQ(args.include_part, true);
		EQ(args.clean, 3*1024*1024);
	}
}


