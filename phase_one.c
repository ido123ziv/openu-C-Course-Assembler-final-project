/*
Project by Eran Cohen and Ido Ziv
*/
#include "assembler.h"
#include "global_functions.h"
#include "phases.h"

/**
 * @brief
 *
 * @param fp
 * @param file_name
 */
void phase_one(FILE *fp, char *file_name)
{
    char current_line[LINE_LEN]; /* holds the current line */
    int line_count = 1;          /* initializing the line counter to 1, the file starts with line 1 */

    printf("---------------------------------------------------------\n");
    printf("let's do this shit %s\n", file_name);
    printf("---------------------------------------------------------\n");
    while (fgets(current_line, LINE_LEN, fp) != NULL)
    {
        error_code = 0;
        if (!ignore(current_line))
            error_code = read_line_am(current_line, line_count);
        if (error_code)
            write_error_code(error_code, line_count);
        /* TODO: add WAS_ERROR */

        line_count++;
    }
    printf("Didn't find errors yet! \n");
}
/**
 * @brief
 *
 * @param line
 * @param line_count
 * @return int
 */
int read_line_am(char *line, int line_count)
{
    int dir_type = NOT_FOUND;
    int command_type = NOT_FOUND;
    labelPtr new_label = NULL;
    char *line_copy, current_word[LINE_LEN];
    int label_count, is_error;
    printf("######################################################\n");
    printf("current line is: %s\n", line);
    /* printf("######################################################\n"); */
    line = skip_spaces(line);
    if (end_of_line(line))
        return 0;

    if (!isalpha(*line) && *line != '.')
        /* first non-blank character must be a letter or a dot */
        return 1;

    line_copy = line;

    copy_word(current_word, line);
    label_count = check_for_label(current_word, TRUE);
    if (label_count)
    {
        if (label_count > 1)
        {
            /* means that we have an error with the label */
            return label_count;
        }
        else
        {
            new_label = add_label(&symbols_table, current_word, 0, line_count);
            if (!new_label)
                return LABEL_ALREADY_EXISTS;
            line = next_word(line);
            if (end_of_line(line))
            { /* line can't be just a label */
                return LABEL_ONLY;
            }
            printf("One label found\t");
            /*TODO: add label to table*/
        }
    }
    /* else
         printf("No label found\n");*/
    copy_word(current_word, line);

    dir_type = find_directive(current_word);
    command_type = find_command(current_word);
    if (dir_type == NOT_FOUND && command_type == NOT_FOUND)
        return COMMAND_NOT_FOUND;

    if (dir_type != NOT_FOUND)
    {
        printf("Directive found\t");
        printf("directive is: %s\n", directives[dir_type]);
        if (label_count)
        {
            if (dir_type == EXTERN || dir_type == ENTRY)
            {
                /*TODO: Remove from table*/
                label_count = 0;
            }
            else
                new_label->address = dc; /* This is a data directive */
        }
        line = next_word(line);
        is_error = handle_directive(dir_type, line);
        if (is_error)
            return is_error;
    }

    if (command_type != NOT_FOUND)
    {
        printf("Command found\t");
        if (label_count)
        {
            /* TODO: edit label struct*/
        }
        printf("command is: %s\n", commands[command_type]);
        line = next_word(line);
        /*TODO: add command to table*/
    }

    return 0;
}
/**
 * @brief
 *
 * @param line
 * @param COLON
 * @return int
 */
int check_for_label(char *line, boolean COLON)
{
    boolean has_digits = FALSE;
    int line_lentgh = strlen(line);
    int i;

    if (line == NULL || line_lentgh < (COLON ? MINIMUM_LABEL_LENGTH_WITH_COLON : MINIMUM_LABEL_LENGTH_WITHOUT_COLON))
    {
        /* printf("check min line length:\n");*/
        return 0;
    }

    if (COLON && line[line_lentgh - 1] != ':')
    {
        /*     printf("line not ending with :\n");*/
        return 0; /* search for :, if colon is a must then we need to have : */
    }

    if (line_lentgh > LABEL_LEN) /* checking label size */
    {
        if (COLON) /*error only if it is a label with ':' */
            return LABEL_TOO_LONG;
        /*  printf("check max line length:\n");*/
        return 0;
    }

    if (!isalpha(*line)) /* first symbol in label must be letter */
        return LABEL_INVALID_FIRST_CHAR;

    if (COLON)
    { /* removing ':' from line*/
        line[line_lentgh - 1] = '\0';
        line_lentgh--;
    }

    /* checking all symbol from here are numbers or letters: */
    for (i = 0; i < line_lentgh; i++)
    {
        if (isdigit(line[i]))
            has_digits = TRUE;
        else if (!isalpha(line[i]))
            return LABEL_ONLY_ALPHANUMERIC;
    }

    if (!has_digits)
    {
        if (find_command(line) != NOT_FOUND)
            return LABEL_CANT_BE_COMMAND;
    }
    /* printf("current REGS is: %s\n", line);*/
    /* labels can't be registers */
    if (is_register(line))
    {
        return LABEL_CANT_BE_REGISTER;
    }

    return 1;
}
/**
 * @brief
 *
 * @param table
 * @param name
 * @param address
 * @param line_count
 * @return labelPtr
 */
labelPtr add_label(labelPtr *table, char *name, unsigned int address, int line_count)
{
    labelPtr temp, table_pointer = *table;
    temp = (labelPtr)malloc(sizeof(Labels));
    if (existing_label(*table, name))
        return NULL;
    if (!temp)
    { /*if we couldn't allocate memory to temp then print an error massage and exit the program*/
        printf("ERROR!! Can Not Allocate Memory\n");
        exit(1);
    }

    /* TODO: check if exists */
    strcpy(temp->name, name);
    temp->address = address;
    temp->next = NULL;
    if (!(*table))
    {
        *table = temp;
        temp->next = NULL;
        return temp;
    }

    /* Setting a pointer to go over the list until he points on the last label and then stting temp to be the new last label */
    while (table_pointer->next != NULL)
        table_pointer = table_pointer->next;
    temp->next = NULL;
    table_pointer->next = temp;

    return temp;
}
/**
 * @brief
 *
 * @param label
 * @param name
 * @return boolean
 */
boolean existing_label(labelPtr label, char *name)
{
    while (label)
    {
        if (strcmp(label->name, name) == 0)
            return TRUE;
        label = label->next;
    }
    return FALSE;
}
/**
 * @brief
 *
 * @param dir_type
 * @param line
 * @return int
 */
int handle_directive(int dir_type, char *line)
{
    if (!line || end_of_line(line))
    {
        return DIRECTIVE_NO_PARAMS;
    }

    switch (dir_type)
    {
    case DATA:
        /* Handle .data directive and insert values separated by ',' to the memory */
        printf("handle data %d\n", DATA);
        return handle_data_directive(line);

    case STRING:
        /* Handle .string directive and insert to memory */
        printf("handle string %d\n", STRING);
        return handle_string_directive(line);

    case STRUCT:
        /* Handle .struct directive and insert both number and string to memory */
        printf("handle struct %d\n", STRUCT);
        return handle_struct_directive(line);

    case ENTRY:
        /* Only check for syntax of entry (should not contain more than one parameter) */
        printf("handle ENTRY %d\n", ENTRY);
        if (!end_of_line(next_word(line))) /* If there's a next word (after the first one) */
        {
            return DIRECTIVE_INVALID_NUM_PARAMS;
        }
        break;

    case EXTERN:
        /* Handle .extern directive */
        printf("handle extern %d\n", EXTERN);
        return handle_extern_directive(line);
    }
    return 0;
}
/**
 * @brief 
 * 
 * @param line 
 * @return int 
 */
int handle_data_directive(char *line)
{
    boolean isnumber = FALSE, iscomma = FALSE;
    char copy[LINE_LEN];
    while (!end_of_line(line))
    {
        copy_word(copy,line);
        line = next_word(line);

        if (strlen(copy) > 0){
            if (!isnumber){
                if (!is_number(copy)){
                    return 12;
                }
                else{
                    isnumber = TRUE;
                    iscomma = FALSE;
                    data[dc++] = (unsigned int) atoi(copy);
                }
            }
            else{
                if (*copy != ',')
                    return 13;
                else{
                    if (iscomma) return DATA_COMMAS_IN_A_ROW;
                    else {
                        iscomma = TRUE;
                        isnumber = FALSE;
                    }
                }
            }
        }
        /* code */
    }
    if (iscomma) return DATA_UNEXPECTED_COMMA;
    
    return 0;
}
/**
 * @brief 
 * 
 * @param line 
 * @return int 
 */
int handle_string_directive(char *line)
{
    int line_len = strlen(line);
    char copy[LINE_LEN], c = copy;
    printf("my line is: %s\n", line);
    copy_word(copy, line);
    if (end_of_line(line) || (line != '"' && line[line_len - 1] != '"')) 
        return STRING_OPERAND_NOT_VALID;
        
    line = skip_spaces(line);
    if (end_of_line(line))
    {
        copy[line_len - 1] = "\0";
        c++;
        printf("%x", data);
        while (!end_of_line(copy))
        {
            data[dc++] = (unsigned int)*copy; /* Inserting a character to data array */
            c++;
        }
        data[dc++] = '\0';
        printf("%x", data);
    }

    return 0;
}
int handle_struct_directive(char *line)
{
    return 0;
}
int handle_extern_directive(char *line)
{
    return 0;
}