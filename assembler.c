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
extPtr ext_list;
unsigned int data[MACHINE_RAM];
unsigned int instructions[MACHINE_RAM];

int main(int argc, char *argv[])
{
    /*****************************/
    int i;
    char *get_filename;
    FILE *file;
    if (argc > 1) /* check if there's atleast one file that is sent with the command */
    {
        for (i = 1; i < argc; i++)
        {
            get_filename = create_file(argv[i], FILE_INPUT);
            file = fopen(get_filename, "r");
            if (file != NULL)
            { /* File exists */
                /* Initialize global vars */
                reset_vars();
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
                        get_filename[strlen(get_filename) -3] = '\0';
                        phase_two(file, get_filename);
                        fclose(file);
                    }
                }
            }
            else
            {
                write_error_code(CANNOT_OPEN_FILE, -1);
            }
        }
    }
    else
    {
        write_error_code(NOT_ENOUGH_ARGUMENTS, -1);
    }
    return 0;
}

/* Reset variables like lists etc. between new files */
void reset_vars()
{
    symbols_table = NULL;
    ext_list = NULL;
    has_entry = FALSE;
    has_extern = FALSE;
    error_exists = FALSE;
}