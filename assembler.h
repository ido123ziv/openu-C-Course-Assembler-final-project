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
    ************          Consts                *******
    ****************************************************
   ****************************************************
*/
const char *commands[] = {
        "mov", "cmp", "add", "sub", "not", "clr", "lea", "inc", "dec", "jmp", "bne",
        "get", "prn", "jsr", "rts", "hlt"
};

const char base32[32] = {
        '!', '@', '#', '$', '%', '^', '&', '*', '<', '>', 'a', 'b', 'c',
        'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p',
        'q', 'r', 's', 't', 'u', 'v'};

const char *directives[] = {
        ".data", ".string", ".struct", ".entry", ".extern"
};

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
/* ****** Global Functions ******* */
int skip_whitespaces_at_begining(char c[], int start);
int skip_whitespaces_at_end(char c[], int end);
boolean is_number(char number[]);
char *create_file(char *original, int type);
boolean syntax_validator(char operand[], int line_num, char *file_name);
/* ****** Assembler.c ******* */