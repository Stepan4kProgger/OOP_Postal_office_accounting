#include "Menu.h"

void Menu::setCur(SHORT x, SHORT y) {
	COORD cursor = { x,y };
	HANDLE hWndConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hWndConsole, cursor);
}

unsigned short Menu::getConsoleHeight() {
	HANDLE hWndConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD size = { 0,0 };
	CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
	if (GetConsoleScreenBufferInfo(hWndConsole, &consoleInfo)) {
		return consoleInfo.srWindow.Bottom - consoleInfo.srWindow.Top + 1;
	}
	throw "bruh";
}

const char* Menu::END() { return "#"; }

byte Menu::ShowMenu(string title, ...) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	va_list vl;  //получаем список переменных
	va_start(vl, title); // получаем указатель на ...
	list<string> menuItems; //list строк menu
	string menuItem = va_arg(vl, char*);
	while (menuItem != END()) {
		menuItems.emplace_back(menuItem);
		menuItem = va_arg(vl, char*);
	}
	byte menuItemsCount = menuItems.size(); //получаем количество пунктов меню
	byte pos = 1, prev = 0, curColor = 15; //pos - текущий элемент меню, prev - предыдущий
	char menuIndex = -1;
	{
		CONSOLE_SCREEN_BUFFER_INFO consInfo;
		if (GetConsoleScreenBufferInfo(hConsole, &consInfo))
			curColor = consInfo.wAttributes;
	}
	system("cls");
	cout << title << ":";

	SetConsoleTextAttribute(hConsole, 31);
	setCur(0, getConsoleHeight() - 2);
	cout << "Используйте стрелки для навигации и нажмите Enter (или пробел), либо нажмите цифру на клавиатуре. ESC - переход назад";

	while (menuIndex == -1) {
		if (pos != prev) { // не перерисовывать если ничего не изменилось
			short i = 1;
			for (string& txt : menuItems) {
				setCur(2, i + 1);
				if (pos == i) SetConsoleTextAttribute(hConsole, 255 - curColor);
				else SetConsoleTextAttribute(hConsole, curColor); //белый
				cout << i++ << ". " << txt;
			}
			prev = pos;
		}
		setCur(0, getConsoleHeight() - 1);  // опустить курсор
		SetConsoleTextAttribute(hConsole, curColor);
		char key = _getch();
		if (key > '0' && key <= menuItemsCount + 48) { //по цифре
			pos = key - 48;
			if (pos == prev)
				menuIndex = pos;
		}
		else if (key == -32 && _kbhit()) {
			key = _getch();
			if (key == 72 && pos > 1) pos--;
			else if (key == 80 && pos < menuItemsCount) pos++;
		}
		else if (key == 13 || key == 32)
			menuIndex = pos;
		else if (key == 27)
			menuIndex = 0;
	}
	system("cls");
	return menuIndex;
}