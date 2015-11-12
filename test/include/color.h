#ifndef _COLOR_H_
#define _COLOR_H_
#include <iostream>

#ifdef WIN32
#include <windows.h>
	void set_console_color(std::ostream& out) {
		HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hOut, FOREGROUND_RED | FOREGROUND_GREEN);
	}
	void unset_console_color(std::ostream& out) {
		HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	}
#endif


#ifdef linux
	void set_console_color(std::ostream& out) {
		out << "\033[31;4m"; 
	}
	void unset_console_color(std::ostream& out) {
		out << "\033[0m"; 
	}
#endif

#endif
