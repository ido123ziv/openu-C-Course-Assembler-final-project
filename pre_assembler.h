
#ifndef PRE_ASSEMBLER_H

#define PRE_ASSEMBLER_H

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
	char name[LABEL_LEN]; /* Macro name (size same as label) */
	unsigned int address; /* the address of the label */
	macroPtr next; /* a pointer to the next label in the list */
} Macros;


/*************** DECLARETIONS  ****************/
void read_line(char *line, boolean is_macro, int line_num);
void pre_assembler(FILE *file, char *file_name);
boolean is_label(char *word);



#endif