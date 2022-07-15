assembler: assembler.o pre_assembler.o global_functions.o phase_one.o 
	gcc -g -ansi -Wall -pedantic assembler.o pre_assembler.o phase_one.o global_functions.o -o assembler

assembler.o: assembler.c assembler.h pre_assembler.h global_functions.h 
	gcc -c -ansi -Wall -pedantic assembler.c -o assembler.o

phase_one.o: phase_one.c assembler.h phase_one.h global_functions.h 
	gcc -c -ansi -Wall -pedantic phase_one.c -o phase_one.o

pre_assembler.o: pre_assembler.c assembler.h pre_assembler.h global_functions.h 
	gcc -c -ansi -Wall -pedantic pre_assembler.c -o pre_assembler.o

global_functions.o: global_functions.c global_functions.h 
	gcc -c -ansi -Wall -pedantic global_functions.c -o global_functions.o

clean: 
	rm *.o assembler