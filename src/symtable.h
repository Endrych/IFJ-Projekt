/* project - IFJ17
 * cast - symbol table
 * Petr Zubalik, Marek Kukucka, Jan Koci, David Endrych
 * soubor - symtable.h
 */

#include "token.h"
#include <stdbool.h>

#ifndef SYMTAB_H
#define SYMTAB_H

// pro ulozeni inicializacni hodnoty
typedef union{
    char *string;
    int value_int;
    double value_double;
} Value;

typedef struct symtab_item {
	Token *token;
	struct symtab_item *next;
	char *key;
	bool declared;
	bool used;
	Value value;
} Tsymtab_item;

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
Tsymtab_item *symtab_insert(Tsymtab *sym_table, Token *token);

// funkce pro vyhledavani v tabulce
Tsymtab_item *symtab_search(Tsymtab *sym_table, Token *token);

// vymaze tabulku a dealokuje pamet tabulky
void symtab_free(Tsymtab *sym_table);

// vymaze a dealokuje pamet jednoho prvku
bool symtab_delete(Tsymtab *sym_table,  Token *token);

#endif /* SYMTAB_H */