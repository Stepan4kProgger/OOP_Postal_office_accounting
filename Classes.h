#pragma once
#include "Common.h"

class Human {
	string name;
	USINT unique_id;
public:
	Human();
	void setName(string);
	string getName();
	void setId(USINT);
	USINT getId();
	virtual string toString() = 0;
};

class User {
	string login, password;
	bool admin_priveleges;
public:
	User();
	void setLogin(string);
	string getLogin();
	void setPassword(string);
	string getPassword();
	void setAdmin(bool);
	bool isAdmin();
	virtual string toString() = 0;
};

class PostMan : public Human {
	string deliv_adress;
public:
	void setAdress(string);
	string getAdress();
	string toString();
};

class PostWorker : public virtual User, public virtual Human {
	string post, last_activity; //post in context of duty
public:
	PostWorker();
	PostWorker(string&);
	string toString();
	void setPost(string);
	string getPost();
	void setActivity(string);
	string getActivity();
};

class Order {
	string type, sent_from, sent_to;
	bool is_delivered;
	unsigned code;
public:
	Order();
	Order(string&);
	string toString();
	~Order();
	void setType(string);
	string getType();
	void setSender(string);
	string getSender();
	void setReceiver(string);
	string getReceiver();
	void defineDelivered(bool);
	bool isDelivered();
	void setCode(unsigned);
	unsigned getCode();
};

class Client : public Human {
	vector<Order> orders;
	vector<string> updates;
	string access_code;
public:
	Client();
	Client(string&);
	Client(unique_ptr<Client>);
	void setAccessCode(string);
	string getAccessCode();
	bool edit(bool&);
	~Client();
	string toString();
	vector<Order>& getOrders();
	vector<string>& getUpdates();
};
