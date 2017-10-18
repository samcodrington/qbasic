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

Account Login(vector<Account> &);
void CreateAccount(vector<Account> &,bool);
void DeleteAccount(vector<Account> &,bool);
bool checkValidAccount(const Account, const string acctNum);

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
        puts("Please Type the Action You Wish To Do");
        cin >> buffer;
        
        if (buffer == "Logout") {
            break;
        }
        else if (buffer == "Createaccount") {
         CreateAccount(validAccts, currentAccount.isAgent()); //pass vector of valid accounts by refrence and if the current account is an agent account   
        }
        else if (buffer == "Deleteaccount") {
            DeleteAccount(validAccts, currentAccount.isAgent()); //pass vector of valid accounts by refrence and if the current account is an agent account
        }
        else if (buffer == "Deposit") {
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

Account CreateAccount(Account currentAccount){

}

//-----------TRANSACTION FUNCTIONS ------------------/
/**CheckValidDestination is a method which returns true if the account Number is the same as the currentAccount logged on
 * or if the account is an agent. 
 * Does not check whether account exists or not!**/
bool checkValidAccount(const Account currentAccount, const string acctNum){
    // Check AcctNum exists 
        /**TODO**/

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
        
    
    
}
bool parseDeposit(){

}


/**CreateAccount is a method which creates and adds a new user defined account to the valid accounts vector by refrence in a void function**/
void CreateAccount(vector<Account> &validAccounts, bool isAgent){ //Accepts the enire valid accounts file by refrence so it can easily add another account onto the end. Also accepts the 'isAgent' accessor value from currentAccount for the isAgent check
    string newNumber, newName, newPIN, buffer;
    bool newAgent;
    
    if (isAgent == 0){ // returns to main if currentAccount is not an agent account
        puts("Invalid session type for command!");
        return;
    }
    puts("Input Account Number:");
    cin>>newNumber;
    
    if (newNumber.find_first_not_of("0123456789") == string::npos) {// Makes sure newNumber is only numerical
        puts("Invalid Account Number: New account number can only have numbers!");
        return;
    }
    
    if (newNumber[0] == '0') {// Makes sure first digit of account number isnt 0
        puts("Invalid Account Number: New account number cannot start with 0!");
        return;
    }
    
    if (newNumber.length() != 7) {// Makes sure account number is exactly 7 digits long
        puts("Invalid Account Number: New account number must be exactly 7 digits long!");
        return;
    }
    
    for (int i =0; i<validAccounts.size(); i++) { // Makes sure that the new account number does not already exist
        if (newNumber == validAccounts.at(i).getNum()){
            puts("Invalid Account Number: New account number already exists!");
            return;
        }
    }
    
    puts("Input Account Name:");
    cin>>newName;
    
    if (newName.find_first_not_of("0123456789 abcdefghijklmnopqurstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ") == string::npos) {// Makes sure newName is only numerical, alphabetical (Spaces allowed)
        puts("Invalid Account Name: Only input letters numbers and spaces!");
        return;
    }
    
    if (newName[0] == ' ') {// Makes sure first digit of account name isnt a space
        puts("Invalid Account Name: Space not allowed at beginning of name!");
        return;
    }
    
    if (newName.length() < 3 || newName.length() > 20) {// Makes sure name is between 3-20 chars
        puts("Invalid Account Name: Space not allowed at beginning of name!");
        return;
    }
    
    puts("Input account PIN:");
    cin>>newPIN;
    
    if (newNumber.find_first_not_of("0123456789") == string::npos) {// Makes sure PIN is only numerical
        puts("Invalid Account PIN: New PIN can only have numbers!");
        return;
    }
    
    if (newPIN.length() != 4) {// Makes sure PIN is 4 numbers long
        puts("Invalid Account PIN: PIN must be 4 digits long!");
        return;
    }
    
    
    puts("Input account type:"); //Last input to the function asks for accout type
    cin>>buffer; // If not agent or machine, the program returns to the beginning
    if (buffer =="Machine") {
        newAgent = 0;
        printf("User account has been created with Number: \"%s\", Name: \"%s\", and PIN: \"%s\"",newNumber.c_str(), newName.c_str(), newPIN.c_str());
    }
    else if (buffer == "Agent"){
        newAgent = 1;
        printf("Agent account has been created with Number: \"%s\", Name: \"%s\", and PIN: \"%s\"",newNumber.c_str(), newName.c_str(), newPIN.c_str());
    }
    else{
        puts("Invalid account type! Must be either\"Machine\" or \"Agent\"! ");
        return;
    }
    
    validAccounts.push_back(Account(newNumber, newName, newPIN, newAgent)); //Creates a new instance of an account using user inputs in the validAccounts vector

    return;
}

/**DeleteAccount is a method which deletes a user defined account from the valid accounts vector by refrence in a void function**/
void DeleteAccount(vector<Account> &validAccounts, bool isAgent){//Accepts the enire valid accounts file by refrence and deletes the single account number the user inputs. Also accepts the 'isAgent' accessor value from currentAccount for the isAgent check
    
    string delNumber,delName,delPIN;
    
    if (isAgent == 0){ // returns to main if currentAccount is not an agent account
        puts("Invalid session type for command!");
        return;
    }
    
    puts("Input account type:");
    cin>>delNumber;
    
    if (delNumber.find_first_not_of("0123456789") == string::npos || delNumber.length() != 7 || delNumber[0] == '0') {// Makes sure delNumber is a valid account number to begin with
        puts("Invalid Account Number: Input is not a possible account number!!");
        return;
    }
    
    
    for (int i =0; i<validAccounts.size(); i++) { // if the account number isnt in the valid accounts vector then it has not been created yet
        if (delNumber == validAccounts.at(i).getNum()){
            
            puts("Input account name:"); //When function has found the corresponding account, it then prompts for name and number
            cin>>delName;
            
            if (delName != validAccounts.at(i).getName()) { //Incorrect name returns
                puts("Incorrect name!");
                return;
            }
            
            puts("Input account PIN:");
            cin>>delName;
            
            if (delPIN != validAccounts.at(i).getPIN()) { //Incorrect PIN returns
                puts("Incorrect PIN!");
                return;
            }
                
                //If all details line up then account is deleted by refrence from the valid accounts vector
                validAccounts.erase(remove(validAccounts.begin(), validAccounts.end(), i), validAccounts.end());
            printf("Account \"%s\" has been deleted!", delNumber.c_str());
                return;
            }
    }
    
    puts("Invalid Account Number: Account does not exist!"); //If delNumber was not in the valid accounts vector, function returns
    return;
}

