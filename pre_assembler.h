
#ifndef PRE_ASSEMBLER_H
#define PRE_ASSEMBLER_H
#include "assembler.h"
/* ******************* CONST *************** */
/* We will assume this constants for maximum macro size as there are no details about this */
# define MACRO_ROWS 10
# define NAME_LEN 30


/* ****************************************************
    ****************************************************
    ****************************************************
    ************          Structures             *******
    ****************************************************
   ****************************************************
*/

/* 
Defining linked list of macros and a pointer to that list
We will assume that Max Macro name length is as Max Label length 
 */
typedef struct structMacros * macroPtr;
typedef struct structMacros {
	char name[NAME_LEN]; /* Macro name */
	char contents[MACRO_ROWS *LINE_LEN]; /* save the line contents of the macro */
	macroPtr next; /* a pointer to the next label in the list */
} Macros;


/*************** DECLARETIONS  ****************/
void read_line(char *line);
void pre_assembler(FILE *file, char *file_name);
boolean is_label(char *word);
void isMacro(char *word, char *line);
void addMacro(macroPtr * macroTable, char * macroName);
void addLine (char * line, char * word);
macroPtr checkMacro(macroPtr macroTable ,char * word);
void free_macros(macroPtr * macroTable);

#endif