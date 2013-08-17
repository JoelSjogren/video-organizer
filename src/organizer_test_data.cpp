#include "organizer_test.h"
using std::string;
const char* OrganizerTest::infiles[] = {
    "Chuck.S05E01.HDTV.XviD-LOL.avi",
    "Community.S04E02.HDTV.x264-LOL.mp4",
    "subdir/Chuck.s05E02.HDTV.XviD-LOL.avi", // note '.s05'
    "The.Prestige.2006.720p.Bluray.x264.anoXmous.mp4",
};
const int OrganizerTest::infilec = sizeof(infiles) / sizeof(*infiles);
const char* OrganizerTest::outfiles[] = {
    "Chuck/S05/E01.avi",
    "Community/S04/E02.mp4",
    "Chuck/S05/E02.avi",
    "The Prestige/CD01.mp4",
};
const char* OrganizerTest::outfiles_ABC[] = { // --manual-name ABC
    "ABC/S05/E01.avi",
    "ABC/S04/E02.mp4",
    "ABC/S05/E02.avi",
    "ABC/CD01.mp4",
};
const int OrganizerTest::outfilec = sizeof(outfiles) /
                                    sizeof(*outfiles);
const char* OrganizerTest::actions[] = { "--move", "--copy", "--link"};
const int OrganizerTest::actionc = sizeof(actions) / sizeof(*actions);
const bool OrganizerTest::indirPreserved[] = { false, true, true };
