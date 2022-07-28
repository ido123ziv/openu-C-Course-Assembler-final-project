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
void write_ob(FILE *file);
void write_files(char *src);
void write_entry(FILE *file);
void write_extern(FILE *file);

/* ************** PHASE ONE ************** */
int read_line_am(char *line, int line_count);
int check_for_label(char *line, boolean COLON);
void phase_one(FILE *fp, char *file_name);
/*labelPtr add_label(labelPtr *table, char *name, unsigned int address, int line_count);*/
boolean existing_label(labelPtr label, char *name);
int handle_directive(int dir_type, char* line);
int handle_data_directive(char * line);
int handle_string_directive(char * line);
int handle_struct_directive(char * line);
int handle_extern_directive(char * line);