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
#include "error.h"

typedef enum{
	_START,
	_LINE_COMMENT,
	_BLOCK_COMMENT,//USED IN SLASH
	_SLASH,
	_NUMBER,
	_NUM_DOUBLE,  //
	_EXCLAMATION,
	_START_STRING, //USED IN EXCLAMATION RENAME IN PICTURE
	_END_STRING, //
	_IDENTIFIER,
	_OPERATOR,
	_LESSER,
	_LESSER_EQUAL, //USED IN LESSER
	_NOT_EQUAL, //
	_GREATER,
	_GREATER_EQUAL, //USED IN GREATER
	_EOF,
	_EOL,
	_COMMA,
	_SEMICOLON,
	_END_BLOCK_COMMENT
}_State;

Token* get_token(){
	bool isIntToken = true;
	Token* token = create_token();
	if(token == NULL){
		fprintf(stderr, "%s\n", COMPILER_MESSAGE);
        dispose_global();
	}
	static char last_char;
	static char last_string_char;
	char escape_seq[4];
	int esc_seq_iter = 0;
	int str_to_int;
	int state = _START;
	int length = 0;
	int size = 10;  //udelat konstantu
	char *str;
	char current_char = '\0';
	char lowering;
	bool is_last_minus = false;
	bool is_last_plus = false;
	bool is_dot_last = false;
	bool e_present = false;
	bool dot_present = false;
	bool e_last_char = false;
	bool esc_active = false;
	bool first_is_0 = false;
	bool second_is_0 = false;
	bool first_is_2 = false;
	bool second_is_5 = false;
	bool string_end = false;
	bool _tab = false;
	bool _new_line =false;
	bool _quotation_mark = false;
	bool _last_backslash = false;
	bool _backslash = false;
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
				else if(current_char == '=' || current_char == '+' || current_char == '-' ||
				current_char == '*' || current_char == '\\' || current_char == '(' || 
				current_char == ')'){
					state = _OPERATOR,
					last_char = current_char;
				}
				else if(current_char == '<'){
					state = _LESSER;
				}
				else if(current_char == '>'){
					state = _GREATER;
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
					state = _EOL;
					last_char = current_char;
				}
				else if(current_char == ';'){
					state = _SEMICOLON;
					last_char = current_char;
				}else if(current_char == ','){
					state = _COMMA;
					last_char = current_char;
				}
				else{
					fprintf(stderr,"Character | %c | is not allowed\n", current_char);
					free(token);
					print_error(LEXICAL_ERROR);
				}
				break;
			case _OPERATOR:
				if(current_char == '='){
					token->type = type_operator;
					token->atribute.operator_value = op_assign;
					return token;
				}
				else if (current_char == '+'){
					token->type = type_operator;
					token->atribute.operator_value = op_add;
					return token;
				}
				else if (current_char == '-'){
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
				else if(current_char == '('){
					token->type = type_operator;
					token->atribute.operator_value = op_bracket;
					return token;					
				}
				else if(current_char == ')'){
					token->type = type_operator;
					token->atribute.operator_value = op_bracket_end;
					return token;
				}
			case _EOL:
				token->type=type_eol;
				return token;
			case _SEMICOLON:
				token->type=type_semicolon;
				return token;
			case _COMMA:
				token->type=type_comma;
				return token;
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
				if(current_char == '\''){
					state = _END_BLOCK_COMMENT;
				}else if(current_char == EOF){
					free(token);
					print_error(LEXICAL_ERROR);
				}
				break;
			case _END_BLOCK_COMMENT:
				if(current_char == '/'){
					state = _START;
				}else if(current_char == EOF){
					free(token);
					print_error(LEXICAL_ERROR);
				}
				else{
					state = _BLOCK_COMMENT;
				}
				break;
			case _NUMBER:
				if(current_char == '\n' || current_char == ' ' ||
				current_char == EOF || current_char == '\t' ||
				current_char == '<' || current_char == '>' ||
				current_char == '=' || current_char == '+' ||
				current_char == '-' || current_char == '/' ||
				current_char == '\\' || current_char == '*' ||
				current_char == ';' || current_char == ','||
				current_char == ')' || current_char == '(' ||
				current_char == '\''){
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
					fprintf(stderr, "Error1: not a number\n");
 					free(token);
					free(str);
					print_error(LEXICAL_ERROR);
				}
				else if(current_char == '.' ||
				current_char == 'e' ||
				current_char == 'E'){
					if(current_char == '.'){
						dot_present = true;
						is_dot_last = true;
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
				!e_last_char) || current_char == '<' ||
				current_char == '>' || current_char == '=' ||
				current_char == '/' || current_char == '\\'
				|| current_char == '*' || current_char == ';' ||
				current_char == ','|| current_char == ')' ||
				current_char == '(' || current_char == '\''){
					if(is_dot_last || e_last_char || is_last_minus || is_last_plus){
						fprintf(stderr, "Error: Fractional-part is empty\n");
						free(token);
						free(str);
						print_error(LEXICAL_ERROR);
					}
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
					fprintf(stderr, "Error1: not a number\n");
					free(token);
					free(str);
					print_error(LEXICAL_ERROR);
				}else{
					if((current_char == 'E' || current_char == 'e')&& e_present){
						fprintf(stderr, "Error2: Not a number\n");
						free(token);
						free(str);
						print_error(LEXICAL_ERROR);
					}else if (current_char == '.' && dot_present){
						fprintf(stderr, "Error3: Not a number\n");
						free(token);
						free(str);
						print_error(LEXICAL_ERROR);
					}else if(current_char == '.' && e_present){
						fprintf(stderr, "Error4: Not a number\n");
						free(token);
						free(str);
						print_error(LEXICAL_ERROR);
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
					if(current_char == 'e' || current_char == 'E'){
						if(is_dot_last){
							fprintf(stderr, "Error: Exponent immediately afer dot\n");
							free(token);
							free(str);
							print_error(LEXICAL_ERROR);
						}
						e_present = true;
						e_last_char = true;
					}else{
						e_last_char = false;
					}
					if(current_char == '.'){
						is_dot_last = true;
					}else{
						is_dot_last = false;
					}
					if(current_char == '-'){
						is_last_minus = true;
					}else{
						is_last_minus = false;
					}
					if(current_char == '+'){
						is_last_plus = true;
					}else{
						is_last_plus = false;
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
					fprintf(stderr, "Problem with string inicialization\n");
					free(token);
					print_error(LEXICAL_ERROR);
				}
			case _START_STRING:
				if((current_char != '\"' && current_char != '\n' && current_char != EOF) || (current_char == '\"' && string_end == false && (esc_seq_iter > 0))){
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
					if(str_to_int == 92 || esc_active){
						esc_active = true;
						if((str_to_int >= 0 && str_to_int <= 32) ||
						str_to_int == 35){
							esc_active = false;
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
							else if(str_to_int == 48){
								first_is_0 = true;
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
							else if(str_to_int == 92){
								_backslash = true;
							}
							else if(str_to_int < 48 || str_to_int > 50){
								esc_active = false;
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
								}

							}
							else if(str_to_int < 48 || str_to_int > 57){
								esc_active = false;
							}
							else if(str_to_int == 48){
								second_is_0 = true;
							}
						}
						else if(esc_seq_iter == 3){
							escape_seq[3] = current_char;
							if(first_is_2 && second_is_5){
								if(str_to_int < 48 || str_to_int > 53){
								esc_active = false;
								}
							}
							else if(str_to_int < 48 || str_to_int > 57){
								esc_active = false;
							}
							else if(first_is_0 && second_is_0 && (str_to_int == 48)){
								fprintf(stderr,"Error: Unrecognized escape sequence \\000\n");
								free(token);
								free(str);
								print_error(LEXICAL_ERROR);
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
						else if(_new_line == true || _tab == true || _quotation_mark == true || _backslash == true){
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
							}else if(_backslash == true){
								str[length] = '\\';
								length++;
								str[length] = '0';
								length++;
								str[length] = '9';
								length++;
								str[length] = '2';
								length++;
							}
							_quotation_mark = false;
							_new_line = false;
							_tab = false;
							_backslash = false;
							esc_seq_iter = 0;
							esc_active = false;
							first_is_2 = false;
							second_is_5 = false;
						}
						else if(esc_active == false){
							fprintf(stderr,"Error: Unrecognized escape sequence\n");
							free(token);
							free(str);
							print_error(LEXICAL_ERROR);
						}
						if(esc_active){
							esc_seq_iter++;
						}
						break;
					}
					else if( str_to_int == 32) {
						str[length] = '\\';
						length++;
						str[length] = '0';
						length++;
						str[length] = '3';
						length++;
						str[length] = '2';
						length++;
						break;
					}else if(str_to_int >= 0 && str_to_int <= 31){
						fprintf(stderr,"Error: Forbidden element!\n");
						free(token);
						free(str);
						print_error(LEXICAL_ERROR);
					}
					else{
						str[length] = current_char;
						length++;
						break;
					}

				}else if(current_char == '\n' || current_char == EOF){
					fprintf(stderr,"Error: String wasnt ended properly\n");
					free(token);
					free(str);
					print_error(LEXICAL_ERROR);
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
				current_char == ';' || current_char == ',' ||
				current_char == '\''){
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
					if(kw != -1 && kw != 2 && kw != 4 && kw != 20 && kw != 31){
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
					fprintf(stderr, "Identifier obtains character which is not allowed: %c", current_char);
					free(token);
					free(str);
					print_error(LEXICAL_ERROR);
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
	free(token);
	free(str);
	print_error(LEXICAL_ERROR);
	return NULL;
}
