/*
Project by Eran Cohen and Ido Ziv
*/
#include "assembler.h"
#include <math.h>

#include "global_functions.h"
/*
This script will include all the global function that will be require for more than 1 script
for example: ignore spaces and tabs on input etc.
*/

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

    while (i < LINE_LEN && !isspace(line[i]) && line[i] != '\0') /* Copying word until its end to *word */
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
        str++;              /* Skip rest of characters in the current line (until a space) */
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
/**
 * @brief this method prints the error code to stdout
 * 
 * @param error_code error code from enum
 * @param current_line line number of errer
 */
void write_error_code(int error_code, int current_line)
{
    if (current_line != -1)
        fprintf(stdout, "ERROR (line %d): ", current_line);
    switch (error_code)
    {
    case SYNTAX_ERR:
        fprintf(stdout, "first non-blank character must be a letter or a dot.\n");

        break;

    case LABEL_ALREADY_EXISTS:
        fprintf(stdout, "label already exists.\n");

        break;

    case LABEL_TOO_LONG:
        fprintf(stdout, "label is too long (LABEL_MAX_LENGTH: %d).\n", LABEL_LEN);

        break;

    case LABEL_INVALID_FIRST_CHAR:
        fprintf(stdout, "label must start with an alphanumeric character.\n");

        break;

    case LABEL_ONLY_ALPHANUMERIC:
        fprintf(stdout, "label must only contain alphanumeric characters.\n");

        break;

    case LABEL_CANT_BE_COMMAND:
        fprintf(stdout, "label can't have the same name as a command.\n");

        break;

    case LABEL_ONLY:
        fprintf(stdout, "label must be followed by a command or a directive.\n");
        break;

    case LABEL_CANT_BE_REGISTER:
        fprintf(stdout, "label can't have the same name as a register.\n");
        break;

    case DIRECTIVE_NO_PARAMS:
        fprintf(stdout, "directive must have parameters.\n");

        break;

    case DIRECTIVE_INVALID_NUM_PARAMS:
        fprintf(stdout, "illegal number of parameters for a directive.\n");

        break;

    case DATA_COMMAS_IN_A_ROW:
        fprintf(stdout, "incorrect usage of commas in a .data directive.\n");

        break;

    case DATA_EXPECTED_NUM:
        fprintf(stdout, ".data expected a numeric parameter.\n");

        break;

    case DATA_EXPECTED_COMMA_AFTER_NUM:
        fprintf(stdout, ".data expected a comma after a numeric parameter.\n");

        break;

    case DATA_UNEXPECTED_COMMA:
        fprintf(stdout, ".data got an unexpected comma after the last number.\n");

        break;

    case STRING_TOO_MANY_OPERANDS:
        fprintf(stdout, ".string must contain exactly one parameter.\n");

        break;

    case STRING_OPERAND_NOT_VALID:
        fprintf(stdout, ".string operand is invalid.\n");

        break;

    case STRUCT_EXPECTED_STRING:
        fprintf(stdout, ".struct first parameter must be a number.\n");

        break;

    case STRUCT_INVALID_STRING:
        fprintf(stdout, ".struct must have 2 parameters.\n");

        break;

    case EXPECTED_COMMA_BETWEEN_OPERANDS:
        fprintf(stdout, ".struct second parameter is not a string.\n");

        break;

    case STRUCT_INVALID_NUM:
        fprintf(stdout, ".struct must not have more than 2 operands.\n");

        break;

    case STRUCT_TOO_MANY_OPERANDS:
        fprintf(stdout, ".struct must have 2 operands with a comma between them.\n");

        break;

    case EXTERN_NO_LABEL:
        fprintf(stdout, ".extern directive must be followed by a label.\n");

        break;

    case EXTERN_INVALID_LABEL:
        fprintf(stdout, ".extern directive received an invalid label.\n");

        break;

    case EXTERN_TOO_MANY_OPERANDS:
        fprintf(stdout, ".extern must only have one operand that is a label.\n");

        break;

    case COMMAND_NOT_FOUND:
        fprintf(stdout, "invalid command or directive.\n");

        break;

    case COMMAND_UNEXPECTED_CHAR:
        fprintf(stdout, "invalid syntax of a command.\n");

        break;

    case COMMAND_TOO_MANY_OPERANDS:
        fprintf(stdout, "command can't have more than 2 operands.\n");

        break;

    case COMMAND_INVALID_METHOD:
        fprintf(stdout, "operand has invalid addressing method.\n");

        break;

    case COMMAND_INVALID_NUMBER_OF_OPERANDS:
        fprintf(stdout, "number of operands does not match command requirements.\n");

        break;

    case COMMAND_INVALID_OPERANDS_METHODS:
        fprintf(stdout, "operands' addressing methods do not match command requirements.\n");

        break;

    case ENTRY_LABEL_DOES_NOT_EXIST:
        fprintf(stdout, ".entry directive must be followed by an existing label.\n");

        break;

    case ENTRY_CANT_BE_EXTERN:
        fprintf(stdout, ".entry can't apply to a label that was defined as external.\n");

        break;

    case COMMAND_LABEL_DOES_NOT_EXIST:
        fprintf(stdout, "label does not exist.\n");

        break;

    case CANNOT_OPEN_FILE:
        fprintf(stdout, "there was an error while trying to open the requested file.\n");
        break;

    case NOT_ENOUGH_ARGUMENTS:
        fprintf(stdout, "Not enough arguments,\ncommand line should be like this: ./assembler file1 file2 .. \n");
        break;

    }
}
/**
 * @brief this function searches the commands array to find the input command
 *
 * @param line current line (text)
 * @return int  command from enum 
 */
int find_command(char *word)
{
    int word_len = strlen(word), i;
    if (word_len != CMD_LEN)
        return NOT_FOUND;
    for (i = 0; i < CMD_LIST_LEN; i++){
        if (strcmp(word, commands[i]) == 0)
            return i;
    }
    return NOT_FOUND;
}
/**
 * @brief this function searches the directives array to find the input directive
 *
 * @param line current line (text)
 * @return int index of directive in enum
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
 * @brief Get the bits object
 * 
 * @param word unsigned int we want to parse
 * @param start 
 * @param end 
 * @return unsigned int 
 */
unsigned int get_bits(unsigned int word, int start, int end){
    
    unsigned int temp;
    int len = end - start + 1; 
    unsigned int mask = (int) pow(2, len) - 1; /* create a mask of '11....11' by len  */

    mask = mask << start;
    temp = word & mask;
    temp = temp >> start;

    return temp;
    
}
/**
 * @brief this function returns the string after the nearest comma and copies the string into word
 * 
 * @param word the string to copy to
 * @param line current line (text)
 * @return char* 
 */
char * next_comma_word(char *word, char * line){
    char *tmp = word;

    line = skip_spaces(line);

    if(end_of_line(line)){
        word[0] = '\0';
        return NULL;
    }

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
/**
 * @brief this function returns the string after the nearest " and copies the string into word
 * 
 * @param word the string to copy to
 * @param line current line (text)
 * @return char* 
 */
char * next_string_word(char *word, char * line){
    char *tmp = word;
    line = next_comma_word(word,line);
    if(end_of_line(line)){
        word[0] = '\0';
        return NULL;
    }

    if(*word != '"'){
        return line;
    }
    word++;
    line++;
    while(!end_of_line(line) && *line != '"'){
        *tmp = *line;
        line++;
        tmp++;
    }
    *tmp = '\0';

    return line;
}

/* Function inserts A/R/E bits into given word bit-sequence (the word is being shifted left) */
unsigned int add_are(unsigned int word, int are)
{
    return (word << 2) | are; /* OR operand allows insertion of the 2 bits because 1 + 0 = 1 */
}

/* Function will search if the label exist in the table and return TRUE if it is, else will return FALSE */
int add_entry(labelPtr ptr, char *name){
    labelPtr tmp = get_label(ptr, name);
    if(tmp != NULL){
        if(tmp -> external){
            error_code = ENTRY_CANT_BE_EXTERN;
            return FALSE;
        }
        tmp -> entry = TRUE;
        has_entry = TRUE;
        return TRUE;
    }
    else
        error_code = ENTRY_LABEL_DOES_NOT_EXIST;
    return FALSE;
}
/**
 * @brief translates unsigned int to base32 represented by a string
 * 
 * @param num number to translate
 * @return char* 
 */
char *to_base_32(unsigned int num)
{
    char *base32_seq = (char *) malloc(BASE32_SEQ_LEN);

    /* To convert from binary to base 32 we can just take the 5 right binary digits and 5 left */
    base32_seq[0] = base32[get_bits(num, 5, 9)];
    base32_seq[1] = base32[get_bits(num, 0, 4)];
    base32_seq[2] = '\0';

    return base32_seq;
}

/* Free the memory we allocated for the labels list */
void free_labels(labelPtr *labelTable)
{
    labelPtr p;
    while (*labelTable)
    {
        p = *labelTable;
        *labelTable = (*labelTable)->next;
        free(p);
    }
}

/* Free the memory we allocated for the extern list */
void free_ext(extPtr *extTable)
{
    unsigned int last_ref;
    unsigned int ref = 0;
    extPtr p = *extTable;
    if(p){ /* Free extern list by free each node */
        last_ref = ((*extTable)->prev)->address;
        do{
            p = *extTable;
            ref = p->address;
            *extTable = (*extTable)->next;
            free(p);
        } while (ref != last_ref);
    }
}

/* Add a node to the end of the external list */
extPtr add_ext(extPtr *ptr, char *name, unsigned int ref)
{
    extPtr t=*ptr;
    extPtr tmp;

    tmp=(extPtr) malloc(sizeof(ext));
    if(!tmp)
    {
        printf("\nerror, cannot allocate memory\n");
        exit(1);
    }

    tmp -> address = ref;
    strcpy(tmp->name, name);

    if(!(*ptr)) /* If the list is empty */
    {
        *ptr = tmp;
        tmp -> next = tmp;
        tmp -> prev = tmp;
        return tmp;
    }


    ((*ptr)->prev)->next = tmp;
    tmp->next = t;
    tmp->prev = t->prev;
    (*ptr)->prev = tmp;

    return tmp;
}

/******************** labels table methods **************/
/* Function will return the address of a given label and FALSE if not exist */
/**
 * @brief Get the label address object
 *
 * @param h label to search
 * @param name name of label
 * @return unsigned int
 */
unsigned int get_label_address(labelPtr p, char *name)
{
    labelPtr label = get_label(p, name);
    if(label != NULL) return label -> address;
    return FALSE;
}

/* Function checks if a given label name is in the list */
/**
 * @brief Get the label object
 *
 * @param label label to search
 * @param name name of label
 * @return labelPtr
 */
labelPtr get_label(labelPtr p, char *name)
{
	while(p)
	{
        if(strcmp(p->name,name)==0) /* we found a label with the name given */
			return p;
		p=p->next;
	}
	return NULL;
}

/* Check if a given name is a name of a label on list */
boolean is_label_exist(labelPtr p, char *name)
{
    return get_label(p, name) != NULL;
}

/* Function check if label on the array and also check if this label is external */
boolean is_label_external(labelPtr p, char *name)
{
    labelPtr label = get_label(p, name);
    printf("label = %s\n", label->name);
    if(label != NULL) return label -> external;
    return FALSE;
}