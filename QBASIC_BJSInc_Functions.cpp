//
//  QBASIC_BJSInc_Functions.cpp
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
#include "QBASIC_BJSInc.hpp"
using namespace std;

//Exception Function
TestException::TestException(const string& msg) : message(msg) {}

string& TestException::what() { return message; }

//Constructors

Account::Account(): accountNumber(""), accountName(""), accountPin(""), accountBalance(0),agent(0)
{
    //Blank constructor to make an empty account
}

//First Account class consructor to be used for the CreateAcct function
Account::Account(string acctNum, string acctName, string acctPin, bool isAgent):
accountNumber(acctNum), accountName(acctName), accountPin(acctPin), accountBalance(0),agent(isAgent)
{
    //CreateAccount function checks if account is valid already
}

//First Account class consructor to be used for the ReadAccountsFile function
Account::Account(string acctNum, string acctName, string acctPin, float acctBalance, bool isAgent):
accountNumber(acctNum), accountName(acctName), accountPin(acctPin), accountBalance(acctBalance),agent(isAgent)
{
    //No checks needed since valid accounts file is assumed to already be checked
}

string Account::getNum(){ //Accessor for the particular "Account" class' account number
    return accountNumber;
}

string Account::getName(){ //Accessor for the particular "Account" class' account name
    return accountName;
}

string Account::getPIN(){ //Accessor for the particular "Account" class' account PIN
    return accountPin;
}

float Account::getBalance(){ //Accessor for the particular "Account" class' account balance
    return accountBalance;
}

bool Account::isAgent(){ //Accessor to see if particular "Account" has agent capabilities
    return agent;
}
void Account::changeBalance(const float amt){
    if (accountBalance + amt >= 0)
        puts ("Cannot Change Balance Below 0.");
    else
        accountBalance += amt;
}
bool Account::overdraftCheck(const float amt){
    if (accountBalance + amt >= 0) return true;
    else return false;
}



