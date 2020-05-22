#!/bin/bash

#Olivia Nye and Jacob Waxman 
#Systems and Networking 
#Lab 4 Part 1 - script1.sh


#question 2 - write to terminal and file
echo "hello world"
echo "hello world" >> results.txt

#questions 3-6
first=$1 #number - 6
second=$2 #number - 7
third=$3 #file name - results.txt
fourth=$4 #folder name - /users/olivianye/Documents

#question 6 - prints second arg
echo "The second argument: ${second}"

#question 7 - compare first and second arg
if test "$second" -lt "$first"; then 
	echo "Second argument (${second}) is less than the first (${first})."
else
	echo "Second argument (${second}) is greater than first (${first})."
fi



#question 7 - check if third arg exists/is a regular file
if test -e "$third"; then  
	echo "$third exists."	
	if test -f "$third"; then 
		echo "$third exists and is a regular file."
	else
		echo "$third exists but is not a regular file (it is a directory)."
	fi
else
	echo "$third does not exist!"
fi

#question 7 - check if fourth arg exists/is a regular file
if test -e "$fourth"; then  
	echo "$fourth exists."	
	if test -f "$fourth"; then 
		echo "$fourth exists and is a regular file."
	else
		echo "$fourth exists but is not a regular file (it is a directory)."
	fi
else
	echo "$fourth does not exist!"
fi

#question 8 - prints names of all of the pdf files in the directory provided as fourth argument
for file in ${4}/*; do
	if [ ${file: -4} == ".pdf" ]; then
		echo "$file"
	fi
done




exit