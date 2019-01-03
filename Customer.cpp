//Written by Paul Grandstaff, Jordan Barge, Jack Avera
#include "Customer.h"
#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>

using namespace std;

Customer::Customer() {
	this->accNum = "";
	this->pin = "";
	this->balance = 0;
	this->fname = "";
	this->lname = "";
	this->city = "";
	this->state = "";
	this->phone = "";
}

//Overloaded constructor
Customer::Customer(string fname, string lname, string city, string state, string phone, string accNum, string pin, string balance) {
    this -> accNum = accNum;
    this -> pin = pin;
    this -> balance = stod(balance);
    this -> fname = fname;
    this -> lname = lname;
    this -> city = city;
    this -> state = state;
    this -> phone = phone;
}

void Customer::updateAcc(string fname, string lname, string city, string state, string phone, string accNum, string pin, string balance) {
	this->accNum = accNum;
	this->pin = pin;

	//Checks if balance is "", replace it with 0, otherwise
	//Strip off & and convert the number into a double and store as balance
	if (!balance.compare("")) {
		this->balance = 0;
	}
	else {
		this->balance = stod(balance);
	}

	this->fname = fname;
	this->lname = lname;
	this->city = city;
	this->state = state;
	this->phone = phone;


}

//Returns the fields in account as a string in the format that is in the file
string Customer::toString() {
	stringstream balanceStream; //Used to set precision of the double values for mainBal

	balanceStream << fixed << setprecision(2) << this->balance;

	string mainString = balanceStream.str();

	string toString = this->fname + " " + this->lname + " " + this->city + " " + this->state + " " + this->phone + " " + this->accNum + " " + this->pin + " " + mainString;
	return toString;
}

//Sets the account to the default constructor values
void Customer::emptyAcc() {
	this->accNum = " ";
	this->pin = "";
	this->balance = 0;
}

void Customer::viewCustInfo() {
	cout << "Account: " << accNum << endl;
	cout << "Name: " << fname << " " << lname << endl;
	cout << "City: " << city << endl;
	cout << "State: " << state << endl;
	cout << "Phone: " << phone << endl;
	cout << "Balance: $" << balance << endl;
	cout << endl;
}

