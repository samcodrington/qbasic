//
//  QBASIC_Backend_BJSInc.cpp
//  QBASIC_BJSInc
//
//  A BJS_Inc Project.
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

Account::Account(): accountNumber(""), accountName(""), accountBalance(0)
{
    //Blank constructor to make an empty account
}

//First Account class consructor to be used for the CreateAcct function
Account::Account(string acctNum, string acctName):
accountNumber(acctNum), accountName(acctName), accountBalance(0)
{
    //createAccount constructor
}

//First Account class consructor to be used for the ReadAccountsFile function
Account::Account(string acctNum, string acctName, int acctBalance):
accountNumber(acctNum), accountName(acctName)
{
    accountBalance = acctBalance/100; //When the program reads the valid accounts file (which has balance in cents; i.e. $1.23 in the program is 123 in accounts file) it needs to divide balance value by 100 to get real dollars and cents
    //No checks needed since valid accounts file is assumed to already be checked
}

string Account::getNum(){ //Accessor for the particular "Account" class' account number
    return accountNumber;
}

string Account::getName(){ //Accessor for the particular "Account" class' account name
    return accountName;
}

float Account::getBalance(){ //Accessor for the particular "Account" class' account balance
    return accountBalance;
}

void Account::changeBalance(const float amt){ //adds deposit amount to account's valance variable (must inut negative value if withdraw)
        accountBalance += amt;
}
bool Account::overdraftCheck(const float amt){ // checks if subtracting an amount will cause the particular account instnace to be overdraft
    if (accountBalance - amt < 0) return true;
    else return false;
}



