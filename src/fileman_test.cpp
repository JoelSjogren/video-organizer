/**********************************
*  fileman_test.cpp              *
**********************************/
#include "fileman_test.h"
#include "fileman.h"
#include <string>
#include <fstream>
//#include <cstring>
using std::ifstream;
using std::ofstream;
using std::string;
const unsigned int random_seed = 10325;
void FileManTest::writeRandom(string outfile, size_t bytec) {
    srand(random_seed);
    const int bufsz = 1000;
    char buf[bufsz];
//    memset(buf, 0, bufsz);
    size_t bufi = 0;
    ofstream ofs(outfile.c_str(), ofstream::binary);
    for (size_t i = 0; i < bytec; i++, bufi++) {
        { // flush
            if (bufi == bufsz) {
                ofs.write(buf, bufi);
                bufi = 0;
            }
        }
        { // store new values
            buf[bufi] = rand() % 256;
        }
    }
    ofs.write(buf, bufi);
}
void FileManTest::checkRandom(string infile, size_t bytec) {
    srand(random_seed);
    const int bufsz = 1000;
    char buf[bufsz];
//    memset(buf, 0, bufsz
    size_t bufi = 0;
    ifstream ifs(infile.c_str(), ifstream::binary);
    size_t readc = 0;
    while (ifs) {
        ifs.read(buf, bufsz);
        for (int i = 0; i < ifs.gcount() && readc < bytec; i++) {
            const char randchar = rand() % 256;
            if (randchar == buf[i]) readc++;
            else { EQ(randchar, buf[i]); }
        }
    }
    EQ(readc, bytec);
}
FileManTest::FileManTest() : Test("FileMan") {
    const size_t bytec = 1024 * 1024 + 243; // ~1M
    Args::Action actions[] = { Args::MOVE, Args::COPY, Args::LINK };
    const int actionc = sizeof(actions) / sizeof(*actions);
	for (int i = 0; i < actionc; i++) {
	    Args args;
	    args.action = actions[i];
	    FileMan fileman(args);
        { // Prepare workspace
            fileman.remove_all(getdir());
            writeRandom(getdir() + "1", bytec);
        }
	    { // Let it do its thing
		    fileman.action(getdir() + "1", getdir() + "2");
	    }
	    { // Check results
		    checkRandom(getdir() + "2", bytec);
	    }
	}
}



