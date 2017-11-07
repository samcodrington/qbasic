//
//  QBASIC_BJSInc.cpp
//  QBASIC_BJSInc
//
//  Created by Jerry Haron on 2017-10-15.
//  Copyright © 2017 Jerry Haron. All rights reserved.
//

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <cstdio>
#include "QBASIC_BJSInc.hpp"
#include "QBASIC_BJSInc_Functions.cpp"

using namespace std;

vector<Account> ReadAccountsFile(const string);

Account Login(vector<Account> acctvector);

void CreateAccount(vector<Account> &,bool);
void DeleteAccount(vector<Account> &,bool);
void Deposit(vector<Account> &, Account &);
void Withdraw(vector<Account> &, Account &, float &);
void Transfer(vector<Account> &, Account &);
void CheckAccountBalance(vector<Account> &, Account &);

void checkLegalTransactionAmount(Account &, float);
Account* getDestAcct(vector<Account> &, string);

void writeNewMasterAccountsFile(const string, vector<Account>);
void writeNewAccountsFile(const string, vector<Account>);

//Constants for Transaction Limits
#define AGENT_MAX 1000000
#define ATM_MAX 1000

// -----Main-----
int main(){
    const string AccountFile = "QBASIC_MasterAccountFile.txt";
    const string SummaryFile = "QBASIC_ValidAccountFile.txt";
    
    
	vector<Account> validAccts = ReadAccountsFile(AccountFile);
    freopen("QBASIC_MergedTransactionSummaryFile.txt", "w", stderr); //deletes any old MTSF and creates a new one
    fclose(stderr); // immediately closes it so logs only are written when logged in
    
	while (true) {

        bool isLoggedIn = false;
        Account currentAccount; //Fixes scope issues by creating currentAccount var with an empty account
		string buffer;
        
		while (!isLoggedIn) { //The very first function input should be 'Login' else an error message is shown to the user
            puts("Type \"Login\" to log in; Type \"Exit\" to leave the program:");
            cin >> buffer;
            
			while (buffer != "Login") {//The program keeps pinging the user for an input until a 'Login'
				
                if (buffer == "Exit") {
					goto exit; // Exit command exits the program
				}
				puts("Must be logged in.");
				cin >> buffer;
			}
            isLoggedIn = true; // If "Login" command is typed into buffer, program proceeds to below try-catch block, where user actually inputs the account they want to log in as

		}
        
        
        try {// if the user sucessfully logs in, then they can enter the second while loop where they can perform the rest of the functionality
            currentAccount = Login(validAccts);
            puts("Login success!");
            isLoggedIn = true;
        }
        catch (TestException& e) { // if user fails to sucessfully login i.e. with invalid PIN or acct number, they will not be logged in, and return to the previous while loop that prompts the user for a "Login command"
            if (e.what() == "Exit")
                goto exit;
            cout << e.what() << endl;
            isLoggedIn = false;
        }
        
        
		while (isLoggedIn) {
            printf("Welcome, %s! Logged in as account %s\n",currentAccount.getName().c_str(),currentAccount.getNum().c_str());
            freopen("QBASIC_MergedTransactionSummaryFile.txt", "a+", stderr); //Opens a log file and writes all "clog" output to it
			float totalWithdrawal = 0;
			cin >> buffer;

            if (buffer == "Login") {
                puts("User already logged in!");
            }
			else if (buffer == "Logout") {
				isLoggedIn = false;
                clog<<"EOS 0000000 0000 00000000 ****"<<endl;
                fclose(stderr);
				puts("Logged out");
			}
            else if (buffer == "Exit") {
                goto exit;
            }
			else if (buffer == "Createaccount") {
                try {
                    CreateAccount(validAccts, currentAccount.isAgent()); //pass vector of valid accounts by refrence and if the current account is an agent account
                } catch (TestException& e) {
                    if (e.what() == "Exit")
                        goto exit;
                    cout << e.what() << endl;
                }
               
			}
			else if (buffer == "Deleteaccount") {
                try {
                    DeleteAccount(validAccts, currentAccount.isAgent()); //pass vector of valid accounts by refrence and if the current account is an agent account
                } catch (TestException& e) {
                    if (e.what() == "Exit")
                        goto exit;
                    cout << e.what() << endl;
                }
                
			}
            
			else if (buffer == "Deposit") {
                try {
                    Deposit(validAccts, currentAccount); //pass vector of valid accounts by refrence and if the current account is an agent account
                } catch (TestException& e) {
                    if (e.what() == "Exit")
                        goto exit;
                    cout << e.what() << endl;
                }
               }
			else if (buffer == "Withdraw") {
                try {
                    Withdraw(validAccts, currentAccount, totalWithdrawal); //pass vector of valid accounts by refrence and if the current account is an agent account
                } catch (TestException& e) {
                    if (e.what() == "Exit")
                        goto exit;
                    cout << e.what() << endl;
                }
            }
			else if (buffer == "Transfer") {
                try {
                    Transfer(validAccts, currentAccount); //pass vector of valid accounts by refrence and if the current account is an agent account
                } catch (TestException& e) {
                    if (e.what() == "Exit")
                        goto exit;
                    cout << e.what() << endl;
                }
            }
			else if (buffer == "Checkaccountbalance") {
                try {
                    CheckAccountBalance(validAccts, currentAccount); //pass vector of valid accounts by refrence and if the current account is an agent account
                } catch (TestException& e) {
                    if (e.what() == "Exit")
                        goto exit;
                    cout << e.what() << endl;
                }
            }
			else {
				puts("Invalid command!");
			}

		}
	}
    exit: //if user types Exit, the goto function leads back to here
    //writeNewMasterAccountsFile(AccountFile, validAccts); FOR ASSIGNMENT 3 THIS IS REMOVED SINCE EACH TEST WILL BE USING THE EXACT SAME MASTER ACCOUNTS FILE
    //writeNewAccountsFile(SummaryFile, validAccts);
    
    freopen("QBASIC_MergedTransactionSummaryFile.txt", "a+", stderr); //appends EOS to merged transaction summary log file
    clog<<"EOS 0000000 0000 00000000 ****"<<endl;
    fclose(stderr);
    
    return 0;
}

//-----IO FUNCTIONS-----

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
        acctBalance.push_back(stof(strhold));
        getline(strng, strhold, ' ');
        acctNames.push_back(strhold);
        getline(strng, strhold, ' ');
        acctPins.push_back(strhold);
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

void writeNewMasterAccountsFile(const string MasterName,vector<Account> validAccounts){
    
    ofstream fileOut(MasterName.c_str());
    
    for (int i = 0; i<validAccounts.size(); i++) {
        fileOut<<validAccounts.at(i).getNum()<<" "<<(validAccounts.at(i).getBalance())*100<<" "<<validAccounts.at(i).getName()<<" "<< validAccounts.at(i).getPIN()<<" "<<validAccounts.at(i).isAgent()<<endl;
    }
    fileOut.close();

}

void writeNewAccountsFile(const string SummaryName, vector<Account> validAccounts){ //Valid account file contains just valid account numbers followed by an invalid number (In this case "0000000")
    
    ofstream fileOut(SummaryName.c_str());
    
    for (int i = 0; i<validAccounts.size(); i++) {
        fileOut<<validAccounts.at(i).getNum()<<endl;
    }
    
    fileOut<<"0000000"<<endl;
    fileOut.close();
    
}

//-----------TRANSACTION FUNCTIONS ------------------/

Account Login(vector<Account> acctvector) {
    string buffer, acctNumber, PIN;
    bool loginType;
    puts("Please input login session type:");
    cin >> buffer;
    
    if (buffer == "Exit")
        throw TestException("Exit");
    
    if (buffer == "Machine") {
        loginType = 0;
    } else if (buffer == "Agent") {
        loginType = 1;
    }else{
        throw TestException("Invalid Session Type");
    }
    puts("Please input login account:");
    cin >> buffer;
    if (buffer == "Exit")
        throw TestException("Exit");
    
    acctNumber = buffer;
    
    puts("Please input PIN:");
    cin >> buffer;
    if (buffer == "Exit")
        throw TestException("Exit");
    PIN = buffer;
    
	for (int i = 0; i < acctvector.size(); i++) {
        if (acctvector.at(i).getNum() == acctNumber){
            if(acctvector.at(i).getPIN() == PIN) {
                if (acctvector.at(i).isAgent() == loginType || acctvector.at(i).isAgent() == 0) {
                    return acctvector.at(i);
                } else{
                    throw TestException("Invalid Agent account number");
                }
            } else{
                throw TestException("Invalid PIN");
            }
        }
	}
	throw TestException("Invalid account number");
}


/**CreateAccount is a method which creates and adds a new user defined account to the valid accounts vector by refrence in a void function**/

void CreateAccount(vector<Account> &validAccounts, bool isAgent){ //Accepts the enire valid accounts file by refrence so it can easily add another account onto the end. Also accepts the 'isAgent' accessor value from currentAccount for the isAgent check
    string newNumber, newName, newPIN, buffer;
    bool newAgent;
    
    if (!isAgent){ // returns to main if currentAccount is not an agent account
        throw TestException("Invalid session type for command!");
    }
    puts("Input Account Number:");
    cin>>newNumber;
    if (newNumber == "Exit")
        throw TestException("Exit");
    
    if (newNumber.find_first_not_of("0123456789") != string::npos) {// Makes sure newNumber is only numerical
        throw TestException("Invalid Account Number: New account number can only have numbers!");
    }
    
    if (newNumber[0] == '0') {// Makes sure first digit of account number isnt 0
        throw TestException("Invalid Account Number: New account number cannot start with 0!");
    }
    
    if (newNumber.length() != 7) {// Makes sure account number is exactly 7 digits long
        throw TestException("Invalid Account Number: New account number must be exactly 7 digits long!");
    }
    
    for (int i =0; i<validAccounts.size(); i++) { // Makes sure that the new account number does not already exist
        if (newNumber == validAccounts.at(i).getNum()){
            throw TestException("Invalid Account Number: New account number already exists!");
        }
    }
    
    puts("Input Account Name:");
    cin>>newName;
    if (newName == "Exit")
        throw TestException("Exit");
    
    
    if (newName.find_first_not_of("0123456789 abcdefghijklmnopqurstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ") != string::npos) {// Makes sure newName is only numerical, alphabetical (Spaces allowed)
        throw TestException("Invalid Account Name: Only input letters numbers and spaces!");
    }
    
    if (newName[0] == ' ') {// Makes sure first digit of account name isnt a space
        throw TestException("Invalid Account Name: Space not allowed at beginning of name!");
    }
    
    if (newName.length() < 3 || newName.length() > 20) {// Makes sure name is between 3-20 chars
        throw TestException("Invalid Account Name: Must be 3-20 characters long!");
    }
    
    puts("Input account PIN:");
    cin>>newPIN;
    if (newPIN == "Exit")
        throw TestException("Exit");
    
    if (newPIN.find_first_not_of("0123456789") != string::npos) {// Makes sure PIN is only numerical
        throw TestException("Invalid Account PIN: New PIN can only have numbers!");
    }
    
    if (newPIN.length() != 4) {// Makes sure PIN is 4 numbers long
        throw TestException("Invalid Account PIN: PIN must be 4 digits long!");
    }
    
    
    puts("Input account type:"); //Last input to the function asks for accout type
    cin>>buffer; // If not agent or machine, the program returns to the beginning
    
    if (buffer == "Exit")
        throw TestException("Exit");
    
    if (buffer =="Machine") {
        newAgent = 0;
        printf("User account has been created with Number: \"%s\", Name: \"%s\", and PIN: \"%s\"\n",newNumber.c_str(), newName.c_str(), newPIN.c_str());
        clog<<"NEW "<< newNumber.c_str()<<" 000 0000000 "<< newName.c_str()<<endl;

        
    }
    else if (buffer == "Agent"){
        newAgent = 1;
        printf("Agent account has been created with Number: \"%s\", Name: \"%s\", and PIN: \"%s\"\n",newNumber.c_str(), newName.c_str(), newPIN.c_str());
    }
    else{
        throw TestException("Invalid account type! Must be either\"Machine\" or \"Agent\"! ");
    }
    
    validAccounts.push_back(Account(newNumber, newName, newPIN, newAgent)); //Creates a new instance of an account using user inputs in the validAccounts vector
    
    return;
}

/**DeleteAccount is a method which deletes a user defined account from the valid accounts vector by refrence in a void function**/
void DeleteAccount(vector<Account> &validAccounts, bool isAgent){//Accepts the enire valid accounts file by refrence and deletes the single account number the user inputs. Also accepts the 'isAgent' accessor value from currentAccount for the isAgent check
    
    string delNumber,delName,delPIN;
    
    if (!isAgent){ // returns to main if currentAccount is not an agent account
        throw TestException("Invalid session type for command!");
    }
    
    puts("Input account number:");
    cin>>delNumber;
    if (delNumber == "Exit")
        throw TestException("Exit");
    
    if (delNumber.find_first_not_of("0123456789") != string::npos || delNumber.length() != 7 || delNumber[0] == '0') {// Makes sure delNumber is a valid account number to begin with
        throw TestException("Invalid Account Number: Input is not a possible account number!!");
    }
    
    for (int i =0; i<validAccounts.size(); i++) { // if the account number isnt in the valid accounts vector then it has not been created yet
        if (delNumber == validAccounts.at(i).getNum()){
            
            puts("Input account name:"); //When function has found the corresponding account, it then prompts for name and number
            cin>>delName;
            if (delName == "Exit")
                throw TestException("Exit");
            
            if (delName != validAccounts.at(i).getName()) { //Incorrect name returns
                throw TestException("Incorrect name!");
            }
            
            puts("Input account PIN:");
            cin>>delPIN;
            if (delPIN == "Exit")
                throw TestException("Exit");
            
            if (delPIN != validAccounts.at(i).getPIN()) { //Incorrect PIN returns
                throw TestException("Incorrect PIN!");
            }
            
            //If all details line up then account is deleted by refrence from the valid accounts vector
            validAccounts.erase(validAccounts.begin() + i);
            printf("Account \"%s\" has been deleted!\n", delNumber.c_str());
            clog<<"DEL "<<delNumber.c_str()<<" 000 0000000 "<<delName.c_str()<<endl;
            return;
        }
    }
    
    throw TestException("Invalid Account Number: Account does not exist!"); //If delNumber was not in the valid accounts vector, function returns
}

void Deposit(vector<Account> &validAccounts,Account &currentAccount){
    string destAcct, buffer;
    float amount;
    
    puts("Enter destination account:");
    cin>>destAcct;
    
    if (destAcct == "Exit")
        throw TestException("Exit");
    
    if(!currentAccount.isAgent() && currentAccount.getNum() != getDestAcct(validAccounts, destAcct)->getNum()){
        throw TestException("Invalid account: User cannot deposit to other accounts!");
    }
    
    puts("Enter the amount you wish to deposit:");
    cin>>buffer;
    if (buffer == "Exit")
        throw TestException("Exit");
    
    if (buffer.find_first_not_of("0123456789") != string::npos) {// Makes sure amount is only numerical
        throw TestException("Invalid Amount: Amount can only have numbers!");
    }
    amount = stof(buffer);
    checkLegalTransactionAmount(currentAccount, amount);
    
    getDestAcct(validAccounts, destAcct)->changeBalance(amount);
    
    clog<<"DEP "<<getDestAcct(validAccounts, destAcct)->getNum()<<" "<< amount*100<<" 0000000 "<<getDestAcct(validAccounts, destAcct)->getName()<<endl;
    
}

void Withdraw(vector<Account> &validAccounts, Account &currentAccount, float &toWithdraw){
    string destAcct, buffer;
    float amount;
    
    puts("Enter destination account:");
    cin>>destAcct;
    
    if (destAcct == "Exit")
        throw TestException("Exit");
    
    if(!currentAccount.isAgent() && currentAccount.getNum() != getDestAcct(validAccounts, destAcct)->getNum()){
        throw TestException("Invalid account: User cannot withdraw from other accounts!");
    }
    
    puts("Enter the amount you wish to withdraw:");
    cin>>buffer;
    if (buffer == "Exit")
        throw TestException("Exit");
    
    if (buffer.find_first_not_of("0123456789") != string::npos) {// Makes sure amount is only numerical
        throw TestException("Invalid Amount: Amount can only have numbers!");
    }
    amount = stof(buffer);
    toWithdraw +=amount;
    
    checkLegalTransactionAmount(currentAccount, amount);
    
    if (!currentAccount.isAgent() && toWithdraw > ATM_MAX) {
        throw TestException("Invalid Amount: Maximum withdraw amount reached for single session!");
    }
    
    if (getDestAcct(validAccounts, destAcct)->overdraftCheck(amount)) {
        throw TestException("Invalid Amount: Withdraw would result in an overdraft!");
    }
    
    getDestAcct(validAccounts, destAcct)->changeBalance(amount*-1);
    
    clog<<"WDR "<<getDestAcct(validAccounts, destAcct)->getNum()<<" "<< amount*100<<" 0000000 "<<getDestAcct(validAccounts, destAcct)->getName()<<endl;
    
}

void Transfer(vector<Account> &validAccounts, Account &currentAccount){
    string fromAccount, toAccount, buffer;
    float amount;
    
    puts("Enter account to transfer from:");
    cin>>fromAccount;
    
    if (fromAccount == "Exit")
        throw TestException("Exit");
    
    if(!currentAccount.isAgent() && currentAccount.getNum() != getDestAcct(validAccounts, fromAccount)->getNum()){
        throw TestException("Invalid account: User cannot transfer from account that is not thier own!");
    }
    
    puts("Enter account to transfer to:");
    cin>>toAccount;
    
    if (toAccount == "Exit")
        throw TestException("Exit");
    
    puts("Enter the amount you wish to transfer:");
    cin>>buffer;
    if (buffer == "Exit")
        throw TestException("Exit");
    
    if (buffer.find_first_not_of("0123456789") != string::npos) {// Makes sure amount is only numerical
        throw TestException("Invalid Amount: Amount can only have numbers!");
    }
    amount = stof(buffer);
    checkLegalTransactionAmount(currentAccount, amount);
    
    if (getDestAcct(validAccounts, fromAccount)->overdraftCheck(amount)) {
        throw TestException("Invalid Amount: Transfer would result in an overdraft!");
    }
    
    getDestAcct(validAccounts, fromAccount)->changeBalance(amount*-1);
    getDestAcct(validAccounts, toAccount)->changeBalance(amount);
    
    clog<<"XFR "<<getDestAcct(validAccounts, fromAccount)->getNum()<<" "<< amount*100<<" "<<getDestAcct(validAccounts, toAccount)->getNum()<<" "<<getDestAcct(validAccounts, fromAccount)->getName()<<endl;
    
    
}

void CheckAccountBalance(vector<Account> &validAccounts, Account &currentAccount){
    string destAccount;
    
    puts("Enter the account to check balance of:");
    cin>>destAccount;
    
    if (destAccount == "Exit")
        throw TestException("Exit");
    
    if(!currentAccount.isAgent() && currentAccount.getNum() != getDestAcct(validAccounts, destAccount)->getNum()){
        throw TestException("Invalid account: User cannot check balance from account that is not thier own!");
    }
    
    cout<<"Balance in account "<<getDestAcct(validAccounts, destAccount)->getNum()<<" is $"<<getDestAcct(validAccounts, destAccount)->getBalance()<<endl;
}
/**
 * CheckLegalTransactionAmount checks the trasnactionAmt parameter agianst account conditions for a legal transaction
 * If they are met the function returns true, otherwise  it returns false */
void checkLegalTransactionAmount(Account &thisAcct, const float transactionAmt){
    
    //Return false for negative amount
    if (transactionAmt < 0){
        throw TestException("Invalid transaction amount: Cannot Deposit Negative Amount!");
    }
    //return false if an agent is attempting to transact amount over the limit
    if (thisAcct.isAgent() && transactionAmt >= AGENT_MAX){
        throw TestException("Invalid transaction amount: Agent Cannot Issue Transactions greater than 999,999.99!");
    }
    //Return false if an ATM is attempting to transact amount over the ATM limit
    if (!thisAcct.isAgent() && transactionAmt > ATM_MAX){
        throw TestException("Invalid transaction amount: Machine Users Cannot Issue Transactions greater than 1000!");
    }
}

/**
 * getDestAcct is a simple helper function that iterates through the validAccts parameter to see if it matches the acctNum provided. 
 * If it does it match the funciton returns that Acccount. If it doesn't it returns NULL.
 * */
Account* getDestAcct(vector<Account> & validAccts, string acctNum){
    
    if (acctNum.find_first_not_of("0123456789") != string::npos) {// Makes sure newNumber is only numerical
        throw TestException("Invalid Account Number: Account number can only have numbers!");
    }
    
    if (acctNum[0] == '0') {// Makes sure first digit of account number isnt 0
        throw TestException("Invalid Account Number: Account number cannot start with 0!");
    }
    
    if (acctNum.length() != 7) {// Makes sure account number is exactly 7 digits long
        throw TestException("Invalid Account Number: Account number must be exactly 7 digits long!");
    }
    
    
    for (int i = 0; i < validAccts.size(); i++){
        if (validAccts.at(i).getNum() == acctNum)
            return &validAccts.at(i);
    }
    throw TestException("Account does not exist!");
}
 
