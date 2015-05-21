CC = gcc
FLAGS = -std=c99 -pedantic-errors

Run : Shell
	./Demo

Shell : Shell.o utilities.o trap.o
	$(CC) $(FLAGS) Shell.o utilities.o trap.o -o Shell

trap.o: trap.c trap.h
	$(CC) $(FLAGS) trap.c -c -o trap.o

Shell.o : Shell.c Shell.h
	$(CC) $(FLAGS) Shell.c -c -o Shell.o

utilities.o : utilities.c utilities.h
	$(CC) $(FLAGS) utilities.c -c -o utilities.o

clean :
	rm -f Shell *.o *~
