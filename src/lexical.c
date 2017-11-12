#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include "token.h"
#include "string_storage.h"

//TODO: CASE INSENSITIVE ?
//TODO: ERRORS
//TODO: TESTS
//TODO: NUMBER CHECK - kontrola jestli se dobre prevedlo cislo
//TODO: co kdyz narazi na EOF?
//TODO: co kdyz dostanu jiny znak napriklad ?

typedef enum{
	_START,
  _LINE_COMMENT,
  _BLOCK_COMMENT,//USED IN SLASH
  _BLOCK_COMMENT_FINISHED, //mozna neni treba
  _SLASH,
  _NUMBER,
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
  _NOT_EQUAL, //
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
		printf("FILE ERROR");
		return;
	}

  source_file = source;
}

void close_file(){
	fclose(source_file);
}

Token* get_token(){
	bool isIntToken = true;
	Token* token = create_token();
	static char last_char;
	char prev_char;
  int state = _START;
	int length = 0;
	int size = 10;  //udelat konstantu
	char *str;
	char current_char = '\0';
	char lowering;					
	
	
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
					str=(char*)calloc(size,sizeof(char));
					state = _NUMBER;
        }
        else if(current_char == '!'){
          state = _EXCLAMATION;
        }
        else if((current_char >= 'a' && current_char <= 'z') ||
				(current_char >= 'A' && current_char <= 'Z') || 
				current_char == '_'){
					state = _IDENTIFIER;
					str=(char*)calloc(size,sizeof(char));					
					last_char = current_char;
				}
        else if(current_char == '='){
          token->type = type_operator;
					token->atribute.operator_value = op_assign;
					return token;
        }
        else if(current_char == '+'){
          token->type = type_operator;
					token->atribute.operator_value = op_add;
					return token;
        }
        else if(current_char == '-'){
          token->type = type_operator;
					token->atribute.operator_value = op_sub;
					return token;
        }
        else if(current_char == '*'){
					token->type = type_operator;
					token->atribute.operator_value = op_mul;
					return token;
        }
        else if(current_char == '\\'){
          token->type = type_operator;
					token->atribute.operator_value = op_division_int;
					return token;
        }
        else if(current_char == '<'){
          state = _LESSER;
        }
        else if(current_char == '>'){
          state = _GREATER;
        }
    	  else if (current_char == '('){
					token->type = type_operator;
					token->atribute.operator_value = op_bracket;
					return token; 
        }
        else if (current_char == ')'){
					token->type = type_operator;
					token->atribute.operator_value = op_bracket_end;
					return token;	
        }
        else if (current_char == EOF){
					state = _EOF;
					isIntToken = false;					
          last_char = current_char;                    
				}
				else if(current_char == ' ' || 
				current_char == '\t'){
					state = _START;
				}
				else if(current_char == '\n'){
					token->type=type_eol;
					return token;
				}
				else if(current_char == ';'){
					token->type=type_semicolon;
					return token;
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
				break;
			case _SLASH:
				if(current_char == '\''){
					state = _BLOCK_COMMENT;
				}
				else{
					token->type = type_operator;
					token->atribute.operator_value = op_slash;
					last_char = current_char;
					return token;
				}
			case _BLOCK_COMMENT:
				if(current_char == '/' && prev_char == '\''){
					state = _START;
					prev_char = '\0';
					break;
				}
				prev_char = current_char;				
				break;
			case _NUMBER:
				if(!((current_char >= '0' && current_char <= '9')|| 
				current_char == '.' || 
				current_char == 'e')){
					int convert;
					convert = atoi(str);
					state = _START;
					token->type = type_integer;
					token->atribute.int_value = convert; //add check for max
					last_char = current_char;
					free(str);
					return token;
				}
				else if(current_char == '.' || 
				current_char == 'e' ||
				current_char == 'E'){
					if(length == size){
            size += 10;
            str = (char *)realloc(str, size*sizeof(char));
          }
					str[length] = current_char;
					length++;
					// bool isExponent = false;
					// bool isPoint = false;

					state = _NUM_DOUBLE;
					break;
				}
				else{
					if(length == size){
            size += 10;
            str = (char *)realloc(str, size*sizeof(char));
          }
					str[length] = current_char;
					length++;
          break;
				}
				break;
			case _NUM_DOUBLE:
				if(!((current_char >= '0' && current_char <= '9')||
				(current_char == '+' || current_char == '-' ||
				 current_char == '.' || current_char == 'e'
				|| current_char == 'E'))){
					double convert;
					convert = atof(str);
					token->type = type_double;
					token->atribute.double_value = convert; //add check for max
					last_char = current_char;
					free(str);
					return token;
				}
				else{
					if(length == size){
            size += 10;
            str = (char *)realloc(str, size*sizeof(char));
					}
					str[length] = current_char;
					length++;
					break;
				}
			case _EXCLAMATION:
				if(current_char == '\"'){
					str=(char*)calloc(size,sizeof(char));					
					state = _START_STRING;
					break;
				}			
				else{
					printf("ERROR");
				}	
				break;
			case _START_STRING:
				if(current_char != '\"'){
					// add to array
					if(length == size){
            size += 10;
            str = (char *)realloc(str, size*sizeof(char));
					}
					str[length] = current_char;
					length++;
					break;
				}
				else{
					state = _END_STRING;
				}
				break;
			case _END_STRING:
				if(length == size){
					size += 10;
					str = (char *)realloc(str, size*sizeof(char));
				}
				str[length] = '\0';
				length++;
				int adress = add_string_to_storage(str);
				// printf("Adresa:%d ,",adress);_NOT_EQUAL
				token->type = type_string;
				token->atribute.int_value = adress;
				last_char = current_char;
				free(str);
				return token;
			case _IDENTIFIER:
				if((current_char >= 'a' && current_char <= 'z') ||
				(current_char >= 'A' && current_char <= 'Z') ||
				(current_char >= '0' && current_char <= '9') || 
				current_char == '_'){
					if(length == size){
						size += 10;
						str = (char *)realloc(str, size*sizeof(char));
					}
					lowering = tolower(current_char);
					str[length] = lowering;
					length++;
				}
				else{
					if(length == size){
						size += 10;
						str = (char *)realloc(str, size*sizeof(char));
					}
					str[length] = '\0';
					length++;
					int kw;
					kw = is_keyword(str);
					if(kw != -1){
						token->type=type_keyword;
						token->atribute.keyword_value = kw;
					}
					else{
						int adress = add_string_to_storage(str);
						// printf("Adresa:%d ,",adress);
						token->type = type_id;
						token->atribute.int_value = adress;
					}
					last_char = current_char;
					free(str);
					return token;
				}
				break;
			case _LESSER:
				if(current_char == '='){
					state = _LESSER_EQUAL;
				}
				else if(current_char == '>'){
					state = _NOT_EQUAL;
				}
				else{
					token->type = type_operator;
					token->atribute.operator_value = op_lesser;
					last_char = current_char;
					return token;
				}
				break;
			case _LESSER_EQUAL:
				token->type = type_operator;
				token->atribute.operator_value = op_lesser_equal;
				last_char = current_char;
				return token;
			case _NOT_EQUAL:
				token->type = type_operator;
				token->atribute.operator_value = op_not_equal;
				last_char = current_char;
				return token;
			case _GREATER:
				if(current_char == '='){
					state = _GREATER_EQUAL;
				}
				else{
					token->type = type_operator;
					token->atribute.operator_value = op_greater;
					last_char = current_char;
					return token;
				}
				break;
			case _GREATER_EQUAL:
				token->type = type_operator;
				token->atribute.operator_value = op_greater_equal;
				last_char = current_char;
				return token;
			case _EOF:
				isIntToken = false;
				return;
		}
	}
	return 0; //mozna neco jinyho sem dat uvidime
}