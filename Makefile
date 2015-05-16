CC = gcc
FLAGS = -std=c99

Run : Shell
	./Shell

Shell : Shell.o utilities.o
	$(CC) Shell.o utilities.o -o Shell

Shell.o : Shell.c Shell.h
	$(CC) Shell.c -c -o Shell.o

utilities.o : utilities.c utilities.h
	$(CC) utilities.c -c -o utilities.o

clean :
	rm -f Shell.o *~
