/*
Project by Eran Cohen and Ido Ziv
*/

/*
This script will replace the macros in the input to the original code that was include in
the macro.
It will run first before the first_transition happen
*/

#include "assembler.h"

void pre_assembler(FILE *file, char *file_name){
    char line[LINE_LEN];
    boolean is_macro = FALSE;
    int line_count = 1; /* Line number we are at starting from 1 */

    while(fgets(line, LINE_LEN, file) != NULL) /* Read lines until end of file */
    {
        /* is_error = FALSE; */
        if(!ignore(line)) /* Ignore line if it's blank or ; */
            read_line(line, is_macro, line_count);
        
        line_count++;
    }
}

/* TODO: added line_num to indicate the line error happen */

void read_line(char *line, boolean is_macro, int line_num){
    
    int c, i = 0;
    char* word;

    macroPtr macro_node = NULL;
    line = skip_whitespaces_at_begining(line, 0);
    
    copy_word(word, line);
    while(i < LINE_LEN && !isspace(line[i]) && line[i] != '\0'){
        word[i] = line[i];
        i++;
    }
    word[i] = '\0';
}