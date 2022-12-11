#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <string>
#include <iostream>
#include <conio.h>
using std::string;
using std::cout;
using std::cin;
using std::endl;

typedef unsigned short USINT;

class Inputs {
	static bool isNumeric(char&);
	static bool isSpecSymb(char&);
	static bool isLoginLetter(char&);
	static bool isValidLetter(char&);
	static bool isKirillic(char&);
	static void Crypt(string&);
	static void clearRow(HANDLE&);
public:
	static string receiveTime();
	static string loginInput(const char title[] = "¬ведите логин");
	static string passwordInput(const char title[] = "¬ведите пароль"); //Returns crypted value
	static string codeInput(const char title[] = "¬ведите свой код");
	static string kirillicInput(const char title[] = "¬ведите строку с кириллицей", bool num_avaliable = false);
	static unsigned numInput(const char title[] = "¬ведите значение", unsigned max_val = 200000, unsigned exc = 0);
	static string EnCrypt(string);
	static string generateAccessCode();
};
