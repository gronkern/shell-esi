CC = gcc
FLAGS = -std=c99

Run : Shell
	./Shell

Shell: Shell.o
	$(CC) Shell.o -o Shell

Shell.o: Shell.c Shell.h
	$(CC) Shell.c -c -o Shell.o

clean:
	rm -f Shell.o *~
