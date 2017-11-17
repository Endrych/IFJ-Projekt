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
Tsymtab* sym_table;

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
					symtab_item = symtab_search(sym_table, token, type_variable);
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
					
					symtab_item = symtab_insert(sym_table, token, type_variable);
					if (symtab_item->type_strct.variable->declared) {
						printf("ERROR: Redefinition of variable %s\n", symtab_item->key);
						return SEMANTIC_ERROR;
					}

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
						
						if (out->Type != symtab_item->type_strct.variable->type) {
							fprintf(stderr, "ERROR: Trying to assign incompatible types\n");
							return SEMANTIC_TYPE_ERROR;
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
			}
		break;

		case type_id:
			// id musi byt deklarovan
			// <<<<<<<<< GENEROVAT <<<<<<<<<<<<<
			symtab_item = symtab_search(sym_table, token, type_variable);
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
			// __<expr>__ TYPOVA KONVERZE ???
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
			
		break;

		default:
			printf("ERROR: Wrong statement\n");
			return SYNTAX_ERROR;
	}
	return OK;
}
int Else();
int Assign()
{
	
	// __EPSILON__
	if (token->type == type_eol) {
		return OK;
	}
	// __ = __
	if (token->type != type_operator) {
		printf("ERROR: Assignment was expected\n");
		return SYNTAX_ERROR;
	}
	if (token->atribute.operator_value != op_assign) {
		printf("ERROR: Assignment was expected\n");
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
int Param_list();
int Param();
int Next_par();
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
	sym_table = symtab_init(42); 
	return Prog();
	symtab_free(sym_table); 
}




int main(int argc, char const *argv[])
{
	int return_value;
	char *test_file;
	if (argc != 2)
	{
		printf("CHYBA: Nebyl zadan testovaci soubor.\n\n");
		printf("Usage:\n\t ./parser test_file \n");
		return 1;
	}
	test_file = argv[1];

	// funkce otevre testovaci soubor
	load_file(test_file);
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
	close_file();
	return 0;
}
