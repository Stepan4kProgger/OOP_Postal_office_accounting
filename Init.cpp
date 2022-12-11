#include "Init.h"

Session::Session(vector<PostWorker>& vect, vector<Client>& clients, unique_ptr<PostWorker> this_user) :
worker_vect(vect), client_vect(clients), changes_on(false) {
	iterator_on_user = worker_vect.begin();
	while (iterator_on_user->getLogin() != this_user->getLogin())
		iterator_on_user++;
	if (iterator_on_user->isAdmin()) {
		while (true) {
			char answer = Menu::ShowMenu("����� ����������, �������������! �������� ��������",
				"���������� ������� ������",
				"����������� ������� ������",
				"����� ������������� � ������� ������",
				"�������������� ������� �������",
				"����������� ������� ������",
				"����� � ������� �������",
				"������� � ����������� ������������",
				Menu::END());
			system("cls");
			if (answer == 1) {
				viewUsers();
			}
			else if (answer == 2) {
				validateUsers();
			}
			else if (answer == 3) {
				unconfirmUsers();
			}
			else if (answer == 4) {
				editUsers();
			}
			else if (answer == 5) {
				sortUsers();
			}
			else if (answer == 6) {
				searchUsers();
			}
			else if (answer == 7) {
				userFunc();
			}
			else break;
			system("pause");
		}
	}
	else {
		userFunc(true);
	}
}

Session::Session() : changes_on(false) {}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=USER BLOCK
void Session::userFunc(bool user_call) {
	while (true) {
		char answer = Menu::ShowMenu((user_call ? "����� ����������, " + iterator_on_user->getName() + "! �������� ��������" : "���������������� ����. �������� ��������"),
			"����������� ���� �����������",
			"����������� ���� ��������",
			"�������� �������",
			"��������� �������",
			"�������� ������ ����� ������� ������",
			Menu::END());
		system("cls");
		if (answer == 1) {
			viewWorkers();
		}
		else if (answer == 2) {
			viewClients();
		}
		else if (answer == 3) {
			addClient();
		}
		else if (answer == 4) {
			editClient();
		}
		else if (answer == 5) {
			changeUserData();
		}
		else break;
		system("pause");
	}
}

void Session::viewWorkers() {
	for (USINT i = 0; i < worker_vect.size(); i++)
		if (worker_vect[i].getName() != ENDSCAPE_DEF)
			buf.push_back(worker_vect[i]);
	if (buf.empty()) {
		cout << "� ������� ����������� ���������\n";
		return;
	}
	string header[4] = { "�", "���", "���������", "��������� ����������" };
	Table table(WORKERS, header);
	for (USINT i = 0; i < buf.size(); i++) {
		string buf_l[4] = { to_string(i + 1), buf[i].getName(), buf[i].getPost(), buf[i].getActivity() };
		table.drawTableText(buf_l);
	}
	table.drawTableBorder(2);
	buf.clear();
}

void Session::viewClients() {
	if (client_vect.empty()) {
		try {
			fillClients();
		}
		catch (Exc ex)	{
			cout << ex;
			return;
		}
	}
	string header[4] = { "�", "��� �������", "���������� �������", "���������� ��������" };
	Table table(CLIENTS, header);
	for (USINT i = 0; i < client_vect.size(); i++) {
		string buf_l[4] = { to_string(i + 1), client_vect[i].getName(),
			to_string(client_vect[i].getOrders().size()), to_string(client_vect[i].getUpdates().size()) };
		table.drawTableText(buf_l);
	}
	table.drawTableBorder(2);
}

void Session::addClient() {
	Client client;
	try {
		client.setName(Inputs::kirillicInput("������� ��� ������������"));
	}
	catch (...) {
		cout << "���� �������\n";
		return;
	}
	client.setId(receiveAvaliableClientId());
	client.setAccessCode(Inputs::generateAccessCode());
	cout << "������ ������. ��� ��� ��� �����:\n";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);
	cout << Inputs::EnCrypt(client.getAccessCode());
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	cout << "\n��������� �������\n";
	system("pause");
	changes_on = true;
	client_vect.push_back(client);
}

void Session::editClient() {
	viewClients();
	if (client_vect.empty())
		return;
	USINT answer = Inputs::numInput("������� ����� ������� ��� ����������� (0 - ������)", client_vect.size());
	if (!answer--)
		return;
	if (client_vect[answer].edit(changes_on))
		client_vect.erase(client_vect.begin() + answer);
}

void Session::changeUserData() {
	char answer = Menu::ShowMenu("��������, ��� ��������",
		"�����",
		"������",
		Menu::END());
	system("cls");
	try {
		if (answer == 1) {
			while (true) {
				string login = Inputs::loginInput();
				vector<PostWorker>::iterator iter = worker_vect.begin();
				for (; iter != worker_vect.end(); iter++)
					if (iter->getLogin() == login) {
						cout << "����� ��� �����. ����������� ������\n";
						system("pause");
						continue;
					}
				iterator_on_user->setLogin(login);
				changes_on = true;
				cout << "����� ������� ������\n";
				break;
			}
		}
		else if (answer == 2) {
			string password = Inputs::passwordInput();
			if (password != Inputs::passwordInput("��������� ���� ������")) {
				cout << "������ ������ ���������! ��������� ��������\n";
				return;
			}
			iterator_on_user->setPassword(password);
			changes_on = true;
			cout << "������ ������� ������. ��������� ���\n";
		}
		else return;
	}
	catch (...) {
		cout << "���� �������\n";
	}
}

USINT Session::receiveAvaliableClientId() {
	USINT id;
	for (id = 0; id < client_vect.size(); id++)
		if (client_vect[id].getId() != id)
			break;
	return id;
}
USINT Session::receiveAvaliableUserId() {
	USINT id;
	for (id = 0; id < worker_vect.size(); id++)
		if (worker_vect[id].getId() != id)
			break;
	return id;
}

//-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-ADMIN BLOCK
void Session::viewUsers() {
	string header[5] = { "�", "�����", "������� �������", "���", "��������� ����������" };
	Table table(USERS, header);
	for (short i = 0; i < worker_vect.size(); i++) {
		string buf[] = { to_string(i + 1),
			worker_vect[i].getLogin(),
			worker_vect[i].isAdmin() ? "�������������" : "������������",
			worker_vect[i].getName() == ENDSCAPE_DEF ? "�� ����������" : worker_vect[i].getName(),
			worker_vect[i].getActivity() };
		table.drawTableText(buf);
	}
	table.drawTableBorder(2);
}

void Session::validateUsers() {
	vector<int> comp;
	for (short i = 0; i < worker_vect.size(); i++)
		if (!worker_vect[i].isAdmin() && worker_vect[i].getName() == ENDSCAPE_DEF) {
			buf.push_back(worker_vect[i]);
			comp.push_back(i);
		}
	if (buf.empty()) {
		cout << "�� ������� ��������������� �������\n";
		return;
	}
	string header[] = { "�", "�����", "������� �������", "������", "��������� ����������" };
	Table table(USERS, header);
	for (short i = 0; i < buf.size(); i++) {
		string buf_l[] = { to_string(i + 1),
			buf[i].getLogin(), "������������", "�� ����������",
			buf[i].getActivity() };
		table.drawTableText(buf_l);
	}
	table.drawTableBorder(2);
	USINT answer = Inputs::numInput("������� ����� ������������ ��� ������������� (0 - ������)", buf.size());
	if (!answer--) {
		buf.clear();
		return;
	}
	try {
		cout << "��������� ���� ��� �������������\n";
		buf[answer].setName(Inputs::kirillicInput("������� ��� ����������"));
		buf[answer].setPost(Inputs::kirillicInput("������� ��������� ����������"));
		buf[answer].setId(receiveAvaliableUserId());
	}
	catch (...) { cout << "���� �������\n"; buf.clear(); return; }
	worker_vect[comp[answer]] = buf[answer];
	changes_on = true;
	buf.clear();
	cout << "������������ ����������\n";
}

void Session::unconfirmUsers() {
	vector<int> comp;
	for (short i = 0; i < worker_vect.size(); i++)
		if (worker_vect[i].getName() != ENDSCAPE_DEF) {
			buf.push_back(worker_vect[i]);
			comp.push_back(i);
		}
	if (buf.empty()) {
		cout << "�� ������� ������������� �������?\n";
		return;
	}
	string header[5] = { "�", "�����", "������� �������", "���", "��������� ����������" };
	Table table(USERS, header);
	for (short i = 0; i < buf.size(); i++) {
		string buf_l[] = { to_string(i + 1),
			buf[i].getLogin(), "������������", buf[i].getName(),
			buf[i].getActivity() };
		table.drawTableText(buf_l);
	}
	table.drawTableBorder(2);
	USINT answer = Inputs::numInput("������� ����� ������������ ��� ������ ������������� (0 - ������)", buf.size());
	if (!answer--) {
		buf.clear();
		return;
	}
	buf[answer].setName(ENDSCAPE_DEF);
	buf[answer].setPost("");
	worker_vect[comp[answer]] = buf[answer];
	changes_on = true;
	buf.clear();
	cout << "������������ ����\n";
}

void Session::editUsers() {
	string header[5] = { "�", "�����", "������� �������", "���", "��������� ����������" };
	Table table(USERS, header);
	for (short i = 0; i < worker_vect.size(); i++) {
		string buf[] = { to_string(i + 1),
			worker_vect[i].getLogin(),
			worker_vect[i].isAdmin() ? "�������������" : "������������",
			worker_vect[i].getName() == ENDSCAPE_DEF ? "�� ����������" : worker_vect[i].getName(),
			worker_vect[i].getActivity() };
		table.drawTableText(buf);
	}
	table.drawTableBorder(2);
	USINT answer = Inputs::numInput("������� ����� ���������� ��� �������������� (0 - ������)", worker_vect.size());
	if (answer-- == 0)
		return;
	if (worker_vect[answer].getLogin() == iterator_on_user->getLogin()) {
		cout << "�� �� ������ ������������� ���� ������� ������ ����� �������. �������� ��� ����� ���������������� �������\n";
		return;
	}
	vector<PostWorker>::iterator ptr = worker_vect.begin() + answer;
	while (true) {
		answer = Menu::ShowMenu("��� �� ������ ������� c " + ptr->getLogin() + " ? (ESC - �����)",
			"�������� �����",
			"�������� ������",
			ptr->isAdmin() ? "����� ����� ��������������" : "������ ����� ��������������",
			"������� ������� ������", Menu::END());
		system("cls");
		if (answer == 1) {
			try {
				ptr->setLogin(Inputs::loginInput());
			}
			catch (...) { continue; }
			changes_on = true;
			cout << "����� ������\n";
			system("pause");
		}
		else if (answer == 2) {
			try {
				ptr->setPassword(Inputs::passwordInput());
			}
			catch (...) { continue; }
			changes_on = true;
			cout << "������ ������\n";
			system("pause");
		}
		else if (answer == 3) {
			ptr->setAdmin(!ptr->isAdmin());
			changes_on = true;
		}
		else if (answer == 4) {
			cout << "�� ��������?\n������� 1 ��� �������������\n";
			if (_getch() == '1') {
				worker_vect.erase(ptr);
				cout << "������� ������ �������\n";
			}
			else cout << "�������� ��������\n";
			system("pause");
		}
		else break;
	}
}

string lowercaseStr(string str) {
	for (int i = 0; i < str.size(); i++) {
		if (str[i] >= 'A' && str[i] <= 'Z')
			str[i] += 32;
		else if (str[i] >= '�' && str[i] <= '�')
			str[i] += 32;
		else if (str[i] == '�')
			str[i] = '�';
	}
	return str;
}
bool operator<(string& left, string& right){
	unsigned short minLen = min(left.size(), right.size());
	for (int i = 0; i < minLen; i++) {
		if (left[i] < right[i])
			return false;
		if (left[i] > right[i])
			return true;
	}
	return false;
}
void Session::sortUsers() {
	char answer = Menu::ShowMenu("�������� �������� ����������",
		"�� ����� - �� �����������",
		"�� ����� - �� ��������",
		"�� ������ - �� �����������",
		"�� ������ - �� ��������", Menu::END());
	if (answer == 1)
		sort(worker_vect.begin(), worker_vect.end(),
			[](PostWorker& left, PostWorker& right)-> bool {
				return lowercaseStr(left.getName()) < lowercaseStr(right.getName());
			});
	else if (answer == 2)
		sort(worker_vect.begin(), worker_vect.end(),
			[](PostWorker& left, PostWorker& right)-> bool {
				return !(lowercaseStr(left.getName()) < lowercaseStr(right.getName()));
			});
	else if (answer == 3)
		sort(worker_vect.begin(), worker_vect.end(),
			[](PostWorker& left, PostWorker& right)-> bool {
				return lowercaseStr(left.getLogin()) < lowercaseStr(right.getLogin());
			});
	else if (answer == 4) 
		sort(worker_vect.begin(), worker_vect.end(),
			[](PostWorker& left, PostWorker& right)-> bool {
				return !(lowercaseStr(left.getLogin()) < lowercaseStr(right.getLogin()));
			});
	else return;
	cout << "������� ������������� �� ������ " << answer << ". ";
}

void Session::searchUsers() {
	char answer = Menu::ShowMenu("�������� �������� ��� ������",
		"�� ������",
		"�� �����",
		"�������� �������������",
		"�������� ���������������",
		"�������� ���� �������������",
		"�������� ���� ���������������", Menu::END());
	system("cls");
	if (answer == 1) {
		string str;
		try {
			str = lowercaseStr(Inputs::loginInput("������� ������� ����� (������� �� �����������)"));
		}
		catch (...) { return; }
		for (USINT i = 0; i < worker_vect.size(); i++)
			if (worker_vect[i].getLogin().find(str) != -1)
				buf.push_back(worker_vect[i]);
	}
	else if (answer == 2) {
		string str;
		try {
			str = lowercaseStr(Inputs::kirillicInput("������� ������� ����� (������� �� �����������)"));
		}
		catch (...) { return; }
		for (USINT i = 0; i < worker_vect.size(); i++)
			if (worker_vect[i].getName().find(str) != -1)
				buf.push_back(worker_vect[i]);
	}
	else if (answer == 3) {
		for (USINT i = 0; i < worker_vect.size(); i++)
			if (worker_vect[i].getName() != ENDSCAPE_DEF)
				buf.push_back(worker_vect[i]);
	}
	else if (answer == 4) {
		for (USINT i = 0; i < worker_vect.size(); i++)
			if (worker_vect[i].getName() == ENDSCAPE_DEF)
				buf.push_back(worker_vect[i]);
	}
	else if (answer == 5) {
		for (USINT i = 0; i < worker_vect.size(); i++)
			if (!worker_vect[i].isAdmin())
				buf.push_back(worker_vect[i]);
	}
	else if (answer == 6) {
		for (USINT i = 0; i < worker_vect.size(); i++)
			if (worker_vect[i].isAdmin())
				buf.push_back(worker_vect[i]);
	}
	else return;
	if (buf.empty()) {
		cout << "������ �� ������� �� ������� �������\n";
		return;
	}
	cout << buf.size() << " ����������\n";
	string header[5] = { "�", "�����", "������� �������", "���", "��������� ����������" };
	Table table(USERS, header);
	for (short i = 0; i < buf.size(); i++) {
		string buf_l[] = { to_string(i + 1),
			buf[i].getLogin(),
			buf[i].isAdmin() ? "�������������" : "������������",
			buf[i].getName() == ENDSCAPE_DEF ? "�� ����������" : buf[i].getName(),
			buf[i].getActivity() };
		table.drawTableText(buf_l);
	}
	table.drawTableBorder(2);
	buf.clear();
}

Session::~Session() {
	while (changes_on) {
		cout << "���� ������������ ���������. ��������� ��? ������� ��������������� �������\n\n\t1 - ��\n\t2 - ���\n\n";
		char act = _getch();
		if (act == '1') {
			ofstream file(DAT_USERS, ios::trunc);
			iterator_on_user = worker_vect.begin();
			while (iterator_on_user != worker_vect.end()) {
				file << iterator_on_user->toString();
				iterator_on_user++;
			}
			file.close();
			file.open(DAT_CLIENTS, ios::trunc);
			USINT i = 0;
			while (i < client_vect.size()) {
				file << client_vect[i++].toString();
			}
			cout << "��������� ��������� �������\n";
			changes_on = false;
		}
		else if (act == '2') {
			cout << "��������� �� ���� ���������\n";
			changes_on = false;
		}
		if (!changes_on) cout << "�� ����� �� ������� ������\n";
	}
}

//-=-=-=-=--==-=-=-=-=-=-=-=-=-=-INIT PART

Init::Init() {
	while (true) {
		char answer = Menu::ShowMenu("�������� ��������",
			"�����",
			"������������������",
			"� - ������", Menu::END());
		system("cls");
		if (answer == 1 || answer == 2) {
			cout << "�������� ���� ����� �������� ESC. ���� ������ �� ����������, ������ �� �� ��������\n";
			if (worker_vect.empty()) {
				try { fillVect(); }
				catch (Exc& ex) { cout << ex; system("pause"); continue; }
				catch (...) { continue; }
			}
		}
		if (answer == 1) {
			try {
				cur_login = Inputs::loginInput();
				Session start(worker_vect, client_vect, tryToLogin(Inputs::passwordInput()));
			}
			catch (Exc& ex) { cout << ex; }
			catch (...) { continue; }
		}
		else if (answer == 2) {
			try { tryToReg(); }
			catch (Exc& ex) { cout << ex; }
			catch (...) { continue; }
		}
		else if (answer == 3) {
			if (client_vect.empty()) {
				try { fillClients(); }
				catch (Exc& ex) { cout << ex << "���������� � ���������� �������������� ��� ������� ��������\n"; system("pause"); continue; }
				catch (...) { continue; }
			}
			cout << "�������� ���� ����� �������� ESC. ���� ������ �� ����������, ������ �� �� ��������\n";
			try { Client start(tryClientLogin()); }
			catch (Exc& ex) { cout << ex; }
			catch (...) { continue; }
		}
		else if (answer == 0) {
			return;
		}
		system("pause");
	}
}

Init::~Init() {
	cout << "�������� ���������\n";
}

unique_ptr<PostWorker> Init::tryToLogin(string password) { //� ������ ������ ���������� ��������� �� ������������. ����� - ����������
	vector<PostWorker>::iterator iterator = worker_vect.begin();
	while (iterator != worker_vect.end())
		if (iterator->getLogin() == cur_login)
			if (iterator->getPassword() == password) {
				if (iterator->getName() == ENDSCAPE_DEF && !iterator->isAdmin())
					throw Exc("������� ������ �� ������������. ���������� � ���������� ��������������");
				else {
					iterator->setActivity(Inputs::receiveTime());
					return unique_ptr<PostWorker>(new PostWorker(static_cast<PostWorker>(*iterator)));
				}
			}
			else throw Exc("�������� ������");
		else iterator++;
	throw Exc("������� ������ � ����� ������� �� �������");
}

void Init::tryToReg() {
	PostWorker registrating;
	registrating.setLogin(Inputs::loginInput());
	vector<PostWorker>::iterator iterator = worker_vect.begin();
	while (iterator != worker_vect.end())
		if (iterator->getLogin() == registrating.getLogin())
			throw Exc("����� ��� �����. ����������� ������");
		else iterator++;
	registrating.setPassword(Inputs::passwordInput());
	if (registrating.getPassword() != Inputs::passwordInput("��������� ���� ������"))
		throw Exc("������ �� ���������. ��� ���������� ��������� ��� � ��������");
	registrating.setName(ENDSCAPE_DEF);
	registrating.setActivity(Inputs::receiveTime());
	registrating.setAdmin(false);
	worker_vect.push_back(registrating);
	ofstream file(DAT_USERS, ios::app);
	file << registrating.toString();
	file.close();
	cout << "�������� ����������� ��� ������� " << registrating.getLogin() << endl;
}

unique_ptr<Client> Init::tryClientLogin() { //� ������ ������ ���������� ��������� �� ������������. ����� - ����������
	string code = Inputs::codeInput();
	vector<Client>::iterator iterator = client_vect.begin();
	while (iterator != client_vect.end())
		if (iterator->getAccessCode() == code) {
			return unique_ptr<Client>(new Client(static_cast<Client>(*iterator)));
		}
		else iterator++;
	throw Exc("��� ����� �������, ���� ���������� �������� �� ����������");
}

void Session::fillVect() {
	fstream file(DAT_USERS, ios::in);
	if (!file.is_open()) {
		cout << "!!! ����������� ���� � �������� �������� !!!\n"
			<< "���� ��� ���������� ������� ���, �������� ������� ������ � ������������ � ������������\n"
			<< "�������� �� �������� ������� ������?\n��� ������������� ������� 1 �� ����������\n���� ���� - ������\n";
		if (_getch() != '1')
			throw Exc("������ ���������� ����������. ����������� ���� " + string(DAT_USERS));
		if (Inputs::loginInput() == REQ_LOGIN && Inputs::passwordInput() == REQ_PASSWORD) {
			file.open(DAT_USERS, ios::out);
			file << REQ_LOGIN << '\t' << REQ_PASSWORD << '\t' << '1' << '\t' << Inputs::receiveTime() << '\t' << ENDSCAPE_DEF << '\t' << endl;
			file.close();
			throw Exc("������� ������ �������. ������� ����� ��");
		}
		else throw Exc("�� �������� ������ ���������� ������. ���� �� ��� ������");
	}
	string buf_line;
	while (getline(file, buf_line, '\n')) {
		try {
			worker_vect.push_back(PostWorker(buf_line));
		}
		catch (...) {
			worker_vect.clear();
			file.close();
			throw Exc("���� ��������");
		}
	}
	file.close();
}

void Session::fillClients() {
	ifstream file(DAT_CLIENTS);
	if (!file.is_open())
		throw Exc("���� � ��������� �� ������");
	string line, line_scnd;
	try {
		while (getline(file, line, '\n')) { //main
			getline(file, line_scnd, '\n');
			line += '\n' + line_scnd; //orders
			getline(file, line_scnd, '\n');
			line += '\n' + line_scnd; //comments
			client_vect.push_back(Client(line));
		}
	}
	catch (...) {
		file.close();
		throw Exc("���� � ��������� ��������", 1);
	}
	file.close();
	if (client_vect.empty())
		throw Exc("���� � �������� ����");
}