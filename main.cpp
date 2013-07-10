/**************************
*   video-organizer 1.0   *
*   by Joel Sjögren       *
**************************/
#include "args.h"
#include "organizer.h"
int main(int argc, char* argv[]) {
	Args args(argc, argv);
	Organizer organizer(args);
}
