#pragma once
#include <string>
class Console {
	int verbosity;
public:
	Console(int pverbosity=0) : verbosity(pverbosity) {}
	void setVerbosity(int pv) { verbosity = pv; }
	// fatal, error, warning, verbose, debug
	void f(std::string, ...); // always printed
	void e(std::string, ...);
	void w(std::string, ...);
	void v(std::string, ...);
	void d(std::string, ...);
};
