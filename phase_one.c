/*
Project by Eran Cohen and Ido Ziv
*/
#include "assembler.h"
#include "global_functions.h"
#include "phase_one.h"


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
     int dir_type = NOT_FOUND;
     int command_type = NOT_FOUND;

    char *line_copy, current_word[LINE_LEN];
    int label_count;
    printf("######################################################\n");
    printf("current line is: %s\n", line);
    printf("######################################################\n");
    line = skip_spaces(line);
    if (end_of_line(line))
        return 0;

    if (!isalpha(*line) && *line != '.')
       /* first non-blank character must be a letter or a dot */
        return 1;
    
    line_copy = line;
    copy_word(current_word,line);
    label_count = check_for_label(current_word, TRUE);
    if (label_count)
    {
        if (label_count > 1){
         /* means that we have an error with the label */   
            return label_count;
        }
        else
        {
            line = next_word(line); 
            if (end_of_line(line)){ /* line can't be just a label */
                return 7;
            }
            printf("One label found\n");
            /*TODO: add label to table*/
        }
    }
   /* else
        printf("No label found\n");*/
    copy_word(current_word,line);

    dir_type = find_directive(current_word);
    command_type = find_command(current_word);
    if (dir_type == NOT_FOUND && command_type == NOT_FOUND)
        return 25;

    if (dir_type != NOT_FOUND){
        printf("Directive found\n");
        if (label_count){
            /* TODO: edit label struct*/
        }
        printf("directive is: %s\n",directives[dir_type]);
         line = next_word(line);
        /*TODO: add directive to table*/
    }
    
    if (command_type != NOT_FOUND){
        printf("Command found\n");
        if (label_count){
            /* TODO: edit label struct*/
        }
        printf("command is: %s\n",commands[command_type]);
         line = next_word(line);
        /*TODO: add command to table*/
    }


    return 0;
}
int check_for_label(char *line, boolean COLON){
    boolean has_digits = FALSE;
    int line_lentgh = strlen(line);
    int i;
    
    if (line == NULL || line_lentgh < (COLON ? MINIMUM_LABEL_LENGTH_WITH_COLON : MINIMUM_LABEL_LENGTH_WITHOUT_COLON)){
       /* printf("check min line length:\n");*/
        return 0;
    }

    if (COLON && line[line_lentgh - 1] != ':'){
   /*     printf("line not ending with :\n");*/
        return 0; /* search for :, if colon is a must then we need to have : */
    }

    if (line_lentgh > LABEL_LEN) /* checking label size */{
        if (COLON) /*error only if it is a label with ':' */
            return 3;
      /*  printf("check max line length:\n");*/
        return 0;
    }

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
    printf("current REGS is: %s\n", line);
    /* labels can't be registers */
    i = is_register(line);
    if (i){
        printf("my valus is: %d\n", i);
        return 8;
    }

    return 1;
}
