/*
Project by Eran Cohen and Ido Ziv
*/

#include "assembler.h"
#include "global_functions.h"
#include "phases.h"

int line_count;
extPtr ext_list;
/**
 * @brief this methods handles the algorithm of phase two -> complete the translation
 *
 * @param file local file path
 * @param file_name name of the file to open
 */
void phase_two(FILE *file, char *file_name)
{
    char line[LINE_LEN];
    line_count = 1;
    error_exists = FALSE;

    ic = 0;

    printf("---------------------------------------------------------\n");
    printf("Phase 2 on file: %s\n", file_name);
    printf("---------------------------------------------------------\n");

    while (fgets(line, LINE_LEN, file) != NULL)
    {
        error_code = 0;
        if (!ignore(line))
            read_line_ph2(line, line_count);
        if (error_code)
        {
            write_error_code(error_code, line_count);
            error_exists = TRUE;
            error_code = 0;
        }

        line_count++;
    }
    if (!error_exists)
        write_files(file_name);

    free_labels(&symbols_table);
    free_ext(&ext_list);
}
/**
 * @brief this function reads line by line from the file
 *
 * @param line current line (text)
 * @param line_num current line (index)
 * @return int
 */
int read_line_ph2(char *line, int line_num)
{
    int label_count;
    char word[LINE_LEN];
    int cmd, dir; /* we need to check if it's directive or command */

    line = skip_spaces(line);

    if (end_of_line(line))
        return;
    copy_word(word, line);
    label_count = check_for_label(word, TRUE);
    if (label_count)
    { /* If label we skip to next word */
        if (label_count > 1)
            return label_count;
        else
        {
            printf("This is a label: %s\n", word);
            line = next_word(line);
            copy_word(word, line);
        }
    }

    /* Encoding the rest of a command from first pass */

    if ((cmd = find_command(word)) != NOT_FOUND)
    {
        line = next_word(line);
        cmd_ph2_binary(cmd, line);
    }

    else if ((dir = find_directive(word)) != NOT_FOUND)
    {
        printf("Dir line is: %s", line);
        line = next_word(line);
        if (dir == ENTRY)
        { /* only need to take care of entry */
            copy_word(word, line);
            add_entry(symbols_table, word); /* Add an entry for this symbol */
            /* print error if needed? - need to check if needed */
        }
    }
}

/* Function will encode words in the command that could not be encode on phase 1 */
void cmd_ph2_binary(int cmd, char *line)
{

    /* Vars for encoding operands */
    char op1[LINE_LEN], op2[LINE_LEN]; /* First and second operands */
    char *src = op1, *dest = op2;
    boolean is_src = FALSE, is_dest = FALSE;
    int src_method = M_UNKNOWN, dest_method = M_UNKNOWN;

    printf("ic = %d\n", ic);
    check_operands(cmd, &is_src, &is_dest);
    printf("op1 = %d , op2 = %d\n", is_src, is_dest);
    printf("instruction is: %u\n", instructions[ic]);
    /* Get src and dest address method */
    if (is_src)
        src_method = get_bits(instructions[ic], SRC_START_POS, SRC_END_POS);
    if (is_dest)
        dest_method = get_bits(instructions[ic], DEST_START_POS, DEST_END_POS);
    printf("src method = %u , dst method = %u\n", src_method, dest_method);
    /* Fix pointers (src,dest) to point the correct ops based on check_operands */
    if (is_src || is_dest)
    {
        line = next_comma_word(op1, line);
        printf("op1 = %s ", op1);
        if (is_src && is_dest)
        { /* We have 2 ops to handle */
            line = next_comma_word(op2, line);
            next_comma_word(op2, line); /* store op2 */
            printf("op2 = %s\n", op2);
        }
        else
        { /* If we have only 1 op it would be dest op */
            dest = op1;
            src = NULL;
        }
    }

    ic++; /* First word already encoded in phase 1 */

    if (is_src && is_dest && src_method == M_REGISTER && dest_method == M_REGISTER)
    { /* Special case when 2 registers share same additional word */
        unsigned int word = build_reg(FALSE, src) | build_reg(TRUE, dest);
        instructions[ic++] = word;
    }
    else
    {
        if (is_src)
            encode_ph2_word(FALSE, src_method, src);
        if (is_dest)
            encode_ph2_word(TRUE, dest_method, dest);
    }
}

/* check which operands exist in the current command */
void check_operands(int cmd, boolean *is_src, boolean *is_dest)
{
    /* cmd is a int this int it's the location of cmd on commands[cmd]
     *  we sore commands[] so all the commands required 2 ops will be 0-4 in the array
     *  1 op will be 5 - 13 and no op are the last 2 */
    if (cmd < TWO_OPERANDS)
    {
        *is_src = TRUE;
        *is_dest = TRUE;
    }
    else if (cmd > TWO_OPERANDS && cmd < ONE_OPERANDS)
    {
        *is_src = FALSE;
        *is_dest = TRUE;
    }
    else
    { /* no operands */
        *is_src = FALSE;
        *is_dest = FALSE;
    }
}

/* Function builds the final word for a register op */
unsigned int build_reg(boolean is_dest, char *reg)
{
    unsigned int word = (unsigned int)atoi(reg + 1); /* Register number */

    if (!is_dest) /* TODO: might be 2 bits - need test */
        word <<= 4;
    word = add_are(word, ABSOLUTE);
    return word;
}
/**
 * @brief This methods encondes given word to memory
 *
 * @param is_dest whether or not the operator is destination operator
 * @param method  method type from enum
 * @param op operator
 */
void encode_ph2_word(boolean is_dest, int method, char *op)
{
    char *tmp;
    unsigned int word = 0;
    printf("is_dest = %d, method = %d , op1 = %s\n", is_dest, method, op);

    if (method == M_REGISTER)
    {
        word = build_reg(is_dest, op);
        instructions[ic++] = word;
    }
    else if (method == M_IMMEDIATE)
    {
        word = (unsigned int)atoi(op + 1);
        word = add_are(word, ABSOLUTE);
        instructions[ic++] = word;
    }
    else if (method == M_DIRECT)
        encode_label(op);
    else
    { /* if method = struct */
        /* struct include label before '.' and a number after it */
        tmp = strchr(op, '.');
        *tmp = '\0';
        encode_label(op);
        *tmp++ = '.';
        word = (unsigned int)atoi(tmp);
        word = add_are(word, ABSOLUTE);
        instructions[ic++] = word;
    }
}

/* Encode label to memory by it's name */
void encode_label(char *label)
{
    unsigned int word;

    if (is_label_exist(symbols_table, label))
    { /* Check if label existes, if it is get address of label */
        word = get_label_address(symbols_table, label);

        if (is_label_external(symbols_table, label))
        {
            printf("External label: %s\n", label);
            add_ext(&ext_list, label, ic + MEM_START);
            word = add_are(word, EXTERNAL);
        }
        else
            word = add_are(word, RELOCATABLE);

        instructions[ic++] = word;
    }
    else
    {
        ic++;
        error_code = COMMAND_LABEL_DOES_NOT_EXIST;
    }
}

/* *********** Write to files functions *************** */

/* Function will write the output files that need to be create: .ob, (.ent, .ext if needed)*/
void write_files(char *src)
{
    FILE *file;

    file = new_file(src, FILE_OBJECT);
    write_ob(file);

    if (has_entry)
    {
        file = new_file(src, FILE_ENTRY);
        write_entry(file);
    }

    if (has_extern)
    {
        file = new_file(src, FILE_EXTERN);
        write_extern(file);
    }
}

/* Write output for the .ob file
 * On first line is the size of instructions and data memory
 * Other lines: left size is address, right side is the word in memory */
void write_ob(FILE *file)
{
    unsigned int address = MEM_START;
    char *param1 = to_base_32(ic), *param2 = to_base_32(dc);
    int i;

    fprintf(file, "%s\t%s\n\n", param1, param2);
    free(param1);
    free(param2);

    for (i = 0; i < ic; address++, i++) /* Instructions memory */
    {
        param1 = to_base_32(address);
        param2 = to_base_32(instructions[i]);

        fprintf(file, "%s\t%s\n", param1, param2);

        free(param1);
        free(param2);
    }

    for (i = 0; i < dc; address++, i++) /* Data memory */
    {
        param1 = to_base_32(address);
        param2 = to_base_32(data[i]);

        fprintf(file, "%s\t%s\n", param1, param2);

        free(param1);
        free(param2);
    }

    fclose(file);
}

/* Writes the output of the .ent file.
 * Left side: name of label.
 * Right side: address of definition. */
void write_entry(FILE *file)
{
    char *base32_address;

    labelPtr ptr = symbols_table;
    /* Go through symbols table and print only symbols that have an entry */
    while (ptr)
    {
        if (ptr->entry)
        {
            base32_address = to_base_32(ptr->address);
            fprintf(file, "%s\t%s\n", ptr->name, base32_address);
            free(base32_address);
        }
        ptr = ptr->next;
    }
    fclose(file);
}

/* Writes the output of the .ext file.
 * Left side: name of label.
 * Right side: address where the external label should be replaced.
 */
void write_extern(FILE *file)
{
    
    char *base32_address;
    extPtr node = ext_list;
    printf("GOT HEREWEEEEEE\n");
    /* Go through external linked list and pulling values */
    do
    {
        base32_address = to_base_32(node->address);
        fprintf(file, "%s\t%s\n", node->name, base32_address); /* Printing to file */
        free(base32_address);
        node = node->next;
    } while (node != ext_list);
    fclose(file);
}

/* Function create a file with write permissions, get the original name and add the relevant externsion (.ob, .ent etc.)*/
FILE *new_file(char *filename, int type)
{
    FILE *file;
    filename = create_file(filename, type); /* Creating filename with extension */

    file = fopen(filename, "w"); /* Opening file with permissions */
    free(filename);              /* Allocated modified filename is no longer needed */

    if (file == NULL)
    {
        error_code = CANNOT_OPEN_FILE;
        return NULL;
    }
    return file;
}
