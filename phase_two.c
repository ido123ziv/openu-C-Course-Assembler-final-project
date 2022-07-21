/*
Project by Eran Cohen and Ido Ziv
*/

#include "assembler.h"
#include "global_functions.h"
#include "phases.h"

FILE *obFile, *entFile, *extFile;
int line_count;
extPtr ext_list;

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
            error_code = read_line_ph2(line, line_count);
        if (error_code)
            write_error_code(error_code, line_count);
        /* TODO: add error_exists */

        line_count++;
    }
    if (!error_exists)
        write_output_files(file_name);
}

void write_extern(FILE *file){
    char *address;
    extPtr node = ext_list;

    /* TODO: complete later this and write_entry */
}

/* Function will write the output files that need to be create: .ob, (.ent, .ext if needed)*/
void write_output_files(char *src)
{
    /* TODO: finish write_output functions */
    FILE *file;

    file = create_file(src, FILE_OBJECT);
    write_output_ob(file);

    if (has_entry)
    {
        file = create_file(src, FILE_ENTRY);
        write_output_entry(file);
    }

    if (has_extern)
    {
        file = create_file(src, FILE_EXTERN);
        write_output_extern(file);
    }
}

int read_line_ph2(char *line, int line_num)
{
    char word[LINE_LEN];
    int cmd, dir; /* we need to check if it's directive or command */

    line = skip_spaces(line);

    if (end_of_line(line))
        return;
    copy_word(word, line);
    if (check_for_label(word, TRUE))
    { /* If label we skip to next word */
        line = next_word(line);
        copy_word(word, line);
    }

    /* Encoding the rest of a command from first pass */

    if ((cmd = find_command(word)) != NOT_FOUND)
    {
        line = next_word(line);
        cmd_ph2_binary(cmd, line);
    }

    else if ((dir = find_directive(word)) != NOT_FOUND)
    {
        line = next_word(line);
        if (dir == ENTRY)
        { /* only need to take care of entry */
            copy_word(word, line);
            /* TODO: make entry */
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

    check_operands(cmd, &is_src, &is_dest);

    /* Get src and dest address method */
    if (is_src)
        src_method = get_bits(instructions[ic], SRC_START_POS, SRC_END_POS);
    if (is_dest)
        dest_method = get_bits(instructions[ic], DEST_START_POS, DEST_END_POS);

    /* Fix pointers (src,dest) to point the correct ops based on check_operands */
    if (is_src || is_dest)
    {
        line = next_comma_word(op1, line);
        if (is_src && is_dest)
        { /* We have 2 ops to handle */
            line = next_comma_word(op2, line);
            next_comma_word(op2, line); /* store op2 */
        }
        else
        { /* If we have only 1 op it would be dest op */
            dest = op1;
            src = NULL;
        }
    }

    ic++; /* First word already encoded in phase 1 */
    if (is_src && is_dest && src_method == M_REGISTER && dest_method == M_REGISTER)
    {
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

void encode_ph2_word(boolean is_dest, int method, char *op)
{
    char *temp;
    unsigned int word = 0;

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
    {
        /* TODO: complete this one */
    }
}

/* Encode label to memory by it's name */
void encode_label(char *label)
{
    unsigned int word;

    if (is_label_exist(symbols_table, label))
    { /* Check if label existes, if it is get address of label */
        word = get_label_address(symbols_table, label);

        if(is_external_label(symbols_table, label)){
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
    /* TODO: finish all sub functions!! */
}