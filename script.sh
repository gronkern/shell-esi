#!/bin/sh

clear
echo "Launching \"ESI SHELL\" .."
echo
echo
echo "Press any key"

read 
clear

./Shell

if [ $? -eq 0 ] ; then
	echo "\n Works fine \n"
else
	echo "\n There was an error \n"
fi

