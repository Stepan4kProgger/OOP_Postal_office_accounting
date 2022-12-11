#pragma once
#include <exception>
#include <iostream>
class Exc : public std::exception {
	unsigned char code;
public:
	Exc(const char*, unsigned char code = 0);
	Exc(std::string);
	unsigned char getCode();
	friend std::ostream& operator<<(std::ostream&, Exc&);
};