#include "set_values.h"
#include <stdlib.h>
#include <stdio.h>

// nastavi hodnoty promenne ulozene v tabulce symbolu
void set_item_variable(Tvariable_item *item, Tvalue value, Tvariable_type type)
{
	item->type = type;
	item->value = value;
}

// nastavi hodnoty symbolu funkce v tabulce symbolu
void set_item_function(Tfunction_item *item, Tvariable_type return_type, Tsymtab *sym_table)
{
	item->return_type = return_type;
	item->sym_table = sym_table;
}

// nastavi hodnoty argumentu dane funkce a priradi je k funkci v tabulce symbolu
void set_args_function(Tfunction_item *item, char *key, Tvariable_type type, Tvalue value)
{	
	(item->arg_count)++;

	if (item->arg_count == 1)
	{
		item->arguments = (Tfunction_argument *) malloc(sizeof(Tfunction_argument));
		if (item->arguments == NULL)
		{
			fprintf(stderr, "Memory allocation failed\n");
			return ;
		}
		(item->arguments[(item->arg_count)-1]).type = type;
		(item->arguments[(item->arg_count)-1]).key = key;
		(item->arguments[(item->arg_count)-1]).value = value;
	} else
	{
		item->arguments = (Tfunction_argument *) realloc(item->arguments, (sizeof(Tfunction_argument) * item->arg_count));
		if (item->arguments == NULL)
		{
			fprintf(stderr, "Memory allocation failed\n");
			return ;
		}
		item->arguments[(item->arg_count)-1].type = type;
		item->arguments[(item->arg_count)-1].key = key;
		item->arguments[(item->arg_count)-1].value = value;
	}
}

// uvolni pamet pro argumenty dane funkce
void free_args_function(Tfunction_item *item)
{
	free(item->arguments);
}

// inicializuje hodnoty symbolu promenne v tabulce
void init_item_variable(Tvariable_item *item)
{
	item->declared = false;
	item->used = false;
}

// inicializuje hodnoty symbolu funkce v tabulce
void init_item_function(Tfunction_item *item)
{
	item->declared = false;
	item->arg_count = 0;
	item->arguments = NULL;
	item->sym_table = NULL;

}