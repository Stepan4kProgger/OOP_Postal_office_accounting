#include "Classes.h"

Human::Human() : unique_id(0) {}
void Human::setName(string str) { name = str; }
string Human::getName() { return name; }
void Human::setId(USINT ushort) { unique_id = ushort; }
USINT Human::getId() { return unique_id; }

User::User() : admin_priveleges(false) {}
void User::setLogin(string str) { login = str; }
string User::getLogin() { return login; }
void User::setPassword(string str) { password = str; }
string User::getPassword() { return password; }
void User::setAdmin(bool state) { admin_priveleges = state; }
bool User::isAdmin() { return admin_priveleges; }

PostWorker::PostWorker() {}
PostWorker::PostWorker(string& line) {
	stringstream ss(line);
	string buf;
	getline(ss, buf, '\t');	setLogin(buf);
	getline(ss, buf, '\t');	setPassword(buf);
	getline(ss, buf, '\t');	setAdmin((bool)stoi(buf));
	getline(ss, buf, '\t'); setActivity(buf);
	getline(ss, buf, '\t'); setName(buf);
	if (getName() == ENDSCAPE_DEF) return;
	getline(ss, buf, '\t'); setPost(buf);
	getline(ss, buf, '\t'); setId(stoi(buf));
}
string PostWorker::toString() {
	return string(getLogin() + '\t' + getPassword() + '\t' + to_string(isAdmin()) + '\t' + getActivity() + '\t'
		+ ((getName() == ENDSCAPE_DEF) ? getName() + '\t' :
			getName() + '\t' + getPost() + '\t' + to_string(getId())) + '\n');
}
void PostWorker::setPost(string str) { post = str; }
string PostWorker::getPost() { return post; }
void PostWorker::setActivity(string str) { last_activity = str; }
string PostWorker::getActivity() { return last_activity; }

Order::Order() : is_delivered(false), code(0) {}
Order::Order(string& line) {
	stringstream ss(line);
	getline(ss, type, '\t');
	getline(ss, sent_from, '\t');
	getline(ss, sent_to, '\t');
	getline(ss, line, '\t'); is_delivered = (bool)stoi(line);
	getline(ss, line); code = std::stoul(line);
}
string Order::toString() {
	return string(type + '\t' + sent_from + '\t' + sent_to + '\t'
		+ to_string(is_delivered) + '\t' + to_string(code));
}
Order::~Order() {}
void Order::setType(string str) { type = str; }
string Order::getType() { return type; }
void Order::setSender(string str) { sent_from = str; }
string Order::getSender() { return sent_from; }
void Order::setReceiver(string str) { sent_to = str; }
string Order::getReceiver() { return sent_to; }
void Order::defineDelivered(bool state) { is_delivered = state; }
bool Order::isDelivered() { return is_delivered; }
void Order::setCode(unsigned val) { code = val; }
unsigned Order::getCode() { return code; }

Client::Client(){}
Client::Client(string& line) { //3 rows required
	stringstream ss(line);
	string buf;
	getline(ss, access_code, '\t');
	getline(ss, buf, '\t'); setName(buf);
	getline(ss, buf, '\n'); setId(stoi(buf));
	getline(ss, buf, '\n');
	stringstream l_ss(buf);
	while (getline(l_ss, buf, '#'))
		orders.push_back(Order(buf));
	getline(ss, buf, '\n');
	l_ss = stringstream(buf);
	while (getline(l_ss, buf, '\t'))
		updates.push_back(buf);
}
Client::Client(unique_ptr<Client> this_client) : orders(this_client->getOrders()), updates(this_client->getUpdates()) {
	while (true) {
		char answer = Menu::ShowMenu(string("����� ����������, " + this_client->getName() + "! �������� ��������"),
			"����������� ������� ������",
			"����������� �����������",
			Menu::END());
		system("cls");
		if (answer == 1) {
			if (orders.empty())
				cout << "� ��� ����������� ������!";
			else {
				string header[6] = { "�", "���", "�����������", "����������", "����������", "���" };
				Table table(ORDERS, header);
				for (USINT i = 0; i < orders.size(); i++) {
					string buf_l[6] = { to_string(i + 1), orders[i].getType(),
						orders[i].getSender(), orders[i].getReceiver(),
						orders[i].isDelivered() ? "��" : "���", to_string(orders[i].getCode()) };
					table.drawTableText(buf_l);
				}
				table.drawTableBorder(2);
			}
		}
		else if (answer == 2) {
			if (updates.empty())
				cout << "��������� �������� �����������\n";
			else {
				cout << "������� �� �������� �� ��� " << this_client->getName()
					<< ":\n� - ������ - ����, �����\n===\n";
				USINT counter = 1;
				for (string val : updates)
					cout << counter++ << ". " << val << "\n---\n";
			}
		}
		else break;
		system("pause");
	}
}

bool Client::edit(bool& changes_on) {
	while (true) {
		USINT answer = Menu::ShowMenu(string("�� ��������� ��� " + getName() + ". �������� ��������"),
			"�������� �������� ��������",
			"���������� / �������� ������������ �������� ��������",
			"�������� ���",
			"�������� ��� �������",
			"������� �������",
			Menu::END());
		system("cls");
		if (answer == 1) {
			answer = Menu::ShowMenu("��������, ����� �������� ��������",
				"�������",
				"����",
				Menu::END());
			system("cls");
			if (answer == 1) {
				cout << "���������� ��������� ��� ����. �������� ���� ����� �������� ������� ESC\n";
				try {
					Order order;
					order.setType(Inputs::kirillicInput("������� ��� �������"));
					order.setSender(Inputs::kirillicInput("������� ����� �����������", true));
					order.setReceiver(Inputs::kirillicInput("������� ����� ����������", true));
					order.setCode(Inputs::numInput("������� ��� �������", 99999999));
					order.defineDelivered(false);
					orders.push_back(order);
					updates.push_back(string("��������� ������� - " + order.getType() + " - " + Inputs::receiveTime()));
					changes_on = true;
					cout << "������� ���������\n";
				}
				catch (...) {
					cout << "���� �������, ���������� ���������\n";
				}
			}
			else if (answer == 2) {
				try {
					updates.push_back(string(Inputs::kirillicInput("������� ��� ��������") + " - " + Inputs::receiveTime()));
					changes_on = true;
					cout << "�������� ���������\n";
				}
				catch (...) {
					cout << "���� �������, ���������� ���������\n";
				}
			}
		}
		else if (answer == 2) {
			if (orders.empty())
				cout << "� ������� ����������� �������� ��������, ��������� ��� ��������������\n";
			else {
				string header[6] = { "�", "���", "�����������", "����������", "����������", "���" };
				Table table(ORDERS, header);
				for (USINT i = 0; i < orders.size(); i++) {
					string buf_l[6] = { to_string(i + 1), orders[i].getType(),
						orders[i].getSender(), orders[i].getReceiver(),
						orders[i].isDelivered() ? "��" : "���", to_string(orders[i].getCode()) };
					table.drawTableText(buf_l);
				}
				table.drawTableBorder(2);
				answer = Inputs::numInput("������� ����� �������� ��� �������������� (0 - �����)");
				if (!answer--)
					continue;
				if (orders[answer].isDelivered()) {
					cout << "������� ����������, �������� � ����������\n";
					system("pause");
					continue;
				}
				char answer_2 = Menu::ShowMenu(string("��� �������� � \"" + orders[answer].getType() + "\"?"),
					"������� ������������",
					Menu::END());
				if (answer_2 == 1) {
					try {
						PostMan poster;
						cout << "����� �������� ������� ��� ������������, ������� ������ ����������\n";
						poster.setAdress(orders[answer].getReceiver());
						cout << "������������ ����� �� ���������? (" << poster.getAdress() << ")\n����� ������ ����, ������� 1 | �������� �� ��������� - ������� 2\n";
						if (_getch() == '1')
							poster.setAdress(Inputs::kirillicInput("������� ����� ��������", true));
						else
							cout << "����������� ����� �� ���������\n";
						poster.setName(Inputs::kirillicInput("������� ��� ����������"));
						updates.push_back(string("������� ����������. ��������� - " + poster.getName() + ", ����� - " + poster.getAdress() + " - " + Inputs::receiveTime()));
						orders[answer].defineDelivered(true);
						changes_on = true;
						cout << "���������� �������\n";
					}
					catch (...) {
						cout << "���������� ��������\n";
					}
				}
				else break;
				system("pause");
			}
		}
		else if (answer == 3) {
			try {
				setName(Inputs::kirillicInput("������� ��� ������������ (ESC - ������)"));
				updates.push_back(string("��� ������������ �������� - " + Inputs::receiveTime()));
				changes_on = true;
				cout << "������� ��������\n";
			}
			catch (...) {
				cout << "���� �������\n";
			}
		}
		else if (answer == 4) {
			cout << "�� ��������, ��� ������ �������� ��� ������� �������?\n��� ������������� ������� 1\n� ���� ������ - ESC\n";
			if (_getch() != '1') {
				cout << "��� �� ��� �������\n";
				continue;
			}
			setAccessCode(Inputs::generateAccessCode());
			cout << "����� ��� ������� ��� �������:\n";
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);
			cout << Inputs::EnCrypt(getAccessCode());
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
			cout << "\n��������� �������\n";
			updates.push_back(string("��� ������� - " + Inputs::receiveTime()));
			changes_on = true;
			system("pause");
		}
		else if (answer == 5) {
			cout << "�� �������, ��� ������ ������� ������� �� �������?\n��� ������������� ������� 1\n� ���� ������ - ESC\n";
			if (_getch() == '1') {
				cout << "������� ������ ������� �������\n";
				changes_on = true;
				return true;
			}
			else cout << "�������� ��������\n";
		}
		else break;
		system("pause");
	}
	return false;
}
Client::~Client() {}
string Client::toString() {
	string main_dat(access_code + '\t' + getName() + '\t' + to_string(getId()));
	string order_dat;
	string upd_dat;
	for (Order obj : orders)
		order_dat += obj.getType() + '\t' + obj.getSender() + '\t' + obj.getReceiver() + '\t'
		+ to_string(obj.isDelivered()) + '\t' + to_string(obj.getCode()) + '#';
	for (string obj : updates)
		upd_dat += obj + '\t';
	return main_dat + '\n' + order_dat + '\n' + upd_dat + '\n';
}
vector<Order>& Client::getOrders() { return orders; }
vector<string>& Client::getUpdates() { return updates; }
void Client::setAccessCode(string val) { access_code = val; }
string Client::getAccessCode() { return access_code; }

void PostMan::setAdress(string str) { deliv_adress = str; }
string PostMan::getAdress() { return deliv_adress; }
string PostMan::toString() { return ""; }