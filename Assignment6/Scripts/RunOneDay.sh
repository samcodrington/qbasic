#!/bin/bash
clear # clear terminal so its easier to see the test cases
cd Documents/Assignment6 # gets into local environment where all the test cases are located

echo "Simulating one day of the QBASIC program ...
"

echo "Running One Day: "
for i in TestInputs/OneDay/*.txt # for each transaction session in a particular day ...

do
	
echo "
Running session: ${i#TestInputs/OneDay/}
"

./QBASIC_FrontEnd_BJSInc.exe < $i  #input the session specific transactions into the qbasic front end program (since the valid accounts file is in the same location it does not need to be passed in as an argument).

mv QBASIC_TransactionSummaryFile.txt FrontEndOutputs/OneDay/${i#TestInputs/OneDay/} # puts the transaction summary file for the particular session into the corresponding "FrontEndOutput" directory
mv FrontEndOutputs/OneDay/${i#TestInputs/OneDay/} FrontEndOutputs/OneDay/QBASIC_TransactionSummaryFile_${i#TestInputs/OneDay/}.txt #Simply renames file as QBASIC_TransactionSummaryFile_(TestSession)

done # Loop until all day transaction sessions are completed

cat FrontEndOutputs/OneDay/*.txt > QBASIC_MergedTransactionSummaryFile.txt # Concatenates all the transaction summary files from the day into a merged transaction summary file
echo "EOS 0000000 000 0000000 ***" >> QBASIC_MergedTransactionSummaryFile.txt #adds an extra EOS transaction onto the end of the merged transaction summary file

./QBASIC_BackEnd_BJSInc.exe # Run the qbasic backend program (since the MTSF and old master accounts files are in the same location no files need to be passed in as arguments)

# After backend has run, the new valid account file and master account file are ready to be used for the next banking day

cp QBASIC_MasterAccountFile.txt BackEndOutputs/OneDay # Copies the master account file and valid account file t a backend output directory for safe keeping but still keeps them in the main directory for current day use
cp QBASIC_ValidAccountFile.txt BackEndOutputs/OneDay

exit

