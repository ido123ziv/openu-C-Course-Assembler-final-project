/* This header file will be used for all the functions in the project in 
order to provide central place for all of the constants and functions declarations */

#ifndef ASSEMBLER_H

#define ASSEMBLER_H

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
typedef enum {FILE_INPUT, FILE_MACRO, FILE_OBJECT,FILE_ENTRY,FILE_EXTERN }filetypes;
/* ****************************************************
    ****************************************************
    ****************************************************
    ************          Consts                *******
    ****************************************************
   ****************************************************
*/

/* Length Constants */
#define LINE_LEN 82  /* Line max size is 80 , extra 2 bits space for \n or \0 */
#define LABEL_LEN 30

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


/* extern macroPtr macro_table; */

#define REGISTER_LENGTH 2 /* a register's name contains 2 characters */
#define MIN_REGISTER 0 /* r0 is the first register */
#define MAX_REGISTER 7 /* r7 is the last register */
/* ****************************************************
    ****************************************************
    ****************************************************
    ************          Functions              *******
    ****************************************************
   ****************************************************
*/

/*************** DECLARETIONS  ****************/
/*void assembler(FILE *file, char *file_name);*/

#endif