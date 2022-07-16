/*
Project by Eran Cohen and Ido Ziv
*/
#include "assembler.h"
#include "global_functions.h"
/*
This script will include all the global function that will be require for more than 1 script
for example: ignore spaces and tabs on input etc.
*/
const char *commands[] = {
    "mov", "cmp", "add", "sub", "not", "clr", "lea", "inc", "dec", "jmp", "bne",
    "get", "prn", "jsr", "rts", "hlt"};
const char *directives[] = {
    ".data", ".string", ".struct", ".entry", ".extern"};
/**
 * @brief
 * This function skippes the whitespaces from the begining of a given string by incrementing the first index
 * @param c string
 * @param start index of first char
 * @return int -> the first index of character from which is not a whitespace
 */
int skip_whitespaces_at_begining(char c[], int start)
{
    for (; c[start] == ' ' || c[start] == '\t'; start++)
        ; /*incrementing start until to skip whitespaces in the begining of a string */
    return start;
}
/**
 * @brief
 * This function skippes the whitespaces from the end of a given string
 * @param c string
 * @param end index of last char
 * @return int the index of the first char from the end that is not a whitespace
 */
int skip_whitespaces_at_end(char c[], int end)
{
    int i;
    for (i = end; i > 0; i--)
        if (!isspace(c[i]))
            return i + 1;
    return 1;
}
/**
 * @brief gets a string that represent a number, if the string contains characters that are not numbers returns false
 *
 * @param number number represented by a string
 * @return boolean whether it is a valid number
 */
boolean is_number(char number[])
{
    int i;
    if (number[0] == '-' || number[0] == '+' || isdigit(number[0])) /*whether the first character is a digit or a sign*/
    {
        for (i = 1; i < strlen(number) - 1; i++)
            if (!isdigit(number[i]))
                return FALSE;
    }
    else
        return FALSE;
    return TRUE;
}

/**
 * @brief Create a file object
 * This function creates a new name for the file with matching extension.
 * @param original
 * @param type
 * @return char* of the new file name
 */
char *create_file(char *original, int type)
{
    char *modified = (char *)malloc(strlen(original) + MAX_EXTENSION_LENGTH); /* extends the string to max */
    if (modified == NULL)
    {
        fprintf(stderr, "Dynamic allocation error."); /* catching errors */
        exit(ERROR);
    }

    strcpy(modified, original); /* Copying original filename to the extended string */

    /* adding required file extension */

    switch (type)
    {
    case FILE_INPUT:
        strcat(modified, ".as");
        break;

    case FILE_OBJECT:
        strcat(modified, ".ob");
        break;

    case FILE_ENTRY:
        strcat(modified, ".ent");
        break;

    case FILE_EXTERN:
        strcat(modified, ".ext");
        break;

    case FILE_MACRO:
        strcat(modified, ".am");
    }
    return modified;
}

/**
 * @brief
 * this function validates the syntax of a given line regarding immediate and relative addresing methods
 * @param operand operand of the line
 * @param line_num number of the line
 * @param file_name name of the file
 * @return boolean whether the syntax is valid
 */
boolean syntax_validator(char operand[], int line_num, char *file_name)
{

    if (operand[0] == '#') /* immediate methods must begin with a '#' and a number must follow*/
    {

        if (strlen(operand) > 1)
        {
            if (!is_number(operand + 1))
            {
                fprintf(stderr, "<file %s, line %d> Illegal input, The operand after '#' must be a real number)\n", file_name, line_num);
                return FALSE;
            }
        }
        else
        {
            fprintf(stderr, "<file %s, line %d> Illegal input, There is no char after the '#' sign must be a real number\n", file_name, line_num);
            return FALSE;
        }
    }
    if (operand[0] == '%')
    {
        if (strlen(operand) > 1)
        {
            if (operand[1] == ' ') /*whether there is a space after % */
            {
                fprintf(stderr, "<file %s, line %d> Illegal space after the precent sign\n", file_name, line_num);
                return FALSE;
            }
        }
        else
        {
            fprintf(stderr, "<file %s, line %d> Illegal input, There is no char after the precent sign\n", file_name, line_num);
            return FALSE;
        }
    }

    return TRUE;
}

/**
 * @brief gets a string that represent a register, and validates the format
 *
 * @param word the address of the string
 * @return boolean whether or not this is valida register
 */
boolean is_register(char *word)
{
     return strlen(word) == REGISTER_LENGTH && word[0] == 'r' && word[1] >= '0' && word[1] <= '7';
}

/* Function will copy the next word (until a space or end of line) to a string */
void copy_word(char *word, char *line)
{
    int i = 0;
    if (word == NULL || line == NULL)
        return;

    while (i < LINE_LEN && !isspace(line[i]) && line[i] != '\0') /* Copying token until its end to *dest */
    {
        word[i] = line[i];
        i++;
    }
    word[i] = '\0';
}

/**
 * @brief skip to the next non spaces to
 *
 * @param line the line we are working on
 * @return char* the pointer to the char that starts next word
 */
char *next_word(char *str)
{
    if (str == NULL)
        return NULL;
    while (!isspace(*str) && !end_of_line(str))
        str++;              /* Skip rest of characters in the current token (until a space) */
    str = skip_spaces(str); /* Skip spaces */
    if (end_of_line(str))
        return NULL;
    return str;
}

/* This function skips spaces of a string and returns a pointer to the first non-blank character */
char *skip_spaces(char *ch)
{
    if (ch == NULL)
        return NULL;
    while (isspace(*ch)) /* Continue the loop if the character is a whitespace */
        ch++;
    return ch;
}

/* Function that ignores a line if it's blank/a comment */
int ignore(char *line)
{
    line = skip_spaces(line);
    return *line == ';' || *line == '\0' || *line == '\n';
}
/**
 * @brief this method copies a line to a new string
 *
 * @param from original string
 * @param to new string
 */
void copy_line(char *from, char *to)
{
    int i = 0;
    if (from == NULL || to == NULL)
        return;

    while (i < LINE_LEN && !isspace(from[i]) && from[i] != '\0') /* Copying line until its end to *dest */
    {
        to[i] = from[i];
        i++;
    }
    to[i] = '\0';
}
/**
 * @brief prints an error message to the user according to a given table of erros (erros.json)
 *
 * @param error_code which error to show message on
 * @param current_line which line had an error
 */

/* Checking for the end of line/given token in the character that char* points to */
int end_of_line(char *line){
    return line == NULL || *line == '\0' || *line == '\n';
}

void write_error_code(int error_code,int current_line){
    if (current_line != -1)
        fprintf(stderr, "ERROR (line %d): ", current_line);
    switch (error_code)
    {
    case 1:
        fprintf(stderr, "first non-blank character must be a letter or a dot.\n");

        break;

    case 2:
        fprintf(stderr, "label already exists.\n");

        break;

    case 3:
        fprintf(stderr, "label is too long (LABEL_MAX_LENGTH: %d).\n", LABEL_LEN);

        break;

    case 4:
        fprintf(stderr, "label must start with an alphanumeric character.\n");

        break;

    case 5:
        fprintf(stderr, "label must only contain alphanumeric characters.\n");

        break;

    case 6:
        fprintf(stderr, "label can't have the same name as a command.\n");

        break;

    case 7:
        fprintf(stderr, "label must be followed by a command or a directive.\n");
        break;

    case 8:
        fprintf(stderr, "label can't have the same name as a register.\n");
        break;

    case 9:
        fprintf(stderr, "directive must have parameters.\n");

        break;

    case 10:
        fprintf(stderr, "illegal number of parameters for a directive.\n");

        break;

    case 11:
        fprintf(stderr, "incorrect usage of commas in a .data directive.\n");

        break;

    case 12:
        fprintf(stderr, ".data expected a numeric parameter.\n");

        break;

    case 13:
        fprintf(stderr, ".data expected a comma after a numeric parameter.\n");

        break;

    case 14:
        fprintf(stderr, ".data got an unexpected comma after the last number.\n");

        break;

    case 15:
        fprintf(stderr, ".string must contain exactly one parameter.\n");

        break;

    case 16:
        fprintf(stderr, ".string operand is invalid.\n");

        break;

    case 17:
        fprintf(stderr, ".struct first parameter must be a number.\n");

        break;

    case 18:
        fprintf(stderr, ".struct must have 2 parameters.\n");

        break;

    case 19:
        fprintf(stderr, ".struct second parameter is not a string.\n");

        break;

    case 20:
        fprintf(stderr, ".struct must not have more than 2 operands.\n");

        break;

    case 21:
        fprintf(stderr, ".struct must have 2 operands with a comma between them.\n");

        break;

    case 22:
        fprintf(stderr, ".extern directive must be followed by a label.\n");

        break;

    case 23:
        fprintf(stderr, ".extern directive received an invalid label.\n");

        break;

    case 24:
        fprintf(stderr, ".extern must only have one operand that is a label.\n");

        break;

    case 25:
        fprintf(stderr, "invalid command or directive.\n");

        break;

    case 26:
        fprintf(stderr, "invalid syntax of a command.\n");

        break;

    case 27:
        fprintf(stderr, "command can't have more than 2 operands.\n");

        break;

    case 28:
        fprintf(stderr, "operand has invalid addressing method.\n");

        break;

    case 29:
        fprintf(stderr, "number of operands does not match command requirements.\n");

        break;

    case 30:
        fprintf(stderr, "operands' addressing methods do not match command requirements.\n");

        break;

    case 31:
        fprintf(stderr, ".entry directive must be followed by an existing label.\n");

        break;

    case 32:
        fprintf(stderr, ".entry can't apply to a label that was defined as external.\n");

        break;

    case 33:
        fprintf(stderr, "label does not exist.\n");

        break;

    case 34:
        fprintf(stderr, "there was an error while trying to open the requested file.\n");
        break;

    case 35:
        fprintf(stderr, "Not enough arguments,\ncommand line should be like this: ./assembler file1 file2 .. \n");
        break;
    }
}
/**
 * @brief whether this is the end of the line
 *
 * @param line line working on
 * @return int
 */

/**
 * @brief
 *
 * @param line
 * @return int
 */
int find_command(char *line)
{
    int line_len = strlen(line), i;
    int size = CMD_LEN;
    if (line_len > MAX_COMMAND_LENGTH || line_len < MIN_COMMAND_LENGTH)
        return NOT_FOUND;
    for (i = 0; i < size; i++){
        if (strcmp(line, commands[i]) == 0)
            return i;
    }
    return NOT_FOUND;
}
/**
 * @brief
 *
 * @param line
 * @return int
 */
int find_directive(char *line)
{
    int i, size = DIRECTIVE_LEN;
    if (line == NULL || *line != '.')
        return NOT_FOUND;
    for (i = 0; i < size; i++){
        if (strcmp(line, directives[i]) == 0)
            return i;
    }
    return NOT_FOUND;
}
/**
 * @brief Get the label address object
 * 
 * @param h 
 * @param name 
 * @return unsigned int 
 */
unsigned int get_label_address(labelPtr h, char *name)
{
    labelPtr label = get_label(h, name);
    if(label != NULL) return label -> address;
    return FALSE;
}
/**
 * @brief Get the label object
 * 
 * @param label 
 * @param name 
 * @return labelPtr 
 */
labelPtr get_label(labelPtr label, char *name){
{
    while (label)
    {
        if (strcmp(label->name, name) == 0)
            return label;
        label = label->next;
    }
    return NULL;
}
}