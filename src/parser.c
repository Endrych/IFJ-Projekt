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
#include "generator.h"
#include "destructor.h"
#include "built_in.h"


// tabulka symbolu
//Tsymtab *sym_tab;
// aktualni token
Token* token;
extern Tsymtab * symtab;
Tsymtab_item* func_symtab_item = NULL;
QStack* qstack;
Tsymtab * global_symtab = NULL;
unsigned functions_inserted = 0;
unsigned functions_defined = 0;

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
				if (token->type != type_eof) {
					fprintf(stderr, "ERROR: No code can be after end of Scope\n");
					return SYNTAX_ERROR;
				}

				// proverime, ze kazda funkce vlozena do tabulky symbolu
				// byla definovana
				if (functions_inserted != functions_defined) {
					fprintf(stderr, "ERROR: All functions have to be defined\n");
					return SEMANTIC_ERROR;
				}

				return OK;

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
				fprintf(stderr,"ERROR: Missing Scope or function definition/declaration at the begining of program\n");
				return SYNTAX_ERROR;
			}
			break;
		default:
			fprintf(stderr,"ERROR: Missing Scope or function definition/declaration at the begining of program\n");
			return SYNTAX_ERROR;
	}
	return OK;
}

int Scope()
{
	// vytvorime qitem a novou frontu pro scope
	ATQItem* qitem = (ATQItem*) malloc(sizeof(ATQItem));
	if (qitem == NULL)
	{
		fprintf(stderr, "%s\n", COMPILER_MESSAGE);
        dispose_global();
	}
	struct ATQueue* scope_queue;
	ATQueue* top_queue;

	scope_queue = (struct ATQueue*)malloc(sizeof(ATQueue));
	if (scope_queue == NULL)
	{
		fprintf(stderr, "%s\n", COMPILER_MESSAGE);
        dispose_global();
	}
	queInit((ATQueue*)scope_queue);
	qitem->GenType = gt_main;
	qitem->GenValue.at_queue = scope_queue;
	top_queue = qstackTop(qstack);
	queUp(top_queue, qitem);
	// pushneme frontu na vrchol zasobniku
	qstackPush(qstack,(ATQueue*) scope_queue);

	int return_value;

	// __SCOPE__
	if (token->type != type_keyword)
	{
		fprintf(stderr,"ERROR: Missing 'Scope' definition\n");
		return SYNTAX_ERROR;
	}

	if (token->atribute.int_value != kw_scope)
	{
		fprintf(stderr,"ERROR: Missing 'Scope' definition\n");
		return SYNTAX_ERROR;
	}
	
	// __EOL__
	token = get_token();
	if (token->type != type_eol)
	{
		fprintf(stderr,"ERROR: Missing end of line after Scope keyword\n");
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
		fprintf(stderr,"ERROR: Missing 'End Scope' at the end of Scope definition\n");
		return SYNTAX_ERROR;
	}
	if (token->atribute.int_value != kw_end)
	{
		fprintf(stderr,"ERROR: Missing 'End Scope' at the end of Scope definition\n");
		return SYNTAX_ERROR;
	}

	// __SCOPE__
	token = get_token();
	if (token->type != type_keyword)
	{
		fprintf(stderr,"ERROR: Missing 'End Scope' at the end of Scope definition\n");
		return SYNTAX_ERROR;
	}
	if (token->atribute.int_value != kw_scope)
	{
		fprintf(stderr,"ERROR: Missing 'End Scope' at the end of Scope definition\n");
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
						fprintf(stderr,"ERROR: Missing end of line after statement\n");
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
				fprintf(stderr,"ERROR: Missing end of line after statement\n");
				return SYNTAX_ERROR;
			}
			// __<st-list>__
			get_non_eol_token();
			return St_list();
		break;

		default:
			fprintf(stderr,"ERROR: Wrong statement\n");
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
						fprintf(stderr,"ERROR: Missing identifier after Intup statement\n");
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
					if (qitem == NULL)
					{
						fprintf(stderr, "%s\n", COMPILER_MESSAGE);
     				    dispose_global();
					}
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
						fprintf(stderr,"ERROR: Missing identifier after 'Dim' keyword\n");
						return SYNTAX_ERROR;
					}

					// ___Uloz identifikator do symtable___
					// nejdrive se podivame do globalni tabulky symbolu
					if ((symtab_item = symtab_search(global_symtab, token)) != NULL) {
						if (symtab_item->type == type_function) {
							fprintf(stderr,"ERROR: Redefinition of variable %s used as function identifier\n", symtab_item->key);
							return SEMANTIC_ERROR;
						}
					}

					if ((symtab_item = symtab_search(symtab, token)) != NULL) {
						fprintf(stderr,"ERROR: Redefinition of variable %s\n", symtab_item->key);
						return SEMANTIC_ERROR;
					}

					symtab_item = symtab_insert(symtab, token, type_variable);

					//__As__
					token = get_token();
					if (token->type != type_keyword) {
						fprintf(stderr,"ERROR: Missing 'As' in variable declaration\n");
						return SYNTAX_ERROR;
					}
					if (token->atribute.int_value != kw_as) {
						fprintf(stderr,"ERROR: Missing 'As' in variable declaration\n");
						return SYNTAX_ERROR;
					}
					// __<type>__
					token = get_token();
					if ((return_value = Tyype()) != OK) {
						return return_value;
					}
					
					// nastavime typ promenne
					if ((return_value = set_type_variable(symtab_item)) != OK) {
						return return_value;
					}
					ATQItem* qitem;
					VarDeclarInput* declar_input;
					declar_input = (VarDeclarInput*) malloc(sizeof(VarDeclarInput));
					if (declar_input == NULL)
					{
						fprintf(stderr, "%s\n", COMPILER_MESSAGE);
        				dispose_global();
					}
					declar_input->id = symtab_item;
					declar_input->expr = NULL;

					qitem = (ATQItem*) malloc(sizeof(ATQItem));
					if (qitem == NULL)
					{
						fprintf(stderr, "%s\n", COMPILER_MESSAGE);
        				dispose_global();
					}
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
						if (token->type != type_eol) {
							fprintf(stderr, "ERROR: Unexpected token at the end of dim statement\n");
							return SYNTAX_ERROR;
						}

						if ((return_value = check_type(symtab_item->type_strct.variable->type, out->Type)) != OK) {
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
					if (qitem == NULL)
					{
						fprintf(stderr, "%s\n", COMPILER_MESSAGE);
        				dispose_global();
					}

					qitem->GenType = gt_print;
					qitem->GenValue.exprs = (eQueue *) malloc(sizeof(eQueue));
					if (qitem->GenValue.exprs == NULL)
					{
						fprintf(stderr, "%s\n", COMPILER_MESSAGE);
        				dispose_global();
					} 
					qitem->Next = NULL; // ??????????????????????

					equeInit(qitem->GenValue.exprs);

					token = get_token();
					out = precedence_analysis(token);
					if (out == NULL) {
						return COMPILER_ERROR;
					}
					token = out->ReturnToken;
					if (token->type != type_semicolon) {
						fprintf(stderr,"ERROR: Missing semicolon in 'print' function\n");
						return SYNTAX_ERROR;
					}
					eQItem * eItem;
					eItem = (eQItem*) malloc(sizeof(eQItem));
					if (eItem == NULL)
					{
						fprintf(stderr, "%s\n", COMPILER_MESSAGE);
        				dispose_global();
					}
					eItem->Next = NULL;
					eItem->etype = eq_tree;
					eItem->eValue.tree_value = out->Tree;
					equeUp(qitem->GenValue.exprs, eItem);

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
						fprintf(stderr, "ERROR: Invalid syntax 'do', did you mean 'do while' ?\n");
						return SYNTAX_ERROR;
					}
					if (token->atribute.int_value != kw_while) {
						fprintf(stderr, "ERROR: Invalid syntax 'do', did you mean 'do while' ?\n");
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
					if (qitem == NULL)
					{
						fprintf(stderr, "%s\n", COMPILER_MESSAGE);
        				dispose_global();
					}
					qitem->GenType = gt_while;
					qitem->GenValue.while_input = (WhileInput*) malloc(sizeof(WhileInput));
					if (qitem->GenValue.while_input == NULL)
					{
						fprintf(stderr, "%s\n", COMPILER_MESSAGE);
        				dispose_global();
					}

					qitem->GenValue.while_input->cond_expr = out->Tree;
					// fronta
					struct ATQueue* while_queue = (struct ATQueue*) malloc(sizeof(ATQueue));
					if (while_queue == NULL)
					{
						fprintf(stderr, "%s\n", COMPILER_MESSAGE);
        				dispose_global();
					}
					queInit((ATQueue*)while_queue);
					qitem->GenValue.while_input->queue = while_queue;

					top_queue = qstackTop(qstack);
					queUp(top_queue , qitem);
					qstackPush (qstack, (ATQueue*) while_queue);

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
					if (qitem == NULL)
					{
						fprintf(stderr, "%s\n", COMPILER_MESSAGE);
     				    dispose_global();
					}
					ATQueue* true_queue = (ATQueue*) malloc(sizeof(ATQueue));
					if (true_queue == NULL)
					{
						fprintf(stderr, "%s\n", COMPILER_MESSAGE);
        				dispose_global();
					}
					ATQueue* false_queue = (ATQueue*) malloc(sizeof(ATQueue));
					if (false_queue == NULL)
					{
						fprintf(stderr, "%s\n", COMPILER_MESSAGE);
        				dispose_global();
					}
					queInit(true_queue);
					queInit(false_queue);
					
					qitem->GenType = gt_if;
					qitem->GenValue.if_input = (IfInput*) malloc(sizeof(IfInput));
					if (qitem->GenValue.if_input == NULL)
					{
						fprintf(stderr, "%s\n", COMPILER_MESSAGE);
        				dispose_global();
					}
					qitem->GenValue.if_input->cond_expr = out->Tree;
					qitem->GenValue.if_input->true_queue = (struct ATQueue*) true_queue;
					qitem->GenValue.if_input->false_queue = (struct ATQueue*) false_queue;

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
						fprintf(stderr, "ERROR: Return can only be used in functions\n");
						return SYNTAX_ERROR;
					}

					// __<Expr>__ 
					qitem = (ATQItem*) malloc(sizeof(ATQItem));
					if (qitem == NULL)
					{
						fprintf(stderr, "%s\n", COMPILER_MESSAGE);
        				dispose_global();
					}
					qitem->GenType = gt_return;
					qitem->GenValue.return_input = (ReturnInput*) malloc(sizeof(ReturnInput));
					if (qitem->GenValue.return_input == NULL)
					{
						fprintf(stderr, "%s\n", COMPILER_MESSAGE);
        				dispose_global();
					}

					token = get_token();
					out = precedence_analysis(token);
					if (out == NULL) {
						return COMPILER_ERROR;
					}
					token = out->ReturnToken;

					if (token->type != type_eol) {
						fprintf(stderr, "ERROR: Unexpected token at the end of a return statement\n");
						return SYNTAX_ERROR;
					}
					// zkontrolujeme navratovy typ
					if ((return_value = check_type(func_symtab_item->type_strct.function->return_type, out->Type)) != OK) {
						fprintf(stderr, "ERROR: Wrong return value\n");
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
			if (qitem == NULL)
			{
				fprintf(stderr, "%s\n", COMPILER_MESSAGE);
        		dispose_global();
			}

			// <expr> / volani funkce
			token = get_token();
			if (token->type == type_id) {
				
				// nejdrive zkusime najit funkci v globalni tabulce symbolu
				symtab_item = symtab_search(global_symtab, token);
				if (symtab_item != NULL) {
					if (symtab_item->type != type_function) {
						// hledam v lokalni
						symtab_item = symtab_search(symtab, token);
						if (symtab_item == NULL) {
							fprintf(stderr, "ERROR: Use of undeclared variable\n");
							return SEMANTIC_ERROR; // ?????????????????????????????
						}
					}
				}
				else {
					// hledam v lokalni
					symtab_item = symtab_search(symtab, token);
					if (symtab_item == NULL) {
						fprintf(stderr, "ERROR: Use of undeclared variable\n");
						return SEMANTIC_ERROR; // ?????????????????????????????
					}
				}

				if (symtab_item->type == type_function) {
					// __VOLANI_FUNKCE__

					if ((return_value = check_type(symtab_item_left->type_strct.variable->type, 
													symtab_item->type_strct.function->return_type)) != OK)
					{
						fprintf(stderr, "Return type of function %s and type of identifier %s differ\n", symtab_item->key, symtab_item_left->key);
						return SEMANTIC_TYPE_ERROR;
					}
					// __(__
					token = get_token();
					if (token->type != type_operator) {
						fprintf(stderr, "ERROR: Unexpected symbol after function identifier, did you mean to call the function like: f_id(params)?\n");
						return SYNTAX_ERROR;
					}
					if (token->atribute.int_value != op_bracket) {
						fprintf(stderr, "ERROR: Unexpected symbol after function identifier, did you mean to call the function like: f_id(params)?\n");
						return SYNTAX_ERROR;
					}

					qitem->GenType = gt_call_func;
					qitem->GenValue.call_func_input = (CallFuncInput*) malloc(sizeof(CallFuncInput));
					qitem->GenValue.call_func_input->id = symtab_item_left;
					qitem->GenValue.call_func_input->sym_item = symtab_item;
					qitem->GenValue.call_func_input->param = (eQueue*) malloc(sizeof(eQueue));

					equeInit(qitem->GenValue.call_func_input->param);

					// ___<args_list>__
					token = get_token();
					if ((return_value = Args_list(symtab_item, qitem->GenValue.call_func_input->param)) != OK) {
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
					top_queue = qstackTop(qstack);
					queUp(top_queue, qitem);
					
					return OK;
				}

			}
			qitem->GenType = gt_assign;
			qitem->GenValue.assign_input = (AssignInput *) malloc(sizeof(AssignInput));
			if (qitem->GenValue.assign_input == NULL)
			{
				fprintf(stderr, "%s\n", COMPILER_MESSAGE);
        		dispose_global();
			}
			qitem->GenValue.assign_input->id = symtab_item_left;

			// __<Expr>__
			out = precedence_analysis(token);
			if (out == NULL) {
				return COMPILER_ERROR;
			}
			token = out->ReturnToken;
			if ((return_value = check_type(symtab_item->type_strct.variable->type, out->Type)) != OK) {
				return return_value;
			}

			if (token->type != type_eol) {
				fprintf(stderr, "ERROR: Unexpected token at the end of assignment\n");
				return SYNTAX_ERROR;
			}
			qitem->GenValue.assign_input->expr = out->Tree;
			top_queue = qstackTop(qstack);
			queUp(top_queue, qitem);

		break;

		default:
			fprintf(stderr,"ERROR: Wrong statement\n");
			return SYNTAX_ERROR;
	}
	return OK;
}

int Args_list(Tsymtab_item* symtab_item, eQueue* eque)
{
	int args_iter = 0;
	int return_value;

	switch (token->type)
	{
		case type_operator:
			// EPSILON
			if (token->atribute.int_value == op_bracket_end)
				break;
			else {
				fprintf(stderr, "ERROR: Invalid argument in function %s\n", symtab_item->key);
				return SYNTAX_ERROR;
			}
		break;
		case type_id:
		case type_integer:
		case type_double:
		case type_string:
			// __<Arg>__
			if ((return_value = Arg(symtab_item->type_strct.function, &args_iter, eque)) != OK) {
				return return_value;
			}
			// ___<Next_arg>__
			token = get_token();
			if ((return_value = Next_arg(symtab_item->type_strct.function, &args_iter, eque)) != OK) {
				return return_value;
			}
		break;
		
		default:
			fprintf(stderr, "ERROR: Invalid argument in function %s\n", symtab_item->key);
			return SYNTAX_ERROR;
		}

	if (args_iter != symtab_item->type_strct.function->arg_count) {
		fprintf(stderr, "ERROR: Too few arguments when calling function '%s'\n", symtab_item->key);
		return SEMANTIC_TYPE_ERROR;
	}
	return OK;
}

int Arg(Tfunction_item* function, int* args_iter, eQueue* eque)
{
	int return_value;
	Tsymtab_item* symtab_item;
	eQItem* eitem;

	if (*args_iter >= function->arg_count) {
		fprintf(stderr, "ERROR: Too many arguments in function call\n");
		return SEMANTIC_TYPE_ERROR;
	}

	eitem = (eQItem*) malloc(sizeof(eQItem));
	eitem->Next = NULL;
	switch (token->type)
	{
		case type_integer:
			// musime porovnat parametry
			if ((return_value = check_type(function->arguments[*args_iter].type_strct.variable->type, type_int)) != OK) {
				fprintf(stderr, "ERROR: Wrong type of argument in function call\n");
				return SEMANTIC_TYPE_ERROR;
			}
			eitem->etype = eq_token;
			eitem->eValue.token_value = token;

		break;

		case type_double:
			if ((return_value = check_type(function->arguments[*args_iter].type_strct.variable->type, type_doub)) != OK) {
				fprintf(stderr, "ERROR: Wrong type of argument in function call\n");
				return SEMANTIC_TYPE_ERROR;
			}
			eitem->etype = eq_token;
			eitem->eValue.token_value = token;
		break;

		case type_string:
			if (function->arguments[*args_iter].type_strct.variable->type != type_str) {
				fprintf(stderr, "ERROR: Wrong type of argument in function call\n");
				return SEMANTIC_TYPE_ERROR;
			}
			eitem->etype = eq_token;
			eitem->eValue.token_value = token;
		break;

		case type_id:
			if ((symtab_item = symtab_search(symtab, token)) == NULL) {
				fprintf(stderr, "ERROR: Undeclared variable as function argument\n");
				return SEMANTIC_TYPE_ERROR;
			}
			if ((return_value = check_type(function->arguments[*args_iter].type_strct.variable->type, 
											symtab_item->type_strct.variable->type)) != OK)
			{
				fprintf(stderr, "ERROR: Wrong type of argument in function call\n");
				return SEMANTIC_TYPE_ERROR;
			}
			eitem->etype = eq_id;
			eitem->eValue.id_value = symtab_item;
		break;

		default:
			fprintf(stderr, "ERROR: Compiler error, nobody is perfect...\n");
			return COMPILER_ERROR;
	}

	equeUp(eque, eitem);
	(*args_iter)++;
	return OK;

}

int Next_arg(Tfunction_item* function, int* args_iter, eQueue* eque)
{
	int return_value;

	switch (token->type)
	{
		case type_operator:
			// EPSILON
			if (token->atribute.int_value == op_bracket_end)
				return OK;
			else {
				fprintf(stderr, "ERROR: Unexpected token in function call arguments\n");
				return SYNTAX_ERROR;
			}
		break;

		case type_comma:
			// __ , <Arg> <Next_arg>__
			// __<Arg>__
			token = get_token();
			if ((return_value = Arg(function, args_iter, eque)) != OK) {
				return return_value;
			}
			// __<Next_arg>__
			token = get_token();
			if ((return_value = Next_arg(function, args_iter, eque)) != OK) {
				return return_value;
			}
		break;

		default:
			fprintf(stderr, "ERROR: Unexpected token in function call arguments\n");
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
		fprintf(stderr,"ERROR: Wrong symbol where assignment was expected\n");
		return SYNTAX_ERROR;
	}
	if (token->atribute.operator_value != op_assign) {
		fprintf(stderr,"ERROR: Wrong symbol where assignment was expected\n");
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
	int param_count = 0;

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
						functions_inserted++;

						// GENEROVANI
						qitem = (ATQItem*) malloc(sizeof(ATQItem));
						if (qitem == NULL)
						{
							fprintf(stderr, "%s\n", COMPILER_MESSAGE);
        					dispose_global();
						}

						//input->queue = NULL;
						qitem->GenType = gt_func_declar;
						qitem->GenValue.func_declar_input = (FuncDeclarInput *) malloc(sizeof(FuncDeclarInput));
						if (qitem->GenValue.func_declar_input == NULL)
						{
							fprintf(stderr, "%s\n", COMPILER_MESSAGE);
        					dispose_global();
						}
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
					if (func_queue == NULL)
					{
						fprintf(stderr, "%s\n", COMPILER_MESSAGE);
        				dispose_global();
					}
					qitem->GenValue.func_declar_input->queue = (struct ATQueue *)func_queue;
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
					param_count = 0;

					// __<param_list>__
					token = get_token();
					if ((return_value = Param_list(symtab_item, &param_count)) != OK) {
						return return_value;
					}

					// __)__
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
					// pokud jeste nebyla deklarovana
					if (!symtab_item->type_strct.function->declared) {
						// nastavime navratovou hodnotu funkce
						if ((return_value = set_return(symtab_item->type_strct.function)) != OK) {
							return return_value;
						}
						// ulozime pocet parametru
						symtab_item->type_strct.function->arg_count = param_count;
					}
					else {
						// zkontrolujeme navratovou hodnotu
						if ((return_value = check_param_type(symtab_item->type_strct.function->return_type)) != OK) {
							fprintf(stderr, "ERROR: Different return types in '%s' function definition and declaration\n", symtab_item->key);
							return SEMANTIC_TYPE_ERROR;
						}
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

					// vratime zpet
					symtab = temp;

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

					// nastavime funkci na defined
					symtab_item->type_strct.function->defined = true;
					functions_defined++;

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
						if (symtab_item->type_strct.function->defined) {
							fprintf(stderr, "ERROR: Function %s defined before its declaration\n", symtab_item->key);
							return SEMANTIC_ERROR;
						}
					}
					else {
						// jinak vlozime id do tabulky symbolu
						symtab_item = symtab_insert(symtab, token, type_function);
						functions_inserted++;
					}
					
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
					param_count = 0;

					// __<param_list>__
					token = get_token();
					if ((return_value = Param_list(symtab_item, &param_count)) != OK) {
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
					// nastavime pocet parametru
					symtab_item->type_strct.function->arg_count = param_count;
					// nastavime, ze funkce jiz byla deklarovana
					symtab_item->type_strct.function->declared = true;


					// _______GENERATOR________
					// nastavime qitem
					qitem = (ATQItem*) malloc(sizeof(ATQItem));
					if (qitem == NULL)
					{
						fprintf(stderr, "%s\n", COMPILER_MESSAGE);
        				dispose_global();
					}

					qitem->GenType = gt_func_declar;
					qitem->GenValue.func_declar_input = (FuncDeclarInput*) malloc(sizeof(FuncDeclarInput));
					if (qitem->GenValue.func_declar_input == NULL)
					{
						fprintf(stderr, "%s\n", COMPILER_MESSAGE);
        				dispose_global();
					}
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

int Param_list(Tsymtab_item *symtab_item, int* params_iter)
{
	int return_value;
	bool declared = symtab_item->type_strct.function->declared;

	switch (token->type)
	{
		case type_operator:
			if (token->atribute.int_value == op_bracket_end) {
				// EPSILON
				break;
			}
		break;

		case type_id:
			// __<Param>__
			if ((return_value = Param(symtab_item, params_iter)) != OK) {
				return return_value;
			}
			// __<Next_par>__
			token = get_token();
			if ((return_value = Next_par(symtab_item, params_iter)) != OK) {
				return return_value;
			}
		break;

		default:
			fprintf(stderr, "ERROR: Unexpected symbol in function '%s' parameters\n", symtab_item->key);
			return SYNTAX_ERROR;
	}
	if (declared) {
		if (symtab_item->type_strct.function->arg_count != *params_iter) {
			fprintf(stderr, "ERROR: Wrong number of parameters in function %s\n", symtab_item->key);
			return SEMANTIC_TYPE_ERROR;
		}
	}
	return OK;
}

int Param(Tsymtab_item *symtab_item, int* params_iter)
{
	bool declared = symtab_item->type_strct.function->declared;
	Tfunction_item* function = symtab_item->type_strct.function;
	Tsymtab_item* symtab_param;
	Token* param_token;
	int return_value;

	if (declared) {
		if (*params_iter >= symtab_item->type_strct.function->arg_count) {
			fprintf(stderr, "ERROR: Number of parameters in definition of function '%s' is bigger than in its declaration\n", symtab_item->key);
			return SEMANTIC_TYPE_ERROR;
		}
	}

	if (token->type != type_id) {
		fprintf(stderr, "ERROR: Invalid parameter in function '%s' \n", symtab_item->key);
		return SYNTAX_ERROR;
	}
	// ulozime si token na identifikator
	param_token = token;

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
	// pokud je jiz funkce deklarovana, pouze zkontrolujeme spravnost typu
	if (declared) {
		if ((return_value = check_param_type(function->arguments[*params_iter].type_strct.variable->type)) != OK) {
			fprintf(stderr, "ERROR: Parameter in function '%s' definition differs from its declaration\n", symtab_item->key);
			return return_value;
		}

	}
	else {
		// pokud jiz id je v tabulce symbolu pak dva parametry maji stejne jmeno
		if ((symtab_param = symtab_search(symtab, param_token)) != NULL) {
			fprintf(stderr, "ERROR: Function parameters cannot have same names, in function '%s' \n", symtab_item->key);
			return SEMANTIC_ERROR;
		}
		// jinak ulozime identifikator do tabulky symbolu
		symtab_param = symtab_insert(symtab, param_token, type_variable);
		// nastavime typ identifikatoru
		if ((return_value = set_type_variable(symtab_param)) != OK) {
			return return_value;
		}
		symtab_param->type = type_variable;
		// void init_item_variable(Tvariable_item *item) ???

		// nastavime parametr funkce
		Tvalue value;
		set_args_function(function, symtab_param->key, symtab_param->type_strct.variable->type, value);
	}

	// zvysime pocet zpracovanych parametru 
	(*params_iter)++;
	return OK;

}

int Next_par(Tsymtab_item* symtab_item, int* params_iter)
{
	int return_value;
	switch (token->type)
	{
		case type_operator:
			if (token->atribute.int_value == op_bracket_end) {
				// __EPSILON__
				return OK;
			}
			else {
				fprintf(stderr, "ERROR: Unexpected token in function '%s' definition\n", symtab_item->key);
				return SYNTAX_ERROR;
			}
		break;

		case type_comma:
			// , <param> <Next_par>
			token = get_token();
			if ((return_value = Param(symtab_item, params_iter)) != OK) {
				return return_value;
			}
			// __<Next_par>__
			token = get_token();
			if ((return_value = Next_par(symtab_item, params_iter)) != OK) {
				return return_value;
			}
		break;

		default:
			fprintf(stderr, "ERROR: Unexpected token in function '%s' definition/declaration\n", symtab_item->key);
			return SYNTAX_ERROR;

	}
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
			eQItem * eItem;
			eItem = (eQItem*) malloc(sizeof(eQItem));
			if (eItem == NULL)
			{
				fprintf(stderr, "%s\n", COMPILER_MESSAGE);
        		dispose_global();
			}
			eItem->Next = NULL;
			eItem->etype = eq_tree;
			eItem->eValue.tree_value = out->Tree;

			equeUp(exprs, eItem);

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
		fprintf(stderr,"ERROR: Data type does not exist\n");
		return SYNTAX_ERROR;
	}
	switch (token->atribute.int_value)
	{
		case kw_integer:
		case kw_double:
		case kw_string:
			return OK;
		default:
			fprintf(stderr,"ERROR: Data type does not exist\n");
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
		default:
			fprintf(stderr, "ERROR: Unknown identifier type in function definition/declaration\n");
			return SYNTAX_ERROR;
	}
	set_item_function(function, f_ret_value, function->sym_table);
	return OK;
}

// nastavi typ a inicializuje promennou v tabulce symbolu
int set_type_variable(Tsymtab_item* symtab_item)
{
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
		default:
			fprintf(stderr, "ERROR: Unknown type in variable declaration\n");
			return (SYNTAX_ERROR);
	}
	return OK;
}

int check_type(Tvariable_type type1, Tvariable_type type2)
{
	if (type2 != type1) {
		switch (type1) {
			case type_int:
				if (type2 == type_doub) {
					break;// upozornit generator aby to pretypoval
				}
				
			
			case type_doub:
				if (type2 == type_int) {
					break; // upozornit generator
				}
				
			
			default:
				fprintf(stderr, "ERROR: Trying to assign incompatible types\n");
				return SEMANTIC_TYPE_ERROR;

		}	
	
	}
	return OK;
}

int check_param_type(Tvariable_type type)
{
	if (token->type != type_keyword) {
		fprintf(stderr, "ERROR: Unexpected type token\n");
		return SYNTAX_ERROR;
	}

	switch (token->atribute.int_value) 
	{
		case kw_integer:
			if (type != type_int) {
				fprintf(stderr, "ERROR: Parameter types in function definition and declaration differ\n");
				return SEMANTIC_TYPE_ERROR;
			}
		break;

		case kw_double:
			if (type != type_doub) {
				fprintf(stderr, "ERROR: Parameter types in function definition and declaration differ\n");
				return SEMANTIC_TYPE_ERROR;
			}
		break;

		case kw_string:
			if (type != type_str) {
				fprintf(stderr, "ERROR: Parameter types in function definition and declaration differ\n");
				return SEMANTIC_TYPE_ERROR;
			}
		break;

		default:
			fprintf(stderr, "ERROR: Unknown data type in function parameter\n");
			return SYNTAX_ERROR;
	}
	return OK;
}

int parse()
{
	ATQueue* global_queue;
	global_queue = (ATQueue*) malloc(sizeof(ATQueue));	
	if (global_queue == NULL)
	{
		fprintf(stderr, "%s\n", COMPILER_MESSAGE);
        dispose_global();
	}

	qstack = (QStack*) malloc(sizeof(QStack));
	if (qstack == NULL)
	{
		fprintf(stderr, "%s\n", COMPILER_MESSAGE);
        dispose_global();
	}

	queInit (global_queue);
	qstackInit(qstack);
	qstackPush (qstack, global_queue);

	symtab = symtab_init(42);
	insert_built_in();
	global_symtab = symtab;
	return Prog();
	symtab_free(symtab); 
}




int main()
{
	int return_value;
	return_value = parse();
	fprintf(stderr,"\n");
	switch (return_value)
	{
		case OK:
			fprintf(stderr,"Everything is OK\n");
			break;
		case LEXICAL_ERROR:
			fprintf(stderr,"LEXICAL_ERROR\n");
			break;
		case SYNTAX_ERROR:
			fprintf(stderr,"SYNTAX_ERROR\n");
			break;
		case SEMANTIC_ERROR:
			fprintf(stderr,"SEMANTIC_ERROR\n");
			break;
		case SEMANTIC_TYPE_ERROR:
			fprintf(stderr,"SEMANTIC_TYPE_ERROR\n");
			break;
		case OTHER_ERROR:
			fprintf(stderr,"OTHER_ERROR\n");
			break;
		case COMPILER_ERROR:
			fprintf(stderr,"COMPILER_ERROR\n");
			break;
	}
	generate_start(qstackTop(qstack));
	dispose_global();
	return 0;
}
