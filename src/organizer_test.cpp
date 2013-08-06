/**********************************
*  organizer_test.cpp             *
**********************************/
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
// except for . and .., of course
bool onlyFileInDir(string file) {
    Args args;
    FileMan fileman(args);
    return fileman.fileCount(Parser::directory(file)) == 3 &&
           fileman.exists(file);
}
OrganizerTest::OrganizerTest() : Test("Organizer") {
    sampleRuns();
    isSorted();
}
void OrganizerTest::isSorted() {
    EQ(Organizer::isSorted("dir/E01.avi"), true);
    EQ(Organizer::isSorted("dir/E01.more.avi"), false);
    EQ(Organizer::isSorted("CD02.mp4"), true);
    EQ(Organizer::isSorted("some.dir/CD02"), true);
    EQ(Organizer::isSorted("some.dir/"), false);
    EQ(Organizer::isSorted("Chuck.S05E01.XviD.avi"), false);
    EQ(Organizer::isSorted("E0x.avi"), false);
    EQ(Organizer::isSorted("CD0x.avi"), false);
}
void OrganizerTest::sampleRuns() {
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
    const bool indirPreserved[] = { false, true, true };
    for (int i = 0; i < actionc; i++) {
        { // Normal run
            { // Prepare workspace
                Args args;
                FileMan fileman(args);
                fileman.remove_all(getdir());
                for (int j = 0; j < infilec; j++)
                    fileman.touch(indir + infiles[j]);
                fileman.dig(outdir);
                Organizer organizer(args);
                EQ(organizer.isValuable(indir), true);
                EQ(organizer.isValuable(outdir), false);
            }
            { // Let it do its thing
            	char* argv[] = {
	                (char*) "video-organizer",
	                (char*) indir.c_str(),
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
                organizer.run();
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
                Organizer organizer(args);
                EQ(organizer.isValuable(indir), indirPreserved[i]);
                EQ(organizer.isValuable(outdir), false);
            }
        }
        { // Undo the normal run
            // The workspace was prepared by the last test
            { // Let it do its thing
            	char* argv[] = {
	                (char*) "video-organizer",
	                (char*) outdir.c_str(),
	                (char*) "-v",
	                (char*) "-1",
	                (char*) "-o",
	                (char*) indir.c_str(),
	                (char*) "-r",
	                (char*) actions[i],
	                (char*) "-u",
                };
                int argc = sizeof(argv) / sizeof(*argv);
                Args args(argc, argv);
                Organizer organizer(args);
                organizer.run();
            }
            { // Check results
                Args args;
                FileMan fileman(args);
                for (int j = 0; j < outfilec; j++) {
                    string outfile = outdir + outfiles[j];
                    string outfiledir = Parser::directory(outfile);
                    EQ(fileman.exists(outfile), false);
                }
                Organizer organizer(args);
                EQ(organizer.isValuable(indir), true);
                EQ(organizer.isValuable(outdir), false);
            }
        }
        { // Simulated run
            { // Prepare workspace
                Args args;
                FileMan fileman(args);
                fileman.remove_all(getdir());
                for (int j = 0; j < infilec; j++)
                    fileman.touch(indir + infiles[j]);
                fileman.dig(outdir);
                Organizer organizer(args);
                EQ(organizer.isValuable(indir), true);
                EQ(organizer.isValuable(outdir), false);
            }
            { // Let it do its thing
            	char* argv[] = {
	                (char*) "video-organizer",
	                (char*) indir.c_str(),
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
                organizer.run();
            }
            { // Check results
                Args args;
                FileMan fileman(args);
                Organizer organizer(args);
                EQ(fileman.exists(outdir), true);
                EQ(fileman.isEmpty(outdir), true);
                EQ(organizer.isValuable(indir), true);
                EQ(organizer.isValuable(outdir), false);
            }
        }
        { // Run with --clean
            { // Prepare workspace
                Args args;
                FileMan fileman(args);
                fileman.remove_all(getdir());
                for (int j = 0; j < infilec; j++)
                    fileman.touch(indir + infiles[j]);
                fileman.dig(outdir);
                Organizer organizer(args);
                EQ(organizer.isValuable(indir), true);
                EQ(organizer.isValuable(outdir), false);
            }
            { // Let it do its thing
            	char* argv[] = {
	                (char*) "video-organizer",
	                (char*) indir.c_str(),
	                (char*) "-v",
	                (char*) "-1",
	                (char*) "-o",
	                (char*) outdir.c_str(),
	                (char*) "-r",
	                (char*) actions[i],
	                (char*) "--clean=1M",
                };
                int argc = sizeof(argv) / sizeof(*argv);
                Args args(argc, argv);
                Organizer organizer(args);
                organizer.run();
            }
            { // Check partial results
                Args args;
                FileMan fileman(args);
                EQ(fileman.isEmpty(outdir), false);
                EQ(fileman.exists(indir), indirPreserved[i]);
            }
            { // Prepare workspace
                Args args;
                FileMan fileman(args);
                fileman.dig(indir);
            }
            { // Let it undo its thing
            	char* argv[] = {
	                (char*) "video-organizer",
	                (char*) outdir.c_str(),
	                (char*) "-v",
	                (char*) "-1",
	                (char*) "-o",
	                (char*) indir.c_str(),
	                (char*) "-r",
	                (char*) actions[i],
	                (char*) "-u",
	                (char*) "--clean=1M",
                };
                int argc = sizeof(argv) / sizeof(*argv);
                Args args(argc, argv);
                Organizer organizer(args);
                organizer.run();
            }
            { // Check results
                Args args;
                FileMan fileman(args);
                Organizer organizer(args);
                EQ(fileman.isEmpty(indir), false);
                EQ(fileman.exists(outdir), false);
                EQ(organizer.isValuable(indir), true);
            }
        }
	}
}







