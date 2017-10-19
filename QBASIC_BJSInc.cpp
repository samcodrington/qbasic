//
//  QBASIC_BJSInc.cpp
//  QBASIC_BJSInc
//
//  Created by Jerry Haron on 2017-10-15.
//  Copyright © 2017 Jerry Haron. All rights reserved.
//

#include <ctime>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include "QBASIC_BJSInc.hpp"

using namespace std;

vector<Account> ReadAccountsFile(const string);

Account Login(vector<Account> acctvector, string accountNumber, string PIN) {
	
	for (int i = 0 ; i < acctvector.size(); i++){
		if (acctvector.at(i).getNum()== accountNumber && acctvector.at(i).getPIN() == PIN ) {
			return acctvector[i];
		}
	}
	throw TestException("Invalid Account Credentials");
}
Account CreateAccount(Account);
//bool checkValidAccount(const Account, const string acctNum);

// -----Main-----
int main(int argc, char *argv[]) {
	string transactionFileName = argv[2];
	ofstream transactionFileStream;

	transactionFileStream.open(transactionFileName);


	//Loop infinitely so the program can continue even after the user logs out
	while (true) {
		//Constant string variable to quickly tell the program which account file to read from
		//(Just in case one would need to test different versions of account files without renaming them)
		const string AccountFile = argv[1]; //QBASIC_AccountFile.txt
		bool isLoggedIn = false;
		string PIN;
		string acctNumber;
		
		//The very first function input should be 'Login' else an error message is shown to the user
		string buffer;
		puts("Type \"Login\" to log in");
		cin >> buffer;
		while (!isLoggedIn) {
			while (buffer != "Login") { //The program keeps pinging the user for an input until a 'Login'
				if (buffer == "exit") {
					goto label;
				}
				puts("Must be logged in.");
				cin >> buffer;
			}
			puts("machine/agent?");
			cin >> buffer;
			if (buffer == "machine") {
				// TODO: Change permissions to machine
				
				transactionFileStream << "Login\nmachine\n";
			}
			else {
				/* Since we're not error checking on the first assignment, we assume the 
				only other possible input is "agent"*/
				
				// TODO: Change permissions to agent
				
				transactionFileStream << "Login\nagent\n";
			}
			puts("Account number:");
			cin >> buffer;
			acctNumber = buffer;
			transactionFileStream << buffer << "\n";
			puts("PIN");
			cin >> buffer;
			PIN = buffer;
			transactionFileStream << buffer << "\n";


			//Loads a vector of valid accounts (of 'Account' type classes) into local memory
			vector<Account> validAccts = ReadAccountsFile(AccountFile);


			//The 'Login' function validates a valid login and loads the current user account into memory
			try {
				Account currentAccount = Login(validAccts, acctNumber, PIN);
				isLoggedIn = true;
				transactionFileStream << "Logged in successfully"<<endl;
			}
			catch (TestException& e){
				cerr << e.what() << endl;
				transactionFileStream << e.what() << endl;
				isLoggedIn = false;
			}
				

		}
		while (isLoggedIn) {
			puts("Please Type the Action You Wish To Do");
			cin >> buffer;

			if (buffer == "Logout") {
				isLoggedIn=false;
				transactionFileStream << "Logout" << "\n";
			}
			else if (buffer == "Createaccount") {

			}
			else if (buffer == "Deleteaccount") {

			}
			else if (buffer == "Deposit") {
				/*
				bool exit = false;
				do{
					puts("Enter the Destination Account");
					cin >> buffer;
					if (buffer == "Exit"){
						break;
						exit = true;
					}
				}while(checkValidAccount(currentAccount, buffer))
				if (exit == true) break;
				*/
			}
			else if (buffer == "Withdraw") {

			}
			else if (buffer == "Transfer") {

			}
			else if (buffer == "Checkaccountbalance") {

			}
			else {
				puts("Invalid command!");
			}

		}

	}
label:;
	transactionFileStream << "EOS";
    return 0;
}

//-----Functions-----

vector<Account> ReadAccountsFile(const string filename) {
    
    vector<string> acctNumbers;
    vector<string> acctNames;
    vector<string> acctPins;
    vector<float> acctBalance;
    vector<bool> acctTypes;
    vector<Account> validAccounts; //Holds the accounts to be passed back to the main function
    
    string ln,strhold;
    int numAccounts = 0;
    bool bholder;
    ifstream fileIn(filename.c_str());
    
    if (fileIn.fail()) {
        return validAccounts;
    }
    
    
    //This while loop iterates once per line for all of the lines in the Accounts file.
    // Pushes back each relevant value into a vector (Assumes space separation of fields)
    while (getline(fileIn, ln)){
        istringstream strng(ln);
        getline(strng, strhold, ' ');
        acctNumbers.push_back(strhold);
        getline(strng, strhold, ' ');
        acctNames.push_back(strhold);
        getline(strng, strhold, ' ');
        acctPins.push_back(strhold);
        getline(strng, strhold, ' ');
        acctBalance.push_back(stof(strhold));
        getline(strng, strhold, ' ');
        istringstream(strhold)>>bholder;
        acctTypes.push_back(bholder);
        numAccounts++;
    }
    fileIn.close();
    
    for (int i = 0;i<numAccounts;i++) {
        validAccounts.push_back(Account(acctNumbers.at(i), acctNames.at(i), acctPins.at(i), acctBalance.at(i), acctTypes.at(i)));
    }
        
    return validAccounts;
}
/*
Account CreateAccount(Account currentAccount){

}
*/
//-----------TRANSACTION FUNCTIONS ------------------/
/**CheckValidDestination is a method which returns true if the account Number is the same as the currentAccount logged on
 * or if the account is an agent. 
 * Does not check whether account exists or not!**/
/*
bool checkValidAccount(const Account currentAccount, const string acctNum){
    // Check AcctNum exists 
        /**TODO**

    // Checks permissions    
    if(currentAccount.getNum == acctNum) //Check if transaction being done on currently logged in Account
        return true; //If the account is being
    else{
        if (currentAccount.isAgent) return true;
        else{
            puts("Attempting Transaction on Invalid Account Number. Please Try Again Or Type \"Exit\".")
            return false;
        }
    }
        
    
    
}*/
bool parseDeposit(){
	return true;
}

