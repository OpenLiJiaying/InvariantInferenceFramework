#ifndef _COLOR_H_
#define _COLOR_H_
#include <iostream>

#ifdef WIN32
#include <windows.h>
	void set_console_color(std::ostream& out, int choice = 0) {
		HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
		switch (choice) {
		case 0: // yellow
			SetConsoleTextAttribute(hOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
			break;
		case 1: // red
			SetConsoleTextAttribute(hOut, FOREGROUND_RED | FOREGROUND_INTENSITY);
			break;
		}
	}
	void unset_console_color(std::ostream& out) {
		HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	}
#endif


#ifdef linux
	void set_console_color(std::ostream& out, int choice = 0) {
		switch (choice) {
		case 0: // yellow
			out << "\033[33;\x1b[33m";
			break;
		case 1: // red
			out << "\033[33;\x1b[31m";
			break;
		}
		
	}
	void unset_console_color(std::ostream& out) {
		out << "\033[0m"; 
	}
#endif

#endif
