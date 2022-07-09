assembler: global_functions.o assembler.h
	gcc -g -ansi -Wall -pedantic global_functions.o -o assembler
global_functions.o: global_functions.c assembler.h
	gcc -c -ansi -Wall -pedantic global_functions.c -o global_functions.o