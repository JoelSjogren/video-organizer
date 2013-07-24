/**********************************
*  console.h                      *
*                                 *
*  Provides conditional output.   *
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
	void f(std::string, ...) const; // always printed
	void e(std::string, ...) const;
	void w(std::string, ...) const;
	void v(std::string, ...) const;
	void d(std::string, ...) const;
	// fallback if printf is not sufficient
	bool show_f() const;
	bool show_e() const;
	bool show_w() const;
	bool show_v() const;
	bool show_d() const;
};
