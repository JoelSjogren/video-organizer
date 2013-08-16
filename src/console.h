/**********************************
*  console.h                      *
*                                 *
*  Provides conditional output.   *
**********************************/
#pragma once
#include "args.h"
#include <string>
#include <sstream>
class Console {
    const class Args& args;
public:
	Console(const Args& pargs) : args(pargs) {}
	enum Verbosity {
	    SILENT = -2,
	    ERROR,
	    WARNING,
	    VERBOSE,
	    DEBUG,
	};
//	void setVerbosity(int pv) { verbosity = pv; }
	// printf wrappers
	void f(std::string, ...) const; // always print
	void e(std::string, ...) const;
	void w(std::string, ...) const;
	void v(std::string, ...) const;
	void d(std::string, ...) const;
	void ui(std::string, ...) const; // always print, no \n
	bool Yn(std::string, ...) const; // always print, no \n, def: yes
	bool yN(std::string, ...) const; // always print, no \n, def: no
	// fallback if printf is not sufficient
	bool show_e() const;
	bool show_w() const;
	bool show_v() const;
	bool show_d() const;
	template <class T>
	std::string str(T t);
};
template <class T>
std::string Console::str(T t) {
    std::ostringstream os;
    os << t;
    return os.str();
}
