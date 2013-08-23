/**********************************
*  console.h                      *
*                                 *
*  Provides conditional output.   *
**********************************/
/****************************************************
*  Whenever a file is sorted, a record of the       *
*  operation is written to a file called 'filelist' *
*  in the new directory. This enables FileMan::     *
*  undo() to read the filelist, find the operation  *
*  and undo it.                                     *
*                                                   *
*  The 'filelist' is designed to be user-readable,  *
*  so that the user can open it up in his favorite  *
*  text editor and find more details about the      *
*  file, e.g. its release group.                    *
*                                                   *
*  The 'filelist' is also used in third party       *
*  applications, such as addic7ed-cli (see --help). *
****************************************************/
// TODO: do we really need all the constness?
#pragma once
#include "args.h"
#include "colormod.h"
#include <string>
#include <cstdio>
#include <sstream>
class Console {
    const class Args& args;
	bool show_e() const;
	bool show_w() const;
	bool show_v() const;
	bool show_d() const;
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
	template <class T>
	std::string str(T t) const;
};
template <class T> // TODO should this really be tied to the object?
std::string Console::str(T t) const {
    std::ostringstream os;
    os << t;
    return os.str();
}
