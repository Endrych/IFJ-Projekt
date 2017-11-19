#include "symtable.h"
#include "string_storage.h"
#include "set_values.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

Tsymtab_item *symtab_search(Tsymtab *sym_table, Token *token, Telement_type type)
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
			if (current_item->type == type_variable)
			{
				free(current_item->type_strct.variable);
			} else if (current_item->type == type_function)
			{
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
	Tsymtab_item *temp = symtab_search(sym_table, token, type);

	if (temp == NULL)
	{
		// allocate memory for new item
		Tsymtab_item *new_item = (Tsymtab_item *) malloc(sizeof(Tsymtab_item));
		if (new_item == NULL)
		{	
			fprintf(stderr, "Memory allocation failed\n");
			return NULL;
		}

		if (type == type_variable)
		{
			new_item->type_strct.variable = (struct var_item *) malloc(sizeof(struct var_item));
			if (new_item->type_strct.variable == NULL)
			{
				fprintf(stderr, "Memory allocation failed\n");
				return NULL;
			}
		} else if (type == type_function)
		{
			new_item->type_strct.function = (struct fc_item *) malloc(sizeof(struct fc_item ));
			if (new_item->type_strct.function == NULL)
			{
				fprintf(stderr, "Memory allocation failed\n");
				return NULL;
			}
		}


		new_item->key = key;
		new_item->type = type;

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
	Telement_type type = type_variable;
	Telement_type type2 = type_function;
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

	symtab_insert(tab, token_a, type);
	symtab_insert(tab, token_b, type);
	symtab_insert(tab, token_c, type);
	symtab_insert(tab, token_d, type);
	symtab_insert(tab, token_n, type2);
	symtab_insert(tab, token_o, type2);
	symtab_insert(tab, token_p, type2);
	symtab_insert(tab, token_q, type2);

	Tvalue value;
	value.value_int = 10;
	Tvariable_type type3;
	type3 = type_int;
	Tsymtab_item *temp = tab->symtab_list[1];

	set_item_variable(temp->type_strct.variable, value, type3);
	printf("%s  %d  %d\n", temp->key, temp->type_strct.variable->value.value_int, temp->type_strct.variable->type);

	Tsymtab_item *temp2 = tab->symtab_list[3];
	Tsymtab *table = symtab_init(101);
	init_item_function(temp2->type_strct.function);
	set_item_function(temp2->type_strct.function, type3, table);
	set_args_function(temp2->type_strct.function, "cus", type_int, value);
	set_args_function(temp2->type_strct.function, "cau", type_int, value);
	printf("%s->%s(%d)->%s(%d)\n", temp2->key, temp2->type_strct.function->arguments[0].key,\
								 temp2->type_strct.function->arguments[0].value.value_int, \
								 temp2->type_strct.function->arguments->key, \
								 temp2->type_strct.function->arguments->value.value_int);
	free_args_function(temp2->type_strct.function);
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
	symtab_free(table);
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
}*/

