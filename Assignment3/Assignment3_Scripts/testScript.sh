#!/bin/bash
clear
for i in Documents/Assignment3/Assignment1_TestInputs/Test1_Login/*.txt
do
echo "Running test: $i
"
./Documents/Assignment3/QBASIC_BJSInc.exe < $i \
	> ../Documents/Assignment3/Assignment3_Output/Test1_Login/$i.log
echo "
"
done
exit

