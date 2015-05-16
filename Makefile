Shell: Shell.o
	gcc Shell.o -o Shell

Shell.o: Shell.c
	gcc Shell.c -c -o Shell.o

clean:
	rm -f Shell.o *~
