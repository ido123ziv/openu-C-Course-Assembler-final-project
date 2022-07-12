#ifndef FUNCS_H

#define FUNCS_H
#include "assembler.h"
/*************** DECLARETIONS  ****************/
int skip_whitespaces_at_begining(char c[], int start);
int skip_whitespaces_at_end(char c[], int end);
boolean is_number(char number[]);
char *create_file(char *original, int type);
boolean syntax_validator(char operand[], int line_num, char *file_name);
boolean is_register(char *index);
void copy_word(char *word, char *line);
char *skip_spaces(char *ch);
int ignore(char *line);
<<<<<<< HEAD
int end_of_line(char *line);
char *next_word(char *str);

=======
void write_error_code(int error_code,int current_line);
>>>>>>> 625aa69805bdf81e71ad4e6199450e3ae2547bc5
#endif