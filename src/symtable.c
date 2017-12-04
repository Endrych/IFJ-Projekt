/* Nazev - Implementace prekladace imperativniho jazyka IFJ17
 * Petr Zubalik - xzubal04
 * Marek Kukucka - xkukuc04
 * Jan Koci - xkocij01
 * David Endrych - xendry02
 */

#include "symtable.h"
#include "string_storage.h"
#include "set_values.h"
#include "destructor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

Tsymtab * symtab;

Tsymtab_item *symtab_search(Tsymtab *sym_table, Token *token)
{
	int position = token->atribute.int_value;
	char *key = get_string(position);
	unsigned int index = hash_func(key) % sym_table->size;
	Tsymtab_item *temp = sym_table->symtab_list[index];

	while(temp)
	{	
		if ((strcmp(temp->key, key)) == 0)
		{	
			free(key);
			return temp;
		}

		if (temp->next == NULL)
		{
			free(key);
			return NULL;
		}

		temp = temp->next;
	}
	free(key);
	return NULL;
}


Tsymtab *symtab_init(unsigned int size) 
{

	Tsymtab *sym_table = (Tsymtab *) malloc(sizeof(*sym_table) + size * sizeof(sym_table->symtab_list[0]));
	if (sym_table == NULL)
	{	
		fprintf(stderr, "%s\n", COMPILER_MESSAGE);
        dispose_global();
	}

	sym_table->size = size;


	for (unsigned int i = 0; i < sym_table->size; i++)
	{
		sym_table->symtab_list[i] = NULL;
	}
	return sym_table;

}

void symtab_free(Tsymtab *sym_table)
{	
	Tsymtab_item *temp_item;

	for (unsigned int i = 0; i < sym_table->size; i++)
	{	
		temp_item = sym_table->symtab_list[i];

		if (temp_item == NULL)
		{
			continue;
		}

		while(temp_item != NULL)
		{
			Tsymtab_item *current_item = temp_item;
			temp_item = temp_item->next;

			if (current_item->type == type_variable)
			{
				free(current_item->type_strct.variable);

			} else if (current_item->type == type_function)
			{	
				for (int i = 0; i < current_item->type_strct.function->arg_count; i++)
				{	
					//free(current_item->type_strct.function->arguments[i].key);
					free(current_item->type_strct.function->arguments[i].type_strct.variable);
				}
				free(current_item->type_strct.function->arguments);
				symtab_free(current_item->type_strct.function->sym_table);
				free(current_item->type_strct.function);
			}

			free(current_item->key);
			free(current_item);
		}
	}

	free(sym_table);
}



Tsymtab_item *symtab_insert(Tsymtab *sym_table, Token *token, Telement_type type)
{	

	int position = token->atribute.int_value;
	char *key = get_string(position);
	unsigned int index = hash_func(key) % sym_table->size;
	Tsymtab_item *temp = symtab_search(sym_table, token);

	if (temp == NULL)
	{
		// allocate memory for new item
		Tsymtab_item *new_item = (Tsymtab_item *) malloc(sizeof(Tsymtab_item));
		if (new_item == NULL)
		{	
			fprintf(stderr, "%s\n", COMPILER_MESSAGE);
        	dispose_global();
		}

		if (type == type_variable)
		{
			new_item->type_strct.variable = (struct var_item *) malloc(sizeof(struct var_item));
			if (new_item->type_strct.variable == NULL)
			{
				fprintf(stderr, "%s\n", COMPILER_MESSAGE);
        		dispose_global();
			}
		} else if (type == type_function)
		{
			new_item->type_strct.function = (struct fc_item *) malloc(sizeof(struct fc_item ));
			if (new_item->type_strct.function == NULL)
			{
				fprintf(stderr, "%s\n", COMPILER_MESSAGE);
       			dispose_global();
			}
			new_item->type_strct.function->sym_table = symtab_init(30); 
		}


		new_item->key = key;
		new_item->type = type;

		if (type == type_variable)
		{	
			new_item->type_strct.variable->declared = true;
		} else if (type == type_function)
		{
			new_item->type_strct.function->declared = false;
			new_item->type_strct.function->defined = false;
		}

		// empty list
		if (sym_table->symtab_list[index] == NULL)
		{
			sym_table->symtab_list[index] = new_item;
			new_item->next = NULL;
		} else 
		{	// on the beginning of the list
			new_item->next = sym_table->symtab_list[index];
			sym_table->symtab_list[index] = new_item;
		}
		return new_item;
	} else
	{	
		free(key);
		return temp;
	}


}

bool symtab_delete(Tsymtab *sym_table,  Token *token)
{
	char *key = get_string(token->atribute.int_value);
	unsigned int index = hash_func(key) % sym_table->size;
	Tsymtab_item *temp = sym_table->symtab_list[index];
	Tsymtab_item *previous = NULL;

	while(temp)
	{
		if ((strcmp(key, temp->key)) == 0)
		{
			if (previous != NULL)
			{
				previous->next = temp->next;
			} else
			{
				sym_table->symtab_list[index] = temp->next;
			}
			
			Tsymtab_item *current = temp;
			free(key);
			free(current);
			return true;
		}

		previous = temp;
		temp = temp->next;
	}

	free(key);
	return false;
}

// sdbm hash function / murmur hash function
unsigned int hash_func( char *key)
{
	unsigned int hash = 0;
	const unsigned char *p = NULL;
	// calculation of sdbm hash
	for (p =(const unsigned char *) key; *p != '\0'; p++)
	{	// 65599 is a magic constant that was gained empirically
		hash = 65599 * hash + *p;
	}
	// return final hash
	return hash;
}
