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

int main (int argc, char *argv[])
{
    int i; 
    char *get_filename;
    FILE *fp;
    
    if(argc > 1) /* check if there's atleast one file that is sent with the command */
    {
        for (i=1; i < argc; i++){
            get_filename = create_file(argv[i], FILE_INPUT); 
            fp = fopen(get_filename, "r");
            
            if(fp != NULL){ /* File exists */
                printf("Start assemble the file: %s\n", get_filename);
                pre_assembler(fp,argv[i]);
				fclose(fp);
            }
            else{
                fprintf(stderr, "there was an error while trying to open the requested file or file not exist.\n");
            }
        }
    }
    else{
        fprintf(stderr, "Not enough arguments,\ncommand line should be like this: ./assembler file1 file2 .. \n");
    }
    return 0;
}


/*
The main function that will translate the file provided in assembler to machine language.
It will read the file line by line in 3 phases, using 3 linked list for the code, data, and symbol list.
The function will also check for errors on each line and will return false if there's an error,
also if there's an error the functio will not create the .ext and .ent files. 
*/
