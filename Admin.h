//Written by Paul Grandstaff, Jordan Barge and Jack Avera
#ifndef ADMIN_H
#define ADMIN_H

#include <string>
#include "Customer.h"

using namespace std;

class Admin {
public:
	string accNum; //The account number for the account used to id the account, should be unique.
	string pin; //The account pin number, should be a 4 digit string of integers
	string fname;
	string lname;
	string city;
	string state;
	string phone;
	//Default constructor
	Admin();
	//This method will change the account to have the new fields that are passed to the method
	void updateAcc(string fname, string lname, string city, string state, string phone, string accNum, string pin);
	//Returns the Account as a string, with the same format in the file
	//string toString();
	//This allows Admins to change pins.
	void changePin(string pin);
	//allows the Admin to create new Customer accounts
	Customer createCustomer(string fname, string lname, string city, string state, string phone, string accNum, string pin, string balance);

    string toString();
};
#endif // ADMIN_H

