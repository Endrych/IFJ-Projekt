#include "token.h"
#include <stdlib.h>

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

int is_keyword(char* text, int length)
{
    switch(length)
    {
        case 2:
            if(strcmp(text,"As") == 0)
                return kw_as;
            else if(strcmp(text,"Do")==0)
                return kw_do;
            else if(strcmp(text,"If")==0)
                return kw_if;
            else if(strcmp(text,"Or")==0)
                return kw_or;
            else
                return -1;
            break;
        case 3:
            if(strcmp(text,"Asc") == 0)
                return kw_asc;
            else if(strcmp(text,"Dim")==0)
                return kw_dim;
            else if(strcmp(text,"End")==0)
                return kw_end;
            else if(strcmp(text,"Chr")==0)
                return kw_chr;
            else if(strcmp(text,"And")==0)
                return kw_and;
            else if(strcmp(text,"For")==0)
                return kw_for;
            else if(strcmp(text,"Not")==0)
                return kw_not;
            else
                return -1;
            break;
        case 4:
            if(strcmp(text,"Else")==0)
                return kw_else;
            else if(strcmp(text,"Loop")==0)
                return kw_loop;
            else if(strcmp(text,"Then")==0)
                return kw_then;
            else if(strcmp(text,"Exit")==0)
                return kw_exit;
            else if(strcmp(text,"Next")==0)
                return kw_next;
            else if(strcmp(text,"True")==0)
                return kw_true;
            else
                return -1;
            break;
        case 5:
            if(strcmp(text,"Input")==0)
                return kw_input;
            else if(strcmp(text,"Print")==0)
                return kw_print;
            else if(strcmp(text,"Scope")==0)
                return kw_scope;
            else if(strcmp(text,"While")==0)
                return kw_while;
            else if(strcmp(text,"False")==0)
                return kw_false;
            else
                return -1;
            break;
        case 6:
            if(strcmp(text,"Double")==0)
                return kw_double;
            else if(strcmp(text,"Length")==0)
                return kw_length;
            else if(strcmp(text,"Return")==0)
                return kw_return;
            else if(strcmp(text,"String")==0)
                return kw_string;
            else if(strcmp(text,"SubStr")==0)
                return kw_subStr;
            else if(strcmp(text,"Elseif")==0)
                return kw_elseif;
            else if(strcmp(text,"Shared")==0)
                return kw_shared;
            else if(strcmp(text,"Static")==0)
                return kw_static;   
            else
                return -1;
            break;
        case 7:
            if(strcmp(text,"Declare")==0)
                return kw_declare;
            else if(strcmp(text,"Integer")==0)
                return kw_integer;
            else if(strcmp(text,"Boolean")==0)
                return kw_boolean;
            else
                return -1;
            break;
        case 8:
            if(strcmp(text,"Function")==0)
                return kw_function;
            else if(strcmp(text,"Continue")==0)
                return kw_continue;
            else
                return -1;
            break;
        default:
            return -1;
            break;
    }
}