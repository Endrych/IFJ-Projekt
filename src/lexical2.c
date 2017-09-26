#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "token.h"
#include "lexical2.h"

typedef enum{
    _START,
    _LINE_COMMENT,
    _BLOCK_COMMENT,
    _BLOCK_COMMENT_FINISHED,
    _SLASH,
    _NUMBER,
    _NUM_DOUBLE_E,
    _NUM_DOUBLE,
    _EXCLAMATION,
    _QUOTATION,
    _END_STRING,
    _IDENTIFIER,
    _ASSIGN,
    _ADD,
    _SUB,
    _MULTIPLY,
    _DIVISION_INT, // {\}
    _LESSER,
    _LESSER_EQUAL,
    _EQUAL,
    _GREATER,
    _GREATER_EQUAL,
}_State;

FILE* source_file = NULL;

void load_file(char *file){
    FILE *source;
    source = fopen(file, "r");
    if(source == NULL){
        printf("ERROR");
    }else{
        printf("SUCESS");
    }

    source_file = source;
}

Token* get_token(){
    bool isIntToken = true;
    static char last_char;
    int state = 0;
    int length = 0;
    char current_char = '\0';

    while(isIntToken){
        if(last_char != '\0'){
            current_char = last_char;
            last_char = '\0';
        }else{
            current_char = fgetc(source_file);
        }
        switch(state){
            case 0:

                isIntToken = false;
        }
    }

}