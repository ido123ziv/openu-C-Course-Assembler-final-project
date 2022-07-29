/*
Project by Eran Cohen and Ido Ziv
*/
#include "assembler.h"
#include "global_functions.h"
#include "phases.h"

/**
 * @brief this methods handles the algorithm of phase one -> encoding to memory the program
 *
 * @param fp local file path
 * @param file_name name of the file to open
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
        if (error_code){
            write_error_code(error_code, line_count);
            error_exists = TRUE;
        }
        line_count++;
    }
    if (!error_exists)
        printf("Didn't find errors yet! \n");
    else
    {
        printf("Fount Errors!\nExiting!\n");
        exit(1);
    }
    /* writing to data the addresses*/
    assign_addresses(symbols_table,100,FALSE);
    assign_addresses(symbols_table,ic+100,TRUE);
}
/**
 * @brief this function reads line by line from the file
 *
 * @param line current line (text)
 * @param line_count current line (index)
 * @return int - error code if exists
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
                delete_label(&symbols_table,new_label->name);
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
            new_label->address = ic;
            new_label->action = TRUE;
            /* TODO: edit label struct*/
        }
        printf("command is: %s\n", commands[command_type]);
        line = next_word(line);
        handle_command(command_type, line);
    }

    return 0;
}
/**
 * @brief this function searches the input for a label from the format - label:
 *
 * @param line current line (text)
 * @param COLON if the line contains a semicolon ':'
 * @return int 1 if a label is found, 0 if not or error code
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
    copy_word(word, line);
    /*printf("current WORD is: %s\n", word);*/
    /* labels can't be registers */
    if (is_register(word))
    {
        return LABEL_CANT_BE_REGISTER;
    }

    return 1;
}
/**
 * @brief adds the new label to memort
 *
 * @param table label table point
 * @param name name of the new label
 * @param address address to assign the new label in the memory
 * @return labelPtr a new label
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

    strcpy(temp->name, name);
    temp->address = address;
    temp->action = FALSE;
    temp->entry = FALSE;
    temp->external = FALSE;
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
 * @brief checks if a label is already in memory
 *
 * @param label label struct to check
 * @param name label name 
 * @return boolean whether or not the label exists
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
 * @param table 
 * @param name 
 * @return boolean 
 */
boolean delete_label(labelPtr *table, char *name){
    labelPtr temp, table_pointer = *table;
    while (table_pointer)
    {
        if(strcmp(table_pointer->name,name) ==0){ /* found it */
            if (strcmp(temp->name, (*table)->name) == 0){ /* first index is the deleted one */
                *table=(*table)->next;
                free(table_pointer);
            }
            else {
                temp->next = table_pointer-> next;
                free(table_pointer);
            }
            return TRUE;
        }
        temp = table_pointer;
        table_pointer = table_pointer->next;
    }
    return FALSE;
    
}
/**
 * @brief add label addresses to memory
 * 
 * @param label 
 * @param address 
 * @param is_data_label 
 */
void assign_addresses(labelPtr label, int address, boolean is_data_label){
    boolean is_external = FALSE, is_action = FALSE;
    while (label)
    {
        is_external = label->external;
        is_action = label->action;
        if (!is_external && (is_data_label ^ is_action)){
            label->address += address;
        }
        label = label->next;
    }
    
}

/**
 * @brief this method calls the right method to handle directives
 *
 * @param dir_type directive index in enum
 * @param line current line (text)
 * @return int (error code or 0 for success)
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
 * @brief this method checks if a data directive is valid and adds to memory
 *
 * @param line current line (text)
 * @return int (error code or 0 for success)
 */
int handle_data_directive(char *line)
{
    boolean isnumber = FALSE, iscomma = FALSE;
    char copy[LINE_LEN];
    while (!end_of_line(line))
    {
        line = next_comma_word(copy, line);
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
 * @brief this method checks if a string directive is valid and adds to memory
 *
 * @param line current line (text)
 * @return int (error code or 0 for success)
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
    if (line[line_len - 2] != '\"')
        return STRING_OPERAND_NOT_VALID;
    line = skip_spaces(line);
    if (!end_of_line(line))
    {
        /*  copy[strlen(copy) - 1] = "\0";*/
        line[line_len - 2] = '\0';
        /*copy[0] = "\0";*/
        /*printf("without \": %s\n", copy);
        printf("with \": %s\n", copy + 1);*/
        write_string_to_data(line);
    }

    return 0;
}
/**
 * @brief this method checks if a struct directive is valid and adds to memory
 *
 * @param line current line (text)
 * @return int (error code or 0 for success)
 */
int handle_struct_directive(char *line)
{
    int line_len = strlen(line);

    char copy[LINE_LEN], c = copy;
    printf("my line is: %s\n", line);
    copy_word(copy, line);

    if (end_of_line(copy) || !is_number(copy))
        return STRUCT_INVALID_NUM;
    data[dc++] = (unsigned int)atoi(copy);
    line = next_comma_word(copy, line);
    printf("after line:%s \t copy: %s\n", line, copy);
    line = next_comma_word(copy, line);
    printf("2x next_comma_word: %s \t copy: %s\n", line, copy);
    /* copy_word(copy, line); */
    if (!end_of_line(line) && *copy == ',')
    {
        line = next_comma_word(copy, line); /* copy equvals string with "" */
        printf("and now copy is: %s\n", copy);
        if (end_of_line(copy))
            return STRUCT_EXPECTED_STRING;
        else
        {
            printf("copy: %c, strlen: %c\n", copy[0], copy[strlen(copy) - 1]);
            if (copy[0] != '"' && copy[strlen(copy) - 1] != '"')
                return STRUCT_INVALID_STRING;
            /*  copy_word(copy, line); */
            copy[line_len - 1] = '\0';
            write_string_to_data(copy + 1);
        }
    }
    else
    {
        return EXPECTED_COMMA_BETWEEN_OPERANDS;
    }
    line = next_word(line);
    if (!end_of_line(line))
        return STRUCT_TOO_MANY_OPERANDS;
    return 0;
}
/**
 * @brief this method checks if a extern directive is valid and adds to memory
 *
 * @param line current line (text)
 * @return int (error code or 0 for success)
 */
int handle_extern_directive(char *line)
{
    labelPtr new_label = NULL;
    char copy[LABEL_LEN];
    copy_word(copy, line);
    if (end_of_line(copy))
        return EXTERN_NO_LABEL;
    if (!check_for_label(copy, FALSE))
        return EXTERN_INVALID_LABEL;
    line = next_word(line);
    if (!end_of_line(line))
        return EXTERN_TOO_MANY_OPERANDS;
    copy_word(copy, line);
    new_label = add_label(&symbols_table, copy, 0);
    if (!new_label)
        return 1;
    return 0;
}
/**
 * @brief this methods writes text memory
 *
 * @param line current line (text)
 */
void write_string_to_data(char *line)
{
    /*    printf("write_string_to_data: %s\n", line); */
    while (!end_of_line(line))
    {
        data[dc++] = (unsigned int)*line; /* Inserting a character to data array */
        line++;
    }
    data[dc++] = '\0';
}
/**
 * @brief this method calls the right method to handle command
 *
 * @param type command type from enum 
 * @param line current line (text)
 * @return int (error code or 0 for success)
 */
int handle_command(int type, char *line)
{
    boolean first_op = FALSE, second_op = FALSE;
    int first, second;
    unsigned int word;
    char op1[20], op2[20];
    printf("handle command\t %s\n", line);
    line = next_comma_word(op1, line);
    /* printf("next command\t %s\n", op1);*/
    if (!end_of_line(op1))
    {
        first_op = TRUE;
        line = next_comma_word(op2, line);
        /*printf("next next command %s\n", op2);*/
        if (!end_of_line(op2))
        { /* if not empty must be a , */
            /*  printf("next next next command %s\n", op2);*/
            if (op2[0] != ',')
            {
                return COMMAND_UNEXPECTED_CHAR;
            }
            line = next_comma_word(op2, line);
            /*printf("next next next next command %s,line: %s\n", op2, line);*/
            if (!end_of_line(line))
            {
                return COMMAND_UNEXPECTED_CHAR;
            }
            second_op = TRUE;
        }
    }
    /*printf("here\n");*/
    line = skip_spaces(line);
    /* printf("handle spaces\t %s\n", line);*/
    if (!end_of_line(line)) /* command a1, a2  a3 is not valid */
        return COMMAND_TOO_MANY_OPERANDS;
  /*  printf("%d-%d\n", first_op, second_op); */
    if (first_op)
    {
/*        printf("first_op here\n"); */
        first = method_type(op1);
    }
 /*   printf("in between here\n"); */
    if (second_op)
    {
        second = method_type(op2);
    }
 /*   printf("second_op here\n"); */
    /* check for input errors */
    if (first == COMMAND_INVALID_METHOD || second == COMMAND_INVALID_METHOD)
        return COMMAND_INVALID_METHOD;
    if (!num_operation_fits_command(type, first_op, second_op))
        return COMMAND_INVALID_NUMBER_OF_OPERANDS;
    if (!method_fits_command(type, first, second))
        return COMMAND_INVALID_OPERANDS_METHODS;
    /* done checking, adding to data */
  /*  printf("done checking\n");*/
    word = word_to_bits(type,first_op,second_op,first,second);
 /*   printf("word id now: %u|%x|%d\n",word,word,word);*/
    write_command_to_instructions(word);
 /*   printf("writing to memory\n"); */
    ic += word_count_by_command(first_op,second_op,first,second);
 /*   printf("FINISH\n"); */
    return 0;
}
/**
 * @brief this method checks with type (register, immediate, direct, struct)
 *
 * @param op operator to check on
 * @return int (index in methods enum)
 */
int method_type(char *op)
{
    char *after_dot, *before_dot;
   /* printf("op is: %s\n", op);*/
    if (end_of_line(op))
        return NOT_FOUND;
    if (*op == '#')
    {
        op++;
        if (is_number(op))
            return M_IMMEDIATE;
    }
    if (is_register(op))
    {
        return M_REGISTER;
    }

    if (check_for_label(op, FALSE) == 1)
    {
        return M_DIRECT;
    }

    printf("this must be a struct!\n");
   /* printf("op was: %s\n", strtok(op, "."));
    printf("new op: %s\n", strtok(NULL, "."));*/
    before_dot = strtok(op, ".");
    printf("op was:: %s\n",before_dot);
    after_dot = strtok(NULL, ".");
    printf("new op: %s\n", after_dot);
    if (check_for_label(before_dot, FALSE))
    {                                                       /* Splitting by dot character */
        if (strlen(after_dot) == 1 && (*after_dot == '1' || /* After the dot there should be '1' or '2' */
                                       *after_dot == '2'))
            return M_STRUCT;
    }
    return COMMAND_INVALID_METHOD;
}
/**
 * @brief this command validates that the command mathces the number of operators
 *
 * @param command_type  command type from enum 
 * @param first_op first operator in command
 * @param second_op second operator in command
 * @return boolean
 */
boolean num_operation_fits_command(int command_type, boolean first_op, boolean second_op)
{
    switch (command_type)
    {
    case MOV:
        return first_op && second_op;
    case CMP:
        return first_op && second_op;
    case ADD:
        return first_op && second_op;
    case SUB:
        return first_op && second_op;
    case LEA:
        return first_op && second_op;
    case NOT:
        return first_op && !second_op;
    case CLR:
        return first_op && !second_op;
    case INC:
        return first_op && !second_op;
    case DEC:
        return first_op && !second_op;
    case JMP:
        return first_op && !second_op;
    case BNE:
        return first_op && !second_op;
    case GET:
        return first_op && !second_op;
    case PRN:
        return first_op && !second_op;
    case JSR:
        return first_op && !second_op;
    case RTS:
        return !first_op && !second_op;
    case HLT:
        return !first_op && !second_op;
    }
    return FALSE;
}
/**
 * @brief this function validates that the command matches the method type
 *
 * @param commant_type  command type from enum 
 * @param first first operator type
 * @param second second operator type
 * @return boolean
 */
boolean method_fits_command(int command_type, int first, int second)
{
    switch (command_type)
    {
    case MOV:
        return all_source_method(first) && non_immediate_method(second);
    case CMP:
        return all_source_method(first) && all_dest_method(second);
    case ADD:
        return all_source_method(first) && non_immediate_method(second);
    case SUB:
        return all_source_method(first) && non_immediate_method(second);
    case LEA:
        return (first == M_DIRECT || first == M_STRUCT) && non_immediate_method(second);
    case NOT:
        return non_immediate_method(second);
    case CLR:
        return non_immediate_method(second);
    case INC:
        return non_immediate_method(second);
    case DEC:
        return non_immediate_method(second);
    case JMP:
        return non_immediate_method(second);
    case BNE:
        return non_immediate_method(second);
    case GET:
        return non_immediate_method(second);
    case PRN:
        return all_dest_method(second);
    case JSR:
        return non_immediate_method(second);
    case RTS:
        return !all_source_method(first) && !all_dest_method(second);
    case HLT:
        return !all_source_method(first) && !all_dest_method(second);
    }
    return FALSE;
}
/**
 * @brief this functions checks that the method type will match all avaliable types for source
 *
 * @param method_type  command type from enum 
 * @return boolean
 */
boolean all_source_method(int method_type)
{
    return method_type == M_IMMEDIATE || method_type == M_DIRECT ||
           method_type == M_STRUCT || method_type == M_REGISTER;
}
/**
 * @brief this functions checks that the method type will match all avaliable types for destination
 *
 * @param method_type  command type from enum 
 * @return boolean
 */
boolean all_dest_method(int method_type)
{
    return method_type == M_IMMEDIATE || method_type == M_DIRECT ||
           method_type == M_STRUCT || method_type == M_REGISTER;
}
/**
 * @brief this functions checks that the method type will non immediate type for destination
 *
 * @param method_type  command type from enum 
 * @return boolean
 */
boolean non_immediate_method(int method_type)
{
    return method_type == M_DIRECT ||
           method_type == M_STRUCT || method_type == M_REGISTER;
}
/**
 * @brief this function checks if a method is struct and returns the word count accordingly
 * 2 for struct 1 for other
 * @param method_type  command type from enum 
 * @return int
 */
int words_count_by_method(int method_type)
{
    if (method_type == M_STRUCT)
        return 2;
    return 1;
}
/**
 * @brief this command calculates the word count to insert to memory according to operators and types
 * 
 * @param first_op if first operator exists
 * @param second_op if second operator exists
 * @param first first method type from enum
 * @param second second method type from enum
 * @return int 
 */
int word_count_by_command(boolean first_op, boolean second_op, int first, int second)
{
    int word_count = 0;
    if (first_op)
        word_count += words_count_by_method(first);
    if (second_op)
        word_count += words_count_by_method(second);

    if (first_op && second_op && first == M_REGISTER && second == M_REGISTER)
    {
        word_count--;
    }
    return word_count;
}
/**
 * @brief this command writes the command to memory
 * 
 * @param word word to write
 */
void write_command_to_instructions(unsigned int word){
    instructions[ic++] = word;
}
/**
 * @brief this function creates an unsigned int from command to insert to memory
 * 
 * @param method_type  command type from enum 
 * @param first_op if first operator exists
 * @param second_op if second operator exists
 * @param first first method type from enum
 * @param second second method type from enum
 * @return unsigned int 
 */
unsigned int word_to_bits(int method_type, boolean first_op, boolean second_op, int first, int second){
    unsigned int word_in_bits = method_type;
  /*  printf("method_type: %d,first_op: %d,second_op: %d,first: %d,second: %d\n",method_type,first_op,second_op,first,second); */
    
    word_in_bits <<= BITS_IN_METHOD; /* add space for method*/

    if (first_op && second_op){
        word_in_bits |= first;
        word_in_bits <<= BITS_IN_METHOD;
        word_in_bits |= second;
    }
    else if (first_op){
        word_in_bits |= first;
    }
    word_in_bits = add_are(word_in_bits,ABSOLUTE);
    return word_in_bits;
}