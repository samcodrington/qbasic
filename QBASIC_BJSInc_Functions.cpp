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

//First Account class consructor to be used for the CreateAcct function
Account::Account(string acctNum, string acctName, string acctPin, bool isAgent):
accountNumber(acctNum), accountName(acctName), accountPin(acctPin), accountBalance(0),agent(isAgent)
{
    //PUT CREATEACCOUNT TESTS IN HERE
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



