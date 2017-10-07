#include "token.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

Token* create_token()
{
    Token* token;
    token = (Token*)malloc(sizeof(Token));
    return token;
}

void destruct_token(Token* token)
{
    free(token);
    token = NULL;
}

int is_keyword(char* text)
{
    int res = -1;
    int length = strlen(text);
    switch(length)
    {
        case 2:
            if(strcmp(text,"as") == 0)
                res =  kw_as;
            else if(strcmp(text,"do")==0)
                res = kw_do;
            else if(strcmp(text,"if")==0)
                res = kw_if;
            else if(strcmp(text,"or")==0)
                res = kw_or;
            break;
        case 3:
            if(strcmp(text,"asc") == 0)
                res = kw_asc;
            else if(strcmp(text,"dim")==0)
                res = kw_dim;
            else if(strcmp(text,"end")==0)
                res = kw_end;
            else if(strcmp(text,"chr")==0)
                res = kw_chr;
            else if(strcmp(text,"and")==0)
                res = kw_and;
            else if(strcmp(text,"for")==0)
                res = kw_for;
            else if(strcmp(text,"not")==0)
                res = kw_not;
            break;
        case 4:
            if(strcmp(text,"else")==0)
                res = kw_else;
            else if(strcmp(text,"loop")==0)
                res = kw_loop;
            else if(strcmp(text,"then")==0)
                res = kw_then;
            else if(strcmp(text,"exit")==0)
                res = kw_exit;
            else if(strcmp(text,"next")==0)
                res = kw_next;
            else if(strcmp(text,"true")==0)
                res = kw_true;
            break;
        case 5:
            if(strcmp(text,"input")==0)
                res = kw_input;
            else if(strcmp(text,"print")==0)
                res = kw_print;
            else if(strcmp(text,"scope")==0)
                res = kw_scope;
            else if(strcmp(text,"while")==0)
                res = kw_while;
            else if(strcmp(text,"false")==0)
                res = kw_false;
            break;
        case 6:
            if(strcmp(text,"double")==0)
                res = kw_double;
            else if(strcmp(text,"length")==0)
                res = kw_length;
            else if(strcmp(text,"return")==0)
                res = kw_return;
            else if(strcmp(text,"string")==0)
                res = kw_string;
            else if(strcmp(text,"substr")==0)
                res = kw_subStr;
            else if(strcmp(text,"elseif")==0)
                res = kw_elseif;
            else if(strcmp(text,"shared")==0)
                res = kw_shared;
            else if(strcmp(text,"static")==0)
                res = kw_static;   
            break;
        case 7:
            if(strcmp(text,"declare")==0)
                res = kw_declare;
            else if(strcmp(text,"integer")==0)
                res = kw_integer;
            else if(strcmp(text,"boolean")==0)
                res = kw_boolean;
            break;
        case 8:
            if(strcmp(text,"function")==0)
                res = kw_function;
            else if(strcmp(text,"continue")==0)
                res = kw_continue;
            break;
    }
    return res;
}
