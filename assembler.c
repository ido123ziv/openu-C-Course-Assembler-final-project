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

const char * commands[] = {
        "mov", "cmp", "add", "sub", "not", "clr", "lea", "inc", "dec", "jmp", "bne",
        "get", "prn", "jsr", "rts", "hlt"
};

const char base32[32] = {
        '!', '@', '#', '$', '%', '^', '&', '*', '<', '>', 'a', 'b', 'c',
        'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p',
        'q', 'r', 's', 't', 'u', 'v'};

const char * directives[] = {
        ".data", ".string", ".struct", ".entry", ".extern"
};


int ic, dc;
int error_code;
boolean error_exists, has_entry, has_extern;
labelPtr symbols_table;

int main (int argc, char *argv[])
{
    /* Initialize global vars */
    error_exists = FALSE;
    has_entry = FALSE;
    has_extern = FALSE;
    ic = 0;
    dc = 0;

    /*****************************/

    int i; 
    char *get_filename;
    FILE *fp;
    
    if(argc > 1) /* check if there's atleast one file that is sent with the command */
    {
        for (i=1; i < argc; i++){
            get_filename = create_file(argv[i], FILE_INPUT); 
            printf("file is: %s\n", get_filename);
            fp = fopen(get_filename, "r");
            if(fp != NULL){ /* File exists */
                printf("Start assemble the file: %s\n", get_filename);
                printf("New name: %s\n", argv[i]);
                pre_assembler(fp,argv[i]);
				fclose(fp);
            }
            else{
                write_error_code(34,-1);
            }
            get_filename = create_file(argv[i], FILE_MACRO); 
            printf("new file is: %s\n", get_filename);
            fp = fopen(get_filename, "r");
            if(fp != NULL){ /* File exists */
                printf("Start ido the file: %s\n", get_filename); 
                phase_one(fp,get_filename);
				fclose(fp);
            }
            else{
                write_error_code(34,-1);
                /* fprintf(stderr, "there was an error while trying to open the requested file or file not exist.\n"); */
            }
        }
    }
    else{
        write_error_code(35,-1);
    }
    return 0;
}


/*
The main function that will translate the file provided in assembler to machine language.
It will read the file line by line in 3 phases, using 3 linked list for the code, data, and symbol list.
The function will also check for errors on each line and will return false if there's an error,
also if there's an error the functio will not create the .ext and .ent files. 
*/
