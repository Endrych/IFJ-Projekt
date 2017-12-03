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

/**
 * [parse description]
 * @return [description]
 */
int parse();

/**
 * [Prog description]
 * @return [description]
 */
int Prog();

/**
 * [Scope description]
 * @return [description]
 */
int Scope();

/**
 * [St_list description]
 * @return [description]
 */
int St_list();

/**
 * [Stat description]
 * @return [description]
 */
int Stat();

/**
 * [Else description]
 * @return [description]
 */
int Else();

/**
 * [Assign description]
 * @return [description]
 */
int Assign();

/**
 * [Func description]
 * @return [description]
 */
int Func();

/**
 * [Param_list description]
 * @param  symtab_item [description]
 * @param  params_iter [description]
 * @param  declaration [description]
 * @return             [description]
 */
int Param_list(Tsymtab_item *symtab_item, int* params_iter, bool declaration);

/**
 * [Param description]
 * @param  symtab_item [description]
 * @param  params_iter [description]
 * @param  declaration [description]
 * @return             [description]
 */
int Param(Tsymtab_item *symtab_item, int* params_iter, bool declaration);

/**
 * [Next_par description]
 * @param  symtab_item [description]
 * @param  params_iter [description]
 * @param  declaration [description]
 * @return             [description]
 */
int Next_par(Tsymtab_item* symtab_item, int* params_iter, bool declaration);

/**
 * [ExprPrint description]
 * @param  exprs [description]
 * @return       [description]
 */
int ExprPrint(eQueue* exprs);

/**
 * [Tyype description]
 * @return [description]
 */
int Tyype();

/**
 * [set_return description]
 * @param  function [description]
 * @return          [description]
 */
int set_return(Tfunction_item *function);

/**
 * [set_type_variable description]
 * @param  symtab_item [description]
 * @return             [description]
 */
int set_type_variable(Tsymtab_item* symtab_item);

/**
 * [check_type description]
 * @param  type1 [description]
 * @param  type2 [description]
 * @return       [description]
 */
int check_type(Tvariable_type type1, Tvariable_type type2);

/**
 * [check_param_type description]
 * @param  type [description]
 * @return      [description]
 */
int check_param_type(Tvariable_type type);

/**
 * [Args_list description]
 * @param  symtab_item [description]
 * @param  eque        [description]
 * @return             [description]
 */
int Args_list(Tsymtab_item* symtab_item, eQueue* eque);

/**
 * [Arg description]
 * @param  function  [description]
 * @param  args_iter [description]
 * @param  eque      [description]
 * @return           [description]
 */
int Arg(Tfunction_item* function, int* args_iter, eQueue* eque);

/**
 * [Next_arg description]
 * @param  function  [description]
 * @param  args_iter [description]
 * @param  eque      [description]
 * @return           [description]
 */
int Next_arg(Tfunction_item* function, int* args_iter, eQueue* eque);

#endif
