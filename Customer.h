//Written by Paul Grandstaff, Jordan Barge and Jack Avera
#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <string>

using namespace std;

class Customer {
public:
	string accNum; //The account number for the account used to id the account, should be unique.
	string pin; //The account pin number, should be a 4 digit string of integers
	double balance; //The balance of the Account's main balance
	string fname;
	string lname;
	string city;
	string state;
	string phone;

	//Default constructor
	Customer();
	//Overloaded constructor
	Customer(string fname, string lname, string city, string state, string phone, string accNum, string pin, string balance);
	//This method will change the account to have the new fields that are passed to the method
	void updateAcc(string fname, string lname, string city, string state, string phone, string accNum, string pin, string balance);
	//Returns the Account as a string, with the same format in the file
	string toString();
	//This will set the account to the default constructor values
	void emptyAcc();
	void viewCustInfo();
};
#endif // CUSTOMER_H

