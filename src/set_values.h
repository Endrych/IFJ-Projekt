/* Nazev - Implementace prekladace imperativniho jazyka IFJ17
 * Petr Zubalik - xzubal04
 * Marek Kukucka - xkukuc04
 * Jan Koci - xkocij01
 * David Endrych - xendry02
 */

#include "symtable.h"

#ifndef SET_VALUES_H
#define SET_VALUES_H

/**
 * [set_item_variable description]
 * @param item  [description]
 * @param value [description]
 * @param type  [description]
 */
void set_item_variable(Tvariable_item *item, Tvalue value, Tvariable_type type);

/**
 * [set_item_function description]
 * @param item        [description]
 * @param return_type [description]
 * @param sym_table   [description]
 */
void set_item_function(Tfunction_item *item, Tvariable_type return_type, Tsymtab *sym_table);

/**
 * [set_args_function description]
 * @param item  [description]
 * @param key   [description]
 * @param type  [description]
 * @param value [description]
 */
void set_args_function(Tfunction_item *item, char *key, Tvariable_type type, Tvalue value);

/**
 * [free_args_function description]
 * @param item [description]
 */
void free_args_function(Tfunction_item *item);

/**
 * [init_item_variable description]
 * @param item [description]
 */
void init_item_variable(Tvariable_item *item);

/**
 * [init_item_function description]
 * @param item [description]
 */
void init_item_function(Tfunction_item *item);

#endif /* SET_VALUES_H */
