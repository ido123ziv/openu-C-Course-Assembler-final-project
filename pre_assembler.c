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
void read_line(char *line, boolean is_macro, int line_num);
int ignore(char *line);
boolean is_label(char *word);


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
    char word[LINE_LEN];
    int words_len = 0;

    macroPtr macro_node = NULL;
    line = skip_whitespaces_at_begining(line, 0);
    
    copy_word(word, line);
    if(is_label(word)){
        /* check next word for macro */
        words_len += strlen(word);
        line = skip_whitespaces_at_begining(line, words_len);
        copy_word(word, line);
    }
    
    printf("The word is: %s", word);
}

boolean is_label(char *word){
    int word_len = strlen(word);
    if(word[word_len - 1] == ':')
        return TRUE;
    else
        return FALSE;
}