//
//  QBASIC_BJSInc.cpp
//  QBASIC_BJSInc
//
//  Created by Jerry Haron on 2017-10-15.
//  Copyright © 2017 Jerry Haron. All rights reserved.
//

#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdio>

using namespace std;

// test exception class and functionality
class TestException {
public:
    TestException(const string& msg);
    string& what();
private:
    string message;
};
TestException::TestException(const string& msg) : message(msg) {}
string& TestException::what() { return message; }


// Pre-declarations for functions

vector<string> ReadAccountsFile(const string);

string Login(vector<string> acctvector, bool &);
void CreateAccount(vector<string> &,bool &);
void DeleteAccount(vector<string> &,bool &);
void Deposit(vector<string> &, string &, bool &);
void Withdraw(vector<string> &, string &, bool &, float &);
void Transfer(vector<string> &, string &, bool &);

void checkLegalTransactionAmount(bool, float);
string getDestAcct(vector<string> &, string);

//Constants for Transaction Limits
#define AGENT_MAX 1000000
#define ATM_MAX 1000

// -----Main-----
int main(){
    const string SummaryFile = "QBASIC_ValidAccountFile.txt";
    
	vector<string> validAccts = ReadAccountsFile(SummaryFile);
    
    freopen("QBASIC_TransactionSummaryFile.txt", "w", stderr);
    fclose(stderr); // immediately closes it so logs only are written when logged in
    
	while (true) {

        bool isLoggedIn = false;
        bool loginType = false;
        string currentAccount; //Fixes scope issues by creating currentAccount var with an empty account
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
            currentAccount = Login(validAccts, loginType);
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
            printf("Logged in as account %s\n",currentAccount.c_str());
            freopen("QBASIC_TransactionSummaryFile.txt", "a+", stderr); //Opens a log file and writes all "clog" output to it
			float totalWithdrawal = 0;
			cin >> buffer;

            if (buffer == "Login") {
                puts("User already logged in!");
            }
			else if (buffer == "Logout") {
				isLoggedIn = false;
                loginType = false;
                clog<<"EOS 0000000 0000 00000000 ***"<<endl;
                fclose(stderr);
				puts("Logged out");
			}
            else if (buffer == "Exit") {
                goto exit;
            }
			else if (buffer == "Createaccount") {
                try {
                    CreateAccount(validAccts, loginType); //pass vector of valid accounts by refrence and if the current account is an agent account
                } catch (TestException& e) {
                    if (e.what() == "Exit")
                        goto exit;
                    cout << e.what() << endl;
                }
               
			}
			else if (buffer == "Deleteaccount") {
                try {
                    DeleteAccount(validAccts, loginType); //pass vector of valid accounts by refrence and if the current account is an agent account
                } catch (TestException& e) {
                    if (e.what() == "Exit")
                        goto exit;
                    cout << e.what() << endl;
                }
                
			}
			else if (buffer == "Deposit") {
                try {
                    Deposit(validAccts,currentAccount, loginType); //pass vector of valid accounts by refrence and if the current account is an agent account
                } catch (TestException& e) {
                    if (e.what() == "Exit")
                        goto exit;
                    cout << e.what() << endl;
                }
               }
			else if (buffer == "Withdraw") {
                try {
                    Withdraw(validAccts,currentAccount, loginType, totalWithdrawal); //pass vector of valid accounts by refrence and if the current account is an agent account
                } catch (TestException& e) {
                    if (e.what() == "Exit")
                        goto exit;
                    cout << e.what() << endl;
                }
            }
			else if (buffer == "Transfer") {
                try {
                    Transfer(validAccts, currentAccount, loginType); //pass vector of valid accounts by refrence and if the current account is an agent account
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
    return 0;
}

//-----IO FUNCTIONS-----

vector<string> ReadAccountsFile(const string masterfilename) {
    // ReadAccountsFile function assumes that master accounts file format is correct since it itself was created by the backend program
    vector<string> acctNumbers;
    
    string ln,strhold;
    ifstream fileIn(masterfilename.c_str());
    
    if (fileIn.fail()) {
        return acctNumbers;
    }
    
    //This while loop iterates once per line for all of the lines in the Accounts file.
    // Pushes back each relevant value into a vector (Assumes space separation of fields)
    while (getline(fileIn, ln)){
        istringstream strng(ln);
        getline(strng, strhold);
        acctNumbers.push_back(strhold);
    }
    fileIn.close();
    
  
    return acctNumbers;
}

//-----------TRANSACTION FUNCTIONS ------------------/

string Login(vector<string> validAccts, bool &loginType) {
    string buffer, acctNumber, PIN;
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
    cin >> acctNumber;
    if (buffer == "Exit")
        throw TestException("Exit");

    if (acctNumber.find_first_not_of("0123456789") != string::npos) {// Makes sure newNumber is only numerical
        throw TestException("Invalid Account Number: Account number can only have numbers!");
    }
    
    if (acctNumber[0] == '0') {// Makes sure first digit of account number isnt 0
        throw TestException("Invalid Account Number: Account number cannot start with 0!");
    }
    
    if (acctNumber.length() != 7) {// Makes sure account number is exactly 7 digits long
        throw TestException("Invalid Account Number: Account number must be exactly 7 digits long!");
    }
    
	for (int i = 0; i < validAccts.size(); i++) {
        if (validAccts.at(i) == acctNumber){
                    return validAccts.at(i);
        }
    }
    throw TestException("Invalid Account Number: Account does not exist!");
}


/**CreateAccount is a method which creates and adds a new user defined account to the valid accounts vector by refrence in a void function**/

void CreateAccount(vector<string> &validAccounts, bool &isAgent){ //Accepts the enire valid accounts file by refrence so it can easily add another account onto the end. Also accepts the 'isAgent' accessor value from currentAccount for the isAgent check
    string newNumber, newName, buffer;
    
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
        if (newNumber == validAccounts.at(i)){
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

        printf("Account has been created with Number: \"%s\", Name: \"%s\"\n",newNumber.c_str(), newName.c_str());
        clog<<"NEW "<< newNumber.c_str()<<" 000 0000000 "<< newName.c_str()<<endl;
    return;
}

/**DeleteAccount is a method which deletes a user defined account from the valid accounts vector by refrence in a void function**/
void DeleteAccount(vector<string> &validAccounts, bool &isAgent){//Accepts the enire valid accounts file by refrence and deletes the single account number the user inputs. Also accepts the 'isAgent' accessor value from currentAccount for the isAgent check
    
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
        if (delNumber == validAccounts.at(i)){
            
            puts("Input account name:"); //When function has found the corresponding account, it then prompts for name and number
            cin>>delName;
            if (delName == "Exit")
                throw TestException("Exit");
            
            if (delName.find_first_not_of("0123456789 abcdefghijklmnopqurstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ") != string::npos) {// Makes sure newName is only numerical, alphabetical (Spaces allowed)
                throw TestException("Invalid Account Name: Only input letters numbers and spaces!");
            }
            
            if (delName[0] == ' ') {// Makes sure first digit of account name isnt a space
                throw TestException("Invalid Account Name: Space not allowed at beginning of name!");
            }
            
            if (delName.length() < 3 || delName.length() > 20) {// Makes sure name is between 3-20 chars
                throw TestException("Invalid Account Name: Must be 3-20 characters long!");
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

void Deposit(vector<string> &validAccounts,string &currentAccount, bool &isAgent){
    string destAcct, buffer;
    float amount = 0.0;
    
    puts("Enter destination account:");
    cin>>destAcct;
    
    if (destAcct == "Exit")
        throw TestException("Exit");
    
    if(!isAgent && currentAccount != getDestAcct(validAccounts, destAcct)){
        throw TestException("Invalid account: User cannot deposit to other accounts!");
    }
    
    destAcct = getDestAcct(validAccounts, destAcct);
    
    puts("Enter the amount you wish to deposit:");
    cin>>buffer;
    if (buffer == "Exit")
        throw TestException("Exit");
    
    if (buffer.find_first_not_of("-0123456789") != string::npos) {// Makes sure amount is only numerical
        throw TestException("Invalid Amount: Amount can only have numbers!");
    }
    try{
        amount = stof(buffer);
    } catch(const std::invalid_argument&){
        amount = 0;
        throw TestException("Invalid Amount!");
    }
    checkLegalTransactionAmount(isAgent, amount);
    
    clog<<"DEP "<<getDestAcct(validAccounts, destAcct)<<" "<< amount*100<<" 0000000 ***"<<endl;
    
}

void Withdraw(vector<string> &validAccounts, string &currentAccount, bool &isAgent,float &toWithdraw){
    string destAcct, buffer;
    float amount;
    
    puts("Enter destination account:");
    cin>>destAcct;
    
    if (destAcct == "Exit")
        throw TestException("Exit");
    
    if(!isAgent && currentAccount != getDestAcct(validAccounts, destAcct)){
        throw TestException("Invalid account: User cannot deposit to other accounts!");
    }
    
    puts("Enter the amount you wish to withdraw:");
    cin>>buffer;
    if (buffer == "Exit")
        throw TestException("Exit");
    
    if (buffer.find_first_not_of("-0123456789") != string::npos) {// Makes sure amount is only numerical
        throw TestException("Invalid Amount: Amount can only have numbers!");
    }
    amount = stof(buffer);
    
    checkLegalTransactionAmount(isAgent, amount);
    toWithdraw = toWithdraw + amount;
    
    if (!isAgent && toWithdraw > 1000) {
        throw TestException("Invalid Amount: Maximum withdraw amount reached for single session!");
    }

    clog<<"WDR "<<destAcct<<" "<< amount*100<<" 0000000 ***"<<endl;
    
}

void Transfer(vector<string> &validAccounts, string &currentAccount, bool &isAgent){
    string fromAccount, toAccount, buffer;
    float amount;
    
    puts("Enter account to transfer from:");
    cin>>fromAccount;
    
    if (fromAccount == "Exit")
        throw TestException("Exit");
    
    fromAccount = getDestAcct(validAccounts, fromAccount);
    
    if(!isAgent && currentAccount != getDestAcct(validAccounts, fromAccount)){
        throw TestException("Invalid account: User cannot transfer from account that is not thier own!");
    }
    
    puts("Enter account to transfer to:");
    cin>>toAccount;
    
    if (toAccount == "Exit")
        throw TestException("Exit");
    
    toAccount = getDestAcct(validAccounts, toAccount);
    
    puts("Enter the amount you wish to transfer:");
    cin>>buffer;
    if (buffer == "Exit")
        throw TestException("Exit");
    
    if (buffer.find_first_not_of("-0123456789") != string::npos) {// Makes sure amount is only numerical
        throw TestException("Invalid Amount: Amount can only have numbers!");
    }
    amount = stof(buffer);
    checkLegalTransactionAmount(isAgent, amount);

    clog<<"XFR "<<fromAccount<<" "<< amount*100<<" "<<toAccount<<" ***"<<endl;
    
    
}

/**
 * CheckLegalTransactionAmount checks the trasnactionAmt parameter agianst account conditions for a legal transaction
 * If they are met the function returns true, otherwise  it returns false */
void checkLegalTransactionAmount(bool isAgent, const float transactionAmt){
    
    //Return false for negative amount
    if (transactionAmt < 0){
        throw TestException("Invalid transaction amount: Cannot Deposit Negative Amount!");
    }
    //return false if an agent is attempting to transact amount over the limit
    if (isAgent && transactionAmt >= AGENT_MAX){
        throw TestException("Invalid transaction amount: Agent Cannot Issue Transactions greater than 999,999.99!");
    }
    //Return false if an ATM is attempting to transact amount over the ATM limit
    if (!isAgent && transactionAmt > ATM_MAX){
        throw TestException("Invalid transaction amount: Machine Users Cannot Issue Transactions greater than 1000!");
    }
}

string getDestAcct(vector<string> &validAccts, string acctNum){
    
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
        if (validAccts.at(i) == acctNum)
            return validAccts.at(i);
    }
    throw TestException("Account does not exist!");
}
