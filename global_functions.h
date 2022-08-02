#ifndef FUNCS_H

#define FUNCS_H
#include "assembler.h"

/*************** DECLARETIONS  ****************/
boolean is_number(char number[]);
char *create_file(char *original, int type);
boolean is_register(char *index);
void copy_word(char *word, char *line);
char *skip_spaces(char *ch);
int ignore(char *line);
int end_of_line(char *line);
char *next_word(char *str);
void write_error_code(int error_code,int current_line);
void write_error_code(int error_code,int current_line);
void copy_line(char *from, char *to);
char *next_word(char *line);
int end_of_line(char *line);
int find_command(char *line);
int find_directive(char *line);
unsigned int get_label_address(labelPtr h, char *name);
labelPtr get_label(labelPtr label, char *name);
void print_data(unsigned int *data,unsigned int *instructions);
boolean is_label_exist(labelPtr p, char *name);
boolean is_label_external(labelPtr p, char *name);
unsigned int get_bits(unsigned int word, int start, int end);
char * next_comma_word(char *word, char * line);
char * next_string_word(char *word, char * line);
unsigned int add_are(unsigned int word, int are);
int add_entry(labelPtr ptr, char *name); 
char *to_base_32(unsigned int num);
void free_labels(labelPtr *labelTable);
void free_ext(extPtr *extTable);
extPtr add_ext(extPtr *ptr, char *name, unsigned int ref);
#endif