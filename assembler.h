/* This header file will be used for all the functions in the project in 
order to provide central place for all of the constants and functions declarations */

/* C libraries required for the project */
/* ****************************************************
    ****************************************************
    ****************************************************
    ************          Includes               *******
    ****************************************************
   ****************************************************
*/
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

/* ****************************************************
    ****************************************************
    ****************************************************
    ************          Enums                  *******
    ****************************************************
   ****************************************************
*/
typedef enum {FALSE,TRUE}boolean;
typedef enum {FILE_MACRO, FILE_OBJECT,FILE_ENTRY,FILE_INPUT,FILE_EXTERN}filetypes;

/* ****************************************************
    ****************************************************
    ****************************************************
    ************          Defines                *******
    ****************************************************
   ****************************************************
*/

#define MAX_EXTENSION_LENGTH 5
#define ERROR 1

/* Length Constants */
#define LINE_LEN 82  /* Line max size is 80 , extra 2 bits space for \n or \0 */
#define LABEL_LEN 30


/* ****************************************************
    ****************************************************
    ****************************************************
    ************          Variables             *******
    ****************************************************
   ****************************************************
*/

extern int ic, dc;
extern boolean is_error, error_exist;


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
} Labels;



/* ****************************************************
    ****************************************************
    ****************************************************
    ************          Functions              *******
    ****************************************************
   ****************************************************
*/
/* ****** Global Functions ******* */
int skip_whitespaces_at_begining(char c[], int start);
int skip_whitespaces_at_end(char c[], int end);
boolean isnumber(char number[]);
char *create_file(char *original, int type);
boolean syntax_validator(char operand[], int line_num, char *file_name);
/* ****** Assembler.c ******* */