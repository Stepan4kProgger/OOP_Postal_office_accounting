#include "Table.h"

USINT Table::getConsoleWidth() {
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &consoleInfo)) {
        return consoleInfo.srWindow.Right - consoleInfo.srWindow.Left + 1;
    }
    return 16;
}

string Table::stringSerialize(string text, USINT& width) {
    if (text.length() > width) {
        text.resize(width - 3);
        return text + "...";
    }
    if (text.length() < width)
        text += string(width - text.length(), ' ');
    return text;
}

Table::Table(table_type type, string* header) {
    colSizes[0] = 4.f;
    if (type == USERS) {
        colSizes[1] = colSizes[2] = .15f; colSizes[3] = .25f;
        colAmount = 4;
        //USER_DISPLAY_VAL[4] = { 4.f, .15f, .15f, .25f }; //# log is_adm Name LastAct
    }
    else if (type == WORKERS) {
        colSizes[1] = colSizes[2] = .3f;
        colAmount = 3;
        //WK_DISPLAY_VAL[3] = { 4.f, .3f, .3f }; //# name post last_act
    }
    else if (type == CLIENTS) {
        colSizes[1] = .45f; colSizes[2] = .25f;
        colAmount = 3;
        //CL_DISPLAY_VAL[3] = { 4.f, .6f, .2f}; //# name ord_am upd_am
    }
    else if (type == ORDERS) {
        colSizes[1] = .17f; colSizes[2] = colSizes[3] = .2f; colSizes[4] = .1f;
        colAmount = 5;
        //ORD_DISPLAY_VAL[5] = { 4.f, .17f, .2f, .2f, .1f}
    }
    currentConsoleCP = GetConsoleOutputCP();
    numberWidth = (int)colSizes[0];
    USINT consWidth = getConsoleWidth() - numberWidth + 1, cursorPosition = 1;
    const unsigned char split = (char)179; //179 - │
    drawTableBorder(0);
    for (int i = 0; i < colAmount; i++) {
        USINT colsWidthS = ceil(consWidth * colSizes[i]);
        if (i == 0) colsWidthS = numberWidth + 1;
        SetConsoleOutputCP(866);
        cout << split;
        SetConsoleOutputCP(currentConsoleCP);
        cout << stringSerialize(header[i], colsWidthS);
        cursorPosition += colsWidthS + 1;
    }
    SetConsoleOutputCP(866);
    cout << split;
    SetConsoleOutputCP(currentConsoleCP);
    USINT lastColWidth = getConsoleWidth() - cursorPosition - 1;
    cout << stringSerialize(header[colAmount], lastColWidth);
    SetConsoleOutputCP(866);
    cout << split;
    SetConsoleOutputCP(currentConsoleCP);
    drawTableBorder(1);
}

void Table::drawTableBorder(byte type) {
    USINT consWidth = getConsoleWidth() - numberWidth + 1,
        cursorPosition = numberWidth + 1;
    SetConsoleOutputCP(866);
    const unsigned char borderSymbs[3][4] = { {(char)196 /* ─ */, (char)218 /* ┌ */, (char)194 /* ┬ */, (char)191 /* ┐ */},  // type = 0 верхушка
                                              {(char)205 /* ═ */, (char)198 /* ╞ */, (char)216 /* ╪ */, (char)181 /* ╡ */},  // type = 1 шапка таблицы
                                              {(char)196 /* ─ */, (char)192 /* └ */, (char)193 /* ┴ */, (char)217 /* ┘ */} };// type = 2 низ
    cout << borderSymbs[type][1] << string(numberWidth + 1, borderSymbs[type][0]) << borderSymbs[type][2]; //для N
    for (int i = 1; i < colAmount; i++) {
        USINT colsWidthS = consWidth * colSizes[i] + 1;
        cout << string(colsWidthS, borderSymbs[type][0]) << borderSymbs[type][2];
        cursorPosition += colsWidthS;
    }
    cout << string(getConsoleWidth() - cursorPosition - 7, borderSymbs[type][0]) << borderSymbs[type][3] << std::endl;
    SetConsoleOutputCP(currentConsoleCP);
}

void Table::drawTableText(string text[]) {
    USINT consWidth = getConsoleWidth() - numberWidth + 1, cursorPosition = 1;
    for (int i = 0; i < colAmount; i++) {
        USINT colsWidthS = ceil(consWidth * colSizes[i]);
        if (i == 0) colsWidthS = numberWidth + 1;
        SetConsoleOutputCP(866);
        cout << split;
        SetConsoleOutputCP(currentConsoleCP);
        cout << stringSerialize(text[i], colsWidthS);
        cursorPosition += colsWidthS + 1;
    }
    SetConsoleOutputCP(866);
    cout << split;
    SetConsoleOutputCP(currentConsoleCP);
    USINT lastColWidth = getConsoleWidth() - cursorPosition - 1;
    cout << stringSerialize(text[colAmount], lastColWidth);
    SetConsoleOutputCP(866);
    cout << split << std::endl;
    SetConsoleOutputCP(currentConsoleCP);
}