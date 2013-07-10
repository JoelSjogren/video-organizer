#include "filename.h"
using std::string;
string extractDirectory(string whole) {
	int lastSlash = whole.rfind("/");
	if (lastSlash == string::npos) return ".";
	return whole.substr(0, lastSlash + 1);
}
string extractFilename(string whole) {
	int lastSlash = whole.rfind('/');
	if (lastSlash == string::npos) return whole;
	return whole.substr(lastSlash + 1, string::npos);
}
