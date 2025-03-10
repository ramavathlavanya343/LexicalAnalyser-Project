#include <stdio.h>
#include "Lexical.h"
#include "types.h"
#include <string.h>
#include <ctype.h>

char data_type;


Status do_parcing(struct Lexical * lexical)

{

    if(open_file(lexical) == e_failure)

    {
	printf("\nINFO : Failed To Open The File\n");

	return e_failure;
    }

    printf("\nINFO : Successfully Opened The File\n");

    printf("\nINFO : Parcing %s Started \n\n", lexical->src_file_name);

    if(do_scanning(lexical) == e_failure)

    {
	printf("\nINFO : Parsing Failed \n");
	printf("\nINFO : Parsing %s Ended\n",lexical->src_file_name);

	return e_failure;

    }

    printf("\nINFO : Success \n");
    printf("\nINFO : Parcing %s ended\n" , lexical -> src_file_name);

    fclose(lexical->src_fptr);

    return e_success;

}

Status open_file(struct Lexical * lexical)

{
    lexical -> src_fptr = fopen(lexical->src_file_name,"r");

    if(lexical->src_fptr == NULL)
    {
	perror("fopen");
	fprintf(stderr,"\nERROR : Unable To OpenFile %s\n",lexical->src_file_name);
	return e_failure;
    }
    return e_success;

}

Status do_scanning(struct Lexical *lexical)

{


    char buffer[50];
    char ch;

    int smpl_bracket=0;
    int flr_bracket=0;
    int sqr_bracket=0;

    int count=0;

    while ( (ch=fgetc(lexical->src_fptr)) != EOF)
    {
	if(ch=='#')
	{
	    buffer[count++]=ch;
	    while((ch=fgetc(lexical->src_fptr))!='\n')
	    {
		buffer[count++]=ch;

	    }
	    strtok(buffer,"<,\",' '");

	    if(is_preprocessor_directives(buffer)==e_failure)
	    {

		printf("\nINFO : Compiler error %s \n",buffer);

		return e_failure;

	    }
	    
	    count=0;
	}

	else if(isalpha(ch))
	{
	    buffer[count++]=ch;

	    while( isalnum(ch=fgetc(lexical->src_fptr)) || ch == '_')
	    {
		buffer[count++]=ch;
	    }
	    buffer[count++]='\0';

	    do_identify(buffer);

	    count=0;
	    fseek(lexical->src_fptr,-1,SEEK_CUR);
	}
	else if(isdigit(ch))
	{
	    buffer[count++]=ch;

	    while( isalnum(ch=fgetc(lexical->src_fptr)) || ch=='.')
	    {
		buffer[count++]=ch;
	    }
	    buffer[count]='\0';

	    if(buffer[0]=='0')
	    {
		if(buffer[1] == 'X' || buffer[1] =='x')
		{
		    int i;
		    for(i=2; buffer[i] != '\0'; i++)
		    {
			if(!(isdigit(buffer[i])))
			{

			    printf("\nINFO : Compiler Error %s\n",buffer);

			    return e_failure;

			}
		    }

		}

		else if(buffer[1] == 'B' || buffer[1] =='b')
		{
		    for(int i=2;buffer[i] != '\0';i++)
		    {
			if(buffer[i] !='0' && buffer[i] !='1')
			{
			    printf("\nINFO : Compiler Error %s\n",buffer);

			    return e_failure;
			}
		    }

		}
		else
		{

		    for(int i=1;buffer[i] !='\0';i++)
		    {
			if(!(buffer[i] >= '0' && buffer[i] <='7'))

			{
			    printf("\nINFO : Compiler Error %s\n",buffer);
			    return e_failure;
			}
		    }

		}
	    }

	    else if(data_type == 'f' || data_type == 'd')
	    {
		int flag =0,i;

		for(i=0;buffer[i] !='\0';i++)
		{

		    flag=(buffer[i] == '.') ?flag++:flag;
		    if(flag>1)

		    {
			printf("\nINFO : Compiler Error %s\n",buffer);

			return e_failure;

		    }

		}

		if(data_type == 'f' && (buffer[i-1] != 'f' && !isdigit(buffer[i-1])))

		{


		    printf("\nINFO : Compiler Error %s\n",buffer);
		    return e_failure;
		}

		else if(data_type == 'd' && !isdigit(buffer[i-1]))
		{
		    printf("\nINFO : Compiler Error %s\n",buffer);
		    return e_failure;

		}
	    }

	    else

	    {

		for(int i=0;buffer[i] !=0; i++)
		{
		    if(buffer[i] == '.')

		    {
			strtok(buffer,".");
			break;
		    }
		    if(!isdigit(buffer[i]))
		    {
			printf("\nINFO : Compiler Error %s\n",buffer);

			return e_failure;
		    }
		}
	    }

	    is_digit(buffer);
	    count=0;

	    fseek(lexical ->src_fptr,-1,SEEK_CUR);
	}

	else if(isspace(ch))
	{
	    if (ch == '\n')

	    {
		if(smpl_bracket !=0)

		{
		    printf("\nINFO : Compiler Error Missing '(' or ')'\n");

		    return e_failure;

		}

		if(sqr_bracket !=0)
		{
		    printf("\nINFO : Compiler Error Missing '[' or ']' \n");

		    return e_failure;
		}
	    }

	    continue;

	}

	else if(ispunct(ch))

	{
	    buffer[count++] =ch;

	    if(is_non_operator(buffer) == e_success)

	    {
		printf("\nINFO : Compiler Error %s\n",buffer);
		return e_failure;

	    }

	    if(is_arit_operator1(buffer) == e_success)
	    {
		ch = fgetc(lexical->src_fptr);

		buffer[count++]=ch;
		buffer[count]= '\0';

		if(is_arit_operator3(buffer)!= e_success)
		{
		    count=1;
		    ch=buffer[0];

		    fseek(lexical->src_fptr,-1,SEEK_CUR);

		}
		else
		    count=0;
	    }
	    if(ch == '"')
	    {
		while((ch=fgetc(lexical ->src_fptr)) != '"')
		{
		    if(ch == EOF || ch == '\n')
		    {

			printf("\nINFO : Compiler Error , Missing \"\" \n");
			return e_failure;

		    }
		    buffer[count++]=ch;
		}
		buffer[count++]=ch;

	    }
	    else if(ch == 39)  //ascii of '\''

	    {
		while((ch=fgetc(lexical->src_fptr)) !=39)

		{
		    if(ch == EOF || ch == '\n')
		    {
			printf("\nINFO : Compiler Error , Missing \'\'' \n");
			return e_failure;

		    }
		    buffer[count++]=ch;
		}
		buffer[count++]=ch;
	    }

	    else if(ch == '/')  //to ignore comment section

	    {
		ch=fgetc(lexical->src_fptr);
		if(ch == '/')

		{
		    while((ch == fgetc(lexical->src_fptr)) != '\n');

		    count=0;

		}
		else if(ch=='*')
		{
		    while((ch=fgetc(lexical->src_fptr)))
		    {
			if(ch=='*')
			{
			    if((ch=fgetc(lexical->src_fptr))=='/')
			    {
				break;
			    }
			}
			if(ch==EOF)
			{
			    printf("\nINFO : Compailer error, missing /* */ \n" );
			    return e_failure;
			}
		    }
		    count=0;
		}
		else
		    fseek(lexical->src_fptr,-1,SEEK_CUR);
	    }

	    if(ch=='(')
	    {
		smpl_bracket++;
	    }
	    else if(ch==')')
	    {
		smpl_bracket--;
	    }
	    else if(ch=='{')
	    {
		flr_bracket++;
	    }
	    else if(ch=='}')
	    {
		flr_bracket--;
	    }
	    else if(ch=='[')
	    {
		sqr_bracket++;
	    }
	    else if(ch==']')
	    {
		sqr_bracket--;
	    }
	    else if(ch==';')
	    {
		data_type=0;
	    }

	    buffer[count]='\0';
	    do_identify(buffer);
	    count=0;
	}
    }
    if(flr_bracket!=0)
    {
	printf("\nINFO : compiler error missing '{' or '}' \n");
	return e_failure;
    }

}

Status do_identify(char * str)
{
    if(str[0]!='\0')
    {
	if(is_keyword(str)==e_success)
	{
	    printf("keyword\t\t\t: %s\n", str);
	    return e_success;
	}
	else if(is_arit_operator1(str)==e_success || is_arit_operator2(str)==e_success )
	{
	    printf("operator\t\t: %s\n", str);
	    return e_success;
	}
	else if(is_symbols(str)==e_success)
	{
	    printf("symbol\t\t\t: %s\n",str);
	    return e_success;
	}
	else if(str[0]=='"')
	{
	    printf("litral\t\t\t: %s\n",str);
	    return e_success;
	}
	else if(str[0]=='\'')
	{
	    printf("character constant\t: %s\n",str);
	    return e_success;
	}
	else
	{
	    printf("identifier\t\t: %s\n", str);
	    return e_success;
	}
    }
    return e_failure;
}


Status is_digit(char * str)
{
    printf("Number Constant\t\t: %s\n",str);
    return e_success;
}

/*  Status is_increment(char *str)

    {
    for(int i=0;i<32;i++)
    {
    if(!(strcmp(str,operators[i])))

    {
    printf("Operator\t\t: %s\n",str);
    return e_success;

    }
    }
    return e_failure;

    }*/
Status is_keyword(char * str)

{
    for(int i=0;i<32;i++)
    {
	if(!(strcmp(str,keyword[i])))
	{
	    data_type=str[0];
	    return e_success;
	}
    }
    return e_failure;

}


/*  Status is_non_res_keyword(char *str)

    {
    for(int i=0;i<13;i++)
    {
    if(!(strcmp(str,non_res_keywords_data[i])))
    {
    return e_success;

    }
    }

    return e_failure;

    }*/

Status is_arit_operator1(char * str)
{
    for(int i=0;i<19;i++)
    {
	if(str[0] == arit_operator1[i])
	{
	    return e_success;
	}
    }
    return e_failure;
}

Status is_arit_operator2(char * str)
{
    for(int i=0;i<9;i++)
    {
	if(str[0] == arit_operator2[i])
	{
	    return e_success;
	}
    }
    return e_failure;

}

Status is_arit_operator3(char * str)
{
    for(int i=0;i<16;i++)

    {
	if(!(strcmp(str,arit_operator3[i])))
	{
	    printf("Operator\t\t: %s\n",str);
	    return e_success;

	}

    }
    return e_failure;
}

Status is_symbols(char * str)
{
    for(int i=0;i<3;i++)
    {
	if(str[0] == symbol[i])
	{
	    return e_success;
	}
    }
    return e_failure;

}

Status is_non_operator(char * str)

{
    for(int i=0;i<4;i++)

    {
	if(str[0] == non_operator[i])
	{
	    return e_success;

	}
    }
    return e_failure;
}

Status is_preprocessor_directives(char * str)
{
    for(int i=0;i<8;i++)
    {
	if(!strcmp(str,preprocessor_directives[i]))
	{
	    return e_success;

	}
    }
    return e_failure;
}

