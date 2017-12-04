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
 * nastavi hodnoty promenne ulozene v tabulce symbolu 
 * @param item  ukazatel na dany item v tabulce symbolu
 * @param value hodnota ukladana do daneho symbolu
 * @param type  typ symbolu
 */
void set_item_variable(Tvariable_item *item, Tvalue value, Tvariable_type type);

/**
 * nastavi hodnoty symbolu funkce v tabulce symbolu
 * @param item        ukazatel na dany item v tabulce symbolu
 * @param return_type typ navratove hodnoty
 * @param sym_table   ukazatel na tabulku symbolu
 */
void set_item_function(Tfunction_item *item, Tvariable_type return_type, Tsymtab *sym_table);

/**
 * nastavi hodnoty argumentu dane funkce a priradi je k funkci v tabulce symbolu
 * @param item  ukazatel na dany item v tabulce symbolu
 * @param key   identifikator parametru
 * @param type  typ parametru
 * @param value hodnota parametru
 */
void set_args_function(Tfunction_item *item, char *key, Tvariable_type type, Tvalue value);

/**
 * [uvolni pamet argumentu dane funkce
 * @param item ukazatel na item v tabulce symbolu
 */
void free_args_function(Tfunction_item *item);

/**
 * inicializuje hodnoty symbolu promenne v tabulce
 * @param item ukazatel na item v tabulce symbolu
 */
void init_item_variable(Tvariable_item *item);

/**
 * inicializuje hodnoty symbolu funkce v tabulce
 * @param item ukazatel na item v tabulce symbolu
 */
void init_item_function(Tfunction_item *item);

#endif /* SET_VALUES_H */
