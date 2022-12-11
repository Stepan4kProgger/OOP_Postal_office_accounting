#pragma once
#include <Windows.h>
#include <iostream>
#include <string>
using std::string;
using std::cout;
//DESCRIPTION
//Sizes of columns are in float. The last column is not needed to be written. The first column in symbols, others in percentage.

enum table_type { USERS, WORKERS, CLIENTS, ORDERS };
typedef unsigned short USINT;

class Table {
	float colSizes[6];
	byte colAmount;
	string* header;
	USINT currentConsoleCP;
	byte numberWidth;
	const unsigned char split = (char)179; //179 - │    
	USINT getConsoleWidth();
	string stringSerialize(string, USINT&);
public:
	Table(table_type, string*);
	void drawTableBorder(byte); //Sizes of columns are in float. The last column is not needed to be written. The first column in symbols, others in percentage.
	void drawTableText(string textData[]); //Sizes of columns are in float. The last column is not needed to be written. The first column in symbols, others in percentage.
};