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
#include <math.h>

/* ****************************************************
    ****************************************************
    ****************************************************
    ************          Enums                  *******
    ****************************************************
   ****************************************************
*/
typedef enum boolean {FALSE,TRUE}boolean;
enum filetypes {FILE_INPUT, FILE_MACRO, FILE_OBJECT,FILE_ENTRY,FILE_EXTERN };
enum directives {DATA, STRING, STRUCT, ENTRY, EXTERN,UNKNOWN_TYPE};
enum commands {MOV, CMP, ADD, SUB, NOT, CLR, LEA, INC, DEC, JMP, BNE, RED, PRN, JSR, RTS, STOP, UNKNOWN_COMMAND};
enum methods {M_IMMEDIATE, M_DIRECT, M_STRUCT, M_REGISTER, M_UNKNOWN};
enum errors {SYNTAX_ERR ,LABEL_ALREADY_EXISTS ,LABEL_TOO_LONG ,
    LABEL_INVALID_FIRST_CHAR ,LABEL_ONLY_ALPHANUMERIC ,LABEL_CANT_BE_COMMAND ,
    LABEL_ONLY ,LABEL_CANT_BE_REGISTER ,DIRECTIVE_NO_PARAMS ,
    DIRECTIVE_INVALID_NUM_PARAMS ,DATA_COMMAS_IN_A_ROW ,
    DATA_EXPECTED_NUM ,DATA_EXPECTED_COMMA_AFTER_NUM ,DATA_UNEXPECTED_COMMA ,
    STRING_TOO_MANY_OPERANDS ,STRING_OPERAND_NOT_VALID ,STRUCT_EXPECTED_STRING ,
    STRUCT_INVALID_STRING ,EXPECTED_COMMA_BETWEEN_OPERANDS ,STRUCT_INVALID_NUM ,
    STRUCT_TOO_MANY_OPERANDS ,EXTERN_NO_LABEL ,EXTERN_INVALID_LABEL ,EXTERN_TOO_MANY_OPERANDS ,
    COMMAND_NOT_FOUND ,COMMAND_UNEXPECTED_CHAR ,COMMAND_TOO_MANY_OPERANDS ,COMMAND_INVALID_METHOD ,
    COMMAND_INVALID_NUMBER_OF_OPERANDS ,COMMAND_INVALID_OPERANDS_METHODS ,ENTRY_LABEL_DOES_NOT_EXIST ,
    ENTRY_CANT_BE_EXTERN ,COMMAND_LABEL_DOES_NOT_EXIST ,CANNOT_OPEN_FILE ,NOT_ENOUGH_ARGUMENTS };
enum ARE {ABSOLUTE, EXTERNAL, RELOCATABLE};
/* ****************************************************
    ****************************************************
    ****************************************************
    ************          Consts                *******
    ****************************************************
   ****************************************************
*/

extern const char base32[32];
extern const char *commands[];
extern const char *directives[];
extern int error_code;
extern boolean error_exists, has_entry, has_extern;
extern int ic, dc;

/* ****************************************************
    ****************************************************
    ****************************************************
    ************          Constants                *******
    ****************************************************
   ****************************************************
*/

/* Length Constants */
#define LINE_LEN 82  /* Line max size is 80 , extra 2 bits space for \n or \0 */
#define LABEL_LEN 30
#define CMD_LIST_LEN 16
#define DIR_LEN 5
#define MACHINE_RAM 2000
#define BASE32_SEQ_LEN 3 /* Base 32 sequence of word contain 2 digits and '\0' */

#define MINIMUM_LABEL_LENGTH_WITH_COLON 2
#define MINIMUM_LABEL_LENGTH_WITHOUT_COLON 1
#define CMD_LEN 3 /* all commands length is 3 */
#define MAX_EXTENSION_LENGTH 5
#define ERROR 1
#define NOT_FOUND -1
#define MEM_START 100
/* ****************************************************
    ****************************************************
    ****************************************************
    ************          Variables             *******
    ****************************************************
   ****************************************************
*/
 
/* Linked list to store program labels */
typedef struct structLabels * labelPtr;
typedef struct  structLabels {
	char name[LABEL_LEN]; /* Label name */
    unsigned int address;
    boolean entry; /* store if label is entry */
    boolean external; /* store if label is extern */
	labelPtr next; /* Pointer to the next label on list */
} Labels;

/* Double linked list to store program extern labels */
typedef struct ext * extPtr;
typedef struct ext {
    char name[LABEL_LEN]; /* Extern label name */
    unsigned int address; 
    extPtr next; /* Pointer to the next label on list */
    extPtr prev; /* Pointer to the previous label on list */
} ext;

extern int ic, dc;
extern labelPtr symbols_table;
unsigned int data[];
unsigned int instructions[];
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