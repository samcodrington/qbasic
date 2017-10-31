//
//  QBASIC_BJSInc.cpp
//  QBASIC_BJSInc
//
//  Created by Jerry Haron on 2017-10-15.
//  Copyright © 2017 Jerry Haron. All rights reserved.
//

#include <iostream>
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

bool checkValidAccount(const Account, const string acctNum);
bool checkValidNumber(const string number, float* num);
bool checkLegalTransactionAmount(Account thisAcct, const float* transactionAmt);

Account* getDestAcct(vector<Account> &, string);

//Constants for Transaction Limits
#define AGENT_MAX 1000
#define ATM_MAX 1000000

// -----Main-----
int main(){
    const string AccountFile = "QBASIC_AccountFile.txt";
    
	vector<Account> validAccts = ReadAccountsFile(AccountFile);
    freopen("QBASIC_LogFile.txt", "w", stderr); //Opens a log file and writes all "clog" output to it
    
	while (true) {

        bool isLoggedIn = false;
        Account currentAccount = Account(); //Fixes scope issues by creating currentAccount var with an empty account
		string buffer;
        
		while (!isLoggedIn) { //The very first function input should be 'Login' else an error message is shown to the user
            puts("Type \"Login\" to log in; Type \"Exit\" to leave the program:");
            clog<<"Type \"Login\" to log in; Type \"Exit\" to leave the program:"<< endl;
            cin >> buffer;
            
			while (buffer != "Login") {//The program keeps pinging the user for an input until a 'Login'
				
                if (buffer == "Exit") {
					goto exit; // Exit command exits the program
				}
				puts("Must be logged in.");
                clog<<"Must be logged in."<<endl;
				cin >> buffer;
			}
            isLoggedIn = true; // If "Login" command is typed into buffer, program proceeds to below try-catch block, where user actually inputs the account they want to log in as

		}
        
        
        try {// if the user sucessfully logs in, then they can enter the second while loop where they can perform the rest of the functionality
            Account currentAccount = Login(validAccts);
            puts("Login success!");
            clog << "Login success!" << endl;
            isLoggedIn = true;
        }
        catch (TestException& e) { // if user fails to sucessfully login i.e. with invalid PIN or acct number, they will not be logged in, and return to the previous while loop that prompts the user for a "Login command"
            cout << e.what() << endl;
            clog << e.what() << endl;
            isLoggedIn = false;
        }
        
        
		while (isLoggedIn) {
            
			float totalWithdrawal = 0;
			cout<<"Please Type the Action You Wish To Do"<<endl;
            clog<<"Please Type the Action You Wish To Do"<<endl;
			cin >> buffer;

            if (buffer == "Login") {
                puts("User already logged in!");
                clog<< "User already logged in!"<<endl;
            }
			else if (buffer == "Logout") {
				isLoggedIn = false;
				puts("Logout");
                clog << "Logout" << endl;
			}
            else if (buffer == "Exit") {
                goto exit;
            }
			else if (buffer == "Createaccount") {
                try {
                    CreateAccount(validAccts, currentAccount.isAgent()); //pass vector of valid accounts by refrence and if the current account is an agent account
                } catch (TestException& e) {
                    cout << e.what() << endl;
                    clog << e.what() << endl;
                }
               
			}
			else if (buffer == "Deleteaccount") {
                try {
                    DeleteAccount(validAccts, currentAccount.isAgent()); //pass vector of valid accounts by refrence and if the current account is an agent account
                } catch (TestException& e) {
                    cout << e.what() << endl;
                    clog << e.what() << endl;
                }
                
			}
			else if (buffer == "Deposit") {
            
			
				bool isFirstAttempt =true;
				bool exit = false;
				//Get Valid Destination Account for Deposit
				//This do-while loop will prompt the user to enter a destiniation account until a correct account is entered.
				
             while (checkValidAccount(currentAccount, buffer)){
					if (!isFirstAttempt)
					puts("Enter the Destination Account Or Type \"Exit\" to Cancel Deposit");
					cin >> buffer;
					if (buffer == "Exit") {
						break;
						exit = true;
					}
				}

				if (exit == true) {break;} //if Exit was entered in the last do-while, leave the deposit loop

				string destAccount = buffer;
				//Get Valid Amount to Deposit
				float* deposit = 0;
				//These do-while loops prompt the user to enter a valid amount and then verify that it is a legal transaction 
				do {
					//Check input is valid

					do {
						puts("Enter the Amount you wish to Deposit Or Type \"Exit\" to Cancel Deposit");
						cin >> buffer;
            
						if (buffer == "Exit") {
							break;
							exit = true;
						}
					} while (checkValidNumber(buffer, deposit));
					//Checks amount is legal
				} while (checkLegalTransactionAmount(currentAccount, deposit));
				if (exit == true) break;
				currentAccount.changeBalance(*deposit);
            }
			else if (buffer == "Withdraw") {
				/*bool exit = false;
				transactionFileStream << "Withdraw" << "\n";
				//Get Valid Destination Account for Withdrawal
				//This do-while loop will prompt the user to enter a withdrawal account until a correct account is entered.
				do {
					puts("Enter the Destination Account Or Type \"Exit\" to Cancel Withdrawal");
					cin >> buffer;
					transactionFileStream << "Withdraw Account:" << "\n";
					transactionFileStream << buffer << "\n";
					if (buffer == "Exit") {
						break;
						exit = true;
					}
				} while (checkValidAccount(currentAccount, buffer));

				if (exit == true) break; //if Exit was entered in the last do-while, leave the loop

				string destAccount = buffer;
				//Get Valid Amount to Deposit
				float* withdraw = 0;
				float* curWithdraw = 0;
				//These do-while loops prompt the user to enter a valid amount and then verify that it is a legal transaction
				do {
					//Check input is valid
					do {
						puts("Enter the Amount you wish to Withdraw Or Type \"Exit\" to Cancel Withdrawal");
						cin >> buffer;
						transactionFileStream << "Withdraw amount:" << "\n";
						transactionFileStream << buffer << "\n";
						if (buffer == "Exit") {
							break;
							exit = true;
						}
					} while (checkValidNumber(buffer, withdraw));

					if (exit == true) break;
					//Checks amount is legal
					*curWithdraw = *withdraw + totalWithdrawal;
				} while (checkLegalTransactionAmount(currentAccount, curWithdraw) && currentAccount.overdraftCheck(*withdraw));

				if (exit == true) break;
				currentAccount.changeBalance(-*withdraw);
				totalWithdrawal += *withdraw;*/
            }
			else if (buffer == "Transfer") {
				/*transactionFileStream << "Transfer" << "\n";
				bool exit = false;
				Account* destAccount = NULL;
				//Get Valid Destination Account for Withdrawal
				//This do-while loop will prompt the user to enter a withdrawal account until a correct account is entered.
				do {
					puts("Enter the Source Account Or Type \"Exit\" to Cancel Transfer");
					transactionFileStream << "Transfer Source Account" << "\n";
					cin >> buffer;
					transactionFileStream << buffer << "\n";
					if (buffer == "Exit") {
						break;
						exit = true;
					}
				} while (checkValidAccount(currentAccount, buffer));

				if (exit == true) break; //if Exit was entered in the last do-while, leave the loop

				//This dowhile loops until the user has entered a valid destination account or has exited.
				do {
					puts("Enter the Destination Account Or Type \"Exit\" to Cancel Transfer");
					transactionFileStream << "Transfer Source Account" << "\n";
					cin >> buffer;
					transactionFileStream << buffer << "\n";
					if (buffer == "Exit") {
						break;
						exit = true;
					}
					destAccount = getDestAcct(validAccts, buffer);
				} while (destAccount != NULL);

				if (exit == true) break; //if Exit was entered in the last do-while, leave the loop

				//Get Valid Amount to Deposit
				float* transfer = 0;
				//These do-while loops prompt the user to enter a valid amount and then 
				do {
					//Check input is valid
					do {
						puts("Enter the Amount you wish to Withdraw Or Type \"Exit\" to Cancel Transfer");
						transactionFileStream << "Transfer Amount" << "\n";
						cin >> buffer;
						transactionFileStream << buffer << "\n";
						if (buffer == "Exit") {
							break;
							exit = true;
						}
					} while (checkValidNumber(buffer, transfer));

					if (exit == true) break;
					//Checks amount is legal
				} while (checkLegalTransactionAmount(currentAccount, transfer) && currentAccount.overdraftCheck(*transfer));

				if (exit == true) break;
				currentAccount.changeBalance(-*transfer);
				destAccount->changeBalance(*transfer);*/
            }
                
			else if (buffer == "CheckAccountBalance") {
				/*transactionFileStream << "CheckAccountBalance" << "\n";
				bool exit = false;
				//Get Valid Destination Account for Withdrawal
				//This do-while loop will prompt the user to enter a withdrawal account until a correct account is entered.
				do {
					puts("Enter the Target Account Or Type \"Exit\" to Cancel Withdrawal");
					transactionFileStream << "Checked Acount" << "\n";
					cin >> buffer;
					transactionFileStream << buffer << "\n";
					if (buffer == "Exit") {
						break;
						exit = true;
					}
				} while (checkValidAccount(currentAccount, buffer));

				if (exit == true) break; //if Exit was entered in the last do-while, leave the deposit loop

				string destAccount = buffer;*/
			}
			else {
				puts("Invalid command!");
                clog<<"Invalid command!"<<endl;
			}

		}
	}
    exit: //if user types Exit, the goto function leads back to here
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

//-----------TRANSACTION FUNCTIONS ------------------/

Account Login(vector<Account> acctvector) {
    string buffer, acctNumber, PIN;
    bool loginType;
    cin >> buffer;
    
    puts("Please input login session type:");
    clog<<"Please input login session type:"<<endl;
    
    if (buffer == "Machine") {
        loginType = 0;
    } else if (buffer == "Agent") {
        loginType = 1;
    }
    else{
        throw TestException("Invalid Session Type");
    }
    puts("Please input login account:");
    clog<<"Please input login account:"<<endl;
    cin >> buffer;
    acctNumber = buffer;
    
    puts("Please input PIN:");
    clog<<"Please input PIN:"<<endl;
    cin >> buffer;
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


/**CheckValidAccount is a method which returns true if the account Number is the same as the currentAccount logged on
 * or if the account is an agent. 
 * Does not check whether account exists or not!**/
bool checkValidAccount(Account currentAccount, const string acctNum){
    // Check AcctNum exists 
        /**TODO**/

    // Checks permissions    
    if(currentAccount.getNum() == acctNum) //Check if transaction being done on currently logged in Account
        return true; //If the account is being
    else{
        if (currentAccount.isAgent()) return true;
        else{
            puts("Attempting Transaction on Invalid Account Number. Please Try Again");
            return false;
        }
    }
}
/**
 * checkValidNumber checks whether a given string can be convereted into a number and returns true or false.
 * The num parameter is changed by reference to the converted string if successful.
 * **/
bool checkValidNumber(const string input, float* num){
    try{
        *num = std::stoi(input);
        return true;
    } catch(std::invalid_argument){
        puts("Error Invalid Number! Please enter only Numbers with no Special Characters or Letters!");
    }
    return false;
}


/**
 * CheckLegalTransactionAmount checks the trasnactionAmt parameter agianst account conditions for a legal transaction
 * If they are met the function returns true, otherwise  it returns false */
bool checkLegalTransactionAmount(Account thisAcct, const float* transactionAmt){
    //Return false for negative amount
    if (*transactionAmt < 0){
        puts("Error: Cannot Deposit Negative Amount");
        return false;
    }
    //return false if an agent is attempting to transact amount over the limit
    if (thisAcct.isAgent() && *transactionAmt >= AGENT_MAX){
       puts("Error! Cannot Deposit Amount equal to or over $" + AGENT_MAX);
       return false;       
    }
    //Return false if an ATM is attempting to transact amount over the ATM limit
    if (!thisAcct.isAgent() && *transactionAmt > ATM_MAX){
        puts("Error! ATM Users Cannot Issue Transactions greater than $" + ATM_MAX);
        return false;
    }
    return true;
}
/**
 * getDestAcct is a simple helper function that iterates through the validAccts parameter to see if it matches the acctNum provided. 
 * If it does it match the funciton returns that Acccount. If it doesn't it returns NULL.
 * */
Account* getDestAcct(vector<Account> & validAccts, string acctNum){
    for (int i = 0; i < validAccts.size(); i++){
        if (validAccts.at(i).getNum() == acctNum)
            return &validAccts.at(i);
    }
    return NULL;
}
// ---------------
            
            
/**CreateAccount is a method which creates and adds a new user defined account to the valid accounts vector by refrence in a void function**/
void CreateAccount(vector<Account> &validAccounts, bool isAgent){ //Accepts the enire valid accounts file by refrence so it can easily add another account onto the end. Also accepts the 'isAgent' accessor value from currentAccount for the isAgent check
    string newNumber, newName, newPIN, buffer;
    bool newAgent;
    
    if (isAgent == 0){ // returns to main if currentAccount is not an agent account
        throw TestException("Invalid session type for command!");
    }
    puts("Input Account Number:");
    clog<<"Input Account Number:"<<endl;
    cin>>newNumber;
    
    if (newNumber.find_first_not_of("0123456789") == string::npos) {// Makes sure newNumber is only numerical
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
    clog<<"Input Account Name:"<<endl;
    cin>>newName;
    
    if (newName.find_first_not_of("0123456789 abcdefghijklmnopqurstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ") == string::npos) {// Makes sure newName is only numerical, alphabetical (Spaces allowed)
        throw TestException("Invalid Account Name: Only input letters numbers and spaces!");
    }
    
    if (newName[0] == ' ') {// Makes sure first digit of account name isnt a space
        throw TestException("Invalid Account Name: Space not allowed at beginning of name!");
    }
    
    if (newName.length() < 3 || newName.length() > 20) {// Makes sure name is between 3-20 chars
       throw TestException("Invalid Account Name: Space not allowed at beginning of name!");
    }
    
    puts("Input account PIN:");
    clog<<"Input account PIN:"<<endl;
    cin>>newPIN;
    
    if (newNumber.find_first_not_of("0123456789") == string::npos) {// Makes sure PIN is only numerical
        throw TestException("Invalid Account PIN: New PIN can only have numbers!");
    }
    
    if (newPIN.length() != 4) {// Makes sure PIN is 4 numbers long
        throw TestException("Invalid Account PIN: PIN must be 4 digits long!");
    }
    
    
    puts("Input account type:"); //Last input to the function asks for accout type
    clog<<"Input account type:"<<endl;
    cin>>buffer; // If not agent or machine, the program returns to the beginning
    
    if (buffer =="Machine") {
        newAgent = 0;
        printf("User account has been created with Number: \"%s\", Name: \"%s\", and PIN: \"%s\"",newNumber.c_str(), newName.c_str(), newPIN.c_str());
         clog<<"Machine account has been created with Number: \""<<newNumber.c_str()<<"\", Name: \""<<newName.c_str()<<"\", and PIN: \""<<newPIN.c_str()<<"\""<<endl;
        
    }
    else if (buffer == "Agent"){
        newAgent = 1;
        printf("Agent account has been created with Number: \"%s\", Name: \"%s\", and PIN: \"%s\"",newNumber.c_str(), newName.c_str(), newPIN.c_str());
        clog<<"Agent account has been created with Number: \""<<newNumber.c_str()<<"\", Name: \""<<newName.c_str()<<"\", and PIN: \""<<newPIN.c_str()<<"\""<<endl;
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
    
    if (isAgent == 0){ // returns to main if currentAccount is not an agent account
        throw TestException("Invalid session type for command!");
    }
    
    puts("Input account type:");
    clog<<"Input account type:"<<endl;
    cin>>delNumber;
    
    if (delNumber.find_first_not_of("0123456789") == string::npos || delNumber.length() != 7 || delNumber[0] == '0') {// Makes sure delNumber is a valid account number to begin with
        throw TestException("Invalid Account Number: Input is not a possible account number!!");
    }
    
    for (int i =0; i<validAccounts.size(); i++) { // if the account number isnt in the valid accounts vector then it has not been created yet
        if (delNumber == validAccounts.at(i).getNum()){
            
            puts("Input account name:"); //When function has found the corresponding account, it then prompts for name and number
            clog<<"Input account name:"<<endl;
            cin>>delName;
            
            if (delName != validAccounts.at(i).getName()) { //Incorrect name returns
                throw TestException("Incorrect name!");
            }
            
            puts("Input account PIN:");
            clog<<"Input account PIN:"<<endl;
            cin>>delName;
            
            if (delPIN != validAccounts.at(i).getPIN()) { //Incorrect PIN returns
                throw TestException("Incorrect PIN!");
            }
                
                //If all details line up then account is deleted by refrence from the valid accounts vector
                validAccounts.erase(validAccounts.begin() + i);
            printf("Account \"%s\" has been deleted!", delNumber.c_str());
                return;
            }
    }
    
    throw TestException("Invalid Account Number: Account does not exist!"); //If delNumber was not in the valid accounts vector, function returns
}

