assembler: assembler.o pre_assembler.o global_functions.o assembler.h 
	gcc -g -ansi -Wall -pedantic assembler.o pre_assembler.o global_functions.o -o assembler
assembler.o: assembler.c global_functions.c assembler.h
	gcc -g -ansi -Wall -pedantic assembler.c -o assembler.o
pre_assembler.o: pre_assembler.c global_functions.c assembler.h
	gcc -g -ansi -Wall -pedantic pre_assembler.c -o pre_assembler.o
global_functions.o: global_functions.c assembler.h
	gcc -c -ansi -Wall -pedantic global_functions.c -o global_functions.o