/* Nazev - Implementace prekladace imperativniho jazyka IFJ17
 * Petr Zubalik - xzubal04
 * Marek Kukucka - xkukuc04
 * Jan Koci - xkocij01
 * David Endrych - xendry02
 */

#include <stdbool.h>
#include "precedence_sa.h"
#include "symtable.h"
#include "at_que.h"

#ifndef PARSER_H
#define PARSER_H

int parse();
int Prog();
int Scope();
int St_list();
int Stat();
int Else();
int Assign();
int Func();
int Param_list(Tsymtab_item *symtab_item, int* params_iter);
int Param(Tsymtab_item *symtab_item, int* params_iter);
int Next_par(Tsymtab_item* symtab_item, int* params_iter);
int ExprPrint(eQueue* exprs);
int Tyype();
int set_return(Tfunction_item *function);
int set_type_variable(Tsymtab_item* symtab_item);
int check_type(Tvariable_type type1, Tvariable_type type2);
int check_param_type(Tvariable_type type);
int Args_list(Tsymtab_item* symtab_item, eQueue* eque);
int Arg(Tfunction_item* function, int* args_iter, eQueue* eque);
int Next_arg(Tfunction_item* function, int* args_iter, eQueue* eque);

#endif
