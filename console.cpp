#include "console.h"
#include <stdarg.h>
#include <stdio.h>
using std::string;
void Console::f(string msg, ...) {
	fprintf(stderr, "E: ");
	va_list args;
	va_start(args, msg);
	vprintf(msg.c_str(), args);
	va_end(args);
	printf("\n");
}
void Console::e(string msg, ...) {
	if (-2 < verbosity) {
		fprintf(stderr, "E: ");
		va_list args;
		va_start(args, msg);
		vprintf(msg.c_str(), args);
		va_end(args);
		printf("\n");
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
