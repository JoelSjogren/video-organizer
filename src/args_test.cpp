#include "args_test.h"
#include "args.h"
#include "fileman.h"
#include <cassert>
#include <iostream>
#include <string>
using std::string;
using std::cout;
using std::endl;
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
        cout << "Preparing workspace\n";
        Args args;
        FileMan fileman(args);
        fileman.remove_all(getdir());
        for (int i = 0; i < filec; i++)
            fileman.touch(indir + files[i]);
        fileman.dig(outdir);
        cout << "Prepared workspace\n";
    }
	{ // Test 1: minimal
		// TODO fix deprecated conversion
		char* argv[] = {
			(char*) "video-organizer",
			(char*) indir.c_str(), // dir will be omitted
			(char*) "--verbosity=-1", // suppress warnings
		};
		int argc = sizeof(argv) / sizeof(*argv);
		Args args(argc, argv);
		EQ(args.undo, false);
		EQ(args.outdir, "./");
		EQ(args.infiles.size(), 0);
		EQ(args.mcl, Args::MOVE);
		EQ(args.verbosity,  -1);
		EQ(args.recursive,  false);
		EQ(args.simulate,  false);
	}
	cout << "Test 1 check\n";
	{ // Test 2: short options
		// TODO fix deprecated conversion
		char* argv[] = {
			(char*) "video-organizer",
			(char*) indir.c_str(), // dir will be omitted
			(char*) "-v",
			(char*) "-1",
			(char*) "-o",
			(char*) outdir.c_str(),
			(char*) "-c",
			(char*) "-r",
			(char*) "-s",
		};
		int argc = sizeof(argv) / sizeof(*argv);
		Args args(argc, argv);
		EQ(args.undo,  false);
		EQ(args.outdir,  outdir);
		EQ(args.infiles.size(), 4);
		EQ(args.mcl,  Args::COPY);
		EQ(args.verbosity,  -1);
		EQ(args.recursive,  true);
		EQ(args.simulate,  true);
	}
	cout << "Test 2 check\n";
	{ // Test 3: long options
		// TODO fix deprecated conversion
		char* argv[] = {
			(char*) "video-organizer",
			(char*) "--link",
			(char*) indir.c_str(), // dir will be omitted
			(char*) "--verbosity=1",
			(char*) ("--outdir=" + outdir).c_str(),
			(char*) "--recursive",
			(char*) "--simulate",
		};
		cout << "pg: " << argv[2] << endl;
		int argc = sizeof(argv) / sizeof(*argv);
		cout << "a\n";
		Args args(argc, argv);
		cout << "b\n";
		EQ(args.undo,  false);
		EQ(args.outdir,  outdir);
		EQ(args.infiles.size(), 4);
		EQ(args.mcl,  Args::LINK);
		EQ(args.verbosity,  -1);
		EQ(args.recursive,  true);
		EQ(args.simulate,  true);
	}
	cout << "Test 3 check\n";
}


