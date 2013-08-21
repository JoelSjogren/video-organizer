/**********************************
*  console.h                      *
*                                 *
*  Provides conditional output.   *
**********************************/
// TODO: do we really need all the constness?
#pragma once
#include "args.h"
#include "colormod.h"
#include <string>
#include <cstdio>
#include <sstream>
class Console {
    const class Args& args;
public:
	Console(const Args& pargs);
	enum Verbosity {
	    SILENT = -2,
	    ERROR,
	    WARNING,
	    VERBOSE,
	    DEBUG,
	}; // TODO move to Args
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
	std::string str(T t) const;
};
template <class T>
std::string Console::str(T t) const {
    std::ostringstream os;
    os << t;
    return os.str();
}
