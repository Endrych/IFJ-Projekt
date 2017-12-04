/* Nazev - Implementace prekladace imperativniho jazyka IFJ17
 * Petr Zubalik - xzubal04
 * Marek Kukucka - xkukuc04
 * Jan Koci - xkocij01
 * David Endrych - xendry02
 */

#include "token.h"
#include <stdbool.h>

#ifndef SYMTAB_H
#define SYMTAB_H

struct symtable;

struct symtab_item;


// Pro ulozeni inicializacni hodnoty
typedef union{
    char *string;
    int value_int;
    double value_double;
} Tvalue;

// type of the variable
typedef enum {
	type_bool,
	type_int,
	type_doub,
	type_str,
} Tvariable_type;

// type of a symbol
typedef enum {
	type_function,
	type_variable,
} Telement_type;

// structure for function arguments
typedef struct {
	Tvariable_type type;
	Tvalue value;
	char *key;
} Tfunction_argument;

//structure for variables
typedef struct var_item{
	bool declared;
	bool used;
	Tvalue value;
	//char *key;
	Tvariable_type type;
} Tvariable_item;

// structure for functions
typedef struct fc_item{
	bool declared;
	bool defined;
	Tvariable_type return_type;
	int arg_count;
	struct symtab_item *arguments;
	struct symtable *sym_table;
	//char *key;
} Tfunction_item;

// symbol for the table
typedef struct symtab_item {
	struct symtab_item *next;
	char *key;
	union {
		Tfunction_item *function;
		Tvariable_item *variable;
	} type_strct;
	Telement_type type;
} Tsymtab_item;

//
typedef struct symtable {
	unsigned int size;
	Tsymtab_item *symtab_list[];
} Tsymtab;

/**
 * hashovaci funkce pro zjisteni indexu v tabulce
 * @param  key klic pro hashovaci funkci
 * @return        finalni hash pro vypocet indexu
 */
unsigned int hash_func( char *key);

/**
 * inicializacni funkce
 * @param  int velikost tabulky
 * @return     ukazatel na tabulku symbolu
 */
Tsymtab *symtab_init(unsigned int size);

/**
 * funkce vlozi prvek na zacatek seznamu na danem indexu
 * pokud je uz prvek v tabulce, vrati ukazatel na dany prvek
 * @param  sym_table ukazatel na tabulku symbolu
 * @param  token     ukazatel na prislusny token
 * @param  type      typ prvku
 * @return           [description]
 */
Tsymtab_item *symtab_insert(Tsymtab *sym_table, Token *token, Telement_type type);


/**
 * funkce pro vyhledavani v tabulce
 * @param  sym_table ukazatel na tabulku symbolu
 * @param  token     ukazatel na prislusny token
 * @return           ukazatel na prvek z tabulky symbolu,
 *					 pokud nenalezen, tak NULL
 */
Tsymtab_item *symtab_search(Tsymtab *sym_table, Token *token);

/**
 * vymaze tabulku a dealokuje pamet tabulky
 * @param sym_table ukazatel na tabulku symbolu
 */
void symtab_free(Tsymtab *sym_table);

/**
 * vymaze a dealokuje pamet jednoho prvku
 * @param  sym_table ukazatel na tabulku symbolu
 * @param  token     ukazatel na prislusny token
 * @return           true, jestli nalezl a smazal
 *					 false, jestli nenalezl
 */
bool symtab_delete(Tsymtab *sym_table,  Token *token);

#endif /* SYMTAB_H */
