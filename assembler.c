/*
Project by Eran Cohen and Ido Ziv
*/

/*
This is the main function it will recieve the assembly input / file and will
translate it to different files as needed
*/

#include "assembler.h"
#include "global_functions.h"
#include "pre_assembler.h"
#include "phases.h"

const char *commands[] = {
    "mov", "cmp", "add", "sub", "not", "clr", "lea", "inc", "dec", "jmp", "bne",
    "get", "prn", "jsr", "rts", "hlt"};

const char base32[32] = {
    '!', '@', '#', '$', '%', '^', '&', '*', '<', '>', 'a', 'b', 'c',
    'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p',
    'q', 'r', 's', 't', 'u', 'v'};

const char *directives[] = {
    ".data", ".string", ".struct", ".entry", ".extern"};

const char *types[] = {
    "M_IMMEDIATE", "M_DIRECT", "M_STRUCT", "M_REGISTER"
};

int ic, dc;
int error_code;
boolean error_exists, has_entry, has_extern;
labelPtr symbols_table;
unsigned int data[MACHINE_RAM];
unsigned int instructions[MACHINE_RAM];

int main(int argc, char *argv[])
{
    /*****************************/
    int i;
    char *get_filename;
    FILE *file;
    /* Initialize global vars */
    error_exists = FALSE;
    has_entry = FALSE;
    has_extern = FALSE;
    ic = 0;
    dc = 0;



    print_data(data, instructions);
    if (argc > 1) /* check if there's atleast one file that is sent with the command */
    {
        for (i = 1; i < argc; i++)
        {
            get_filename = create_file(argv[i], FILE_INPUT);
            file = fopen(get_filename, "r");
            if (file != NULL)
            { /* File exists */
                printf("Start assemble the file: %s\n", get_filename);
                pre_assembler(file, argv[i]);
                fclose(file);
            }
            else
            {
                write_error_code(CANNOT_OPEN_FILE, -1);
            }
            get_filename = create_file(argv[i], FILE_MACRO);
            file = fopen(get_filename, "r");
            if (file != NULL)
            { /* File exists */
                printf("Starting phase one on file: %s\n", get_filename);
                phase_one(file, get_filename);
                fclose(file);
                /* Start phase 2 if there are no errors */
                if (!error_exists)
                {
                    file = fopen(get_filename, "r");
                    if (file != NULL)
                    { /* File exists */
                        printf("Starting second phase on file: %s\n", get_filename);
                        phase_two(file, get_filename);
                        fclose(file);
                    }
                }
            }
            else
            {
                write_error_code(CANNOT_OPEN_FILE, -1);
                /* filerintf(stderr, "there was an error while trying to open the requested file or file not exist.\n"); */
            }
        }
    }
    else
    {
        write_error_code(NOT_ENOUGH_ARGUMENTS, -1);
    }
    print_data(data, instructions);
    return 0;
}
