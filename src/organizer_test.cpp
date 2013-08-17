/**********************************
*  organizer_test.cpp             *
**********************************/
#include "organizer_test.h"
#include "organizer.h"
#include "args.h"
#include "fileman.h"
#include "parser.h"
#include <cassert>
#include <iostream>
#include <string>
using std::string;
using std::cout;
using std::endl;
OrganizerTest::OrganizerTest() : Test("Organizer"),
    indir(getdir() + "input/"), outdir(getdir() + "output/") {
    sampleRuns();
    isSorted();
}
void OrganizerTest::prepareWorkspace() {
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
    for (int i = 0; i < actionc; i++) {
        { // Shallow output
            for (int j = 0; j < infilec; j++) {
                prepareWorkspace();   
                { // Let it do its thing
                    string infile = indir + infiles[j];
                	char* argv[] = {
	                    (char*) "video-organizer",
	                    (char*) infile.c_str(),
	                    (char*) "-v",
	                    (char*) "-1",
	                    (char*) "-o",
	                    (char*) outdir.c_str(),
	                    (char*) actions[i],
	                    (char*) "--shallow-output",
                    };
                    int argc = sizeof(argv) / sizeof(*argv);
                    Args args(argc, argv);
                    Organizer organizer(args);
                    organizer.run();
                }
                { // Check results
                    Args args;
                    FileMan fileman(args);
                    string outfile = outdir +
                                     Parser::filename(outfiles[j]);
                    string filelist = outdir + "filelist";
                    EQ(fileman.exists(outfile), true);
                    EQ(fileman.exists(filelist), true);
                }
            }
        }
        { // Manual name
            for (int j = 0; j < infilec; j++) {
                prepareWorkspace();       
                { // Let it do its thing
                    string infile = indir + infiles[j];
                	char* argv[] = {
	                    (char*) "video-organizer",
	                    (char*) infile.c_str(),
	                    (char*) "-v",
	                    (char*) "-1",
	                    (char*) "-o",
	                    (char*) outdir.c_str(),
	                    (char*) actions[i],
	                    (char*) "--manual-name",
	                    (char*) "ABC",
                    };
                    int argc = sizeof(argv) / sizeof(*argv);
                    Args args(argc, argv);
                    Organizer organizer(args);
                    organizer.run();
                }
                { // Check results
                    Args args;
                    FileMan fileman(args);
                    string outfile = outdir + outfiles_ABC[j];
                    string outfiledir = Parser::directory(outfile);
                    string filelist = outfiledir + "filelist";
                    EQ(fileman.exists(outfile), true);
                    EQ(fileman.exists(filelist), true);
                }
            }
        }
        { // Normal run
            prepareWorkspace();
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
        { // Run with --clean
            prepareWorkspace();
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
            { // Check final results
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







