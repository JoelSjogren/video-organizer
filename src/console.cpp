/**********************************
*  console.cpp                    *
**********************************/
#include "console.h"
#include <cstdarg>
#include <cstdio>
#include <cctype>
#include <iostream>
using std::string;
using std::cin;
void Console::f(string msg, ...) const {
    if (true) {
	    fprintf(stderr, "E: ");
	    va_list args;
	    va_start(args, msg);
	    vfprintf(stderr, msg.c_str(), args);
	    va_end(args);
	    fprintf(stderr, "\n");
	}
}
void Console::e(string msg, ...) const {
	if (show_e()) {
		fprintf(stderr, "E: ");
		va_list args;
		va_start(args, msg);
		vfprintf(stderr, msg.c_str(), args);
		va_end(args);
		fprintf(stderr, "\n");
	}
}
void Console::w(string msg, ...) const {
	if (show_w()) {
		printf("W: ");
		va_list args;
		va_start(args, msg);
		vprintf(msg.c_str(), args);
		va_end(args);
		printf("\n");
	}
}
void Console::v(string msg, ...) const {
	if (show_v()) {
		va_list args;
		va_start(args, msg);
		vprintf(msg.c_str(), args);
		va_end(args);
		printf("\n");
	}
}
void Console::d(string msg, ...) const {
	if (show_d()) {
		va_list args;
		va_start(args, msg);
		vprintf(msg.c_str(), args);
		va_end(args);
		printf("\n");
	}
}
void Console::ui(string msg, ...) const {
	if (true) {
		va_list args;
		va_start(args, msg);
		vprintf(msg.c_str(), args);
		va_end(args);
	}
}
bool yn(bool def) {
    const char response = cin.get();
    const char nocase = tolower(response);
    if (nocase == '\n') return def;
    cin.ignore(1000, '\n');
    if (nocase == 'y') return true;
    if (nocase == 'n') return false;
    return false;
}
bool Console::Yn(string msg, ...) const {
	if (true) {
		va_list args;
		va_start(args, msg);
		vprintf(msg.c_str(), args);
		va_end(args);
	}
	return yn(true);
}
bool Console::yN(string msg, ...) const {
	if (true) {
		va_list args;
		va_start(args, msg);
		vprintf(msg.c_str(), args);
		va_end(args);
	}
	return yn(false);
}
bool Console::show_e() const { return ERROR <= args.verbosity; } 
bool Console::show_w() const { return WARNING <= args.verbosity; }
bool Console::show_v() const { return VERBOSE <= args.verbosity; }
bool Console::show_d() const { return DEBUG <= args.verbosity; }
