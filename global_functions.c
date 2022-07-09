/*
Project by Eran Cohen and Ido Ziv
*/
#include "assembler.h"
/* 
This script will include all the global function that will be require for more than 1 script
for example: ignore spaces and tabs on input etc.
*/

/**
 * @brief 
 * This function skippes the whitespaces from the begining of a given string by incrementing the first index
 * @param c string
 * @param start index of first char
 * @return int -> the first index of character from which is not a whitespace
 */
int skip_whitespaces_at_begining(char c[], int start){
    for (; c[start] == ' ' || c[start] == '\t'; start++); /*incrementing start until to skip whitespaces in the begining of a string */
    return start;
}
/**
 * @brief 
 * This function skippes the whitespaces from the end of a given string
 * @param c string
 * @param end index of last char
 * @return int the index of the first char from the end that is not a whitespace
 */
int skip_whitespaces_at_end(char c[], int end){
    int i;
    for (i = end; i > 0; i --)
        if (!isspace(c[i]))
            return i+1;
    return 1;
}
/**
 * @brief gets a string that represent a number, if the string contains characters that are not numbers returns false
 * 
 * @param number number represented by a string
 * @return boolean whether it is a valid number
 */
boolean isnumber(char number[])
{
	int i;
	if(number[0] == '-' || number[0]== '+' || isdigit(number[0]))/*whether the first character is a digit or a sign*/
	{
		for(i=1; i < strlen(number) -1; i++)
			if(!isdigit(number[i]))
				return FALSE;
	}
	else
		return FALSE;
	return TRUE;
}

/**
 * @brief Create a file object
 * This function creates a new name for the file with matching extension.
 * @param original 
 * @param type 
 * @return char* of the new file name
 */
char *create_file(char *original, int type)
{
    char *modified = (char *) malloc(strlen(original) + MAX_EXTENSION_LENGTH); /* extends the string to max */
    if(modified == NULL)
    {
        fprintf(stderr, "Dynamic allocation error."); /* catching errors */
        exit(ERROR);
    }

    strcpy(modified, original); /* Copying original filename to the extended string */

    /* adding required file extension */

    switch (type)
    {
        case FILE_INPUT:
            strcat(modified, ".as");
            break;

        case FILE_OBJECT:
            strcat(modified, ".ob");
            break;

        case FILE_ENTRY:
            strcat(modified, ".ent");
            break;

        case FILE_EXTERN:
            strcat(modified, ".ext");

    }
    return modified;
}

/**
 * @brief 
 * this function validates the syntax of a given line regarding immediate and relative addresing methods
 * @param operand operand of the line
 * @param line_num number of the line
 * @param file_name name of the file
 * @return boolean whether the syntax is valid
 */
boolean syntax_validator(char operand[], int line_num, char *file_name)
{
	
	if(operand[0] == '#') /* immediate methods must begin with a '#' and a number must follow*/
	{
		
		if(strlen(operand)>1)
		{
			if(!isnumber(operand+1))
			{
			fprintf(stderr,"<file %s, line %d> Illegal input, The operand after '#' must be a real number)\n", file_name, line_num);
			return FALSE;
			}
		}
		else
			{
			fprintf(stderr,"<file %s, line %d> Illegal input, There is no char after the '#' sign must be a real number\n", file_name, line_num);
			return FALSE;
			}
	}
	if(operand[0] == '%')
	{
		if(strlen(operand) > 1)
		{
			if(operand[1]==' ')/*whether there is a space after % */
			{
			fprintf(stderr,"<file %s, line %d> Illegal space after the precent sign\n", file_name, line_num);
			return FALSE;
			}
		}else
			{
			fprintf(stderr,"<file %s, line %d> Illegal input, There is no char after the precent sign\n", file_name, line_num);
			return FALSE;
			}
	}
	
	return TRUE;
}