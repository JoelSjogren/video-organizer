#include "args_test.h"
#include "args.h"
#include <cassert>
#include <iostream>
#include <string>
using std::string;
using std::cout;
using std::endl;
ArgsTest::ArgsTest() : Test("Args") {
	{ // Test 1: minimal
		// TODO fix deprecated conversion
		char* argv[] = {
			(char*) "video-organizer",
			(char*) "test/args/input", // dir will be omitted
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
	{ // Test 2: short options
		// TODO fix deprecated conversion
		char* argv[] = {
			(char*) "video-organizer",
			(char*) "test/args/input", // dir will be omitted
			(char*) "-v",
			(char*) "-1",
			(char*) "-o",
			(char*) "test/args/output",
			(char*) "-c",
			(char*) "-r",
			(char*) "-s",
		};
		int argc = sizeof(argv) / sizeof(*argv);
		Args args(argc, argv);
		EQ(args.undo,  false);
		EQ(args.outdir,  "test/args/output/");
		EQ(args.infiles.size(), 4);
		EQ(args.mcl,  Args::COPY);
		EQ(args.verbosity,  -1);
		EQ(args.recursive,  true);
		EQ(args.simulate,  true);
	}
	{ // Test 3: long options
		// TODO fix deprecated conversion
		char* argv[] = {
			(char*) "video-organizer",
			(char*) "--link",
			(char*) "test/args/input", // dir will be omitted
			(char*) "--verbosity=-1",
			(char*) "--outdir=test/args/output",
			(char*) "--recursive",
			(char*) "--simulate",
		};
		int argc = sizeof(argv) / sizeof(*argv);
		Args args(argc, argv);
		EQ(args.undo,  false);
		EQ(args.outdir,  "test/args/output/");
		EQ(args.infiles.size(), 4);
		EQ(args.mcl,  Args::LINK);
		EQ(args.verbosity,  -1);
		EQ(args.recursive,  true);
		EQ(args.simulate,  true);
	}
}


