/*
Project by Eran Cohen and Ido Ziv
*/

#include "assembler.h"
#include "global_functions.h"
#include "phases.h"

FILE *obFile, *entFile, *extFile;

void phase_two(FILE *file, char *file_name)
{
    char line[LINE_LEN];
    int line_count = 1;
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
        /* TODO: handle command */
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
int cmd_ph2_binary(int cmd, char *line)
{
    char op1[LINE_LEN], op2[LINE_LEN]; /* First and second operands */
    char *src = op1, *dest = op2;
    boolean is_src = FALSE, is_dest = FALSE;
    int src_method = M_UNKNOWN, dest_method = M_UNKNOWN;

    check_operands(cmd, &is_src, &is_dest);

    if(is_src)
        src_method = get_bits(instructions[ic], SRC_START_POS, SRC_END_POS);
    if(is_dest)
        dest_method = get_bits(instructions[ic], DEST_START_POS, DEST_END_POS);


    return 0;
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
    else{ /* no operands */
        *is_src = FALSE;
        *is_dest = FALSE;
    }
}