#include <stdio.h>
#include "../src/lexical.h"
#include "../src/token.h"
#include "../src/string_storage.h"
int main(){
    Token *token;
    char * str;
    while(1){
        token = get_token();
        // str = get_string();
        fprintf(stdout,"%g\n",token->atribute.double_value);
    }
}