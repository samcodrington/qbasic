//
//  QBASIC_Backend_BJSInc.cpp
//  QBASIC_BJSInc
//
//  A BJS_Inc Project.
// Intention of this program is to read the merged transaction summary file from the previous bank day and apply the outlined changes (throwing standard output errors if particular trnasactions are deemed invalid). After all lines of the trnasaction summary file have been parsed, program then ourputs an updated master accounts file and valid accounts list for use in front end sessions the following day

#include <iostream>
#include <string>
#include <vector>
#include <cstdio>
#include "QBASIC_BJSInc.hpp"
#include "QBASIC_BJSInc_Functions.cpp"

vector<Account> ReadAccountsFile(const string);
void writeNewMasterAccountsFile(const string, vector<Account>);
void writeNewValidAccountsFile(const string, vector<Account>&);

void createAccount(vector<Account>&, string, string);
void deleteAccount(vector<Account>&, string, string);
void deposit(vector<Account>&, string, float);
void withdraw(vector<Account>&, string , float);
void transfer(vector<Account>&, string, string, float);

int main(){
    
    const string MasterAccountFile = "QBASIC_MasterAccountFile.txt";
    const string ValidAccountFile = "QBASIC_ValidAccountFile.txt";
    const string MgdTrscnSummaryFile = "QBASIC_MergedTransactionSummaryFile.txt";
    
    vector<Account> validAccts = ReadAccountsFile(MasterAccountFile); // Loads the valid accounts from the master file into memory

    string ln, strhold; // two holder variables for the main while loop function
    string cmd, firstAccount, secondAccount, name; // create temporary holder variables for each sepearate value in transaction summary file
    float value;
    
    ifstream fileIn(MgdTrscnSummaryFile.c_str()); // Open merged transaction summary file
    
    while (getline(fileIn, ln)){ // Main while loop that iterates through each line in the mgd transtaction summary file
        
        istringstream strng(ln);
        getline(strng, strhold, ' ');
        cmd = strhold;
        getline(strng, strhold, ' ');
        firstAccount = strhold;
        getline(strng, strhold, ' ');
        try {
            value = (stof(strhold))/100;
        } catch (const std::invalid_argument&) { // try catch block to stop program from crashing if there is invalid numerical input
            value = 0;
        }
        getline(strng, strhold, ' ');
        secondAccount = strhold;
        getline(strng, strhold, ' ');
        name = strhold;
        
        if (cmd == "EOS") {
            // If there is and EOS line, do nothing as this represents either a logout or a merged transaction summary
        }
        else if (cmd == "NEW"){
            try {
                createAccount(validAccts, firstAccount, name);
            } catch (TestException& e) {
                cout << e.what() << endl;
            }
        }
        else if (cmd == "DEL"){
            try {
                deleteAccount(validAccts, firstAccount, name);
            } catch (TestException& e) {
                cout << e.what() << endl;
            }
        }
        else if (cmd == "DEP"){
            try {
                deposit(validAccts, firstAccount, value);
            } catch (TestException& e) {
                cout << e.what() << endl;
            }
        }
        else if (cmd == "WDR"){
            try {
                withdraw(validAccts, firstAccount, value);
            } catch (TestException& e) {
                cout << e.what() << endl;
            }
        }
        else if (cmd == "XFR"){
            try {
                transfer(validAccts, firstAccount, secondAccount, value);
            } catch (TestException& e) {
                cout << e.what() << endl;
            }
        }
        else{
            puts("Invalid command! Skipping line command"); // if command is invalid for whatever front end reason, skip it
        }
    }
    
    writeNewMasterAccountsFile(MasterAccountFile, validAccts);
    writeNewValidAccountsFile(ValidAccountFile, validAccts);
    return 0;
}

// ---------------- I/O FUNCTIONS ---------------------

vector<Account> ReadAccountsFile(const string masterfilename) {
    // ReadAccountsFile function assumes that master accounts file format is correct since it itself was created by the backend program
    vector<string> acctNumbers;
    vector<string> acctNames;
    vector<float> acctBalance;
    vector<Account> validAccounts; //Holds the accounts to be passed back to the main function
    
    string ln,strhold;
    int numAccounts = 0;
    ifstream fileIn(masterfilename.c_str());
    
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
        try {
            acctBalance.push_back(stof(strhold));
        } catch (const std::invalid_argument&) {
            numAccounts--;
        }
        getline(strng, strhold, ' ');
        acctNames.push_back(strhold);
        numAccounts++;
    }
    fileIn.close();
    
    for (int i = 0;i<numAccounts;i++) {
                validAccounts.push_back(Account(acctNumbers.at(i), acctNames.at(i), acctBalance.at(i)));
        } // load accounts in master accounts file into an account type vector
    
    return validAccounts;
}
// -------------------------------------
// Writes a full master accounts list from scratch, overwriting existing accounts list in same file location. Inputs all the details of each valid account into .txt file
void writeNewMasterAccountsFile(const string MasterName,vector<Account> validAccounts){
    
    ofstream fileOut(MasterName.c_str());
    
    for (int i = 0; i<validAccounts.size(); i++) { //write the details of each account in memory to the master account file
        fileOut<<validAccounts.at(i).getNum()<<" "<<(validAccounts.at(i).getBalance())*100<<" "<<validAccounts.at(i).getName()<<endl;
    }
    fileOut.close();
}
// -------------------------------------
// Writes a full valid accounts list from scratch, overwriting existing accounts list in same file location. Inputs only account number to file and ends with invalid '0000000' number
void writeNewValidAccountsFile(const string validName, vector<Account> &validAccounts){ //Valid account file contains just valid account numbers followed by an invalid number (In this case "0000000")
    
    ofstream fileOut(validName.c_str());
    
    for (int i = 0; i<validAccounts.size(); i++) { // simply write each valid account number to the validaccountfile
                fileOut<<validAccounts.at(i).getNum()<<endl;
    }
    fileOut<<"0000000"<<endl;
    fileOut.close();
}


// ---------------- FUNCTIONS ---------------------
//Takes an account number and name as input. Adds created account to valid accounts list (name will be included in master/valid accounts .txt file). Makes sure that input number does not match and existing number in valid accounts list
void createAccount(vector<Account> &validAccounts, string createdAccount,string accountName){
    for (int i = 0; i<validAccounts.size(); i++) {
        if (createdAccount == validAccounts.at(i).getNum()) { //If account already exists, throw an error
            throw TestException("Invalid number! Account number to be created already exists!");
        }
    }
    validAccounts.push_back(Account(createdAccount, accountName)); //else add the new account to the list of valid accounts
    return;
}
// -------------------------------------
//Takes an account number and name as input. Deletes account from valid accounts list (name will not show up in master/valid accounts .txt file). Makes sure that account has a zero balance before deletion and that the inut name matches up with account name
void deleteAccount(vector<Account> &validAccounts, string deletedAccount,string accountName){
    for (int i = 0; i<validAccounts.size(); i++) {
        if (deletedAccount == validAccounts.at(i).getNum()) {
            if (accountName == validAccounts.at(i).getName()) {
            throw TestException("Invalid name! Account name and input name must match");
            }
            if (validAccounts.at(i).getBalance() != 0) { // if target deleted account is not zero, throw an error
                throw TestException("Invalid balance! Account must be empty before deletion");
            }
            validAccounts.erase(validAccounts.begin() + i); //Erase deleted account from vector list
            return;
        }
    }
    throw TestException("Invalid number! Account number does not exist!");
}
// -------------------------------------
//Takes an account number and a monetary value as input. Adds money to the corresponding account instance. Makes sure that account will not exceed the maximum balance of 1000000 dollars
void deposit(vector<Account> &validAccounts, string accountNum, float value){
    
    for (int i = 0; i<validAccounts.size(); i++) {
        if (accountNum == validAccounts.at(i).getNum()) {
            if ((validAccounts.at(i).getBalance()+value) > 1000000){ // Make sure deposited amount toes not exceed upper limit of value
                throw TestException("Invalid value! Account will exceed maximum value!");
                
            }
            validAccounts.at(i).changeBalance(value); // add value to target account
            return;
        }
    }
    throw TestException("Invalid number! Account number does not exist!");
}
// -------------------------------------
// Takes an account number and a monetary value as input. Removes money from the corresponding account instance. Makes sure that account exists and will not be overdrawn
void withdraw(vector<Account> &validAccounts, string accountNum, float value){
    for (int i = 0; i<validAccounts.size(); i++) {
        if (accountNum == validAccounts.at(i).getNum()) {
            if(validAccounts.at(i).overdraftCheck(value)){ //If overdraft, throw error
                throw TestException("Invalid value! Account will be overdraft!");
            }
            validAccounts.at(i).changeBalance(-1*value); // withdraw value from target account
            return;
        }
    }
    throw TestException("Invalid number! Account number does not exist!");
}
// -------------------------------------
//Transfer function takes two accounts and and a numerical vlue as input. Transfers a balance from the first account instance to the second instance, making sure that: Both accounts exist, the 'from account' will not be overdrawn and the 'to account' will not exceed the maximum balance
void transfer(vector<Account> &validAccounts, string fromAccount, string toAccount, float value){
    for (int i = 0; i<validAccounts.size(); i++) {
        if (fromAccount == validAccounts.at(i).getNum()) {
            for (int j = 0; j<validAccounts.size(); j++) {
                if (toAccount == validAccounts.at(j).getNum()){
                    if(validAccounts.at(i).overdraftCheck(value)){ //If overdraft, throw error
                        throw TestException("Invalid value! Account will be overdraft!");
                    }
                    if ((validAccounts.at(j).getBalance()+value) > 1000000){ // Make sure deposited amount toes not exceed upper limit of value
                        throw TestException("Invalid value! Account will exceed maximum value!");
                    }
                    validAccounts.at(i).changeBalance(-1*value); //Subtract value from 'fromAccount'
                    validAccounts.at(j).changeBalance(value); // Add value to 'toAccount'
                    return;
                }
            }
        throw TestException("Invalid number! 'To' account number does not exist!");
        }
    }
    throw TestException("Invalid number! 'From' account number does not exist!");
}
