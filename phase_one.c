/*
Project by Eran Cohen and Ido Ziv
*/
#include "assembler.h"
#include "global_functions.h"

const char *directives[] = {
        ".data", ".string", ".struct", ".entry", ".extern"
};

void phase_one(FILE *fp){
    char current_line[LINE_LEN]; /* holds the current line */
    int line_count = 1; /* initializing the line counter to 1, the file starts with line 1 */
    int error_code;
    ic = 0;
    dc = 0;

    while (fgets(current_line,line_count,fp) != NULL)
    {
        error_code = 0;
        if (!ignore(current_line))
            error_code = read_line(current_line);
        if (error_code != 0)
            write_error_code(error_code,current_line);
        line_count ++;
        /* code */
    }
    
}

int read_line(char *line){
    
}
