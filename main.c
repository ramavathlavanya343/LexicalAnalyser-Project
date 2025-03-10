#include <stdio.h>
#include <string.h>
#include "Lexical.h"
#include "types.h"

/*
Name:
Date:
Description:
Sample Input:
Sample Output:
*/
int main(int argc,char *argv[])

{

    char *str;

    if(argc == 2)

    {
	str=strstr(argv[1],".");

	if(!strcmp(str,".c"))

	{
     
	    struct Lexical lexical;
	    lexical.src_file_name = argv[1];
	    if(do_parcing(&lexical) == e_success)
	    {
		printf("\nLexical Analysis Completed Successfully\n");
	    }

	    else
	    {
		printf("\nLexical Analysis Process Failed\n");

	    }
	}
	    else
	    {
		printf("Please Enter .c File Through CommandLine Argument");

	    }
    }
    else
    {
	printf("Please Enter 1.c File name");
    }

    return 0;

}









