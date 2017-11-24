#include <stdio.h>
#include "../src/lexical.h"
#include "../src/token.h"
#include "../src/string_storage.h"
int main(){
    Token *token;
    char * str;
    while(1){
        token = get_token();
        str = get_string(token->atribute.int_value);
        fprintf(stdout,"%s\n",str);
        destruct_token(token);
    }
}