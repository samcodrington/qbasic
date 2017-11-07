#!/bin/bash
clear
cd Documents/Assignment3

echo "Checking difference of actual tested standard outputs and expected outputs:
"

echo "CHECKING DIFFERENCE IN LOGIN STD OUTPUT:
"
for i in Assignment3_STDOutputs/Test1_Login/*.txt
do
	
echo "Running test: ${i#Assignment3_STDOutputs/Test1_Login/}"

diff $i Assignment1_STDOutputs/Test1_Login/${i#Assignment3_STDOutputs/Test1_Login/}

done

echo "
CHECKING DIFFERENCE IN LOGOUT STD OUTPUT:
"
for i in Assignment3_STDOutputs/Test2_Logout/*.txt
do
	
echo "Running test: ${i#Assignment3_STDOutputs/Test2_Logout/}"

diff $i Assignment1_STDOutputs/Test2_Logout/${i#Assignment3_STDOutputs/Test2_Logout/}

done

echo "
CHECKING DIFFERENCE IN CREATEACCOUNT STD OUTPUT:
"
for i in Assignment3_STDOutputs/Test3_Createaccount/*.txt
do
	
echo "Running test: ${i#Assignment3_STDOutputs/Test3_Createaccount/}"

diff $i Assignment1_STDOutputs/Test3_Createaccount/${i#Assignment3_STDOutputs/Test3_Createaccount/}

done

echo "
CHECKING DIFFERENCE IN DELETEACCOUNT STD OUTPUT:
"
for i in Assignment3_STDOutputs/Test4_Deleteaccount/*.txt
do
	
echo "Running test: ${i#Assignment3_STDOutputs/Test4_Deleteaccount/}"

diff $i Assignment1_STDOutputs/Test4_Deleteaccount/${i#Assignment3_STDOutputs/Test4_Deleteaccount/}

done

echo "
CHECKING DIFFERENCE IN DEPOSIT STD OUTPUT:
"
for i in Assignment3_STDOutputs/Test5_Deposit/*.txt
do
	
echo "Running test: ${i#Assignment3_STDOutputs/Test5_Deposit/}"

diff $i Assignment1_STDOutputs/Test5_Deposit/${i#Assignment3_STDOutputs/Test5_Deposit/}

done

echo "
CHECKING DIFFERENCE IN WITHDRAW STD OUTPUT:
"
for i in Assignment3_STDOutputs/Test6_Withdraw/*.txt
do
	
echo "Running test: ${i#Assignment3_STDOutputs/Test6_Withdraw/}"

diff $i Assignment1_STDOutputs/Test6_Withdraw/${i#Assignment3_STDOutputs/Test6_Withdraw/}

done

echo "
CHECKING DIFFERENCE IN TRANSFER STD OUTPUT:
"
for i in Assignment3_STDOutputs/Test7_Transfer/*.txt
do
	
echo "Running test: ${i#Assignment3_STDOutputs/Test7_Transfer/}"

diff $i Assignment1_STDOutputs/Test7_Transfer/${i#Assignment3_STDOutputs/Test7_Transfer/}

done

echo "
CHECKING DIFFERENCE IN TRANSFER STD OUTPUT:
"
for i in Assignment3_STDOutputs/Test8_Checkaccountbalance/*.txt
do
	
echo "Running test: ${i#Assignment3_STDOutputs/Test8_Checkaccountbalance/}"

diff $i Assignment1_STDOutputs/Test8_Checkaccountbalance/${i#Assignment3_STDOutputs/Test8_Checkaccountbalance/}

done

echo "done ...
"

exit

