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
FILE *macroFile;

void pre_assembler(FILE *file, char *file_name){
    char line[LINE_LEN];
    is_macro = FALSE;
    int line_count = 1; /* Line number we are at starting from 1 */
    macroFile = fopen(strcat(file_name, '.am'), "w");

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

    char word[LINE_LEN];
    int words_len = 0;

    char * all_line = line;

    macro_node = NULL;
    line = skip_spaces(line);
    copy_word(word,line);
    
    /* check if first word is label, if it is we will check the next word */
    if(is_label(word)){
        /* check next word for macro */
        line = next_word(line);
        copy_word(word, line);
        printf("The word is: %s\n", word);
        }
    
    if (is_macro){ /* if it's macro we will just add the lines to the macro table until endmacro */
        if (!strcmp(word, "endmacro")){
            is_macro = FALSE;
            return;
        }
        addLine(all_line);
    }
    else{ /* Check for macro */
        isMacro(word,line);
        if(!is_macro){
            /* TODO: add check if word in line equal to macro in the macro table */
            addLine(all_line);
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
                /* TODO: add test to check macro name is allowed (not other macro name, command, etc.) */
                addMacro(&macro_node, word); 
    }
}

/* Function used to add a new macro name to the macro table we have */
void addMacro(macroPtr * macroTable, char * macroName){
    macroPtr ptr1, ptr2;

    ptr1 = (macroPtr) malloc(sizeof(Macros));
    if (!ptr1){
        fprintf(stderr, "Dynamic allocation error."); /* catching errors */
        exit(ERROR);
    }
    /* Save the new macro as a node in our table */
    strcpy(ptr1->name, macroName);
    if(*macroTable == NULL) /* if table empty */
        *macroTable = ptr1;
    else{ /* add the new macro as the last node in table */
        ptr2 = *macroTable;
        *macroTable = ptr1;
        ptr1 -> next = ptr2;
    }
}

/* Create new line on the new file without macros */
void addLine (char * line){
    if(is_macro){
        strcat(macro_node->contents, line);
    }
    else{
        /* TODO: add line depend if it's macro name or regular */
    }
}