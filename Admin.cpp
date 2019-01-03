//Written by Paul Grandstaff, Jordan Barge and Jack Avera
#include "Admin.h"
#include <string>
#include <sstream>
#include <iomanip>
#include "Customer.h"

using namespace std;

Admin::Admin() {
	this->accNum = "";
	this->pin = "";
	this->fname = "";
	this->lname = "";
	this->city = "";
	this->state = "";
	this->phone = "";
}

void Admin::updateAcc(string fname, string lname, string city, string state, string phone, string accNum, string pin) {
	this->accNum = accNum;
	this->pin = pin;
	this->fname = fname;
	this->lname = lname;
	this->city = city;
	this->state = state;
	this->phone = phone;
}

void Admin::changePin(string pin) {
	this->pin = pin;
}

Customer Admin::createCustomer(string fname, string lname, string city, string state, string phone, string accNum, string pin, string balance) {

	Customer newCustomer(fname, lname, city, state, phone, accNum, pin, balance);

	return newCustomer;
}

//Returns the fields in account as a string in the format that is in the file
string Admin::toString() {
    string toString = "admin " + this->fname + " " + this->lname + " " + this->city + " " + this->state + " " + this->phone + " " + this->accNum + " " + this->pin;
	return toString;
}

