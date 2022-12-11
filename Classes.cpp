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
		char answer = Menu::ShowMenu(string("Добро пожаловать, " + this_client->getName() + "! Выберите действие"),
			"Просмотреть текущие заказы",
			"Просмотреть подробности",
			Menu::END());
		system("cls");
		if (answer == 1) {
			if (orders.empty())
				cout << "У вас отсутствуют заказы!";
			else {
				string header[6] = { "№", "Тип", "Отправитель", "Получатель", "Доставлена", "Код" };
				Table table(ORDERS, header);
				for (USINT i = 0; i < orders.size(); i++) {
					string buf_l[6] = { to_string(i + 1), orders[i].getType(),
						orders[i].getSender(), orders[i].getReceiver(),
						orders[i].isDelivered() ? "Да" : "Нет", to_string(orders[i].getCode()) };
					table.drawTableText(buf_l);
				}
				table.drawTableBorder(2);
			}
		}
		else if (answer == 2) {
			if (updates.empty())
				cout << "Новостные сведения отсутствуют\n";
			else {
				cout << "Выписка по аккаунту на имя " << this_client->getName()
					<< ":\n№ - Сводка - Дата, время\n===\n";
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
		USINT answer = Menu::ShowMenu(string("Вы работаете над " + getName() + ". Выберите действие"),
			"Добавить почтовую операцию",
			"Посмотреть / изменить существующие почтовые операции",
			"Изменить имя",
			"Изменить код доступа",
			"Удалить клиента",
			Menu::END());
		system("cls");
		if (answer == 1) {
			answer = Menu::ShowMenu("Выберите, какую операцию добавить",
				"Посылка",
				"Иное",
				Menu::END());
			system("cls");
			if (answer == 1) {
				cout << "Необходимо заполнить все поля. Прервать ввод можно нажатием клавиши ESC\n";
				try {
					Order order;
					order.setType(Inputs::kirillicInput("Введите тип посылки"));
					order.setSender(Inputs::kirillicInput("Введите адрес отправителя", true));
					order.setReceiver(Inputs::kirillicInput("Введите адрес получателя", true));
					order.setCode(Inputs::numInput("Введите код посылки", 99999999));
					order.defineDelivered(false);
					orders.push_back(order);
					updates.push_back(string("Добавлена посылка - " + order.getType() + " - " + Inputs::receiveTime()));
					changes_on = true;
					cout << "Посылка добавлена\n";
				}
				catch (...) {
					cout << "Ввод прерван, добавление пропущено\n";
				}
			}
			else if (answer == 2) {
				try {
					updates.push_back(string(Inputs::kirillicInput("Введите тип операции") + " - " + Inputs::receiveTime()));
					changes_on = true;
					cout << "Операция добавлена\n";
				}
				catch (...) {
					cout << "Ввод прерван, добавление пропущено\n";
				}
			}
		}
		else if (answer == 2) {
			if (orders.empty())
				cout << "У клиента отсутствуют почтовые операции, доступные для редактирования\n";
			else {
				string header[6] = { "№", "Тип", "Отправитель", "Получатель", "Доставлена", "Код" };
				Table table(ORDERS, header);
				for (USINT i = 0; i < orders.size(); i++) {
					string buf_l[6] = { to_string(i + 1), orders[i].getType(),
						orders[i].getSender(), orders[i].getReceiver(),
						orders[i].isDelivered() ? "Да" : "Нет", to_string(orders[i].getCode()) };
					table.drawTableText(buf_l);
				}
				table.drawTableBorder(2);
				answer = Inputs::numInput("Введите номер операции для редактирования (0 - назад)");
				if (!answer--)
					continue;
				if (orders[answer].isDelivered()) {
					cout << "Посылка доставлена, изменить её невозможно\n";
					system("pause");
					continue;
				}
				char answer_2 = Menu::ShowMenu(string("Что изменить в \"" + orders[answer].getType() + "\"?"),
					"Сделать доставленным",
					Menu::END());
				if (answer_2 == 1) {
					try {
						PostMan poster;
						cout << "Чтобы оформить посылку как доставленную, введите данные почтальона\n";
						poster.setAdress(orders[answer].getReceiver());
						cout << "Использовать адрес по умолчанию? (" << poster.getAdress() << ")\nЧтобы ввести иной, нажмите 1 | Оставить по умолчанию - нажмите 2\n";
						if (_getch() == '1')
							poster.setAdress(Inputs::kirillicInput("Введите адрес доставки", true));
						else
							cout << "Использован адрес по умолчанию\n";
						poster.setName(Inputs::kirillicInput("Введите имя почтальона"));
						updates.push_back(string("Посылка доставлена. Почтальон - " + poster.getName() + ", адрес - " + poster.getAdress() + " - " + Inputs::receiveTime()));
						orders[answer].defineDelivered(true);
						changes_on = true;
						cout << "Оформление успешно\n";
					}
					catch (...) {
						cout << "Оформление отменено\n";
					}
				}
				else break;
				system("pause");
			}
		}
		else if (answer == 3) {
			try {
				setName(Inputs::kirillicInput("Введите имя пользователя (ESC - отмена)"));
				updates.push_back(string("Имя пользователя изменено - " + Inputs::receiveTime()));
				changes_on = true;
				cout << "Успешно изменено\n";
			}
			catch (...) {
				cout << "Ввод прерван\n";
			}
		}
		else if (answer == 4) {
			cout << "Вы уверенны, что хотите обновить код доступа клиента?\nДля подтверждения нажмите 1\nВ ином случае - ESC\n";
			if (_getch() != '1') {
				cout << "Код не был обновлён\n";
				continue;
			}
			setAccessCode(Inputs::generateAccessCode());
			cout << "Новый код доступа для клиента:\n";
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);
			cout << Inputs::EnCrypt(getAccessCode());
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
			cout << "\nПередайте клиенту\n";
			updates.push_back(string("Код доступа - " + Inputs::receiveTime()));
			changes_on = true;
			system("pause");
		}
		else if (answer == 5) {
			cout << "Вы уеренны, что хотите удалить клиента из системы?\nДля подтверждения нажмите 1\nВ ином случае - ESC\n";
			if (_getch() == '1') {
				cout << "Учётная запись клиента удалена\n";
				changes_on = true;
				return true;
			}
			else cout << "Удаление отменено\n";
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