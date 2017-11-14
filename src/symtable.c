#include "symtable.h"
#include "string_storage.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

Tsymtab_item *symtab_search(Tsymtab *sym_table, Token *token)
{
	int position = token->atribute.int_value;
	char *key = get_string(position);
	unsigned int index = hash_func(key) % sym_table->size;
	Tsymtab_item *temp = sym_table->symtab_list[index];

	while(temp)
	{	
		temp->declared = false;
		if ((strcmp(temp->key, key)) == 0)
		{
			temp->declared = true;
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
		fprintf(stderr, "Memory allocation failed!\n");
		return NULL;
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
			free(current_item->key);
			free(current_item);
		}
	}

	free(sym_table);
}



Tsymtab_item *symtab_insert(Tsymtab *sym_table, Token *token)
{	
	int position = token->atribute.int_value;
	char *key = get_string(position);
	unsigned int index = hash_func(key) % sym_table->size;
	Tsymtab_item *temp = symtab_search(sym_table, token);

	if (temp == NULL)
	{
		// allocate memory for new item
		Tsymtab_item *new_item = (Tsymtab_item *) malloc(sizeof(*new_item));
		if (new_item == NULL)
		{	
			fprintf(stderr, "Memory allocation failed\n");
			return NULL;
		}

		new_item->key = key;
		new_item->token = token;

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
/***************************************

 MAIN - POUZE NA ZKOUSKU FUNKCNOSTI

*/
/*
int main()
{	
	Token *token_a;
	token_a = malloc(sizeof(Token));
	Token *token_b;
	token_b = malloc(sizeof(Token));
	Token *token_c;
	token_c = malloc(sizeof(Token));
	Token *token_d;
	token_d = malloc(sizeof(Token));
	Token *token_n;
	token_n = malloc(sizeof(Token));
	Token *token_o;
	token_o = malloc(sizeof(Token));
	Token *token_p;
	token_p = malloc(sizeof(Token));
	Token *token_q;
	token_q = malloc(sizeof(Token));
	Tsymtab *tab = symtab_init(10);

	int a = add_string_to_storage("aaoj");
	token_a->type = type_string;
	(token_a->atribute).int_value = a; 
	int b = add_string_to_storage("aaoj");
	token_b->type = type_string;
	token_b->atribute.int_value = b;
	int c = add_string_to_storage("aaoj");
	token_c->type = type_string;
	token_c->atribute.int_value = c;
	int d = add_string_to_storage("adoj");
	token_d->type = type_string;
	token_d->atribute.int_value = d;
	int n = add_string_to_storage("anoj");
	token_n->type = type_string;
	token_n->atribute.int_value = n;
	int o = add_string_to_storage("aooj");
	token_o->type = type_string;
	token_o->atribute.int_value = o;
	int p = add_string_to_storage("zdar");
	token_p->type = type_string;
	token_p->atribute.int_value = p;
	int q = add_string_to_storage("aqoj");
	token_q->type = type_string;
	token_q->atribute.int_value = q;
	symtab_insert(tab, token_a);
	symtab_insert(tab, token_b);
	symtab_insert(tab, token_c);
	symtab_insert(tab, token_d);
	symtab_insert(tab, token_n);
	symtab_insert(tab, token_o);
	symtab_insert(tab, token_p);
	symtab_insert(tab, token_q);

	for (unsigned int i = 0; i < tab->size; i++)
	{
		Tsymtab_item *temp = tab->symtab_list[i];
		if (temp == NULL)
		{
				printf("NULL");
		}
		while(temp != NULL)
		{
	
			printf("%s ", temp->key);
			temp = temp->next;
		}
		printf("\n");
	}

	destruct_storage();
	symtab_free(tab);
	free(token_a);
	free(token_b);
	free(token_c);
	free(token_d);
	free(token_n);
	free(token_o);
	free(token_p);
	free(token_q);
	return 0;
}

*/