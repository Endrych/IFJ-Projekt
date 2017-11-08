/* project - IFJ17
 * part - symbol table
 * Petr Zubalik, Marek Kukucka, Jan Koci, David Endrych
 * file - symtable.h
 */

#include "token.h"
#include <stdbool.h>

// if not defined, define
#ifndef SYMTAB_H
#define SYMTAB_H

typedef struct symtab_item {
	Token token;
	struct symtab_item *next;
	char *key;
	bool found;
} Tsymtab_item;

typedef struct symtable {
	unsigned int size;
	Tsymtab_item *symtab_list[];
} Tsymtab;

// hash function to gain an index of item in array of pointers
unsigned int hash_func( char *);

// initialize function
Tsymtab *symtab_init(unsigned int);

// function that inserts an item to the table 
Tsymtab_item *symtab_insert(Tsymtab *sym_table,  int position, Token token);

// function that searches for an item in the table
Tsymtab_item *symtab_search(Tsymtab *sym_table,  int position);

// delete and dealocate the table
void symtab_free(Tsymtab *sym_table);

// delete and dealocate one item from the table
bool symtab_delete(Tsymtab *sym_table,  int position);

#endif /* SYMTAB_H */