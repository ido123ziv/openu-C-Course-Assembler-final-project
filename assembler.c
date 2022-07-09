/*
Project by Eran Cohen and Ido Ziv
*/

/*
This is the main function it will recieve the assembly input / file and will
translate it to different files as needed 
*/

#include "assembler.h"

int main (int argc, char *argv[]){
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
            }
            else{
                fprintf(stderr, "there was an error while trying to open the requested file.\n");
            }
        }
    }
    else{
        fprintf(stderr, "Not enough arguments,\ncommand line should be like this: ./assembler file1 file2 .. \n");
    }
    return 0;
}