#pragma once
#include <windows.h>
#include <list>
#include <iostream>
#include <conio.h>
using std::string;
using std::list;
using std::cout;

class Menu {
	static void setCur(SHORT, SHORT);
	static unsigned short getConsoleHeight();
public:
	static const char* END();
	static byte ShowMenu(string, ...);
};