/*
Project by Eran Cohen and Ido Ziv
*/
#include "assembler.h"
#include "global_functions.h"
#include "phase_one.h"

const char *directives[] = {
        ".data", ".string", ".struct", ".entry", ".extern"
};
int ic;
int dc;

void phase_one(FILE *fp, char *file_name){
    char current_line[LINE_LEN]; /* holds the current line */
    int line_count = 1; /* initializing the line counter to 1, the file starts with line 1 */
    int error_code;
    ic = 0;
    dc = 0;
    printf("---------------------------------------------------------\n");
    printf("let's do this shit %s\n", file_name);
    printf("---------------------------------------------------------\n");
    while (fgets(current_line,LINE_LEN,fp) != NULL)
    {
        error_code = 0;
        if (!ignore(current_line))
            error_code = read_line_am(current_line, line_count);
        if (error_code != 0)
            write_error_code(error_code,line_count);
        
        line_count ++;
        /* code */
    }
    printf("Didn't find errors yet! \n");
    
}

int read_line_am(char *line, int line_count){
   /* int dir_type = UNKNOWN_TYPE;
    int command_type = UNKNOWN_COMMAND;

    boolean is_label_am = FALSE; */
    char *line_copy;
    int k;

    line = skip_spaces(line);
    if (end_of_line(line))
        return 0;
    
    if(!isalpha(*line) && *line != '.') { /* first non-blank character must be a letter or a dot */
      /*  write_error_code(1,line_count); */
        return 1;
    }
    line_copy = line;
    k = check_for_label(line,line_count);
    if (k){
        line = next_word(line);
        printf("One label found\n");
    }
    else printf("No label found\n");
    
    return 0;    
}

boolean is_label_am(char *line, int colon, int line_count){
    boolean has_digits = FALSE;
    int line_len = strlen(line);
    int i;

    if(line == NULL ||
        line_len < (colon ? MINIMUM_LABEL_LENGTH_WITH_COLON: MINIMUM_LABEL_LENGTH_WITHOUT_COLON))
    return FALSE;

    if (colon && line[line_len -1] != ':') return FALSE;
    
    /* label size is ok */
    if (line_len > LABEL_LEN){
        if (colon)
            write_error_code(3,line_count);
        return FALSE;
    }
    /* first char is a letter */
    if (!isalpha(*line)){
        write_error_code(4,line_count);
        return FALSE;
    }

    /* remove ':' */
    if (colon){
        line[line_len -1] = '\0';
        line_len--;
    }

     /* Check if all characters are digits or letters */
    for(i = 1; i < line_len; i++) 
    {
        if(isdigit(line[i]))
            has_digits = TRUE;
        else if(!isalpha(line[i])) {
            /* It's not a label but it's an error only if someone put a colon at the end of the token */
            if(colon) 
                write_error_code(5,line_count);
            return FALSE;
        }
    }
    /* checking if label don't have a command name */
    if (!has_digits){
        if (find_command(line) != NOT_FOUND){
            if (colon)
                write_error_code(6,line_count);
            return FALSE;
        }
    }
    /* checking if a label is a register */
     if(is_register(line)) 
    {
        if(colon) 
            write_error_code(7,line_count);
        return FALSE;
    }

    return TRUE;
}

int check_for_label(char *line, int line_count){
    if (!is_label_am(line,1,line_count))
        return 0;
   /* label_node = add_label(&symbols_table, current_token, 0, FALSE, FALSE); 
    if(label_node == NULL) There was an error creating label 
        return 0;*/
    line = next_word(line);
    if (end_of_line(line))
        return 0;
    return 1;
}

