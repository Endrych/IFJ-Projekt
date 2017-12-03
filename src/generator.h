/* Nazev - Implementace prekladace imperativniho jazyka IFJ17
 * Petr Zubalik - xzubal04
 * Marek Kukucka - xkukuc04
 * Jan Koci - xkocij01
 * David Endrych - xendry02
 */

#include "abstract_tree.h"
#include "at_que.h"

#ifndef GENERATOR_H
#define GENERATOR_H

//
typedef enum{
    gt_variable,
    gt_label
}GType;

/**
 * Funkce vygeneruje nazev pro promenou nebo label
 * @param  type Parametr urcuje jestli vygenerovane jmeno ma vypadat jako promena nebo label
 * @return funkce vraci ukazatel na pole ve kterem je ulozen vygenerovany nazev
 */
char * generate_name(GType type);

/**
 * Funkce generuje vestavenou funkci Asc
 *
 */
void generate_Asc();

/**
 * Funkce generuje vestavenou funkci Length
 *
 */
void generate_Length();

/**
 * Funkce generuje vestavenou funkci SubStr
 *
 */
void generate_SubStr();

/**
 * Funkce generuje vestavenou funkci Chr
 *
 */
void generate_Chr();

/**
 * Funkce postupne zavola generatory vestavenych funkci
 */
void generate_built_in();

/**
 * [generate_main description]
 * @param queue [description]
 */
void generate_main(ATQueue * queue);

/**
 * [generate_start description]
 * @param queue [description]
 */
void generate_start(ATQueue *queue);

/**
 * [generate_program description]
 * @param queue [description]
 */
void generate_program(ATQueue *queue);

/**
 * [generate_variable_declaration description]
 * @param id        [description]
 * @param expr      [description]
 * @param expr_type [description]
 */
void generate_variable_declaration(Tsymtab_item * id, ATLeaf * expr, Tvariable_type expr_type);

/**
 * [generate_assign description]
 * @param id        [description]
 * @param expr      [description]
 * @param expr_type [description]
 */
void generate_assign(Tsymtab_item* id, ATLeaf * expr, Tvariable_type expr_type);

/**
 * [generate_input description]
 * @param id [description]
 */
void generate_input(Tsymtab_item * id);

/**
 * [generate_print description]
 * @param exprs [description]
 */
void generate_print(eQueue * exprs);

/**
 * [generate_call_function description]
 * @param id       [description]
 * @param sym_item [description]
 * @param param    [description]
 */
void generate_call_function(Tsymtab_item * id, Tsymtab_item * sym_item, eQueue * param);

/**
 * [generate_return description]
 * @param sym_item [description]
 * @param expr     [description]
 */
void generate_return(Tsymtab_item * sym_item, PrecendentOutput * expr);

/**
 * [open_output description]
 */
void open_output();

/**
 * [generate_expression description]
 * @param  leaf [description]
 * @return      [description]
 */
char * generate_expression(ATLeaf *leaf);

/**
 * [generate_if description]
 * @param condition   [description]
 * @param state_true  [description]
 * @param state_false [description]
 */
void generate_if(ATLeaf * condition, ATQueue * state_true, ATQueue * state_false);

/**
 * [generate_condition description]
 * @param leaf  [description]
 * @param label [description]
 */
void generate_condition(ATLeaf *leaf, char* label);

/**
 * [generate_while description]
 * @param condition [description]
 * @param state     [description]
 */
void generate_while(ATLeaf * condition, ATQueue * state);

/**
 * [generate_function description]
 * @param item  [description]
 * @param state [description]
 */
void generate_function(Tsymtab_item * item, ATQueue * state);
#endif
