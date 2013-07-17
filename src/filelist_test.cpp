#include "filelist_test.h"
#include "args.h"
#include "filelist.h"
#include <cassert>
#include <iostream>
using std::cout;
FileListTest::FileListTest() : Test("FileList") {
	const char* records[][2] = {
		{ "Some.Film.2013.asdf.asdf.mkv", "Some Film.mkv" },
		{ "Hello.World.2009.avi", "Hello World.avi" },
		{ "from", "to" },
		{ "My name.srt", "is Joel.srt" },
	};
	const int records_length = sizeof(records) / sizeof(*records);
	Args args;
	{
		FileList fl(getdir(), args);
		for (int i = 0; i < records_length; i++) {
			fl.add(records[i][0], records[i][1]);
		}
		for (int i = 0; i < records_length; i++) {
			EQ(fl.get(records[i][1]).from, records[i][0]);
		}
		fl.write();
	} // Close filelist
	{ // Reopen filelist
		FileList fl(getdir(), args);
		for (int i = 0; i < records_length; i++) {
			EQ(fl.get(records[i][1]).from, records[i][0]);
		}
	}
}
