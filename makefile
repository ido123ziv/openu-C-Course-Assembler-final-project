assembler: assembler.o pre_assembler.o global_functions.o 
	gcc -g -ansi -Wall -pedantic assembler.o pre_assembler.o global_functions.o -o assembler

assembler.o: assembler.c assembler.h pre_assembler.h global_functions.h 
	gcc -c -ansi -Wall -pedantic assembler.c -o assembler.o

pre_assembler.o: pre_assembler.c assembler.h pre_assembler.h global_functions.h 
	gcc -c -ansi -Wall -pedantic pre_assembler.c -o pre_assembler.o

global_functions.o: global_functions.c global_functions.h 
	gcc -c -ansi -Wall -pedantic global_functions.c -o global_functions.o