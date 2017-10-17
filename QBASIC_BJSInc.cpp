//
//  QBASIC_BJSInc.cpp
//  QBASIC_BJSInc
//
//  Created by Jerry Haron on 2017-10-15.
//  Copyright © 2017 Jerry Haron. All rights reserved.
//

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include "QBASIC_BJSInc_Functions.cpp"
#include "QBASIC_BJSInc.hpp"

using namespace std;

vector<Account> ReadAccountsFile(const string);

Account Login(vector<Account>&);
Account CreateAccount(Account);

// -----Main-----
int main(){
    
    //Constant string variable to quickly tell the program which account file to read from
    //(Just in case one would need to test different versions of account files without renaming them)
    const string AccountFile = "QBASIC_AccountFile.txt";

    //Loads a vector of valid accounts (of 'Account' type classes) into local memory
    vector<Account> validAccts = ReadAccountsFile(AccountFile);
    
    //The very first function input should be 'Login' else an error message is shown to the user
    string buffer;
    cin >> buffer;
    while (buffer != "Login") { //The program keeps pinging the user for an input until a 'Login'
        puts("Must be logged in.");
        cin >> buffer;
    }
    
    Account currentAccount = Login(validAccts); //The 'Login' function validates a valid login and loads the current user account into memory
    
    while (1) {
        cin >> buffer;
        
        if (buffer == "Logout") {
            break;
        }
        else if (buffer == "Createaccount") {
            
        }
        else if (buffer == "Deleteaccount") {
            
        }
        else if (buffer == "Deposit") {
            
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

Account CreateAccount(Account currentAccount)


