#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "token.h"
#include "lexical2.h"

typedef enum{
	_START,
  _LINE_COMMENT,
  _BLOCK_COMMENT,//USED IN SLASH
  _BLOCK_COMMENT_FINISHED, //mozna neni treba
  _SLASH,
  _NUMBER,
  //_NUM_DOUBLE_E, //USED IN _NUMBER NOT SURE IF NEEDED NOW
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
	}
	else{
  	printf("SUCESS");
  }

  source_file = source;
}

Token* get_token(){
  bool isIntToken = true;
	static char last_char;
	char prev_char;
  int state = _START;
  int length = 0;
  char current_char = '\0';

  while(isIntToken){
    if(last_char != '\0'){
      current_char = last_char;
      last_char = '\0';
		}
		else{
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
        else if(current_char >= '0' && current_char <= '9'){
					last_char = current_char;					
					state = _NUMBER;
        }
        else if(current_char == '!'){
          state = _EXCLAMATION;
        }
        else if((current_char >= 'a' && current_char <= 'z') ||
				(current_char >= 'A' && current_char <= 'Z') || 
				current_char == '_'){
        	state = _IDENTIFIER;
					last_char = current_char;
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
          // last_char = current_char;                    
          state = _LESSER;
        }
        else if(current_char == '>'){
          // last_char = current_char;                    
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
          last_char = current_char;                    
        }
				break;
			case _LINE_COMMENT:
				if(current_char == '\n'){
					state = _START;
				}
				else if(current_char == EOF){
					state = _EOF;
					last_char = current_char;
				}
				printf("%c", current_char);
				break;
			case _SLASH:
				if(current_char == '\''){
					state = _BLOCK_COMMENT;
				}
				else{
					//poslat token jedna se o deleni
					printf("Deleni");
					last_char = current_char;
					//konec
					state = _START;
				}
			case _BLOCK_COMMENT:
				if(current_char == '/' && prev_char == '\''){
					printf("\nFOUND\n");
					state = _START;
					prev_char = '\0';
					break;
				}
				prev_char = current_char;				
				break;
			case _NUMBER:
				if(!(current_char >= '0' && current_char <= '9')){
					state = _START; //posle token
					last_char = current_char;
					break;
				}
				else if(current_char == '.'){
					//pushnout do array pridat length
					state = _NUM_DOUBLE;
					break;
				}
				printf("%c", current_char);
				break;
			case _NUM_DOUBLE:
				if(!(current_char >= '0' && current_char <= '9')){
					state = _START; //posle token
					last_char = current_char;
					break;
				}
				printf("%c", current_char);
				break;
			case _EXCLAMATION:
				if(current_char == '\"'){
					state = _START_STRING;
					break;
				}			
				else{
					printf("ERROR");
				}	
				break;
			case _START_STRING:
				if(current_char != '\"'){
					//add to array
					printf("%c",current_char);
				}
				else{
					state = _END_STRING;
				}
				break;
			case _END_STRING:
				printf("\n");
				last_char = current_char;
				//return token
				state = _START;
				break;
			case _IDENTIFIER:
				if((current_char >= 'a' && current_char <= 'z') ||
				(current_char >= 'A' && current_char <= 'Z') ||
				(current_char >= '0' && current_char <= '9') || 
				current_char == '_'){
					printf("%c", current_char);
					//add to arr, length ++
				}
				else{
					last_char = current_char;
					state = _START;
					//odesleme token;
				}
				break;
			case _ASSIGN:
				last_char = current_char;
				//send token 
				printf("=");
				break;
			case _ADD:
				last_char = current_char;	
				//send token
				printf("+");
				state = _START;
				break;
			case _SUB:
				last_char = current_char;
				//send token 
				printf("-");
				state = _START;
				break;
			case _MULTIPLY:
				last_char = current_char;
				//send token 
				printf("*");
				state = _START;
				break;
			case _DIVISION_INT:
				last_char = current_char;
				//send token 
				printf("\\");
				state = _START;
				break;
			case _LESSER:
				if(current_char == '='){
					state = _LESSER_EQUAL;
				}
				else if(current_char == '>'){
					state = _EQUAL;
				}
				else{
					last_char = current_char;					
					//send token 
					state = _START;
					printf("<");
				}
				break;
			case _LESSER_EQUAL:
				//send token 
				last_char = current_char;
				state = _START;
				printf("<=");
				break;
			case _EQUAL:
				//send token 
				last_char = current_char;
				state = _START;
				printf("<>");
				break;
			case _GREATER:
				if(current_char == '='){
					state = _GREATER_EQUAL;
				}
				else{
					last_char = current_char;
					//send token 
					state = _START;
					printf(">");
				}
				break;
			case _GREATER_EQUAL:
				//send token 
				last_char = current_char;
				state = _START;
				printf(">=");
				break;
			case _BRACKET:
				//send token
				last_char = current_char;
				state = _START;
				printf("(");
				break;
			case _BRACKET_END:
				//send token
				last_char = current_char;
				state = _START;
				printf(")");
				break;				
			case _EOF:
				isIntToken = false;
			default:
				state = _START;
		}
  }
}