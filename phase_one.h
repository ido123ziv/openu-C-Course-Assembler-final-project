/*************** DECLARETIONS  ****************/
#include "assembler.h"
int read_line_am(char *line, int line_count);
int check_for_label(char *line, boolean COLON);

void phase_one(FILE *fp, char *file_name);

labelPtr add_label(labelPtr *table, char *name, unsigned int address, int line_count);
boolean existing_label(labelPtr label, char *name);
int handle_directive(int dir_type, char* line);
int handle_data_directive(char * line);
int handle_string_directive(char * line);
int handle_struct_directive(char * line);
int handle_extern_directive(char * line);