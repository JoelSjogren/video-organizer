#include "console.h"
#include <stdarg.h>
#include <stdio.h>
using std::string;
void Console::f(string msg, ...) {
	fprintf(stderr, "E: ");
	va_list args;
	va_start(args, msg);
	vfprintf(stderr, msg.c_str(), args);
	va_end(args);
	fprintf(stderr, "\n");
}
void Console::e(string msg, ...) {
	if (-2 < verbosity) {
		fprintf(stderr, "E: ");
		va_list args;
		va_start(args, msg);
		vfprintf(stderr, msg.c_str(), args);
		va_end(args);
		fprintf(stderr, "\n");
	}
}
void Console::w(string msg, ...) {
	if (-1 < verbosity) {
		printf("W: ");
		va_list args;
		va_start(args, msg);
		vprintf(msg.c_str(), args);
		va_end(args);
		printf("\n");
	}
}
void Console::v(string msg, ...) {
	if (0 < verbosity) {
//		printf("V: ");
		va_list args;
		va_start(args, msg);
		vprintf(msg.c_str(), args);
		va_end(args);
		printf("\n");
	}
}
void Console::d(string msg, ...) {
	if (1 < verbosity) {
//		printf("D: ");
		va_list args;
		va_start(args, msg);
		vprintf(msg.c_str(), args);
		va_end(args);
		printf("\n");
	}
}
bool Console::show_f() { return true; }
bool Console::show_e() { return ERROR <= verbosity; } 
bool Console::show_w() { return WARNING <= verbosity; }
bool Console::show_v() { return VERBOSE <= verbosity; }
bool Console::show_d() { return DEBUG <= verbosity; }
