#include "Inputs.h"

bool Inputs::isNumeric(char& act) {
	return act >= '0' && act <= '9';
}
bool Inputs::isSpecSymb(char& act) {
	for (char spec : { '@', '#', '$', '%', '^', '*' })
		if (act == spec) return true;
	return false;
}
bool Inputs::isLoginLetter(char& act) {
	return (act >= 'A' && act <= 'Z') || (act >= 'a' && act <= 'z');
}
bool Inputs::isValidLetter(char& act) {
	return (act >= 'A' && act <= 'Z') || (act >= 'a' && act <= 'z')
		|| (act >= 'А' && act <= 'Я') || (act >= 'а' && act <= 'я')
		|| act == ' ' || act == '\'' || act == '`'
		|| act == 'Ё' || act == 'ё';
}
bool Inputs::isKirillic(char& act) {
	return (act >= 'А' && act <= 'Я')
		|| (act >= 'а' && act <= 'я')
		|| act == 'Ё' || act == 'ё'
		|| act == '-' || act == ' ' || act == '.';
}

void Inputs::Crypt(string& pas) {
	char atrib = 1;
	for (unsigned char i = 0; i < pas.length(); i++) {
		if (i % 3 == 0)
			if (atrib == 1) atrib = -1;
			else atrib = 1;
		pas[i] = (int)pas[i] + atrib * (i % 3 + 1);
	}
}

void Inputs::clearRow(HANDLE& hConsole) {
	CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
	GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
	cout << '\r' << string(consoleInfo.srWindow.Right - consoleInfo.srWindow.Left + 1, ' ') << '\r';
}

string Inputs::receiveTime() {
	time_t rawtime;
	struct tm* timeinfo;
	char buffer[20];
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	strftime(buffer, sizeof(buffer), "%d.%m.%Y %H:%M:%S", timeinfo);
	return string(buffer);
}

string Inputs::loginInput(const char title[]) {
	string str;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	byte USERFIELD_MAX_LENGTH = 32, curColor = 15;
	{
		CONSOLE_SCREEN_BUFFER_INFO consInfo;
		if (GetConsoleScreenBufferInfo(hConsole, &consInfo))
			curColor = consInfo.wAttributes;
	}
	bool limit_reached = false;
	cout << title << ":\n";
	while (true) {
		char act = _getch();
		if (act == 27) throw nullptr;
		if (act == 13) {
			if (str == string()) continue;
			if (limit_reached) {
				clearRow(hConsole);
				cout << str;
			}
			cout << endl;
			break;
		}
		if (act == -32 && _kbhit()) {
			act = _getch();
			continue;
		}
		if (act == 8) {
			if (!str.length()) continue;
			str.resize(str.length() - 1);
			if (limit_reached) {
				clearRow(hConsole);
				cout << str;
				limit_reached = false;
			}
			else
				cout << "\b \b";
		}
		else {
			if (!isLoginLetter(act) && !isNumeric(act)) continue;
			if (limit_reached) continue;
			if (str.length() >= USERFIELD_MAX_LENGTH) {
				limit_reached = true;
				clearRow(hConsole);
				SetConsoleTextAttribute(hConsole, 12);
				cout << str;
				SetConsoleTextAttribute(hConsole, curColor);
				cout << "\t\aСлишком много символов для этого поля!";
				continue;
			}
			str += act;
			cout << act;
		}
	}
	return str;
}

string Inputs::passwordInput(const char title[]) { //Returns crypted value
	string str;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	byte USERFIELD_MAX_LENGTH = 32, curColor = 15;
	{
		CONSOLE_SCREEN_BUFFER_INFO consInfo;
		if (GetConsoleScreenBufferInfo(hConsole, &consInfo))
			curColor = consInfo.wAttributes;
	}
	char PASSWORD_STAR = '*';
	bool limit_reached = false;
	cout << title << ":\n";
	while (true) {
		char act = _getch();
		if (act == 27) throw nullptr;
		if (act == 13) {
			if (str == string()) continue;
			if (limit_reached) {
				clearRow(hConsole);
				cout << string(str.length(), PASSWORD_STAR);
			}
			cout << endl;
			break;
		}
		if (act == -32 && _kbhit()) {
			act = _getch();
			continue;
		}
		if (act == 8) {
			if (!str.length()) continue;
			str.resize(str.length() - 1);
			if (limit_reached) {
				clearRow(hConsole);
				cout << string(str.length(), PASSWORD_STAR);
				limit_reached = false;
			}
			else
				cout << "\b \b";
		}
		else {
			if (!isLoginLetter(act) && !isNumeric(act) && !isSpecSymb(act)) continue;
			if (limit_reached) continue;
			if (str.length() >= USERFIELD_MAX_LENGTH) {
				limit_reached = true;
				clearRow(hConsole);
				SetConsoleTextAttribute(hConsole, 12);
				cout << string(str.length(), PASSWORD_STAR);
				SetConsoleTextAttribute(hConsole, curColor);
				cout << "\t\aСлишком много символов для этого поля!";
				continue;
			}
			str += act;
			cout << PASSWORD_STAR;
		}
	}
	Crypt(str);
	return str;
}

string Inputs::codeInput(const char title[]) { //Returns crypted value
	string str;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	byte CODE_MAX_LENGTH = 8, curColor = 15;
	{
		CONSOLE_SCREEN_BUFFER_INFO consInfo;
		if (GetConsoleScreenBufferInfo(hConsole, &consInfo))
			curColor = consInfo.wAttributes;
	}
	char PASSWORD_STAR = '*';
	bool limit_must_be = false;
	cout << title << ":\n";
	while (true) {
		char act = _getch();
		if (act == 27) throw nullptr;
		if (act == 13) {
			if (str == string()) continue;
			if (str.length() != 8) {
				clearRow(hConsole);
				SetConsoleTextAttribute(hConsole, 12);
				cout << string(str.length(), PASSWORD_STAR);
				SetConsoleTextAttribute(hConsole, curColor);
				cout << "\t\aСлишком мало символов для этого поля!";
				limit_must_be = true;
				continue;
			}
			cout << endl;
			break;
		}
		if (act == -32 && _kbhit()) {
			act = _getch();
			continue;
		}
		if (act == 8) {
			if (!str.length()) continue;
			str.resize(str.length() - 1);
			if (limit_must_be) {
				clearRow(hConsole);
				cout << string(str.length(), PASSWORD_STAR);
				limit_must_be = false;
			}
			else
				cout << "\b \b";
		}
		else {
			if (!isNumeric(act)) continue;
			if (str.length() == CODE_MAX_LENGTH - 1) {
				str += act;
				Crypt(str);
				cout << endl;
				return str;
			}
			if (limit_must_be) {
				clearRow(hConsole);
				cout << string(str.length(), PASSWORD_STAR);
				limit_must_be = false;
			}
			str += act;
			cout << PASSWORD_STAR;
		}
	}
	Crypt(str);
	return str;
}

string Inputs::kirillicInput(const char title[], bool num_avaliable) {
	string str;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	byte NAME_MAX_LENGTH = 128, curColor = 15;
	{
		CONSOLE_SCREEN_BUFFER_INFO consInfo;
		if (GetConsoleScreenBufferInfo(hConsole, &consInfo))
			curColor = consInfo.wAttributes;
	}
	bool limit_reached = false;
	cout << title << ":\n";
	while (true) {
		char act = _getch();
		if (act == 27) throw nullptr;
		if (act == 13) {
			if (str == string()) continue;
			if (limit_reached) {
				clearRow(hConsole);
				cout << str;
			}
			cout << endl;
			break;
		}
		if (act == -32 && _kbhit()) {
			act = _getch();
			continue;
		}
		if (act == 8) {
			if (!str.length()) continue;
			str.resize(str.length() - 1);
			if (limit_reached) {
				clearRow(hConsole);
				cout << str;
				limit_reached = false;
			}
			else
				cout << "\b \b";
		}
		else {
			if (!isKirillic(act) && !(num_avaliable && isNumeric(act))) continue;
			if (limit_reached) continue;
			if (str.length() >= NAME_MAX_LENGTH) {
				limit_reached = true;
				clearRow(hConsole);
				SetConsoleTextAttribute(hConsole, 12);
				cout << str;
				SetConsoleTextAttribute(hConsole, curColor);
				cout << "\a";
				continue;
			}
			str += act;
			cout << act;
		}
	}
	return str;
}

unsigned Inputs::numInput(const char title[], unsigned max_val, unsigned exc) {
	unsigned inp;
	cout << title << ":\n";
	while ((!(cin >> inp) || inp > max_val) && inp != exc) {
		if (!cin) {
			cout << "Ошибочные данные. ";
			cin.clear();
			cin.ignore(5, '\n');
		}
		else
			cout << "Выход за пределы значений. ";
		cout << "Повторите ввод:\n";
	}
	cin.clear();
	cin.get();
	return inp;
}

string Inputs::EnCrypt(string field) {
	char atrib = -1;
	for (unsigned char i = 0; i < field.length(); i++) {
		if (i % 3 == 0)
			if (atrib == 1) atrib = -1;
			else atrib = 1;
		field[i] = (int)field[i] + atrib * (i % 3 + 1);
	}
	return field;
}

string Inputs::generateAccessCode() {
	string code;
	byte i = 0;
	srand(time(0));
	while (i++ < 8)
		code += (char)(48 + rand() % 10);
	Crypt(code);
	return code;
}