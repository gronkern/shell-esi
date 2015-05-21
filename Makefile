CC = gcc
FLAGS = -std=c99 -pedantic-errors

Run : Shell
	./Demo

Shell : Shell.o utilities.o 
	$(CC) $(FLAGS) Shell.o utilities.o -o Shell

Shell.o : Shell.c Shell.h
	$(CC) $(FLAGS) Shell.c -c -o Shell.o

utilities.o : utilities.c utilities.h
	$(CC) $(FLAGS) utilities.c -c -o utilities.o

clean :
	rm -f Shell *.o *~
