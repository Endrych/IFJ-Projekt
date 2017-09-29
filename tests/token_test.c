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
        
    // Operator test
    printf("\n6) Operators test \n");
    Token* operator_token = create_token();
    operator_token->type = type_operator;
    operator_token->atribute.operator_value = op_bracket;
    if(operator_token->atribute.operator_value == op_bracket && operator_token->type == type_operator)
        printf("Success!\n");
    else
        printf("Wrong!\n"); 
    if(operator_token->atribute.operator_value != op_bracket_end && operator_token->type == type_operator)
        printf("Success!\n");
    else
        printf("Wrong!\n"); 

    // Is Keyword test
    printf("\n7) Keywords test\n");
    char * test_keywords[] ={
        "And",
        "As",
        "Asc",
        "Boolean",
        "Chr",
        "Continue",
        "Declare",
        "Dim",
        "Do",
        "Double",
        "Else",
        "Elseif",
        "End",
        "Exit",
        "False",
        "For",
        "Function",
        "If",
        "Input",
        "Integer",
        "Length",
        "Loop",
        "Next",
        "Not",
        "Or",
        "Print",
        "Return",
        "Scope",
        "Shared",
        "Static",
        "String",
        "SubStr",
        "Then",
        "True",
        "While"
    };
    for(int i=0;i<=kw_while;i++)
    {
        printf("Test keywords %d: %s ",i,test_keywords[i]);
        if(is_keyword(test_keywords[i]) == i)
            printf("Success\n");
        else
            printf("Wrong\n");
    }
    char * not_keywords[] = {
        "tak","co","funguje","to","dobre"
    };
    for(int j=0;j<5;j++)
    {
        printf("Test !keywords %d: %s ",j,not_keywords[j]);
        if(is_keyword(not_keywords[j]) == -1)
            printf("Success\n");
        else
            printf("Wrong\n");
    }
    // Destruct token
    destruct_token(operator_token);
    destruct_token(integer_token);
    destruct_token(double_token);
    destruct_token(keyword_token);
    destruct_token(id_token);
    destruct_token(string_token);
    free(string);
    printf("\n\n");
    return 0;
}