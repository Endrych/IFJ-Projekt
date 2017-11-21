/* IFJ project
 * @authors:
 * @date:
 * @brief:
 * 
 TODO: 
 	- error.h 
*/

#include <stdbool.h>
#include <stdio.h>
#include "parser.h"
#include "lexical.h"
#include "token.h"
#include "error.h"
#include "symtable.h"
#include "abstract_tree.h"
#include "precedence_sa.h"
#include "set_values.h"

// tabulka symbolu
//Tsymtab *sym_tab;
// aktualni token
Token* token;
extern Tsymtab * symtab;

void get_non_eol_token()
{
	do {
		token = get_token();
	} while (token->type == type_eol);
}

int Prog()
{
	int return_value;

	get_non_eol_token();
	switch (token->type)
	{
		case type_keyword:
			if (token->atribute.int_value == kw_scope)
			{
				// __<scope>__
				return_value = Scope();
				if (return_value != OK)
				{
					return return_value;
				}
				return_value = Func();
				return return_value;
			}
			else if (token->atribute.int_value == kw_declare ||
					token->atribute.int_value == kw_function)
			{
				return_value = Func();
				if (return_value != OK)
				{
					return return_value;
				}
				return_value = Prog();
				return return_value;
			}
			else
			{
				printf("ERROR: Missing Scope or function definition/declaration at the begining of program\n");
				return SYNTAX_ERROR;
			}
			break;
		default:
			printf("ERROR: Missing Scope or function definition/declaration at the begining of program\n");
			return SYNTAX_ERROR;
	}
}

int Scope()
{
	int return_value;

	// __SCOPE__
	if (token->type != type_keyword)
	{
		printf("ERROR: Missing 'Scope' definition\n");
		return SYNTAX_ERROR;
	}

	if (token->atribute.int_value != kw_scope)
	{
		printf("ERROR: Missing 'Scope' definition\n");
		return SYNTAX_ERROR;
	}
	
	// __EOL__
	token = get_token();
	if (token->type != type_eol)
	{
		printf("ERROR: Missing end of line after Scope keyword\n");
		return SYNTAX_ERROR;
	}

	// __<st-list>__
	get_non_eol_token();
	if ((return_value = St_list()) != OK)
	{
		return return_value;
	}
	// __End__ (token mne pripravil <st-list>)
	if (token->type != type_keyword)
	{
		printf("ERROR: Missing 'End Scope' at the end of Scope definition\n");
		return SYNTAX_ERROR;
	}
	if (token->atribute.int_value != kw_end)
	{
		printf("ERROR: Missing 'End Scope' at the end of Scope definition\n");
		return SYNTAX_ERROR;
	}

	// __SCOPE__
	token = get_token();
	if (token->type != type_keyword)
	{
		printf("ERROR: Missing 'End Scope' at the end of Scope definition\n");
		return SYNTAX_ERROR;
	}
	if (token->atribute.int_value != kw_scope)
	{
		printf("ERROR: Missing 'End Scope' at the end of Scope definition\n");
		return SYNTAX_ERROR;
	}
	return return_value;
}

int St_list()
{
	int return_value;

	switch (token->type)
	{
		case type_keyword:
			switch (token->atribute.int_value)
			{
				case kw_dim:
				case kw_print:
				case kw_input:
				case kw_do:
				case kw_if:
				case kw_return:
					// __<stat>__
					if ((return_value = Stat()) != OK) {
						return return_value;
					}
					// __EOL__
					if (token->type != type_eol) {
						token = get_token();
					}
					if (token->type != type_eol) {
						printf("ERROR: Missing end of line after statement\n");
						return SYNTAX_ERROR;
					}
					// __<st-list>__
					get_non_eol_token();
					return St_list();
				break;

				case kw_end:
				case kw_loop:
				case kw_then:
				case kw_else:
					// __EPSILON__
					return OK;
				break;
			}
		break;
		
		case type_id:
			// __<stat>__
			if ((return_value = Stat()) != OK) {
				return return_value;
			}
			// __EOL__
			if (token->type != type_eol) {
				token = get_token();
			}
			if (token->type != type_eol) {
				printf("ERROR: Missing end of line after statement\n");
				return SYNTAX_ERROR;
			}
			// __<st-list>__
			get_non_eol_token();
			return St_list();
		break;

		default:
			printf("ERROR: Wrong statement\n");
			return SYNTAX_ERROR;
	}
	return OK;
}

int Stat()
{
	int return_value;
	Tsymtab_item *symtab_item;
	PrecendentOutput* out;
	ATData data;
	ATLeaf *leaf;
	ATLeaf *tree;
	Tsymtab* local_symtab;

	switch (token->type)
	{
		case type_keyword:
			switch (token->atribute.int_value)
			{
				case kw_input:
					// __Input id__
					// JAK GENEROVAT ? <<<<<<<<<<<<<<<
					token = get_token();
					if (token->type != type_id)
					{
						printf("ERROR: Missing identifier after Intup statement\n");
						return SYNTAX_ERROR;
					}
					symtab_item = symtab_search(symtab, token, type_variable);
					if (symtab_item == NULL) {
						fprintf(stderr, "ERROR: Identifier used in Input statement was not declared.\n");
						return SEMANTIC_ERROR;
					}
					// PROVED AKCI <<<<<<<<<<<<<<<<<<<<<
				break;

				case kw_dim:
				// __Dim__
					// __id__
					token = get_token();
					if (token->type != type_id)
					{
						printf("ERROR: Missing identifier after 'Dim' keyword\n");
						return SYNTAX_ERROR;
					}

					// ___Uloz identifikator do symtable___
					if (symtab_search(symtab, token, type_variable) != NULL) {
						printf("ERROR: Redefinition of variable %s\n", symtab_item->key);
						return SEMANTIC_ERROR;
					}

					symtab_item = symtab_insert(symtab, token, type_variable);

					//__As__
					token = get_token();
					if (token->type != type_keyword) {
						printf("ERROR: Missing 'As' in variable declaration\n");
						return SYNTAX_ERROR;
					}
					if (token->atribute.int_value != kw_as) {
						printf("ERROR: Missing 'As' in variable declaration\n");
						return SYNTAX_ERROR;
					}
					// __<type>__
					token = get_token();
					if ((return_value = Tyype()) != OK) {
						return return_value;
					}
					Tvalue init_value;
					// ___Uloz typ identifikatoru___ 
					switch (token->atribute.int_value)
					{
						case kw_integer:
							init_value.value_int = 0;
							set_item_variable(symtab_item->type_strct.variable, init_value, type_int);
						break;
						case kw_double:
							init_value.value_double = 0.0;
							set_item_variable(symtab_item->type_strct.variable, init_value, type_doub);
						break;
						case kw_string:
							init_value.string = "";
							set_item_variable(symtab_item->type_strct.variable, init_value, type_str);
						break;
					}

					// __<assign>__
					token = get_token();
					if ((return_value = Assign()) != OK) {
						return return_value;
					}
					// __<expr>__
					if (token->type != type_operator) {
						;
					}
					else if (token->atribute.operator_value == op_assign) {
						token = get_token();
						out = precedence_analysis(token);
						if (out == NULL) {
							return COMPILER_ERROR;
						}
						
						token = out->ReturnToken;
						if ((return_value = check_type(symtab_item, out)) != OK) {
							return return_value;
						}
						/*
						data.type = type_id;
						data.Atr = symtab_item->variable;
						leaf = make_leaf(data);
						data.type = type_operator;
						data.Atr = op_assign;
						tree = make_tree(leaf , out->Tree, data);
						generate_instruction(tree);
						*/
					}
					
				break;

				case kw_print:
					// __Print__ JAK GENEROVAT ???
					// __<expr>__
					token = get_token();
					out = precedence_analysis(token);
					if (out == NULL) {
						return COMPILER_ERROR;
					}
					token = out->ReturnToken;
					if (token->type != type_semicolon) {
						printf("ERROR: Missing semicolon in 'print' function\n");
						return SYNTAX_ERROR;
					}
					// __<exprPrint>__
					token = get_token();
					if ((return_value = ExprPrint()) != OK) {
						return return_value;
					}

				break;

				case kw_do:
					//__while__
					token = get_token();
					if (token->type != type_keyword) {
						fprintf(stderr, "ERROR: Invalide syntax 'do', did you mean 'do while' ?\n");
						return SYNTAX_ERROR;
					}
					if (token->atribute.int_value != kw_while) {
						fprintf(stderr, "ERROR: Invalide syntax 'do', did you mean 'do while' ?\n");
						return SYNTAX_ERROR;	
					}
					//__<expr>__
					token = get_token();
					out = precedence_analysis(token);
					if (out == NULL) {
						return COMPILER_ERROR;
					}
					token = out->ReturnToken;
					if (out->Type != type_int) {
						fprintf(stderr, "ERROR: Do While expression has to be of type integer\n"); //or boolean
						return SEMANTIC_TYPE_ERROR;
					}
					// __EOL__
					if (token->type != type_eol) {
						fprintf(stderr, "ERROR: Missing end of line after 'do while <expr>' statement\n");
						return SYNTAX_ERROR;
					}
					// generate_while(out->Tree);
					// __<St_list>__
					token = get_token();
					if ((return_value = St_list()) != OK) {
						return return_value;
					}
					// __LOOP__
					if (token->type != type_keyword) {
						fprintf(stderr, "ERROR: Missing 'Loop' keyword at the end of 'do while' statement\n");
						return SYNTAX_ERROR;
					} 
					if (token->atribute.int_value != kw_loop) {
						fprintf(stderr, "ERROR: Missing 'Loop' keyword at the end of 'do while' statement\n");
						return SYNTAX_ERROR;	
					}
				break;

				case kw_if: // generovat <<<<<<<<<<<<<<<<<<<<<<<
					// __<expr>__
					token = get_token();
					out = precedence_analysis(token);
					if (out == NULL) {
						return COMPILER_ERROR;
					}
					token = out->ReturnToken;
					if (out->Type != type_int) {
						fprintf(stderr, "ERROR: Do While expression has to be of type integer\n"); //or boolean
						return SEMANTIC_TYPE_ERROR;
					}
					// __THEN__
					if (token->type != type_keyword) {
						fprintf(stderr, "ERROR: Missing 'Then' keyword in 'If <expr> Then' statement\n");
						return SYNTAX_ERROR;
					}
					if (token->atribute.int_value != kw_then) {
						fprintf(stderr, "ERROR: Missing 'Then' keyword in 'If <expr> Then' statement\n");
						return SYNTAX_ERROR;
					}
					//__EOL__
					token = get_token();
					if (token->type != type_eol) {
						fprintf(stderr, "ERROR: Missing end of line after 'If <expr> Then' statement\n");
						return SYNTAX_ERROR;
					}
					//__<St_list>__
					token = get_token();
					if ((return_value = St_list()) != OK) {
						return return_value;
					}
					// __<Else>__
					if ((return_value = Else()) != OK) {
						return return_value;
					}
					// __End__
					if (token->type != type_keyword) {
						fprintf(stderr, "ERROR: Missing 'End if' at the end of 'If <expr> Then' statement\n");
						return SYNTAX_ERROR;
					}
					if (token->atribute.int_value != kw_end) {
						fprintf(stderr, "ERROR: Missing 'End if' at the end of 'If <expr> Then' statement\n");
						return SYNTAX_ERROR;
					}
					// __IF__
					token = get_token();
					if (token->type != type_keyword) {
						fprintf(stderr, "ERROR: Missing 'End if' at the end of 'If <expr> Then' statement\n");
						return SYNTAX_ERROR;
					}
					if (token->atribute.int_value != kw_if) {
						fprintf(stderr, "ERROR: Missing 'End if' at the end of 'If <expr> Then' statement\n");
						return SYNTAX_ERROR;
					}
				break;

				case kw_return:
					// __<Expr>__ GENEROVAT <<<<<<<<<<<<<<<
					// resit navratovy typ funkce ?? (JAK)
					token = get_token();
					out = precedence_analysis(token);
					if (out == NULL) {
						return COMPILER_ERROR;
					}

				break;
			}
		break;

		case type_id:
			// id musi byt deklarovan
			// <<<<<<<<< GENEROVAT <<<<<<<<<<<<<
			symtab_item = symtab_search(symtab, token, type_variable);
			if (symtab_item == NULL) {
				fprintf(stderr, "ERROR: Identifier was not declared before assining to it\n");
				return SEMANTIC_ERROR;
			}
			// __ = __
			token = get_token();
			if (token->type != type_operator) {
				fprintf(stderr, "ERROR: Undefined symbol after identifier\n");
				return SYNTAX_ERROR;
			}
			if (token->atribute.int_value != op_assign) {
				fprintf(stderr, "ERROR: Undefined symbol after identifier\n");
				return SYNTAX_ERROR;
			}
			// <expr> / volani funkce
			token = get_token();
			if (token->type == type_id) {
				symtab_item = symtab_search(symtab, token, type_variable);
				if (symtab_item != NULL) {
					;
				}
				else {
					symtab_item = symtab_search(symtab, token, type_function);
					if (symtab_item == NULL) {
						fprintf(stderr, "ERROR: Using variable before declaration\n");
						return SEMANTIC_ERROR;
					}
					// __VOLANI_FUNKCE__ <<<<<<<<<<<<<<<<< generuj kod <<<<<<<<<<<<<
					// __ ( __
					token = get_token();
					if (token->type != type_operator) {
						fprintf(stderr, "ERROR: Unexpected symbol after function identifier, did you mean to call the function like: f_id(params)?\n");
						return SYNTAX_ERROR;
					}
					if (token->atribute.int_value != op_bracket) {
						fprintf(stderr, "ERROR: Unexpected symbol after function identifier, did you mean to call the function like: f_id(params)?\n");
						return SYNTAX_ERROR;
					}
					// ___<Param_list>__
					/*
					token = get_token();
					if ((return_value = Param_list()) != OK) {
						return return_value;
					}
					
					if (token->type != type_operator) {
						fprintf(stderr, "ERROR: Missing closing bracket\n");
						return SYNTAX_ERROR;
					}
					if (token->atribute.int_value != op_bracket_end) {
						fprintf(stderr, "ERROR: Missing closing bracket\n");
						return SYNTAX_ERROR;
					}
					*/
				}

			}
			// __<Expr>__
			out = precedence_analysis(token);
			if (out == NULL) {
				return COMPILER_ERROR;
			}
			token = out->ReturnToken;
			if ((return_value = check_type(symtab_item, out)) != OK) {
				return return_value;
			}
			/*
			data.type = type_id;
			data.Atr = symtab_item->variable;
			leaf = make_leaf(data);
			data.type = type_operator;
			data.Atr = op_assign;
			tree = make_tree(leaf , out->Tree, data);
			generate_instruction(tree);
			*/
			
		break;

		default:
			printf("ERROR: Wrong statement\n");
			return SYNTAX_ERROR;
	}
	return OK;
}
int Else()
{
	int return_value;
	switch (token->atribute.int_value)
	{
		// __EPSILON__
		case kw_end:
			return OK;
		break;

		case kw_else:
			//__EOL__
			token = get_token();
			if (token->type != type_eol) {
				fprintf(stderr, "ERROR: Missing end of line after 'Else' keyword\n");
				return SYNTAX_ERROR;
			}
			// __<St_list>__
			token = get_token();
			if ((return_value = St_list()) != OK) {
				return return_value;
			}
		break;
		default:
			fprintf(stderr, "ERROR: What your doin maan\n"); // Unexpected usage of keyword
			return SYNTAX_ERROR;
	}
	return OK;
}

int Assign()
{
	
	// __EPSILON__
	if (token->type == type_eol) {
		return OK;
	}
	// __ = __
	if (token->type != type_operator) {
		printf("ERROR: Wrong symbol where assignment was expected\n");
		return SYNTAX_ERROR;
	}
	if (token->atribute.operator_value != op_assign) {
		printf("ERROR: Wrong symbol where assignment was expected\n");
		return SYNTAX_ERROR;
	}
	// <<<<<<<<<<<<<<<< call <expr> 
	// zkontrolovat typ, udelej strom ???
	return OK;

}
int Func()
{
	return OK;
}
int Param_list()
{
	int return_value;
	switch (token->type)
	{
		case type_operator:
			if (token->atribute.int_value != op_bracket_end) {
				fprintf(stderr, "ERROR: Missing closing bracket\n");
				return SYNTAX_ERROR;
			}
		break;

		case type_id:
			// __<Param>__
			if ((return_value = Param()) != OK) {
				return return_value;
			}
			// __<Next_par>__
			token = get_token();
			if ((return_value = Next_par()) != OK) {
				return return_value;
			}
		break;

		default:
			fprintf(stderr, "ERROR: Unexpected symbol in function parameters list\n");
			return SYNTAX_ERROR;
	}
	return OK;
}

int Param()
{
	Tsymtab_item* symtab_item;
	int return_value;

	if (token->type != type_id) {
		fprintf(stderr, "ERROR: Invalide parameter in function\n");
		return SYNTAX_ERROR;
	}
	// vklada do lokalni tabulky funkce ??????????? je to treba ???????????
	symtab_item = symtab_insert(symtab, token, type_variable);
	// __AS__
	token = get_token();
	if (token->type != type_keyword) {
		fprintf(stderr, "ERROR: Missing 'As' keyword in function parameter definition: 'id As <type>'\n");
		return SYNTAX_ERROR;
	}
	if (token->atribute.int_value != kw_as) {
		fprintf(stderr, "ERROR: Missing 'As' keyword in function parameter definition: 'id As <type>'\n");
		return SYNTAX_ERROR;
	}
	// __<Tyype>__
	token = get_token();
	if ((return_value = Tyype()) != OK) {
		return return_value;
	}
	// ulozime 

}

int Next_par()
{
	return OK;
}
int ExprPrint()
{
	int return_value;
	PrecendentOutput* out;
	switch (token->type)
	{
		case type_eol:
			return OK;
		default:
			out = precedence_analysis(token);
			if (out == NULL) {
				return COMPILER_ERROR;
			}
			token = out->ReturnToken;

			if (token->type != type_semicolon) {
				fprintf(stderr, "ERROR: Missing semicolon in 'print' function\n");
				return SYNTAX_ERROR;
			}
			token = get_token();
			if ((return_value = ExprPrint()) != OK) {
				return return_value;
			}
	}
	return OK;
}
int Tyype()
{
	if (token->type != type_keyword) {
		printf("ERROR: Data type does not exist\n");
		return SYNTAX_ERROR;
	}
	switch (token->atribute.int_value)
	{
		case kw_integer:
		case kw_double:
		case kw_string:
			return OK;
		default:
			printf("ERROR: Data type does not exist\n");
			return SYNTAX_ERROR;
	}
}

int check_type(Tsymtab_item* symtab_item, PrecendentOutput* out)
{
	if (out->Type != symtab_item->type_strct.variable->type) {
		switch (symtab_item->type_strct.variable->type) {
			case type_int:
				if (out->Type == type_doub) {
					break;// upozornit generator aby to pretypoval
				}
				
			
			case type_doub:
				if (out->Type == type_int) {
					break; // upozornit generator
				}
				
			
			default:
				fprintf(stderr, "ERROR: Trying to assign incompatible types\n");
				return SEMANTIC_TYPE_ERROR;

		}	
	
	}
	return OK;
}

int parse()
{
	symtab = symtab_init(42); 
	return Prog();
	symtab_free(symtab); 
}




int main(int argc, char const *argv[])
{
	int return_value;

	return_value = parse();
	printf("\n");
	switch (return_value)
	{
		case OK:
			printf("Everything is OK\n");
			break;
		case LEXICAL_ERROR:
			printf("LEXICAL_ERROR\n");
			break;
		case SYNTAX_ERROR:
			printf("SYNTAX_ERROR\n");
			break;
		case SEMANTIC_ERROR:
			printf("SEMANTIC_ERROR\n");
			break;
		case SEMANTIC_TYPE_ERROR:
			printf("SEMANTIC_TYPE_ERROR\n");
			break;
		case OTHER_ERROR:
			printf("OTHER_ERROR\n");
			break;
		case COMPILER_ERROR:
			printf("COMPILER_ERROR\n");
			break;
	}
	return 0;
}
