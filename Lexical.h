#ifndef LEXICAL_H
#define LEXICAL_H

#include "types.h"

static char keyword[35][20] = {"const", "volatile", "extern", "auto", "register", "static", "signed", "unsigned",
    "short", "long", "double", "char", "int",  "float", "struct",  "union",  "enum", "void", "typedef", "goto",  "continue",
    "return", "break", "if", "else","for", "while", "do", "switch", "case", "default", "sizeof"};

static char arit_operator1[19] = {'+', '-', '*', '/', '%', '=', '>', '<', '&', '!', '^', '|'};
static char arit_operator2[9]={'~', ',', '(', ')', '[', ']', ':', '.', '?'};

static char arit_operator3[22][3]={"+=", "-=", "*=", "/=", "%=", "<<=", ">>=", "&=", "^=", "|=", "++", "--", "==", "!=", "<=", ">=", "&&", "||","<<",">>" , "->"};

static char non_operator[4]={'@','#','$','`'};

static char preprocessor_directives[8][10]={"#define", "#undef", "#include", "#ifdef", "#ifndef", "#if", "#else", "#endif"};

static char symbol[3] = {'{','}',';'};


struct Lexical

{

    char *src_file_name;
    FILE *src_fptr;

};

Status do_parcing (struct Lexical * lexical);

Status open_file(struct Lexical * lexical);

Status do_scanning (struct Lexical * lexical);

Status do_identify(char * str);

Status is_keyword(char * str);

Status is_string_literals(char * str);

Status is_digit(char * str);

Status is_operator(char * str);

Status is_arit_operator1(char * str);

Status is_arit_operator2(char * str);

Status is_arit_operator3(char * str);

Status is_symbols(char * str);

Status is_non_operator(char * str);

Status is_preprocessor_directives(char * str);

#endif

