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

boolean is_macro;
macroPtr macro_node;
macroPtr pointer;
FILE *macroFile;

void pre_assembler(FILE *file, char *file_name){
    char line[LINE_LEN];
    is_macro = FALSE;
    int line_count = 1; /* Line number from 1 */
    macroFile = fopen(strcat(file_name, ".am"), "w");  
    macro_node = NULL;

    while(fgets(line, LINE_LEN, file) != NULL) /* Read lines until end of file */
    {
        /* is_error = FALSE; */
        if(!ignore(line)) /* Ignore line if it's blank or ; */
            read_line(line, line_count);
        
        line_count++;
    }

    freelist(&macro_node);
    fclose(macroFile);
}

/* TODO: added line_num to indicate the line error happen */

void read_line(char *line, int line_num){

    char word[LINE_LEN];
    int words_len = 0;

    char * all_line = line;
    line = skip_spaces(line);
    copy_word(word,line);
    printf("First word of line is: %s\n", word);
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
        addLine(all_line, word);
    }
    else{ /* Check for macro */
        isMacro(word,line);
        if(!is_macro){
            /* TODO: add check if word in line equal to macro in the macro table */
            addLine(all_line, word);
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
                printf("Macro name is: %s\n", word);
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
void addLine (char * line, char * word){
    if(is_macro){
        strcat(macro_node->contents, line);
    }
    else{
        /* add line depend if it's macro name or regular */
        if((pointer = checkMacro(macro_node, word)) != NULL){
            fputs(pointer -> contents, macroFile);
        }
        else
            fputs(line, macroFile);
    }
}

macroPtr checkMacro(macroPtr macroTable ,char * word){
    macroPtr ptr1; 
    ptr1 = macroTable;
    while (ptr1 != NULL)
    {
        if(!strcmp(ptr1->name, word))
            return ptr1;
        ptr1 = ptr1 -> next;
    }
    return NULL;
} 

/* Free the memory we allocated for the macro list */
void freelist(macroPtr * macroTable){
    
    macroPtr p;
    while (*macroTable)
    {
         p=*macroTable;
        *macroTable = (*macroTable)->next;
        free(p);
    }
}