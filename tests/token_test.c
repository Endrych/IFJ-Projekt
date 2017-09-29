#include <stdio.h>
#include "../src/token.h"
#include <stdlib.h>
#include <string.h>

int main()
{
    printf("\n\nTEST START\n________________________________________________________________________________\n\n");
    char* string = NULL;
    string = calloc(20,sizeof(char));
    int str_pos = 0;

    // Integer Token
    printf("1) Integer: ");
    Token* integer_token = create_token();
    integer_token->type = type_integer;
    integer_token->atribute.int_value = 15;
    if(integer_token->atribute.int_value == 15 && integer_token->type == type_integer)
        printf("Success!\n");
    else
        printf("Wrong!\n");      
    
    // Double Token
    printf("2) Double: ");
    Token* double_token = create_token();
    double_token->type = type_double;
    double_token->atribute.double_value = 5.4234545484;
    if(double_token->atribute.double_value == 5.4234545484 && double_token->type == type_double)
        printf("Success!\n");
    else
        printf("Wrong!\n");    

    // Keyword Token
    printf("3) Keyword ");
    Token* keyword_token = create_token();
    keyword_token->type = type_keyword;
    keyword_token->atribute.keyword_value = kw_boolean;
    if(keyword_token->atribute.keyword_value == kw_boolean && keyword_token->type == type_keyword)
        printf("Success!\n");
    else
        printf("Wrong!\n");   

    // Id Token
    printf("4) Id ");
    Token* id_token = create_token();
    strcpy(string,"pos#");
    id_token->atribute.chain_pointer = &string[str_pos];
    str_pos += 4;
    id_token->type = type_id;
    if(id_token->type == type_id)
    {
        printf("Type Success!\n");
        int pos = 0;
        printf("     Expected value: pos\n");
        printf("     In token: ");
        while(id_token->atribute.chain_pointer[pos] != '#')
        {
            putchar(id_token->atribute.chain_pointer[pos]);
            pos++;
        }
    }
    else
        printf("Wrong Type!\n");   

    // String Token
    printf("\n5) String ");
    Token* string_token = create_token();
    strcpy(&string[str_pos],"Ahoj kamo#");
    string_token->atribute.chain_pointer = &string[str_pos];
    str_pos += 10;
    string_token->type = type_string;
    if(string_token->type == type_string)
    {
        printf("Type Success!\n");
        int pos = 0;
        printf("     Expected value: Ahoj kamo\n");
        printf("     In token: ");
        while(string_token->atribute.chain_pointer[pos] != '#')
        {
            putchar(string_token->atribute.chain_pointer[pos]);
            pos++;
        }
    }
    else
        printf("Wrong Type!\n");   

    // Destruct token
    destruct_token(integer_token);
    destruct_token(double_token);
    destruct_token(keyword_token);
    destruct_token(id_token);
    destruct_token(string_token);
    free(string);
    printf("\n\n");
    return 0;
}