//Written by: Paul Grandstaff, Jordan Barge, & Jack Avera
/*This program will simulate an ATM interface.
It will display a login screen that will prompt the user will enter
an account number (or enter "E" to exit the program) then will prompt
the user to enter their pin, and will move to the next screen if the
user enters correct account and password. The 2nd screen displays a
list of actions the user can enter to do with their account. The user
must type an integer corresponding to the number on the list and press
enter. The user can check their balance, make a deposit, make a
withdrawal, change their pin, transfer money between accounts, or exit.*/

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdio>
#include <vector>
#include <regex>
#include <iomanip>>
#include "Customer.h"
#include "Admin.h"

using namespace std;

vector<Customer> customerList;
int custIndex = -1;
Admin admin;
const int NUM_OF_ACC_FIELDS = 8; //The number of fields an account has

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//	PROTOTYPE FUNCTIONS
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main();				//Main

void header(); 			//Prints the opening of the title screen
void greetingEnd(); 	//Prints the end of the title screen
void customerMenu(); 	//Moves the ATM Customer Function Menu where the customer can make choices to do with their account
void adminMenu(); 		//Moves the ATM Admin Function Menu where the admin can make choices to do with their account

void balance(Customer &); 	//Displays users balance
void deposit(Customer &); 	//Allows user to deposit money into main account
void withdrawal(Customer &);//Allows user to withdrawal money from main account
void changePin(Customer &); //Allows user to change their account pin
void transfer(); 			//Allows user to move money from one account to the other

void changeAdminPin(); 		//allows admins to change their pin
void changeCustomerInfo(); 	//allows admins to changes a specific customer's info
void viewCustomerInfo();	//views a report of all customers in the system
void printTop5Accounts();	//views a report of the 5 accounts with the most savings
void createNewCustomer();	//allows admins to create new customers
void deleteCustomer();		//allows admins to delete customers
void showTotal();			//displays how much money all customers have combined

void initialFileLoad(); 			//reads everything from file, loads customerList
int  checkId(const string input);	//checks that the ID provided is actually stored in our "database"
bool checkPin(const string input);	//checks that the PIN a user entered is valid for the given account
void stopFunction(); 				//Displays the exit message to the user
int rewriteFile(); 					//Rewrites file after account info has changed
double getDouble(); 				//Gets a double value from the user
void pressEnter(); 					//Handles the user pressing enter when prompted
bool goodPIN(string PIN); 			//ensures PINs are not of the form 1111 or 2222


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//	UTILITY FUNCTIONS
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main() {
	bool keepRunning = true;//Change to false when the user enters "E" to exit program
	string input; //Will hold the input from the user

	initialFileLoad();

	while (keepRunning) {
		//Prints intro to the program and prompt for user account input
		header();

		cout << "Enter \"C\" for Customer or \"A\" for Administrator Menu or " << endl;
		cout << "\"E\" to exit the program: ";
		getline(cin, input);

		//Checks if the user entered "E" to exit program
		if (!input.compare("E")) {
			keepRunning = false;
			cout << "Exiting the program.";
			continue;
		}
		else if (!input.compare("C")) {
			customerMenu();

		}
		else if (!input.compare("A")) {
			adminMenu();

		}
		else {
			cout << endl << "You did not enter a valid response." << endl;
			cout << endl;
			pressEnter();
			cout << flush;
			system("CLS");
		}

		cout << flush;
		system("CLS");
	}

	rewriteFile();

	return 0;
}

//ensures PINs are not of the form 1111 or 2222
bool goodPIN(string PIN) {

	if (PIN.at(0) == PIN.at(1) && PIN.at(1) == PIN.at(2) && PIN.at(2) == PIN.at(3)) {
		return false;
	}
	else {
		return true;
	}
}

//Rewrites file after account info has changed
int rewriteFile() {
	ofstream rewrite;
	rewrite.open("CustomerData.txt");

	//Checks if the new file opened successfully
	if (!rewrite) {
		cout << "File CustomerData.txt could not be opened." << endl;
		return -2;
	}

    for(int i = 0; i < customerList.size(); i++) {
        rewrite << customerList[i].toString() << "\n";
    }

    rewrite << admin.toString();

	return 0;
}

//Handles the user pressing enter when prompted
void pressEnter() {
	string enterPressed; //Used for when the user presses enter to continue
    cout << "Press Enter to continue...." << endl;
	getline(cin, enterPressed);

}

//Retrieves a double amount from the user
double getDouble() {
	bool invalidValue = true; //Change to false when the user enters a valid double value
	double amtEntered;

	while (invalidValue) {
		cin >> amtEntered;

		if (cin.fail()) {
			invalidValue = true;
			cin.clear();
			cin.ignore(256, '\n');

			cout << "You entered an invalid amount." << endl;
			cout << "Only enter numbers and a decimal if needed: $";
		}
		else {
			invalidValue = false;
			cin.ignore();
		}
	}

	return amtEntered;
}

//reads everything from file, loads customerList and adminList
void initialFileLoad() {
	fstream ioFile; //Read/Write file "CustomerData.txt"
	ioFile.open("CustomerData.txt"); //Opens ioFile, if the file is not found, print error and return -1

	if (!ioFile) {
		cout << "File not found, exiting program." << endl;
		//return -1;
	}
	else {
		string fileLine; //Holds the line taken from the file

		//Reads file until account number is found, or until end of file.
		while (!ioFile.eof()) {
			getline(ioFile, fileLine);

			vector<string> THING;
			istringstream stringSplit(fileLine); //Used to split strings

			for (string fileline; stringSplit >> fileLine;) {
				THING.push_back(fileLine);
			}//end for

			if (!THING[0].compare("admin")) {
                admin.updateAcc(THING[1], THING[2], THING[3], THING[4], THING[5], THING[6], THING[7]);
			} else {
				/*Create a new Customer object, load it with data from file, then push it to CustomerList to be used later*/
				Customer cust(THING[0], THING[1], THING[2], THING[3], THING[4], THING[5], THING[6], THING[7]);
				customerList.push_back(cust); //
			}//end else
		}//end while
	}//end else

	ioFile.close();
}

//checks that the PIN a user entered is valid for the given account
bool checkPin(const string input) {
	if (!customerList.at(custIndex).pin.compare(input)) { return true; }
	else { cout << "-->Incorrect PIN. Try Again." << endl; return false;  }
}

//checks that the ID provided is actually stored in our "database"
/* If a match is found, return that index, else return -1*/
int checkId(const string input) {
	for (int i = 0; i < customerList.size(); i++) {
		if (!customerList.at(i).accNum.compare(input)) {
            return i;
		}
	}

	cout << "-->No such ID exists. Try Again." << endl;
	return -1;

}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//	CUSTOMER FUNCTIONS
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//User can send money from one account to the other
void transfer() {
    double transAmt; //Holds the value that the user wants to transfer
    string accChoice;
	bool invalidAcc = true; //Change to false when the user enters "Main" or "Other"
    bool result;

	cout << "Execute Transfer Money" << endl;

	do {
        cout << "Enter amount to transfer: $";

        //Gets the amount the user wants to transfer
        transAmt = getDouble();

        result = (customerList[custIndex].balance - transAmt) < 0;

        if(result) {
            cout << "You do not have enough in your account to transfer. Enter a smaller number." << endl;

        }
	} while(result);

	//Prompts user to enter "Main" or "Other" account to transfer from
	while (invalidAcc) {
		cout << "Enter the account number of the account to transfer funds to: ";
		getline(cin, accChoice);

		//Checks if the user entered a valid account number
		int tempIndex = checkId(accChoice);

		//Checks if accChoice.id is "", which means the account was not found
		if (tempIndex == -1) {
			cout << "Could not find account." << endl << endl;
			continue;
		}
		else {
            cout << "Transferring $" << transAmt << " from account " + customerList[custIndex].accNum + " to account " + customerList[tempIndex].accNum << endl;
			customerList[custIndex].balance -= transAmt;
			customerList[tempIndex].balance += transAmt;

            cout << "Balance of " << customerList[custIndex].accNum << ": $" << customerList[custIndex].balance << endl;
            cout << "Balance of " << customerList[tempIndex].accNum << ": $" << customerList[tempIndex].balance << endl;
            invalidAcc = false;
		}
	}
}

//User can change their pin
void changePin(Customer & currAcct) {
	string userString; //Holds the value the user wants their new pin to be
	bool isDigitCheck = true; //Changes to false if there is a non-digit character in the userString
	bool invalidPin = true; //Change to false when the user enters a valid pin
	cout << "Execute Change Pin" << endl;
	cout << "Enter New Pin: ";

	//Gets the users input for a pin until the user enters a 4 digit integer
	while (invalidPin) {
		bool isDigitCheck = true; //Changes to false if there is a non-digit character in the userString
		getline(cin, userString);

		//Check if the input is an integer
		for (int i = 0; i < userString.length(); i++) {
			//If not a digit, set isDigitCheck to false;
			if (!isdigit(userString.at(i))) {
				isDigitCheck = false;
			}
		}

				//Checks if length of input is 4, and if isDigitCheck is true
		if (userString.length() == 4 && isDigitCheck) {
			invalidPin = false;
			if (goodPIN(userString)) {
				invalidPin = false;
				continue;
			}
			else {
				invalidPin = true;
				cout << "--> PIN cannot be of the form 1111 or 2222, use more than one distinct digit." << endl;
			}
		}
		else {
			invalidPin = true;
			cout << "--> PIN must be four digits: no more, no less." << endl;
		}

		cout << "Enter New PIN: ";

	}

	//Sets the user's value to their pin
	currAcct.pin = userString;

	cout << "New Pin is " << currAcct.pin << endl;

}

//User can withdraw money from main account
void withdrawal(Customer & currAcct) {
	double withdrawlAmt; //Holds value the user wants to withdraw
	double result;

	cout << "Execute Make a Withdrawal" << endl;

	do {
        cout << "Enter amount to withdraw: $";

        //Gets double amount from user
        withdrawlAmt = getDouble();
        //Subtracts amount from users main balance
        result = currAcct.balance - withdrawlAmt;

        if(result < 0) {
            cout << "You do not have that much in your account, enter a smaller amount." << endl;
        }

	} while(result < 0);

	currAcct.balance = result;

	cout << endl << "New Balance is: $" << currAcct.balance << endl;
}

//User can deposit money
void deposit(Customer & currAcct) {
	double depositAmt; //Holds the value the user wants to deposit.
	cout << "Execute Make a Deposit" << endl;
	cout << "Enter amount to deposit: $";

	//Gets double from user for deposit amount
	depositAmt = getDouble();
	//Adds deposited amount to the account
	currAcct.balance += depositAmt;

	cout << "Money deposited successfully!" << endl;
	cout << "New Balance is $" << currAcct.balance << endl << endl;

}

//Displays users main balance
void balance(Customer & currAcct) {
	cout << "Executed Check Balance" << endl;
	cout << "Balance is $" << currAcct.balance << endl << endl;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//	ADMIN FUNCTIONS
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void showTotal() {
    double total = 0;
    for(int i = 0; i < customerList.size(); i++) {
        total += customerList[i].balance;
    }

    cout << fixed << setprecision(2);
    cout << "Total amount in bank: $" << total << endl;
}

void deleteCustomer() {
    string input;
    int index;

    do {
        cout << "Enter account number of customer to delete: ";
        getline(cin, input);

        index = checkId(input);
    } while (index == -1);

    customerList.erase(customerList.begin() + index);

    cout << "Account deleted." << endl;
}

void changeCustomerInfo() {
    bool cond;
    string fname;
    string lname;
    string city;
    string state;
    string phone;
    string accNum;
    string pin;
    string balance;
    int index;

    do {
        cout << "Enter the account number of the customer: ";
        getline(cin, accNum);
        index = checkId(accNum);
    } while (index == -1);

    cout << "Enter first name of customer: ";
    getline(cin, fname);

    cout << "Enter last name of customer: ";
    getline(cin, lname);

    cout << "Enter city of customer: ";
    getline(cin, city);

    do {
        cout << "Enter state of customer (2 letters): ";
        getline(cin, state);

        if(regex_match(state, regex("[A-Z][A-Z]"))) {
            cond = false;
        } else {
            cout << "Not a valid value. Try again." << endl;
            cond = true;
        }

    } while(cond);

    do {
        cout << "Enter phone number of customer (In format: ###-####): ";
        getline(cin, phone);

        if(regex_match(phone, regex("[0-9][0-9][0-9]-[0-9][0-9][0-9][0-9]"))) {
            cond = false;
        } else {
            cout << "Not a valid value. Try again." << endl;
            cond = true;
        }
    } while(cond);

    do {
        cout << "Enter pin of customer (4 digits): ";
        getline(cin, pin);

        if(regex_match(pin, regex("[0-9][0-9][0-9][0-9]"))) {
            if(!pin.compare("1111") || !pin.compare("2222") || !pin.compare("3333") ||
                !pin.compare("4444") || !pin.compare("5555") || !pin.compare("6666") ||
                !pin.compare("7777") || !pin.compare("8888") || !pin.compare("9999") ||
                !pin.compare("0000")) {
                    cout << "Pin cannot be the same digit 4 times." << endl;
                    cond = true;
            } else {
                cond = false;
            }
        } else {
            cout << "Not a valid value. Try again." << endl;
            cond = true;
        }

    }while(cond);

    do {
        cout << "Enter balance of customer: $";
        getline(cin, balance);

        if(regex_match(balance, regex("[0-9]+\\.*[0-9]?[0-9]?"))) {
            cond = false;
        } else {
            cout << "Not a valid value. Try again." << endl;
            cond = true;
        }

    } while(cond);

    customerList[index].updateAcc(fname, lname, city, state, phone, accNum, pin, balance);
}

void printTop5Accounts() {
    const int length = 5;
    double highest[length];
    int index[length];

    //Initialize index to -1
    for(int i = 0; i < length; i++) {
        index[i] = -1;
    }

    //Cycle through all elements of the customerList
    for(int i = 0; i < customerList.size(); i++) {
        //If first in customerList, initialize highest and index
        if(i == 0) {
            highest[0] = customerList[0].balance;
            index[0] = 0;
        } else {
            //Check sort through all the values of highest[] and update if needed
            for(int j = 0; j < length; j++) {
                //Highest[j] has not been filled in, so automatically goes there
                if(index[j] == -1) {
                    highest[j] = customerList[i].balance;
                    index[j] = i;
                    break;
                //Check if current customer is has higher balance than at highest[j]
                } else {
                    //It is, so put move all values of highest down a one and put
                    //customerList[i].balance in the the spot of highest[j]
                    if(customerList[i].balance > highest[j]) {
                        for(int k = length-1; k >= j; k--) {
                            highest[k+1] = highest[k];
                            index[k+1] = index[k];
                        }

                        highest[j] = customerList[i].balance;
                        index[j] = i;
                        break;
                    }
                }
            }
        }
    }

	cout << "The Top " << length << " Accounts ordered by balance" << endl;
	for(int i = 0; i < length; i++) {
        cout << "\t" << customerList[index[i]].accNum << ": $" << customerList[index[i]].balance << endl;
	}
}

void viewCustomerInfo() {
	cout << "-All Customer Data-" << endl;
	for(int i = 0; i < customerList.size(); i++) {
		customerList[i].viewCustInfo();
    }
}

double getTotalBalance() {
	double totalBalance = 0;
	for(int i = 0; i < customerList.size(); i++) {
		totalBalance += customerList[i].balance;
	}//end for
	return totalBalance;
}

void changeAdminPin() {
	string userString; //Holds the value the user wants their new pin to be
	bool isDigitCheck = true; //Changes to false if there is a non-digit character in the userString
	bool invalidPin = true; //Change to false when the user enters a valid pin
	cout << "Execute Change Pin" << endl;
	cout << "Enter New Pin: ";

	//Gets the users input for a pin until the user enters a 4 digit integer
	while (invalidPin) {
		bool isDigitCheck = true; //Changes to false if there is a non-digit character in the userString
		getline(cin, userString);

		//If user enters anything other than admin as new pin,
		//check normal constraints for a user pin
		if(userString.compare("admin")) {
            //Check if the input is an integer
            for (int i = 0; i < userString.length(); i++) {
                //If not a digit, set isDigitCheck to false;
                if (!isdigit(userString.at(i))) {
                    isDigitCheck = false;
                }
            }

                    //Checks if length of input is 4, and if isDigitCheck is true
            if (userString.length() == 4 && isDigitCheck) {
                invalidPin = false;
                if (goodPIN(userString)) {
                    invalidPin = false;
                    continue;
                }
                else {
                    invalidPin = true;
                    cout << "--> PIN cannot be of the form 1111 or 2222, use more than one distinct digit." << endl;
                }
            }
            else {
                invalidPin = true;
                cout << "--> PIN must be four digits: no more, no less." << endl;
            }

            cout << "Enter New PIN: ";
		} else {
            invalidPin = false;
		}

	}

	//Sets the user's value to their pin
	admin.pin = userString;

	cout << "New Pin is " << admin.pin << endl;

}

void createNewCustomer(){
    bool cond;
    string fname;
    string lname;
    string city;
    string state;
    string phone;
    string accNum;
    string pin;
    string balance;
    int index;

    do {
        cout << "Enter the account number of the customer(6 digits): ";
        getline(cin, accNum);

        if(regex_match(accNum, regex("[0-9][0-9][0-9][0-9][0-9][0-9]"))) {
            cond = false;
        } else {
            cout << "Not a valid value. Try again." << endl;
            cond = true;
        }

    } while (cond);

    cout << "Enter first name of customer: ";
    getline(cin, fname);

    cout << "Enter last name of customer: ";
    getline(cin, lname);

    cout << "Enter city of customer: ";
    getline(cin, city);

    do {
        cout << "Enter state of customer (2 letters): ";
        getline(cin, state);

        if(regex_match(state, regex("[A-Z][A-Z]"))) {
            cond = false;
        } else {
            cout << "Not a valid value. Try again." << endl;
            cond = true;
        }

    } while(cond);

    do {
        cout << "Enter phone number of customer (In format: ###-####): ";
        getline(cin, phone);

        if(regex_match(phone, regex("[0-9][0-9][0-9]-[0-9][0-9][0-9][0-9]"))) {
            cond = false;
        } else {
            cout << "Not a valid value. Try again." << endl;
            cond = true;
        }
    } while(cond);

    do {
        cout << "Enter pin of customer (4 digits): ";
        getline(cin, pin);

        if(regex_match(pin, regex("[0-9][0-9][0-9][0-9]"))) {
            if(!pin.compare("1111") || !pin.compare("2222") || !pin.compare("3333") ||
                !pin.compare("4444") || !pin.compare("5555") || !pin.compare("6666") ||
                !pin.compare("7777") || !pin.compare("8888") || !pin.compare("9999") ||
                !pin.compare("0000")) {
                    cout << "Pin cannot be the same digit 4 times." << endl;
                    cond = true;
            } else {
                cond = false;
            }
        } else {
            cout << "Not a valid value. Try again." << endl;
            cond = true;
        }

    }while(cond);

    do {
        cout << "Enter balance of customer: $";
        getline(cin, balance);

        if(regex_match(balance, regex("[0-9]+\\.*[0-9]?[0-9]?"))) {
            cond = false;
        } else {
            cout << "Not a valid value. Try again." << endl;
            cond = true;
        }

    } while(cond);

    customerList.push_back(admin.createCustomer(fname,lname,city,state,phone,accNum,pin,balance));
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//	MENU FUNCTIONS
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Prints the intro to the title screen
void header() {
    string line; //Holds a string of = to be a border line
    line.assign(55, '=');

    cout << line << endl;
    cout << "Welcome to Best Bank ATM" << endl;
    cout << line << endl;

}

//Prints the end of the title screen
void greetingEnd() {
    cout << "Successful ATM Login! " << endl << endl;
}

//ATM function menu for the customer
void customerMenu() {
	bool atmRunning = true; //Stays true until the user enters 6 to close the
	string input; //Holds input from the user
	int choice; //Holds menu choice input
	string line; //Holds a string of = to be a border line
	line.assign(55, '=');

	system("CLS");
	header();

	//Gets the user's account id number
	//Gets the user's account id number
	do {
		cout << "Enter your Customer ID: ";
		getline(cin, input);
		custIndex = checkId(input);
        cout << endl;

	} while (custIndex == -1);

	//Gets the user's account pin
	do {
		cout << "Enter you Customer PIN: ";
		getline(cin, input);
        cout << endl;
	} while (!checkPin(input));

	//Call title closing
	greetingEnd();
	pressEnter();

    while(atmRunning) {
        cout << flush;
        system("CLS");
        cout << line << endl;
        cout << "Welcome to Best Bank ATM Customer Menu" << endl;
        cout << line << endl;
        cout << "Please enter a number from 1 to 7 to select" << endl;
        cout << "a menu choice." << endl;
        cout << "1) Change PIN" << endl;
        cout << "2) View Customer Information" << endl;
        cout << "3) View Balance" << endl;
        cout << "4) Make a Deposit" << endl;
        cout << "5) Transfer Money" << endl;
        cout << "6) Withdraw Money" << endl;
        cout << "7) Exit to Login Menu" << endl;

        cin >> choice;

        //Check if cin failed
        if(cin.fail()) {
            //Clear input buffer
            cin.clear();
            cin.ignore(256, '\n');
            choice = 8;
        } else {
            cin.ignore();
        }

        //Handles the input from the user and leads to the appropriate function for the user choice
        switch (choice) {
            case 1:
                //cout << "pin" << endl;
                changePin(customerList[custIndex]);
                break;
            case 2:
                cout << flush;
                system("CLS");
                cout << "-Customer Info-" << endl;
                customerList[custIndex].viewCustInfo();
                break;
            case 3:
                cout << flush;
                system("CLS");
                balance(customerList[custIndex]);
                break;
            case 4:
                cout << flush;
                system("CLS");
                deposit(customerList[custIndex]);
                break;
            case 5:
                cout << flush;
                system("CLS");
                transfer();
                break;
            case 6:
                cout << flush;
                system("CLS");
                withdrawal(customerList[custIndex]);
                break;
            case 7:
                cout << flush;
                system("CLS");
                stopFunction();
                atmRunning = false;
                break;
            default:
                cout << "You did not enter a valid number." << endl;
                break;
		}


        cout << endl;

        pressEnter();

        cout << endl;
    }
}

//ATM function menu for the admin
void adminMenu() {
    bool atmRunning = true; //Stays true until the user enters 6 to close the
    string input; //Holds input from the user
    int choice; //Holds menu choice input
    string line; //Holds a string of = to be a border line
    line.assign(55, '=');

    //Gets the user's account pin
    do {
        cout << "Enter your Admin Pin: ";
        getline(cin, input);
    } while(input.compare(admin.pin));

    //Call title closing
    greetingEnd();

    pressEnter();

    while(atmRunning) {
        cout << flush;
        system("CLS");
        cout << line << endl;
        cout << "Welcome to Best Bank ATM Admin Menu" << endl;
        cout << line << endl;
        cout << "Please enter a number from 1 to 8 to select" << endl;
        cout << "a menu choice." << endl;
        cout << "1) Create Customer Account" << endl;
        cout << "2) Change Own Password" << endl;
        cout << "3) View Customer Information" << endl;
        cout << "4) Change Customer Information" << endl;
        cout << "5) Delete Customer Account" << endl;
        cout << "6) Show Top 5 Savings Accounts by Balance" << endl;
        cout << "7) Show Total Amount of Money in all Bank Accounts" << endl;
        cout << "8) Exit to Login Menu" << endl;

        cin >> choice;

        //Check if cin failed
        if(cin.fail()) {
            //Clear input buffer
            cin.clear();
            cin.ignore(256, '\n');
            choice = 8;
        } else {
            cin.ignore();
        }

        //Handles the input from the user and leads to the appropriate function for the user choice
        switch (choice) {
            case 1:
                cout << flush;
                system("CLS");
                createNewCustomer();
                break;
            case 2:
                cout << flush;
                system("CLS");
                changeAdminPin();
                break;
            case 3:
                cout << flush;
                system("CLS");
                viewCustomerInfo();
                break;
            case 4:
                cout << flush;
                system("CLS");
                changeCustomerInfo();
                break;
            case 5:
                cout << flush;
                system("CLS");
                deleteCustomer();
                break;
            case 6:
                cout << flush;
                system("CLS");
                printTop5Accounts();
                break;
            case 7:
                cout << flush;
                system("CLS");
                showTotal();
                break;
            case 8:
                cout << flush;
                system("CLS");
                stopFunction();
                atmRunning = false;
                break;
            default:
                cout << flush;
                system("CLS");
                cout << "You did not enter a valid number." << endl;
                break;
        }

        cout << endl;
        pressEnter();
        cout << endl;
    }
}

//Tells the user the function menu is exiting
void stopFunction() {
	cout << "Execute Exit" << endl;
	cout << "Logging out of system" << endl;
	cout << "Have a nice day!" << endl << endl;
}



