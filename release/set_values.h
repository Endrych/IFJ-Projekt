/* Nazev - Implementace prekladace imperativniho jazyka IFJ17
 * Petr Zubalik - xzubal04
 * Marek Kukucka - xkukuc04
 * Jan Koci - xkocij01
 * David Endrych - xendry02
 */

#include "symtable.h"

#ifndef SET_VALUES_H
#define SET_VALUES_H

void set_item_variable(Tvariable_item *item, Tvalue value, Tvariable_type type);

void set_item_function(Tfunction_item *item, Tvariable_type return_type, Tsymtab *sym_table);

void set_args_function(Tfunction_item *item, char *key, Tvariable_type type, Tvalue value); 

void free_args_function(Tfunction_item *item);

void init_item_variable(Tvariable_item *item);

void init_item_function(Tfunction_item *item);

#endif /* SET_VALUES_H */