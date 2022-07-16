/*
Project by Eran Cohen and Ido Ziv
*/

#include "assembler.h"
#include "global_functions.h"
#include "phases.h"

FILE *obFile, *entFile, *extFile;

void phase_two(FILE *file, char *file_name){
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
    if(!error_exists)
        write_output_files(file_name);
}

/* Function will write the output files that need to be create: .ob, (.ent, .ext if needed)*/
void write_output_files(char *src)
{
    /* TODO: finish write_output functions */
    FILE *file;

    file = create_file(src, FILE_OBJECT);
    write_output_ob(file);

    if(has_entry) {
        file = create_file(src, FILE_ENTRY);
        write_output_entry(file);
    }

    if(has_extern)
    {
        file = create_file(src, FILE_EXTERN);
        write_output_extern(file);
    }
}

int read_line_ph2(char * line, int line_num){
    char word[LINE_LEN];
    int cmd, dir;

    line = skip_spaces(line);

    if(end_of_line(line))
        return;
    copy_word(word, line);
    if(check_for_label(word, TRUE)) { /* If label we skip to next word */
        line = next_word(line);
        copy_word(word,line);
    }

    /* Encoding the rest of a command from first pass */
    if ((cmd = find_command(word)) != NOT_FOUND){ 
        line = next_word(line);
        /* TODO: handle command */
    }

    else if((dir = find_directive(word)) != NOT_FOUND){
        line = next_word(line);
        if(dir == ENTRY){
            copy_word(word, line);
            /* TODO: make entry */
        }
    }
}
