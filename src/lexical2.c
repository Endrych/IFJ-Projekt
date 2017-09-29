#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "token.h"
#include "lexical2.h"

typedef enum{
    _START,
    _LINE_COMMENT,
    _BLOCK_COMMENT,//USED IN SLASH
    _BLOCK_COMMENT_FINISHED, //
    _SLASH,
    _NUMBER,
    _NUM_DOUBLE_E, //USED IN _NUMBER
    _NUM_DOUBLE,  // 
    _EXCLAMATION,
    _START_STRING, //USED IN EXCLAMATION RENAME IN PICTURE
    _END_STRING, //
    _IDENTIFIER, 
    _ASSIGN,
    _ADD,
    _SUB,
    _MULTIPLY,
    _DIVISION_INT, // {\}
    _LESSER,
    _LESSER_EQUAL, //USED IN LESSER
    _EQUAL, //
    _GREATER,
    _GREATER_EQUAL, //USED IN GREATER
    _BRACKET,
    _BRACKET_END,
    _EOF
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
    int state = _START;
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
            case _START:
                if(current_char == '\''){
                    state = _LINE_COMMENT;
                }
                else if(current_char == '/'){
                    state = _SLASH;                    
                }
                else if(current_char >= 0 && current_char <= 9){
                    state = _NUMBER;
                }
                else if(current_char == '!'){
                    state = _EXCLAMATION;
                }
                else if((current_char <= 'a' && current_char >= 'z') ||
                 (current_char <= 'A' && current_char >= 'Z')){
                    state = _IDENTIFIER;
                 }
                 else if(current_char == '='){
                     state = _ASSIGN;
                 }
                 else if(current_char == '+'){
                     state = _ADD;
                 }
                 else if(current_char == '-'){
                     state = _SUB;
                 }
                 else if(current_char == '*'){
                     state = _MULTIPLY;
                 }
                 else if(current_char == '\\'){
                     state = _DIVISION_INT;
                 }
                 else if(current_char == '<'){
                     state = _LESSER;
                 }
                 else if(current_char == '>'){
                     state = _GREATER;
                 }
                 else if (current_char == '('){
                     state = _BRACKET; 
                 }
                 else if (current_char == ')'){
                     state = _BRACKET_END;
                 }
                 else if (current_char == EOF){
                     state = _EOF;
                     isIntToken = false;
                 }
                 break;
            default:
                 state = _START;
        }

    }

}