# openu-C-Course-Assembler-final-project
The final project of [20465 Course](https://www.openu.ac.il/courses/20465.htm)  in the Open University of Israel, Building Assembly using C.  
The project accepts file names without extenstions, assembles them and returns to the user the binary files to run as the assembler program.  

## Main Language 
```C
printf("We write in C !?!")
```
## Prepare
In order to run our project you need to have installed:   
* `make`
* `gcc`

## How to run?
```bash
./assembler Tests/my_test
```
**OR**  
```bash
./assembler macro
```
**To run automated testing**
```bash
./run_test.sh
```

## Directory Structure
```bash
├── assembler.c -> main file
├── assembler.h -> main headers file for this project
├── assignment.pdf -> explanation of the assignments
├── global_functions.c -> utilites file
├── global_functions.h -> headers of utilites file
├── makefile -> make file to run `make` and build the project
├── phase_one.c -> first phase that encodes the program
├── phases.h -> headers for phases
├── phase_two.c  -> second phase that write the output for user
├── pre_assembler.c -> spreads the macros in the program
├── pre_assembler.h -> headers for pre_assembler
├── README.md -> YOU ARE HERE
├── run_test.sh -> script for running tests
└── Tests -> folder which we hold the test files 
```
## Authors
[@ido123ziv](https://github.com/ido123ziv)
[@codename470](https://github.com/codename470)
