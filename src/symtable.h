/* project - IFJ17
 * cast - symbol table
 * Petr Zubalik, Marek Kukucka, Jan Koci, David Endrych
 * soubor - symtable.h
 */

#include "token.h"
#include <stdbool.h>

#ifndef SYMTAB_H
#define SYMTAB_H

struct symtable;


// pro ulozeni inicializacni hodnoty
typedef union{
    char *string;
    int value_int;
    double value_double;
} Tvalue;

// type of the variable
typedef enum {
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
	Tvariable_type return_type;
	int arg_count;
	Tfunction_argument *arguments;
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

/*
typedef struct symtab_item {
	Token *token;
	struct symtab_item *next;
	char *key;
	bool declared;
	bool used;
	Tvalue value;
	Tsymtab_item *param_list;
} Tsymtab_item;
*/

typedef struct symtable {
	unsigned int size;
	Tsymtab_item *symtab_list[];
} Tsymtab;

// hashovaci funkce pro zjisteni indexu v tabulce
unsigned int hash_func( char *);

// inicializacni funkce
Tsymtab *symtab_init(unsigned int);

// funkce vlozi prvek na zacatek seznamu na danem indexu
// pokud je uz prvek v tabulce, vrati ukazatel 
Tsymtab_item *symtab_insert(Tsymtab *sym_table, Token *token, Telement_type type);

// funkce pro vyhledavani v tabulce
Tsymtab_item *symtab_search(Tsymtab *sym_table, Token *token, Telement_type type);

// vymaze tabulku a dealokuje pamet tabulky
void symtab_free(Tsymtab *sym_table);

// vymaze a dealokuje pamet jednoho prvku
bool symtab_delete(Tsymtab *sym_table,  Token *token);

#endif /* SYMTAB_H */