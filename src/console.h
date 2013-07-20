/**********************************
*   console.h                     *
*                                 *
*   Provides conditional output.  *
**********************************/
#pragma once
#include <string>
class Console {
	int verbosity;
public:
	Console(int pverbosity=0) : verbosity(pverbosity) {}
	enum Verbosity {
	    SILENT = -2,
	    ERROR,
	    WARNING,
	    VERBOSE,
	    DEBUG,
	};
	void setVerbosity(int pv) { verbosity = pv; }
	// printf wrappers
	void f(std::string, ...); // always printed
	void e(std::string, ...);
	void w(std::string, ...);
	void v(std::string, ...);
	void d(std::string, ...);
	// fallback if printf is not sufficient
	bool show_f();
	bool show_e();
	bool show_w();
	bool show_v();
	bool show_d();
};
