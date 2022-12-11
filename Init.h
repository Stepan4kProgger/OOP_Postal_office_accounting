#pragma once
#include "Classes.h"

class Session {
	void userFunc(bool display_name = false);
	void viewUsers();
	void validateUsers();
	void unconfirmUsers();
	void editUsers();
	void sortUsers();
	void searchUsers();
	bool changes_on;
	vector<PostWorker> buf;
	void viewWorkers();
	void viewClients();
	void addClient();
	void editClient();
	void changeUserData();
	USINT receiveAvaliableClientId();
	USINT receiveAvaliableUserId();
protected:
	vector<PostWorker> worker_vect;
	vector<Client> client_vect;
	vector<PostWorker>::iterator iterator_on_user;
	void fillVect();
	void fillClients();
public:
	Session(vector<PostWorker>&, vector<Client>&, unique_ptr<PostWorker>);
	Session();
	~Session();
};

class Init : public Session {
	string cur_login;
	unique_ptr<PostWorker> tryToLogin(string);
	void tryToReg();
	unique_ptr<Client> tryClientLogin();
public:
	Init();
	~Init();
};