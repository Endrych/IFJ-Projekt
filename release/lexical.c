/* Nazev - Implementace prekladace imperativniho jazyka IFJ17
 * Petr Zubalik - xzubal04
 * Marek Kukucka - xkukuc04
 * Jan Koci - xkocij01
 * David Endrych - xendry02
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include "token.h"
#include "string_storage.h"
#include "destructor.h"

//TODO: TESTS

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

Token* get_token(){
	bool isIntToken = true;
	bool is_not_esc = false;
	bool esc_active = false;
	Token* token = create_token();
	if(token == NULL){
		fprintf(stderr, "%s\n", COMPILER_MESSAGE);
        dispose_global();
	}
	static char last_char;
	static char last_string_char;
	char prev_char;
	char int_to_str_1[1]; // pro escape sekvence vzdycky dostanu jedno cislo!	
	char int_to_str_2[2]; // pro escape sekvence vzdycky dostanu dve cisla!
	char escape_seq[4];
	int esc_seq_iter = 0;
	int str_to_int;
	int state = _START;
	int length = 0;
	int size = 10;  //udelat konstantu
	char *str;
	char current_char = '\0';
	char lowering;					
	bool e_present,dot_present = false;
	bool e_last_char = false;
	bool first_is_2 = false;
	bool second_is_5 = false;
	bool string_end = false;
	bool _tab = false;
	bool _new_line =false;
	bool _quotation_mark = false;
	bool _last_backslash = false;
	// bool _
	while(isIntToken){
    	if(last_char != '\0'){
    	  	current_char = last_char;
      		last_char = '\0';
		}
		else{
			current_char = getchar();
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
					if (str == NULL)
					{
						fprintf(stderr, "%s\n", COMPILER_MESSAGE);
       					dispose_global();
					}		
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
					if (str == NULL)
					{
						fprintf(stderr, "%s\n", COMPILER_MESSAGE);
        				dispose_global();
					}					
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
				}else if(current_char == ','){
					token->type=type_comma;
					return token;
				}
				else{
					fprintf(stderr,"Character | %c | is not allowed\n", current_char);
					token->type=type_wrong;
					return token;
				}
				break;
			case _LINE_COMMENT:
				if(current_char == '\n'){
					state = _START;
					last_char = current_char;
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
				if(current_char == '\n' || current_char == ' ' ||
				current_char == EOF || current_char == '\t' ||
				current_char == '<' || current_char == '>' ||
				current_char == '=' || current_char == '+' ||
				current_char == '-' || current_char == '/' ||
				current_char == '\\' || current_char == '*' ||
				current_char == ';' || current_char == ','||
				current_char == ')' || current_char == '('){
					int convert;
					convert = atoi(str);
					state = _START;
					token->type = type_integer;
					token->atribute.int_value = convert; //add check for max
					last_char = current_char;
					free(str);
					return token;
				}else if((!(current_char >= '0' && current_char <= '9'))&&
				(current_char != '.' && current_char != 'e' && 
				current_char != 'E')){
					token->type = type_wrong;
					fprintf(stderr, "Error: not a number\n");
					free(str);
					return token;
				}
				else if(current_char == '.' || 
				current_char == 'e' ||
				current_char == 'E'){
					if(current_char == '.'){
						dot_present = true;
					}else if(current_char == 'e' || current_char == 'E'){
						e_present = true;
						e_last_char = true;
					}
					if(length == size){
	           			size += 10;
    	        		str = (char *)realloc(str, size*sizeof(char));
						if (str == NULL)
						{
							fprintf(stderr, "%s\n", COMPILER_MESSAGE);
        					dispose_global();
						}		
				  	}		
					str[length] = current_char;
					length++;
					state = _NUM_DOUBLE;
					break;
				}
				else{
					if(length == size){
        				size += 10;
            			str = (char *)realloc(str, size*sizeof(char));
						if (str == NULL)
						{
							fprintf(stderr, "%s\n", COMPILER_MESSAGE);
        					dispose_global();
						}		
					}
					str[length] = current_char;
					length++;
        			break;
				}
				break;
			case _NUM_DOUBLE:
				if(current_char == '\n' || current_char == ' ' ||
				current_char == EOF || current_char == '\t' ||
				((current_char == '+' || current_char == '-') &&
				e_last_char) || current_char == '<' || 
				current_char == '>' || current_char == '=' || 
				current_char == '/' || current_char == '\\' 
				|| current_char == '*' || current_char == ';' ||
				current_char == ','|| current_char == ')' ||
				current_char == '('){
					double convert;
					convert = strtof(str, NULL);  // Zkusit strtof
					token->type = type_double;
					token->atribute.double_value = convert; //add check for max
					last_char = current_char;
					free(str);
					return token;
				}else if((!(current_char >= '0' && current_char <= '9'))&&
				(current_char != '.' && current_char != 'e' && 
				current_char != 'E') && current_char != '+' &&
				current_char != '-'){
					token->type = type_wrong;
					fprintf(stderr, "Error1: not a number\n");
					free(str);
					return token;
				}else{
					if((current_char == 'E' || current_char == 'e')&&
					e_present){
						fprintf(stderr, "Error2: Not a number\n");
						token->type = type_wrong;
						free(str);
						return token;
					}else if (current_char == '.' && dot_present){
						fprintf(stderr, "Error3: Not a number\n");
						token->type = type_wrong;
						free(str);
						return token;
					}else if(current_char == '.' && e_present){
						fprintf(stderr, "Error4: Not a number\n");
						token->type = type_wrong;
						free(str);
						return token;
					}
					if(length == size){
           				size += 10;
            			str = (char *)realloc(str, size*sizeof(char));
						if (str == NULL)
						{
							fprintf(stderr, "%s\n", COMPILER_MESSAGE);
        					dispose_global();
						}		
					}
					str[length] = current_char;
					length++;
					if(e_last_char){
						e_last_char = false;
					}
					break;
				}
			case _EXCLAMATION:
				if(current_char == '\"'){
					str=(char*)calloc(size,sizeof(char));	
					if (str == NULL)
					{
						fprintf(stderr, "%s\n", COMPILER_MESSAGE);
       					dispose_global();
					}						
					state = _START_STRING;
					break;
				}			
				else{
					token->type = type_wrong;
					fprintf(stderr, "Problem with string inicialization\n");
					// free(str);
					return token;
				}	
			case _START_STRING:
				if((current_char != '\"' && current_char != '\n') || (current_char == '\"' && string_end == false && (esc_seq_iter > 0))){
					// add to array
					if(last_string_char == '\\'){
						_last_backslash = true;
					}else{
						_last_backslash = false;
					}
					last_string_char = current_char;
					if(current_char == '\"' && !_last_backslash){
						string_end = true;
						state = _END_STRING;
						last_char = current_char;
					}
					if((length + 3) >= size){
            			size += 10;
            			str = (char *)realloc(str, size*sizeof(char));
						if (str == NULL)
						{
							fprintf(stderr, "%s\n", COMPILER_MESSAGE);
       						dispose_global();
						}		
					}
					str_to_int = (int)current_char;
					if(string_end && esc_seq_iter > 0){
						int i = 0;
						if(esc_seq_iter == 1){
							str[length] = '\\';
							length++;
							str[length] = '0';
							length++;
							str[length] = '9';
							length++;
							str[length] = '2';
							length++;
							i++;
						}else{
							while(i <= esc_seq_iter){
								if((length + 3) >= size){
									size += 10;
									str = (char *)realloc(str, size*sizeof(char));
									if (str == NULL)
									{
										fprintf(stderr, "%s\n", COMPILER_MESSAGE);
										dispose_global();
									}		
								}
								if(i == 0){
									str[length] = '\\';
									length++;
									str[length] = '0';
									length++;
									str[length] = '9';
									length++;
									str[length] = '2';
									length++;
									i++;								
								}else{
									str[length] = escape_seq[i];
									length++;
									i++;
								}
							}
						}
						break;
					}
					else if((str_to_int == 92 && (is_not_esc == false && esc_active == true)) || is_not_esc == true){
						int i = 0;
						first_is_2 = false;
						second_is_5 = false;
						if(((str_to_int >= 0 && str_to_int <= 32) || 
						str_to_int == 35 || str_to_int == 92) && esc_seq_iter == 1){
							if((length + 3) >= size){
								size += 10;
								str = (char *)realloc(str, size*sizeof(char));
								if (str == NULL)
								{
									fprintf(stderr, "%s\n", COMPILER_MESSAGE);
									dispose_global();
								}		
							}
							str[length] = '\\';
							length++;
							str[length] = '0';
							length++;
							str[length] = '9';
							length++;
							str[length] = '2';
							length++;
						}
						else{
							while(i <= esc_seq_iter){
								if((length + 3) >= size){
									size += 10;
									str = (char *)realloc(str, size*sizeof(char));
									if (str == NULL)
									{
										fprintf(stderr, "%s\n", COMPILER_MESSAGE);
										dispose_global();
									}		
								}
								str_to_int=(int)escape_seq[i];
								if(i == 0){
									str[length] = '\\';
									length++;
									str[length] = '0';
									length++;
									str[length] = '9';
									length++;
									str[length] = '2';
									length++;
									i++;								
								}else{
									str[length] = escape_seq[i];
									length++;
									i++;
								}
							}
						}
						if((str_to_int >= 0 && str_to_int <= 32) || 
						str_to_int == 35){
							last_char = current_char;
							esc_seq_iter = 0;
						}
						else if(current_char != '\\'){
							if((length + 3) >= size){
            					size += 10;
            					str = (char *)realloc(str, size*sizeof(char));
								if (str == NULL)
								{
									fprintf(stderr, "%s\n", COMPILER_MESSAGE);
       								dispose_global();
								}		
							}
							str[length] = current_char;
							length++;
							esc_seq_iter = 0;
						}else{
							// escape_seq[0] = current_char;
							// esc_seq_iter = 1;
							esc_seq_iter = 0;

						}
						is_not_esc = false;
						esc_active = false;
						break;
					}
					else if(str_to_int == 92 || esc_active){
						esc_active = true;
						if((str_to_int >= 0 && str_to_int <= 32) || 
						str_to_int == 35){
							esc_active = false;
							is_not_esc = true;
							last_char = current_char;
							break;
						}
						else if(esc_seq_iter == 0){
							escape_seq[0] = current_char;
						}
						else if(esc_seq_iter == 1){
							escape_seq[1] = current_char;
							if(str_to_int == 50){
								first_is_2 = true;
							}
							else if(str_to_int == 110){
								_new_line = true;
							}
							else if(str_to_int == 34){
								_quotation_mark = true;
							}
							else if(str_to_int == 116){
								_tab = true;
							}
							else if(str_to_int < 48 || str_to_int > 50){
								esc_active = false;
								is_not_esc = true;								
							}
						}
						else if(esc_seq_iter == 2){
							escape_seq[2] = current_char;
							if(first_is_2){
								if(str_to_int == 53){
									second_is_5 = true;
								}
								if(str_to_int < 48 || str_to_int > 53){
									esc_active = false;
									is_not_esc = true;								
								}
							}
							else if(str_to_int < 48 || str_to_int > 57){
								esc_active = false;
								is_not_esc = true;								
							}
						}
						else if(esc_seq_iter == 3){
							escape_seq[3] = current_char;
							if(first_is_2 && second_is_5){
								if(str_to_int < 48 || str_to_int > 53){
								esc_active = false;
								is_not_esc = true;
								}
							}
							else if(str_to_int < 48 || str_to_int > 57){
								esc_active = false;
								is_not_esc = true;
							}
						}
						if(esc_seq_iter == 3 && esc_active){
							int i = 0;
							while(i <= esc_seq_iter){
								if((length + 3) >= size){
            						size += 10;
            						str = (char *)realloc(str, size*sizeof(char));
									if (str == NULL)
									{
										fprintf(stderr, "%s\n", COMPILER_MESSAGE);
       									dispose_global();
									}		
								}
								str[length] = escape_seq[i];
								length++;
								i++;
							}
							esc_seq_iter = 0;
							esc_active = false;		
							first_is_2 = false;
							second_is_5 = false;					
						}
						else if(_new_line == true || _tab == true || _quotation_mark == true){
							if(_new_line == true){
								str[length] = '\\';
								length++;
								str[length] = '0';
								length++;
								str[length] = '1';
								length++;
								str[length] = '0';
								length++;
							}else if (_tab == true){
								str[length] = '\\';
								length++;
								str[length] = '0';
								length++;
								str[length] = '0';
								length++;
								str[length] = '9';
								length++;
							}else if(_quotation_mark == true){
								str[length] = '\\';
								length++;
								str[length] = '0';
								length++;
								str[length] = '3';
								length++;
								str[length] = '4';
								length++;
							}
							_quotation_mark = false;
							_new_line = false;
							_tab = false;
							esc_seq_iter = 0;
							esc_active = false;		
							first_is_2 = false;
							second_is_5 = false;	
						}
						if(esc_active){
							esc_seq_iter++;
						}
						break;
					}
					else if((str_to_int >= 10 && str_to_int <= 32) || 
					str_to_int == 35){
						sprintf(int_to_str_2, "%d", str_to_int);						
						str[length] = '\\';
						length++;
						str[length] = '0';
						length++;
						str[length] = int_to_str_2[0];
						length++;
						str[length] = int_to_str_2[1];
						length++;
						break;
					}else if(str_to_int >= 0 && str_to_int <= 9){
						sprintf(int_to_str_1, "%d", str_to_int);						
						str[length] = '\\';
						length++;
						str[length] = '0';
						length++;
						str[length] = '0';
						length++;
						str[length] = int_to_str_1[0];
						length++;
						break;
					}
					else{
						str[length] = current_char;
						length++;
						break;
					}
					
				}else if(current_char == '\n'){
					fprintf(stderr,"Error: String wasnt ended properly\n");
					free(str);
					token->type = type_wrong;
					return token;
				}else{
					last_char = current_char;
					state = _END_STRING;
					break;
				}
			case _END_STRING:
				if(length == size){
					size += 10;
					str = (char *)realloc(str, size*sizeof(char));
					if (str == NULL)
					{
						fprintf(stderr, "%s\n", COMPILER_MESSAGE);
        				dispose_global();
					}		
				}
				str[length] = '\0';
				length++;
				int adress = add_string_to_storage(str);
				token->type = type_string;
				token->atribute.int_value = adress;
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
						if (str == NULL)
						{
							fprintf(stderr, "%s\n", COMPILER_MESSAGE);
        					dispose_global();
						}		
					}
					lowering = tolower(current_char);
					str[length] = lowering;
					length++;
					break;
				}else if(current_char == '\n' || current_char == ' ' ||
				current_char == EOF || current_char == '\t' || 
				current_char == '<' || current_char == '>' ||
				current_char == '=' || current_char == '/' ||
				current_char == '\\' || current_char == '*' ||
				current_char == '+' || current_char == '-' ||
				current_char == ')' || current_char == '(' ||
				current_char == ';' || current_char == ','){
					if(current_char == '\n'){
						last_char = '\n';
					}else if(current_char == EOF){
						last_char = EOF;
					}
					if(length == size){
						size += 10;
						str = (char *)realloc(str, size*sizeof(char));
						if (str == NULL)
						{
							fprintf(stderr, "%s\n", COMPILER_MESSAGE);
        					dispose_global();
						}		
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
						token->type = type_id;
						token->atribute.int_value = adress;
					}
					last_char = current_char;
					free(str);
					return token;
				}else{
					token->type = type_wrong;
					fprintf(stderr, "Identifier obtains character which is not allowed: %c", current_char);
					free(str);
					return token;
				}
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
				token->type = type_eof;				
				return token;
		}
	}
	fprintf(stderr, "Something went wrong during lexical analyzation");
	token->type = type_wrong;
	return token; //mozna neco jinyho sem dat uvidime
}