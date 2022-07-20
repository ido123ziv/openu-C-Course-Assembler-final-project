/*
Project by Eran Cohen and Ido Ziv
*/
#include "assembler.h"
#include "global_functions.h"
#include "phases.h"
#include "phase_one.h"

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
            new_label = add_label(&symbols_table, current_word, 0);
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
    char word[LINE_LEN];
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
     copy_word(word,line);
     /*printf("current WORD is: %s\n", word);*/
    /* labels can't be registers */
    if (is_register(word))
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
labelPtr add_label(labelPtr *table, char *name, unsigned int address)
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
        return handle_data_directive(line);

    case STRING:
        /* Handle .string directive and insert to memory */
        return handle_string_directive(line);

    case STRUCT:
        /* Handle .struct directive and insert both number and string to memory */
        return handle_struct_directive(line);

    case ENTRY:
        /* Only check for syntax of entry (should not contain more than one parameter) */
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
        line = next_comma_word(copy,line);
       /* printf("next_comma_word: %s\n%s\n", line, copy);
        printf(".....comma_word: %s\n%s\n", line, copy);*/
        if (strlen(copy) > 0)
        {
            if (!isnumber)
            {
                if (!is_number(copy))
                {
                    return DATA_EXPECTED_NUM;
                }
                else
                {
                    isnumber = TRUE;
                    iscomma = FALSE;
                    data[dc++] = (unsigned int)atoi(copy);
                }
            }
            else
            {
                if (*copy != ',')
                    return DATA_EXPECTED_COMMA_AFTER_NUM;
                else
                {
                    if (iscomma)
                        return DATA_COMMAS_IN_A_ROW;
                    else
                    {
                        iscomma = TRUE;
                        isnumber = FALSE;
                    }
                }
            }
        }
        /* code */
    }
    if (iscomma)
        return DATA_UNEXPECTED_COMMA;

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
    int line_len;
    char copy[LINE_LEN]; 
 /*   printf("my line is %s\n", line); */
     if (end_of_line(line)) /* || (line != '"' && line[line_len - 1] != '"'))*/
        return STRING_OPERAND_NOT_VALID;
    /*printf("im not empty: %s\n", line);*/
   /* next_string_word(copy,line);*/
    next_word(line);
  /*  printf("after next word %s\n", line); */
    if (line[0] != '\"')
        return STRING_OPERAND_NOT_VALID;
    line++;
    line_len = strlen(line);
  /*  printf("line++ %s\t strlen: %d, char: %c\n", line, line_len, line[line_len -2]);*/
    if (line[line_len -2] != '\"')
        return STRING_OPERAND_NOT_VALID;
    
    /*TODO: Copy entire line */
    /*printf("my copy is: %s\n", copy);*/
  /*  printf("my line is: %s\n", line); */
 /*   printf("line: %c, end: %c\n", line[0],line[line_len - 2]); */
    line = skip_spaces(line);
    /*printf("my line is: %s\n", line);*/
/*    printf("end of line: %d\n", end_of_line(line));*/
    if (!end_of_line(line))
    {
      /*  copy[strlen(copy) - 1] = "\0";*/
        line[line_len -2] = '\0';
        /*copy[0] = "\0";*/
        /*printf("without \": %s\n", copy);
        printf("with \": %s\n", copy + 1);*/
       write_string_to_data(line);
    }

    return 0;
}
/**
 * @brief 
 * 
 * @param line 
 * @return int 
 */
int handle_struct_directive(char *line)
{
    int line_len = strlen(line);

    char copy[LINE_LEN], c = copy;
    printf("my line is: %s\n", line);
    copy_word(copy, line);

    if (end_of_line(copy) || !is_number(copy))
        return STRUCT_INVALID_NUM;
    data[dc++] = (unsigned int) atoi(copy);
    line = next_comma_word(copy,line);
    printf("after line:%s \t copy: %s\n", line, copy);
    line = next_comma_word(copy,line);
    printf("2x next_comma_word: %s \t copy: %s\n", line, copy);
   /* copy_word(copy, line); */
    if (!end_of_line(line) && *copy == ','){
        line = next_comma_word(copy,line); /* copy equvals string with "" */
        printf("and now copy is: %s\n", copy);
        if (end_of_line(copy))
            return STRUCT_EXPECTED_STRING;
        else{
            printf("copy: %c, strlen: %c\n", copy[0], copy[strlen(copy) - 1]);
            if (copy[0] != '"' && copy[strlen(copy) - 1] != '"')
                return STRUCT_INVALID_STRING;
          /*  copy_word(copy, line); */
            copy[line_len - 1] = '\0';
            write_string_to_data(copy + 1);
        }
    }
    else{
        return EXPECTED_COMMA_BETWEEN_OPERANDS;
    }
    line = next_word(line);
    if (!end_of_line(line))
        return STRUCT_TOO_MANY_OPERANDS;
    return 0;
}
/**
 * @brief 
 * 
 * @param line 
 * @return int 
 */
int handle_extern_directive(char *line)
{
    labelPtr new_label = NULL;
    char copy[LABEL_LEN];
    copy_word(copy,line);
    if (end_of_line(copy))
        return EXTERN_NO_LABEL;
    if (!check_for_label(copy,FALSE))
        return EXTERN_INVALID_LABEL;
    line = next_word(line);
    if (!end_of_line(line))
        return EXTERN_TOO_MANY_OPERANDS;
    copy_word(copy,line);
    new_label = add_label(&symbols_table, copy, 0);
    if (!new_label)
        return 1;
    return 0;
}
/**
 * @brief 
 * 
 * @param line 
 */
void write_string_to_data(char *line){
/*    printf("write_string_to_data: %s\n", line); */
     while (!end_of_line(line))
        {
            data[dc++] = (unsigned int)*line; /* Inserting a character to data array */
            line++;
        }
        data[dc++] = '\0';
}

int handle_command(int type, char *line){
    return 0;
}