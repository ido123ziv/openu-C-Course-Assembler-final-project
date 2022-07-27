/*************** DECLARETIONS  ****************/
#include "assembler.h"
int read_line_am(char *line, int line_count);
int check_for_label(char *line, boolean COLON);

void phase_one(FILE *fp, char *file_name);

labelPtr add_label(labelPtr *table, char *name, unsigned int address);
boolean existing_label(labelPtr label, char *name);
int handle_directive(int dir_type, char* line);
int handle_data_directive(char * line);
int handle_string_directive(char * line);
int handle_struct_directive(char * line);
int handle_extern_directive(char * line);
void write_string_to_data(char *line);
int method_type(char *op);
boolean num_operation_fits_command(int command_tye, boolean first_op, boolean second_op);
boolean method_fits_command(int command_type, int first, int second);
boolean all_source_method(int method_type);
boolean all_dest_method(int method_type);
boolean non_immediate_method(int method_type);
void write_command_to_instructions(unsigned int word);
int words_count_by_method(int method_type);
int word_count_by_command(boolean first_op, boolean second_op, int first, int second);
unsigned int word_to_bits(int method_type, boolean first_op, boolean second_op, int first, int second);
