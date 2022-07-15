/*
Project by Eran Cohen and Ido Ziv
*/
#include "assembler.h"
#include "global_functions.h"
#include "phase_one.h"

const char *directives[] = {
    ".data", ".string", ".struct", ".entry", ".extern"};
int ic;
int dc;

void phase_one(FILE *fp, char *file_name)
{
    char current_line[LINE_LEN]; /* holds the current line */
    int line_count = 1;          /* initializing the line counter to 1, the file starts with line 1 */
    int error_code;
    ic = 0;
    dc = 0;
    printf("---------------------------------------------------------\n");
    printf("let's do this shit %s\n", file_name);
    printf("---------------------------------------------------------\n");
    while (fgets(current_line, LINE_LEN, fp) != NULL)
    {
        error_code = 0;
        if (!ignore(current_line))
            error_code = read_line_am(current_line, line_count);
        if (error_code)
            write_error_code(error_code, line_count);
        /* TODO: add WAS_ERROR */

        line_count++;
    }
    printf("Didn't find errors yet! \n");
}

int read_line_am(char *line, int line_count)
{
    /* int dir_type = UNKNOWN_TYPE;
     int command_type = UNKNOWN_COMMAND;

     boolean is_label_am = FALSE; */
    char *line_copy, current_word[LINE_LEN];
    int k;

    line = skip_spaces(line);
    if (end_of_line(line))
        return 0;

    if (!isalpha(*line) && *line != '.')
    {   /* first non-blank character must be a letter or a dot */
        /*  write_error_code(1,line_count); */
        return 1;
    }
    line_copy = line;
    copy_word(current_word,line);
    k = check_for_label(current_word, TRUE);
    if (k)
    {
        if (k != 1)
            write_error_code(k, line_count);
        else
        {
            line = next_word(line);
            printf("One label found\n");
        }
    }
    else
        printf("No label found\n");

    return 0;
}
int check_for_label(char *line, boolean COLON){
    boolean has_digits = FALSE;
    int line_lentgh = strlen(line);
    int i;

    printf("check min line length:\n");
    if (line == NULL || line_lentgh < (COLON ? MINIMUM_LABEL_LENGTH_WITH_COLON : MINIMUM_LABEL_LENGTH_WITHOUT_COLON))
        return 0;

    printf("line end with :\n");
    if (COLON && line[line_lentgh - 1] != ':')
        return 0; /* search for :, if colon is a must then we need to have : */

    printf("check max line length:\n");
    if (line_lentgh > LABEL_LEN) /* checking label size */
        return 3;

    if (!isalpha(*line)) /* first symbol in label must be letter */
        return 4;
    
    if (COLON) { /* removing ':' from line*/
        line[line_lentgh -1 ] = '\0';
        line_lentgh --;
    }

    /* checking all symbol from here are numbers or letters: */
    for (i = 0; i < line_lentgh; i++){
        if (isdigit(line[i])) 
            has_digits = TRUE;
        else
            if (!isalpha(line[i]))
                return 5;
    }

    if (!has_digits){
        if (find_command(line) != NOT_FOUND)
            return 6;
    }

    /* labels can't be registers */
    if (is_register(line))
        return 8;

    return 1;
}
