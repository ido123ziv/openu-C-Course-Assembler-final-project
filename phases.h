#include "assembler.h"

/* Bits location in first word of a command */
#define SRC_START_POS 4
#define SRC_END_POS 5
#define DEST_START_POS 2
#define DEST_END_POS 3

/* How many operands a command needs by it's location on commands[] table */
#define TWO_OPERANDS 5
#define ONE_OPERANDS 14


/*************** DECLARETIONS  ****************/
/*************** PHASE TWO *****************/
void phase_two(FILE *file, char *file_name);
int read_line_ph2(char * line, int line_num);
void cmd_ph2_binary(int cmd, char *line);
void check_operands(int cmd, boolean *is_src, boolean *is_dest);
unsigned int build_reg(boolean is_dest, char *reg);
void encode_ph2_word(boolean is_dest, int method, char * op);
void encode_label(char *label);
void write_ob(FILE *file);
void write_files(char *src);
void write_entry(FILE *file);
void write_extern(FILE *file);
FILE *new_file(char *filename, int type);

/* ************** PHASE ONE ************** */
int read_line_am(char *line, int line_count);
int check_for_label(char *line, boolean COLON);

void phase_one(FILE *fp, char *file_name);

labelPtr add_label(labelPtr *table, char *name, unsigned int address);
boolean delete_label(labelPtr *table, char *name);
void assign_addresses(labelPtr label, int address, boolean is_data_label);
void print_label(labelPtr h);
void print_labels(labelPtr *table);
boolean existing_label(labelPtr label, char *name);
int handle_directive(int dir_type, char* line);
int handle_data_directive(char * line);
int handle_string_directive(char * line);
int handle_struct_directive(char * line);
int handle_extern_directive(char * line);
int handle_command(int type, char *line);
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
