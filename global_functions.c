/*
Project by Eran Cohen and Ido Ziv
*/
#include "assembler.h"
#include "global_functions.h"
/*
This script will include all the global function that will be require for more than 1 script
for example: ignore spaces and tabs on input etc.
*/
/*
const char *commands[] = {
    "mov", "cmp", "add", "sub", "not", "clr", "lea", "inc", "dec", "jmp", "bne",
    "get", "prn", "jsr", "rts", "hlt"};
const char *directives[] = {
    ".data", ".string", ".struct", ".entry", ".extern"};*/
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
        fprintf(stdout, "Dynamic allocation error."); /* catching errors */
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
                fprintf(stdout, "<file %s, line %d> Illegal input, The operand after '#' must be a real number)\n", file_name, line_num);
                return FALSE;
            }
        }
        else
        {
            fprintf(stdout, "<file %s, line %d> Illegal input, There is no char after the '#' sign must be a real number\n", file_name, line_num);
            return FALSE;
        }
    }
    if (operand[0] == '%')
    {
        if (strlen(operand) > 1)
        {
            if (operand[1] == ' ') /*whether there is a space after % */
            {
                fprintf(stdout, "<file %s, line %d> Illegal space after the precent sign\n", file_name, line_num);
                return FALSE;
            }
        }
        else
        {
            fprintf(stdout, "<file %s, line %d> Illegal input, There is no char after the precent sign\n", file_name, line_num);
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
int end_of_line(char *line)
{
    return line == NULL || *line == '\0' || *line == '\n';
}

void write_error_code(int error_code, int current_line)
{
    if (current_line != -1)
        fprintf(stdout, "ERROR (line %d): ", current_line);
    switch (error_code)
    {
    case 1:
        fprintf(stdout, "first non-blank character must be a letter or a dot.\n");

        break;

    case 2:
        fprintf(stdout, "label already exists.\n");

        break;

    case 3:
        fprintf(stdout, "label is too long (LABEL_MAX_LENGTH: %d).\n", LABEL_LEN);

        break;

    case 4:
        fprintf(stdout, "label must start with an alphanumeric character.\n");

        break;

    case 5:
        fprintf(stdout, "label must only contain alphanumeric characters.\n");

        break;

    case 6:
        fprintf(stdout, "label can't have the same name as a command.\n");

        break;

    case 7:
        fprintf(stdout, "label must be followed by a command or a directive.\n");
        break;

    case 8:
        fprintf(stdout, "label can't have the same name as a register.\n");
        break;

    case 9:
        fprintf(stdout, "directive must have parameters.\n");

        break;

    case 10:
        fprintf(stdout, "illegal number of parameters for a directive.\n");

        break;

    case 11:
        fprintf(stdout, "incorrect usage of commas in a .data directive.\n");

        break;

    case 12:
        fprintf(stdout, ".data expected a numeric parameter.\n");

        break;

    case 13:
        fprintf(stdout, ".data expected a comma after a numeric parameter.\n");

        break;

    case 14:
        fprintf(stdout, ".data got an unexpected comma after the last number.\n");

        break;

    case 15:
        fprintf(stdout, ".string must contain exactly one parameter.\n");

        break;

    case 16:
        fprintf(stdout, ".string operand is invalid.\n");

        break;

    case 17:
        fprintf(stdout, ".struct first parameter must be a number.\n");

        break;

    case 18:
        fprintf(stdout, ".struct must have 2 parameters.\n");

        break;

    case 19:
        fprintf(stdout, ".struct second parameter is not a string.\n");

        break;

    case 20:
        fprintf(stdout, ".struct must not have more than 2 operands.\n");

        break;

    case 21:
        fprintf(stdout, ".struct must have 2 operands with a comma between them.\n");

        break;

    case 22:
        fprintf(stdout, ".extern directive must be followed by a label.\n");

        break;

    case 23:
        fprintf(stdout, ".extern directive received an invalid label.\n");

        break;

    case 24:
        fprintf(stdout, ".extern must only have one operand that is a label.\n");

        break;

    case 25:
        fprintf(stdout, "invalid command or directive.\n");

        break;

    case 26:
        fprintf(stdout, "invalid syntax of a command.\n");

        break;

    case 27:
        fprintf(stdout, "command can't have more than 2 operands.\n");

        break;

    case 28:
        fprintf(stdout, "operand has invalid addressing method.\n");

        break;

    case 29:
        fprintf(stdout, "number of operands does not match command requirements.\n");

        break;

    case 30:
        fprintf(stdout, "operands' addressing methods do not match command requirements.\n");

        break;

    case 31:
        fprintf(stdout, ".entry directive must be followed by an existing label.\n");

        break;

    case 32:
        fprintf(stdout, ".entry can't apply to a label that was defined as external.\n");

        break;

    case 33:
        fprintf(stdout, "label does not exist.\n");

        break;

    case 34:
        fprintf(stdout, "there was an error while trying to open the requested file.\n");
        break;

    case 35:
        fprintf(stdout, "Not enough arguments,\ncommand line should be like this: ./assembler file1 file2 .. \n");
        break;

    case 36:
        fprintf(stdout, "string must begin with "
                        " \n");
        break;
    }
}
/**
 * @brief
 *
 * @param line
 * @return int
 */
int find_command(char *word)
{
    int word_len = strlen(word), i;
    if (word_len > MAX_COMMAND_LENGTH || word_len < MIN_COMMAND_LENGTH)
        return NOT_FOUND;
    for (i = 0; i < CMD_LEN; i++){
        if (strcmp(word, commands[i]) == 0)
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
    int i, size = DIR_LEN;
    if (line == NULL || *line != '.')
        return NOT_FOUND;
    for (i = 0; i < size; i++)
    {
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
    if (label != NULL)
        return label->address;
    return FALSE;
}
/**
 * @brief Get the label object
 *
 * @param label
 * @param name
 * @return labelPtr
 */
labelPtr get_label(labelPtr label, char *name)
{
    while (label)
    {
        if (strcmp(label->name, name) == 0)
            return label;
        label = label->next;
    }
    return NULL;
}

void print_data(unsigned int *data)
{
    int i;
    printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\nMe Printing Data ah?\n");
    for (i = 0; i < (sizeof(data) / sizeof(data[0])); i++)
    {
        printf(" %u ", data[i]);
    }
    printf("\n$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
}

unsigned int get_bits(unsigned int word, int start, int end){
    
    unsigned int temp;
    int len = end - start + 1; 
    unsigned int mask = (int) pow(2, len) - 1; /* create a mask of '11....11' by len */

    mask = mask << start;
    temp = word & mask;
    temp = temp >> start;

    return temp;
    
}

char * next_operand(char *word, char * line){
    char *tmp = word;

    if(end_of_line){
        word[0] = '\0';
        return NULL;
    }

    line = skip_spaces(line);

    if(*line == ','){
        line++;
        strcpy(word, ",");
        return line;
    }

    while(!isspace(*line) && !end_of_line(line) && *line != ','){
        *tmp = *line;
        line++;
        tmp++;
    }
    *tmp = '\0';

    return line;
}
