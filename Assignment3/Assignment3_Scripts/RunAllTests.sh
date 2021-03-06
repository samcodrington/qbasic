#!/bin/bash
clear # clear terminal so its easier to see the test cases
cd Documents/Assignment3 # gets into local environment where all the test cases are located

echo "Testing front end and standard outputs of the QBASIC program ...
"

echo "RUNNING LOGIN TESTS:
"
for i in Assignment1_TestInputs/Test1_Login/*.txt # for each file text file in Assignment1_TestInputs/Test1_Login/ ...
do
	
echo "Running test: ${i#Assignment1_TestInputs/Test1_Login/}"

./QBASIC_BJSInc.exe < $i > ${i#Assignment1_TestInputs/Test1_Login/} #input the text from appropriate assignment 1 test case into the QBASIC.exe program then pipe the standard output into a text file identical in name to the input file
mv ${i#Assignment1_TestInputs/Test1_Login/} Assignment3_STDOutputs/Test1_Login # put new standard output file in an appropriate associated file for easier testing in the compare all tests script# loops identical to above are ran albiet with different path location for different tests (i.e. a loop for login, createaccount ...)


done
# loops identical to above are ran albiet with different path location for different tests (i.e. a loop for login, createaccount ...)
echo "
RUNNING LOGOUT TESTS:
"
for i in Assignment1_TestInputs/Test2_Logout/*.txt
do
	
echo "Running test: ${i#Assignment1_TestInputs/Test2_Logout/}"

./QBASIC_BJSInc.exe < $i > ${i#Assignment1_TestInputs/Test2_Logout/}
mv ${i#Assignment1_TestInputs/Test2_Logout/} Assignment3_STDOutputs/Test2_Logout

done

echo "
RUNNING CREATEACCOUNT TESTS:
"
for i in Assignment1_TestInputs/Test3_Createaccount/*.txt
do
	
echo "Running test: ${i#Assignment1_TestInputs/Test3_Createaccount/}"

./QBASIC_BJSInc.exe < $i > ${i#Assignment1_TestInputs/Test3_Createaccount/}
mv ${i#Assignment1_TestInputs/Test3_Createaccount/} Assignment3_STDOutputs/Test3_Createaccount

done

echo "
RUNNING DELETEACCOUNT TESTS:
"
for i in Assignment1_TestInputs/Test4_Deleteaccount/*.txt
do
	
echo "Running test: ${i#Assignment1_TestInputs/Test4_Deleteaccount/}"

./QBASIC_BJSInc.exe < $i > ${i#Assignment1_TestInputs/Test4_Deleteaccount/}
mv ${i#Assignment1_TestInputs/Test4_Deleteaccount/} Assignment3_STDOutputs/Test4_Deleteaccount

done

echo "
RUNNING DEPOSIT TESTS:
"
for i in Assignment1_TestInputs/Test5_Deposit/*.txt
do
	
echo "Running test: ${i#Assignment1_TestInputs/Test5_Deposit/}"

./QBASIC_BJSInc.exe < $i > ${i#Assignment1_TestInputs/Test5_Deposit/}
mv ${i#Assignment1_TestInputs/Test5_Deposit/} Assignment3_STDOutputs/Test5_Deposit

done

echo "
RUNNING WITHDRAW TESTS:
"
for i in Assignment1_TestInputs/Test6_Withdraw/*.txt
do
	
echo "Running test: ${i#Assignment1_TestInputs/Test6_Withdraw/}"

./QBASIC_BJSInc.exe < $i > ${i#Assignment1_TestInputs/Test6_Withdraw/}
mv ${i#Assignment1_TestInputs/Test6_Withdraw/} Assignment3_STDOutputs/Test6_Withdraw

done

echo "
RUNNING TRANSFER TESTS:
"
for i in Assignment1_TestInputs/Test7_Transfer/*.txt
do
	
echo "Running test: ${i#Assignment1_TestInputs/Test7_Transfer/}"

./QBASIC_BJSInc.exe < $i > ${i#Assignment1_TestInputs/Test7_Transfer/}
mv ${i#Assignment1_TestInputs/Test7_Transfer/} Assignment3_STDOutputs/Test7_Transfer

done

echo "
RUNNING CHECKACCOUNTBALANCE TESTS:
"
for i in Assignment1_TestInputs/Test8_Checkaccountbalance/*.txt
do
	
echo "Running test: ${i#Assignment1_TestInputs/Test8_Checkaccountbalance/}"

./QBASIC_BJSInc.exe < $i > ${i#Assignment1_TestInputs/Test8_Checkaccountbalance/}
mv ${i#Assignment1_TestInputs/Test8_Checkaccountbalance/} Assignment3_STDOutputs/Test8_Checkaccountbalance

done

echo "done ...
"

exit

