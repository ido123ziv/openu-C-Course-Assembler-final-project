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
void write_error_code(int error_code,int current_line);
void copy_line(char *from, char *to);
char *next_word(char *line);
int end_of_line(char *line);
int find_command(char *line);
int find_directive(char *line);
#endif