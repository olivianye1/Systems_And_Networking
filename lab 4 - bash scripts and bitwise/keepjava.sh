#!/bin/bash

#Olivia Nye and Jacob Waxman 
#Systems and Networking 
#Lab 4 Part 2 - keepjava.sh

for file in ${1}/*; do
	if [ ${file: -5} != ".java" ]; then
		rm "$file"
	fi
done