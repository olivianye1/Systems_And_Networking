#!/bin/bash

#Olivia Nye and Jacob Waxman 
#Systems and Networking 
#Lab 4 Part 3 - nameonly.sh

for file in ${1}/*; do
	#split file name on underscores, put all parts into array
	name_parts=($(echo ${file} | tr "_" "\n"))
	#rename as first part in array + .py
	mv ${file} "${name_parts[0]}.py"
done