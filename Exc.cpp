#include "Exc.h"
Exc::Exc(const char* info, unsigned char dat) : exception(info), code(dat) {}
Exc::Exc(std::string info) : exception(std::data(info)) {}
unsigned char Exc::getCode() { return code; }
std::ostream& operator<<(std::ostream& ostr, Exc& ex) { ostr << ex.what() << '\n'; return ostr; }