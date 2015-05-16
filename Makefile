CC = gcc
FLAGS = -std=c99

Run : Shell
	./script.sh

Shell : Shell.o utilities.o trap.o
	$(CC) Shell.o utilities.o -o Shell

trap.o: trap.c trap.h
	$(CC) trap.c -c -o trap.o

Shell.o : Shell.c Shell.h
	$(CC) Shell.c -c -o Shell.o

utilities.o : utilities.c utilities.h
	$(CC) utilities.c -c -o utilities.o

clean :
	rm -f Shell.o *~
