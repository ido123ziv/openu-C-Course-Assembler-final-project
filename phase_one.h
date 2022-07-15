/*************** DECLARETIONS  ****************/
#include "assembler.h"
int read_line_am(char *line, int line_count);
int check_for_label(char *line, boolean COLON);

void phase_one(FILE *fp, char *file_name);