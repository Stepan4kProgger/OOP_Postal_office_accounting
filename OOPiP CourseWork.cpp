#include "Init.h"
int main() {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15); //������������� ���� 240, ���� 15
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	Init start;
	return 0;
}