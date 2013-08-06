/**********************************
*  filelist_test.cpp              *
**********************************/
#include "filelist_test.h"
#include "args.h"
#include "filelist.h"
#include "fileman.h"
#include <dirent.h> // directory browsing
FileListTest::FileListTest() : Test("FileList") {
	const char* records[][2] = {
		{ "Some.Film.2013.asdf.asdf.mkv", "Some Film.mkv" },
		{ "Hello.World.2009.avi", "Hello World.avi" },
		{ "from", "to" },
		{ "My name.srt", "is Joel.srt" },
	};
	const int records_length = sizeof(records) / sizeof(*records);
	const Args args;
	FileMan fileman(args);
	{ // Normal run
        { // Prepare workspace
            fileman.remove_all(getdir());
            getdir();
        }
	    { // Write to filelist
		    FileList fl(getdir(), args);
		    for (int i = 0; i < records_length; i++) {
			    fl.add(records[i][0], records[i][1]);
		    }
		    for (int i = 0; i < records_length; i++) {
			    EQ(fl.get(records[i][1]).from, records[i][0]);
		    }
		    fl.write();
	    } // Close filelist
	    { // Reopen and read filelist
		    FileList fl(getdir(), args);
		    for (int i = 0; i < records_length; i++) {
			    EQ(fl.get(records[i][1]).from, records[i][0]);
		    }
	    }
	}
	{ // Simulated run
        { // Prepare workspace
            fileman.remove_all(getdir());
            getdir();
        }
	    { // Write to filelist
	        Args simargs;
	        simargs.simulate = true;
		    FileList fl(getdir(), simargs);
		    for (int i = 0; i < records_length; i++) {
			    fl.add(records[i][0], records[i][1]);
		    }
		    for (int i = 0; i < records_length; i++) {
			    EQ(fl.get(records[i][1]).from, records[i][0]);
		    }
		    fl.write();
	    } // Close filelist
	    { // Check that the outdir is empty
            EQ(fileman.isEmpty(getdir()), true);
	    }
    }
}



