#!/bin/sh

clear
echo "@Launching \"ESI SHELL\" .."
echo

./Shell

if [ $? -eq 0 ] ; then
	echo "\n@Works fine \n"
else
	echo "\n@There was an error \n"
fi

