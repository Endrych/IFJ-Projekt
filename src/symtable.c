#include "symtable.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Tsymtab_item *symtab_search(Tsymtab *sym_table, char *key)
{
	unsigned int index = hash_func(key) % sym_table->size;
	Tsymtab_item *temp = sym_table->symtab_list[index];

	while(temp)
	{
		temp->found = false;
		if ((strcmp(temp->key, key)) == 0)
		{
			temp->found = true;
			return temp;
		}

		if (temp->next == NULL)
		{
			return NULL;
		}

		temp = temp->next;
	}
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
			free(current_item);
		}
	}

	free(sym_table);
}


Tsymtab_item *symtab_insert(Tsymtab *sym_table,  char *key, Token token)
{	
	unsigned int index = hash_func(key) % sym_table->size;
	Tsymtab_item *temp = symtab_search(sym_table, key);

	if (temp == NULL)
	{
		// allocate memory for new item
		Tsymtab_item *new_item = (Tsymtab_item *) malloc(sizeof(*new_item));
		if (new_item == NULL)
		{	
			fprintf(stderr, "Memory allocation failed\n");
			return NULL;
		}

		/*new_item->key = (char *) malloc(sizeof(char) * strlen(key) + 1);
		memcpy(new_item->key, key, strlen(key));
		new_item->key[strlen(key)] = '\0';*/

		new_item->key = key;
		new_item->token = token;

		// store the item at the end of the list
		if (temp != NULL)
		{
			temp->next = new_item;
		} else {

			sym_table->symtab_list[index] = new_item;
		}

		return new_item;
	} else
	{
		return temp;
	}


}

bool symtab_delete(Tsymtab *sym_table,  char *key)
{
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
			free(current);
			return true;
		}

		previous = temp;
		temp = temp->next;
	}
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
int main()
{	
	Token token;
	Tsymtab *tab = symtab_init(10);

	symtab_insert(tab, "ahoj", token);

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
	symtab_free(tab);
	return 0;
}