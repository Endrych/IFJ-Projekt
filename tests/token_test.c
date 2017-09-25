#include<stdio.h>
#include "../src/token.h"

int main()
{
    printf("\n\nTEST START\n________________________________________________________________________________\n\n");
   
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
    keyword_token->atribute.keyword = kw_boolean;
    if(keyword_token->atribute.keyword == kw_boolean && keyword_token->type == type_keyword)
        printf("Success!\n");
    else
        printf("Wrong!\n");   
    
    // Id Token
    printf("3) Keyword ");
    Token* id_token = create_token();
    id_token->type = type_id;
    if(id_token->type == type_id)
        printf("Success!\n");
    else
        printf("Wrong!\n");   

    // Destruct token
    destruct_token(integer_token);
    destruct_token(double_token);
    destruct_token(keyword_token);
    destruct_token(id_token);
    printf("\n\n");
    return 0;
}