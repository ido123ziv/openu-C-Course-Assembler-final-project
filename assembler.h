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
typedef enum {FILE_OBJECT,FILE_ENTRY,FILE_INPUT,FILE_EXTERN}filetypes;

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
#define LINE_LEN 82

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