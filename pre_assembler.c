/*
Project by Eran Cohen and Ido Ziv
*/

/*
This script will replace the macros in the input to the original code that was include in
the macro.
It will run first before the first_transition happen
*/

#include "assembler.h"
#include "pre_assembler.h"
#include "global_functions.h"

/*************** DECLARETIONS  ****************/
void read_line(char *line, int line_num);
int ignore(char *line);
boolean is_label(char *word);

boolean is_macro;
macroPtr macro_node;

void pre_assembler(FILE *file, char *file_name){
    char line[LINE_LEN];
    is_macro = FALSE;
    int line_count = 1; /* Line number we are at starting from 1 */

    while(fgets(line, LINE_LEN, file) != NULL) /* Read lines until end of file */
    {
        /* is_error = FALSE; */
        if(!ignore(line)) /* Ignore line if it's blank or ; */
            read_line(line, line_count);
        
        line_count++;
    }
}

/* TODO: added line_num to indicate the line error happen */

void read_line(char *line, int line_num){
    
    int c, i = 0;
    char word[LINE_LEN];
    int words_len = 0;

    macro_node = NULL;
    line = skip_spaces(line);
    
    if (is_macro){ /* if it's macro we will just add the lines to the macro table until endmacro */

    }
    else{ /* else, we will check if first word is label if so we will check next word for macro */
        copy_word(word, line);
        printf("The word is: %s\n", word);

        if(is_label(word)){
        /* check next word for macro */
        line = next_word(line);
        copy_word(word, line);
        printf("The word is: %s\n", word);
        }
        else{
            isMacro(word, line);
        }
    }
}
    

boolean is_label(char *word){
    int word_len = strlen(word);
    if(word[word_len - 1] == ':')
        return TRUE;
    else
        return FALSE;
}

void isMacro(char *word, char *line){
    if(!strcmp(word, "macro")){ /* enter if macro */
                is_macro = TRUE;
                /* get the name of the macro and enter to Macro table */
                line = next_word(line);
                copy_word(word, line);
                /*addMacro(&macro_node, word); */
    }
}