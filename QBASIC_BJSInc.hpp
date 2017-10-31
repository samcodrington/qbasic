//
//  QBASIC_BJSInc.hpp
//  QBASIC_BJSInc
//
//  Created by Jerry Haron on 2017-10-15.
//  Copyright © 2017 Jerry Haron. All rights reserved.
//


#ifndef QBASIC_BJSInc_hpp
#define QBASIC_BJSInc_hpp

#include <string>

using namespace std;

//Exception class to throw test exceptions
class TestException {
public:
    TestException(const string& msg);
    string& what();
private:
    string message;
};

class Account{
public:
    Account(); // Empty constructor that just creates an account with nulls
    Account(string,string,string,bool); //Constructor for the "createacct" functionality
    Account(string,string,string,float,bool); //Constructor for the initial read of the valid accounts file
    
    //Accessors for all the below Account class values:
    string getNum();
    string getName();
    string getPIN();
    float getBalance();
    bool isAgent();

    bool overdraftCheck(const float amt);
    //Modifier for Account balance
    void changeBalance(const float amt);

    
private:
    //Private variables for each account detail
    string accountNumber;
    string accountName;
    string accountPin;
    float accountBalance;
    bool agent; //'1' if agent, '0' if user
    
    
};

#endif /* QBASIC_BJSInc_hpp */

