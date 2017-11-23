/* IFJ project
 * @authors:
 * @date:
 * @brief:
 * 
 TODO: 
 	- error.h 
*/

#include <stdbool.h>
#include <stdlib.h> // exit
#include <stdio.h>
#include "parser.h"
#include "lexical.h"
#include "token.h"
#include "error.h"
#include "symtable.h"
#include "abstract_tree.h"
#include "precedence_sa.h"
#include "set_values.h"
#include "at_que.h"


// tabulka symbolu
//Tsymtab *sym_tab;
// aktualni token
Token* token;
extern Tsymtab * symtab;
Tsymtab_item* func_symtab_item = NULL;
QStack* qstack;


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
				get_non_eol_token();
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
	// vytvorime qitem a novou frontu pro scope
	ATQItem* qitem = (ATQItem*) malloc(sizeof(ATQItem));
	ATQueue* scope_queue;
	ATQueue* top_queue;

	scope_queue = (ATQueue*)malloc(sizeof(ATQueue));
	queInit(scope_queue);
	qitem->GenType = gt_main;
	qitem->GenValue.at_queue = scope_queue;
	top_queue = qstackTop(qstack);
	queUp(top_queue, qitem);
	// pushneme frontu na vrchol zasobniku
	qstackPush(qstack, scope_queue);

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
	// popneme frontu ze zasobniku
	qstackPop(qstack);
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
	ATQueue* top_queue;
	int return_value;
	Tsymtab_item *symtab_item;
	PrecendentOutput* out;
	ATData data;
	Tsymtab_item* symtab_item_left;
	ATQItem* qitem;

	switch (token->type)
	{
		case type_keyword:
			switch (token->atribute.int_value)
			{
				case kw_input:
					// __Input id__ input id
					// JAK GENEROVAT ? <<<<<<<<<<<<<<<

					token = get_token();
					if (token->type != type_id)
					{
						printf("ERROR: Missing identifier after Intup statement\n");
						return SYNTAX_ERROR;
					}
					symtab_item = symtab_search(symtab, token);
					if (symtab_item == NULL) {
						fprintf(stderr, "ERROR: Identifier used in Input statement was not declared.\n");
						return SEMANTIC_ERROR;
					}
					if (symtab_item->type == type_function) {
						fprintf(stderr, "ERROR: Trying to use function identifier in 'Input' statement.\n");
						return SEMANTIC_ERROR;
					}
					// generuj kod
					// qitem
					qitem = (ATQItem *) malloc(sizeof(ATQItem));
					qitem->GenType = gt_input;
					qitem->GenValue.id = symtab_item;
					qitem->Next = NULL;
					// zjisti co je na vrcholu zasobniku (muze to byt Scope nebo Funkce...)
					top_queue = qstackTop(qstack);
					// vloz instrukci do fronty
					queUp(top_queue, qitem);

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
					if ((symtab_item = symtab_search(symtab, token)) != NULL) {
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
					
					// nastavime typ promenne
					if ((return_value = set_type_variable(symtab_item->type_strct.variable)) != OK) {
						return return_value;
					}
					ATQItem* qitem;
					VarDeclarInput* declar_input;
					declar_input = (VarDeclarInput*) malloc(sizeof(VarDeclarInput));
					declar_input->id = symtab_item;
					declar_input->expr = NULL;

					qitem = (ATQItem*) malloc(sizeof(ATQItem));
					qitem->GenType = gt_var_declar;
					qitem->GenValue.var_declar_input = declar_input;

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
						if ((return_value = check_type(symtab_item->type_strct.variable->type, out)) != OK) {
							return return_value;
						}
						declar_input->expr = out->Tree;
						
					}
					top_queue = qstackTop(qstack);
					queUp(top_queue, qitem);
					
				break;

				case kw_print:
					// __<expr>__
					// vyrazy nemuzou byt bool
					qitem = (ATQItem*) malloc(sizeof(ATQItem));

					qitem->GenType = gt_print;
					qitem->GenValue.exprs = (eQueue *) malloc(sizeof(eQueue));
					qitem->Next = NULL; // ??????????????????????

					equeInit(qitem->GenValue.exprs);

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
					equeUp(qitem->GenValue.exprs, out->Tree);

					// __<exprPrint>__
					token = get_token();
					if ((return_value = ExprPrint(qitem->GenValue.exprs)) != OK) {
						return return_value;
					}
					top_queue = qstackTop(qstack);
					queUp(top_queue, qitem);

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
					if (out->Type != type_bool) {
						fprintf(stderr, "ERROR: Do While expression has to be of type boolean\n");
						return SEMANTIC_TYPE_ERROR;
					}
					// __EOL__
					if (token->type != type_eol) {
						fprintf(stderr, "ERROR: Missing end of line after 'do while <expr>' statement\n");
						return SYNTAX_ERROR;
					}

					qitem = (ATQItem*) malloc(sizeof(ATQItem));
					qitem->GenType = gt_while;
					qitem->GenValue.while_input = (WhileInput*) malloc(sizeof(WhileInput));

					qitem->GenValue.while_input->cond_expr = out->Tree;
					// fronta
					ATQueue* while_queue = (ATQueue*) malloc(sizeof(ATQueue));
					queInit(while_queue);
					qitem->GenValue.while_input->queue = while_queue;

					top_queue = qstackTop(qstack);
					queUp(top_queue , qitem);
					qstackPush (qstack, while_queue);

					// __<St_list>__
					get_non_eol_token();
					if ((return_value = St_list()) != OK) {
						return return_value;
					}
					qstackPop (qstack);
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
					if (out->Type != type_bool) {
						fprintf(stderr, "ERROR: 'If' expression has to be of type boolean\n"); //or boolean
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
					qitem = (ATQItem*) malloc(sizeof(ATQItem));
					ATQueue* true_queue = (ATQueue*) malloc(sizeof(ATQueue));
					ATQueue* false_queue = (ATQueue*) malloc(sizeof(ATQueue));
					queInit(true_queue);
					queInit(false_queue);
					
					qitem->GenType = gt_if;
					qitem->GenValue.if_input = (IfInput*) malloc(sizeof(IfInput));
					qitem->GenValue.if_input->cond_expr = out->Tree;
					qitem->GenValue.if_input->true_queue = true_queue;
					qitem->GenValue.if_input->false_queue = false_queue;

					top_queue = qstackTop(qstack);
					queUp(top_queue, qitem);

					qstackPush(qstack, true_queue);

					//__<St_list>__
					get_non_eol_token();
					if ((return_value = St_list()) != OK) {
						return return_value;
					}

					qstackPop(qstack);
					qstackPush(qstack, false_queue);
					// __else__
					if (token->type != type_keyword) {
						fprintf(stderr, "ERROR: Missing 'else' in 'if' statement\n");
						return SYNTAX_ERROR;
					}
					if (token->atribute.int_value != kw_else) {
						fprintf(stderr, "ERROR: Missing 'else' in 'if' statement\n");
						return SYNTAX_ERROR;
					}
					// __EOL__
					token = get_token();
					if (token->type != type_eol) {
						fprintf(stderr, "ERROR: Missing end of line after 'Else' keyword\n");
						return SYNTAX_ERROR;
					}
					// __<St_list>__
					get_non_eol_token();
					if ((return_value = St_list()) != OK) {
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
					qstackPop(qstack);
				break;

				case kw_return:
					// zkontrolujeme jestli zname adresu funkce v tabulce symbolu
					if (func_symtab_item == NULL) {
						fprintf(stderr, "ERROR: fatal af\n");
						return SYNTAX_ERROR; // ???????????????????????????
					}

					// __<Expr>__ 
					qitem = (ATQueue*) malloc(sizeof(ATQueue));
					qitem->GenType = gt_return;
					qitem->GenValue.return_input = (ReturnInput*) malloc(sizeof(ReturnInput));

					token = get_token();
					out = precedence_analysis(token);
					if (out == NULL) {
						return COMPILER_ERROR;
					}
					// zkontrolujeme navratovy typ
					if ((return_value = check_type(func_symtab_item->type_strct.function->return_type, out)) != OK) {
						return return_value;
					}
					// naplnime qitem
					qitem->GenValue.return_input->sym_item = func_symtab_item;
					qitem->GenValue.return_input->expr = out;
					// vlozime qitem do fronty
					top_queue = qstackTop(qstack);
					queUp(top_queue, qitem);

				break;
			}
		break;

		case type_id:
			// id musi byt deklarovan
			
			symtab_item = symtab_search(symtab, token);
			if (symtab_item == NULL) {
				fprintf(stderr, "ERROR: Identifier was not declared before assining to it\n");
				return SEMANTIC_ERROR;
			}
			if (symtab_item->type == type_function) {
				fprintf(stderr, "ERROR: Use of function identifier where variable was expected\n");
				return SEMANTIC_ERROR;
			}
			symtab_item_left = symtab_item;
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

			ATQItem* qitem;
			qitem = (ATQItem*) malloc(sizeof(ATQItem));

			// <expr> / volani funkce
			token = get_token();
			if (token->type == type_id) {
				// nejdrive hledame jesti to neni promenna
				symtab_item = symtab_search(symtab, token);
				if (symtab_item == NULL) {
					fprintf(stderr, "ERROR: Use of undeclared variable\n");
					return SEMANTIC_ERROR; // ?????????????????????????????
				}
				if (symtab_item->type == type_function) {
					// __VOLANI_FUNKCE__
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
					
					token = get_token();
					if ((return_value = Param_list(symtab_item->type_strct.function)) != OK) {
						return return_value;
					}
					// __)__
					if (token->type != type_operator) {
						fprintf(stderr, "ERROR: Missing closing bracket\n");
						return SYNTAX_ERROR;
					}
					if (token->atribute.int_value != op_bracket_end) {
						fprintf(stderr, "ERROR: Missing closing bracket\n");
						return SYNTAX_ERROR;
					}
					
					return OK;
				}

			}
			qitem->GenType = gt_assign;
			qitem->GenValue.assign_input = (AssignInput *) malloc(sizeof(AssignInput));
			qitem->GenValue.assign_input->id = symtab_item_left;

			// __<Expr>__
			out = precedence_analysis(token);
			if (out == NULL) {
				return COMPILER_ERROR;
			}
			token = out->ReturnToken;
			if ((return_value = check_type(symtab_item->type_strct.variable->type, out)) != OK) {
				return return_value;
			}
			qitem->GenValue.assign_input->expr = out->Tree;
			top_queue = qstackTop(qstack);
			queUp(top_queue, qitem);

		break;

		default:
			printf("ERROR: Wrong statement\n");
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

	int return_value;
	Tsymtab_item* symtab_item;
	ATQItem* qitem = NULL;
	ATQueue* top_queue;
	Tsymtab* temp;

	switch (token->type) {
		case type_eof:
			return OK;
		break;

		case type_keyword:
			switch (token->atribute.int_value) {
				// function definition
				case kw_function:
					// __id__
					token = get_token();
					if (token->type != type_id) {
						fprintf(stderr, "ERROR: Missing identifier after function keyword\n");
						return SYNTAX_ERROR;
					}
					// fronta na vrcholu zasobniku (mela by to byl globalni)
					top_queue = qstackTop(qstack);

					// zkusime najit id v tabulce symbolu
					symtab_item = symtab_search(symtab, token);
					// jestli uz je v tabulce
					if (symtab_item == NULL) {
						// id v tabulce jeste neni
						symtab_item = symtab_insert(symtab, token, type_function); // vlozime do tabulky
						symtab_item->type_strct.function->defined = true; // defined = definice funkce

						// GENEROVANI
						qitem = (ATQItem*) malloc(sizeof(ATQItem));

						//input->queue = NULL;
						qitem->GenType = gt_func_declar;
						qitem->GenValue.func_declar_input = (FuncDeclarInput *) malloc(sizeof(FuncDeclarInput));
						qitem->GenValue.func_declar_input->sym_item = symtab_item;
						queUp(top_queue, qitem);
					}

					else {
						// pokud uz v tabulce je
						if (symtab_item->type == type_variable) {
							fprintf(stderr, "ERROR: Function identifier '%s' already used as variable identifier\n", symtab_item->key);
							return SEMANTIC_ERROR;
						}
						// jestli jiz byla funkce definovana
						if (symtab_item->type_strct.function->defined) {
							fprintf(stderr, "ERROR: Redefinition of function %s\n", symtab_item->key);
							return SEMANTIC_ERROR;
						}

						// byla pouze deklarovana, uloz do defined = true
						symtab_item->type_strct.function->defined = true;

						// jelikoz uz byla funkce deklarovana, ma jiz vygenerovany qitem
						// a proto jej zkusime najit ve fronte na vrcholu zasobniku (coz je globalni fronta definic funkci a Scope)
						ATQItem* temp = queFront(top_queue);
						while (temp != NULL) {
							if (temp->GenType == gt_func_declar) {
								if (temp->GenValue.func_declar_input->sym_item == symtab_item) {
									qitem = temp;
									break;
								}
							}

							temp = temp->Next;
						}
						if (temp == NULL) {
							fprintf(stderr, "ERROR: WTF\n");
							return COMPILER_ERROR;
						}
						// zde jiz mame qitem
					}
					// nasledujici faze je jiz shodna pro obe varianty
					// vytvorime frontu pro instrukce funkce a pushneme ji na vrchol zasobniku
					ATQueue* func_queue;
					func_queue = (ATQueue*) malloc(sizeof(ATQueue));
					qitem->GenValue.func_declar_input->queue = func_queue;
					queInit(func_queue);
					qstackPush (qstack, func_queue);

					// do globalni promenne ulozime adresu funkce v tabulce symbolu
					func_symtab_item = symtab_item;

					// __(__
					token = get_token();
					if (token->type != type_operator) {
						fprintf(stderr, "ERROR: Missing bracket after function identifier\n");
						return SYNTAX_ERROR;
					}
					if (token->atribute.int_value != op_bracket) {
						fprintf(stderr, "ERROR: Missing bracket after function identifier\n");
						return SYNTAX_ERROR;
					}

					// budeme pouzivat lokalni tabulku symbolu
					temp = symtab;
					symtab = symtab_item->type_strct.function->sym_table;

					// __<param_list>__
					token = get_token();
					if ((return_value = Param_list()) != OK) {
						return return_value;
					}
					// vratime zpet
					symtab = temp;

					// __)__
					token = get_token();
					if (token->type != type_operator) {
						fprintf(stderr, "ERROR: Missing closing bracket in function definition\n");
						return SYNTAX_ERROR;
					}
					if (token->atribute.int_value != op_bracket_end) {
						fprintf(stderr, "ERROR: Missing closing bracket in function definition\n");
						return SYNTAX_ERROR;
					}

					// __As__
					token = get_token();
					if (token->type != type_keyword) {
						fprintf(stderr, "ERROR: Missing 'As' keyword in function definition\n");
						return SYNTAX_ERROR;
					}
					if (token->atribute.int_value != kw_as) {
						fprintf(stderr, "ERROR: Missing 'As' keyword in function definition\n");
						return SYNTAX_ERROR;
					}
					// ___<type>__
					token = get_token();
					if ((return_value = Tyype()) != OK) {
						return return_value;
					}
					// nastavime navratovou hodnotu funkce
					if ((return_value = set_return(symtab_item->type_strct.function)) != OK) {
						return return_value;
					}

					// __EOL__
					token = get_token();
					if (token->type != type_eol) {
						fprintf(stderr, "Missing end of line in function definition\n");
						return SYNTAX_ERROR;
					}
					// __ST_list__
					get_non_eol_token();
					if ((return_value = St_list()) != OK) {
						return return_value;
					}
					// __END__
					if (token->type != type_keyword) {
						fprintf(stderr, "ERROR: Missing 'End Function' at the end of function definition\n");
						return SYNTAX_ERROR;
					}
					if (token->atribute.int_value != kw_end) {
						fprintf(stderr, "ERROR: Missing 'End Function' at the end of function definition\n");
						return SYNTAX_ERROR;
					}
					// __Function__
					token = get_token();
					if (token->type != type_keyword) {
						fprintf(stderr, "ERROR: Missing 'End Function' at the end of function definition\n");
						return SYNTAX_ERROR;
					}
					if (token->atribute.int_value != kw_function) {
						fprintf(stderr, "ERROR: Missing 'End Function' at the end of function definition\n");
						return SYNTAX_ERROR;
					}
					// nakonec popneme frontu funkce ze zasobniku
					qstackPop(qstack);

					// a do globalni promenne misto adresy funkce v tabulce symbolu ulozime NULL
					func_symtab_item = NULL;
				

				break;
				case kw_declare:
					// __function__
					token = get_token();
					if (token->type != type_keyword) {
						fprintf(stderr, "ERROR: Missing 'function' keyword in function declaration\n");
						return SYNTAX_ERROR;
					}
					if (token->atribute.int_value != kw_function) {
						fprintf(stderr, "ERROR: Missing 'function' keyword in function declaration\n");
						return SYNTAX_ERROR;
					}
					// __id__
					token = get_token();
					if (token->type != type_id) {
						fprintf(stderr, "ERROR: Missing identifier in function declaration\n");
						return SYNTAX_ERROR;
					}

					symtab_item = symtab_search(symtab, token);
					// pokud jiz id je v tabulce symbolu
					if (symtab_item != NULL) {
						if (symtab_item->type == type_variable) {
							fprintf(stderr, "ERROR: Redeclaration of variable %s\n", symtab_item->key);
							return SEMANTIC_ERROR;
						}
						if (symtab_item->type_strct.function->declared) {
							fprintf(stderr, "ERROR: Function %s was declared more than once\n", symtab_item->key);
							return SEMANTIC_ERROR;
						}
					}
					else {
						// jinak vlozime id do tabulky symbolu
						symtab_item = symtab_insert(symtab, token, type_function);
					}
					// nastavime, ze funkce jiz byla deklarovana
					symtab_item->type_strct.function->declared = true;
					
					// __(__
					token = get_token();
					if (token->type != type_operator) {
						fprintf(stderr, "ERROR: Missing bracket in function definition\n");
						return SYNTAX_ERROR;
					}
					if (token->atribute.int_value != op_bracket) {
						fprintf(stderr, "ERROR: Missing bracket in function definition\n");
						return SYNTAX_ERROR;
					}

					// budeme pouzivat lokalni tabulku symbolu
					temp = symtab;
					symtab = symtab_item->type_strct.function->sym_table;

					// __<param_list>__
					token = get_token();
					if ((return_value = Param_list(symtab_item->type_strct.function)) != OK) {
						return return_value;
					}

					// vratime tabulku zpet
					symtab = temp;

					// __)__ 
					if (token->type != type_operator) {
						fprintf(stderr, "ERROR: Missing closing bracket in function declaration\n");
						return SYNTAX_ERROR;
					}
					if (token->atribute.int_value != op_bracket_end) {
						fprintf(stderr, "ERROR: Missing closing bracket in function declaration\n");
						return SYNTAX_ERROR;
					}
					// __AS__
					token = get_token();
					if (token->type != type_keyword) {
						fprintf(stderr, "ERROR: Missing 'As' keyword in function declaration\n");
						return SYNTAX_ERROR;
					}
					if (token->atribute.int_value != kw_as) {
						fprintf(stderr, "ERROR: Missing 'As' keyword in function declaration\n");
						return SYNTAX_ERROR;
					}
					// __<type>__
					token = get_token();
					if ((return_value = Tyype()) != OK) {
						return return_value;
					}
					// nastavime navratovou hodnotu funkce
					if ((return_value = set_return(symtab_item->type_strct.function)) != OK) {
						return return_value;
					}
					// _______GENERATOR________
					// nastavime qitem
					qitem = (ATQItem*) malloc(sizeof(ATQItem));

					qitem->GenType = gt_func_declar;
					qitem->GenValue.func_declar_input = (FuncDeclarInput*) malloc(sizeof(FuncDeclarInput));
					qitem->GenValue.func_declar_input->sym_item = symtab_item;
					qitem->GenValue.func_declar_input->queue = NULL;
					// pushneme qitem na vrchol zasobniku
					top_queue = qstackTop(qstack);
					queUp(top_queue, qitem);

				break;

				default:
					fprintf(stderr, "ERROR: Unexpected symbol in function declaration/definition\n");
					return SYNTAX_ERROR;
			}
		break;

		default:
			fprintf(stderr, "ERROR: Unexpected symbol in function declaration/definition\n");
			return SYNTAX_ERROR;
	}
	return OK;
}

int Param_list(Tfunction_item *function)
{
	int return_value;
	switch (token->type)
	{
		case type_operator:
			if (token->atribute.int_value == op_bracket_end) {
				return OK;
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
	// vklada do lokalni tabulky funkce
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
	return OK;

}

int Next_par()
{
	return OK;
}
int ExprPrint(eQueue* exprs)
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
			equeUp(exprs, out->Tree);

			token = get_token();
			if ((return_value = ExprPrint(exprs)) != OK) {
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

// nastavi navratovou hodnotu funkce v tabulce symbolu
int set_return(Tfunction_item *function)
{
	// nastavime navratovou hodnotu
	Tvariable_type f_ret_value;
	
	// ___Uloz typ identifikatoru___ 
	switch (token->atribute.int_value)
	{
		case kw_integer:
			f_ret_value = type_int;
			break;
		case kw_double:
			f_ret_value = type_doub;
			break;
		case kw_string:
			f_ret_value = type_str;
			break;
		case kw_boolean:
			f_ret_value = type_bool;
			break;
		default:
			fprintf(stderr, "ERROR: Unknown identifier type in function definition/declaration\n");
			return SYNTAX_ERROR;
	}
	set_item_function(function, f_ret_value, symtab);
	return OK;
}

// nastavi typ a inicializuje promennou v tabulce symbolu
int set_type_variable(Tvariable_item *variable)
{
	Tvalue init_value;
	// ___Uloz typ identifikatoru___ 
	switch (token->atribute.int_value)
	{
		case kw_integer:
			init_value.value_int = 0;
			set_item_variable(variable, init_value, type_int);
			break;
		case kw_double:
			init_value.value_double = 0.0;
			set_item_variable(variable, init_value, type_doub);
			break;
		case kw_string:
			init_value.string = "";
			set_item_variable(variable, init_value, type_str);
			break;
		default:
			fprintf(stderr, "ERROR: Unknown type in variable declaration\n");
			return SYNTAX_ERROR;
	}
	return OK;
}

int check_type(Tvariable_type type, PrecendentOutput* out)
{
	if (out->Type != type) {
		switch (type) {
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

void swap_symtab(Tsymtab* tab1, Tsymtab* tab2)
{
	Tsymtab* temp = tab1;
	tab1 = tab2;
	tab2 = temp;
}

int parse()
{
	ATQueue* global_queue;
	global_queue = (ATQueue*) malloc(sizeof(ATQueue));	
	qstack = (QStack*) malloc(sizeof(QStack));

	queInit (global_queue);
	qstackInit(qstack);
	qstackPush (qstack, global_queue);

	symtab = symtab_init(42); 
	return Prog();
	symtab_free(symtab); 
}




int main()
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
