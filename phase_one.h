/*************** DECLARETIONS  ****************/
#include "assembler.h"
int read_line_am(char *line, int line_count);
int check_for_label(char *line, int line_count);
boolean is_label_am(char *line, int colon, int line_count);
void phase_one(FILE *fp, char *file_name);