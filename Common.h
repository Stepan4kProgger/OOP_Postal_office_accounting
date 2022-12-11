#pragma once
#include "Exc.h"
#include "Menu.h"
#include "Table.h"
#include "Inputs.h"
#include <iomanip>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
using std::fixed;
using std::setprecision;
using std::stringstream;
using std::ifstream;
using std::ofstream;
using std::fstream;
using std::ios;
using std::vector;
using std::find;
using std::to_string;
using std::getline;
using std::stoi;
using std::unique_ptr;

const char DAT_USERS[] = "PostWorkerData.txt";
const char DAT_CLIENTS[] = "PostClientData.txt";

const char REQ_LOGIN[] = "admin";
const char REQ_PASSWORD[] = "0/.";
const char ENDSCAPE_DEF[] = "#";