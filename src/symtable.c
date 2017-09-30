#include "symtable.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Tsymtab *symtab_init(unsigned int size) 
{
	// allocate memory for the table
	Tsymtab *sym_table = (Tsymtab *) malloc(sizeof(*sym_table) + size * sizeof(sym_table->symtab_list[0]));
	if (sym_table == NULL)
	{	// if allocation failed, return 99
		exit(99);
	}
	// save size of the table to the structure
	sym_table->size = size;

	// every pointer points to NULL 
	for (unsigned int i = 0; i < sym_table->size; i++)
	{
		sym_table->symtab_list[i] = NULL;
	}
	// return the table
	return sym_table;

}

void symtab_free(Tsymtab *sym_table)
{	// temporary pointer 
	Tsymtab_item *temp_item;
	// for the whole table
	for (unsigned int i = 0; i < sym_table->size; i++)
	{	// save current list to the temporary variable
		temp_item = sym_table->symtab_list[i];
		// if the list is empty, go on
		if (temp_item == NULL)
		{
			continue;
		}
		// deallocate memory of all items in the current list
		while(temp_item != NULL)
		{
			Tsymtab_item *current_item = temp_item;
			temp_item = temp_item->next;
			free(current_item->key);
			free(current_item);
		}
	}
	// deallocate the memory of the table itself
	free(sym_table);
}

Tsymtab_item *symtab_insert(Tsymtab *sym_tab, const char *key/*, Token token*/)
{	// calculating of the index 
	unsigned int index = hash_func(key) % sym_tab->size;
	// store a list on the index to temporary variable
	Tsymtab_item *temp = sym_tab->symtab_list[index];
	// until the end of the list
	while(temp != NULL)
	{	
		if ((strcmp(temp->key, key)) == 0)
		{	// i found an item, it's already in the table
			return temp;
		}
		// if I'm at the end of the list
		if (temp->next == NULL)
		{	// i didn't found an item and i have to create new one
			break;
		}
		// move forward in the list
		temp = temp->next;
	}
	// allocate memory for new item
	Tsymtab_item *new_item = (Tsymtab_item *) malloc(sizeof(*new_item));
	if (new_item == NULL)
	{	// if allocation failed
		exit(99);
	}
	new_item->key = (char *) malloc(sizeof(char) * strlen(key));
	// copy the name of id to the item 
	strncpy(new_item->key, key, strlen(key));
	// store the token 
	/*new_item->token = token;*/
	// store the item at the end of the list
	if (temp != NULL)
	{
		temp->next = new_item;
	} else {
		// the list is empty so store it at the first place
		sym_tab->symtab_list[index] = new_item;
	}
	// return new item
	return new_item;



}
/*
bool symtab_delete(Tsymtab *sym_tab, const char *key)
{

}
*/
// sdbm hash function / murmur hash function
unsigned int hash_func(const char *key)
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
	Tsymtab *tab = symtab_init(10);
	symtab_insert(tab, "ahoj");
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