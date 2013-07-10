#pragma once
#include <string>
std::string extractDirectory(std::string whole);
// e.g. "somedir/Film.2010.mp4" -> "Film.2010.mp4"
std::string extractFilename(std::string whole);
