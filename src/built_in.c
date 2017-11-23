#include "built_in.h"
#include "error.h"
#include "string_storage.h"
#include "destructor.h"
#include "set_values.h"
#include "symtable.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


extern Tsymtab *symtab; 

void insert_length()
{	
	int position = add_string_to_storage("Length");

	Tsymtab_item *temp_item;

	Token *token = (Token *) malloc(sizeof(Token));
	if (token == NULL)
	{
		fprintf(stderr, "%s\n", COMPILER_MESSAGE);
        dispose_global();
	}

	token->atribute.int_value = position;
	Telement_type type = type_function;

	temp_item = symtab_insert(symtab, token, type);

	temp_item->type_strct.function->defined = true;
	temp_item->type_strct.function->declared = true;
	temp_item->type_strct.function->return_type = type_int;

	// parametr
	position = add_string_to_storage("s");

	token->atribute.int_value = position;
	type = type_variable;

	char *key = get_string(position);
	Tvalue value; 

	temp_item->type_strct.function->arg_count = 0;

	set_args_function(temp_item->type_strct.function, key, type_str, value);

	//temp_item->type_strct.function->sym_table = symtab_init(11);
	
	Tsymtab_item *param1 = symtab_insert(temp_item->type_strct.function->sym_table, token, type);

	param1->type_strct.variable->declared = true;
	param1->type_strct.variable->type = type_str;

	free(token);
}

void insert_substr()
{
	int position = add_string_to_storage("SubStr");
	
	Tsymtab_item *temp_item;

	Token *token = (Token *) malloc(sizeof(Token));
	if (token == NULL)
	{
		fprintf(stderr, "%s\n", COMPILER_MESSAGE);
        dispose_global();
	}

	token->atribute.int_value = position;
	
	Telement_type type = type_function;

	temp_item = symtab_insert(symtab, token, type);

	temp_item->type_strct.function->declared = true;
	temp_item->type_strct.function->defined = true;
	temp_item->type_strct.function->return_type = type_str;

	// prvni parametr
	position = add_string_to_storage("s");
	token->atribute.int_value = position;
	
	type = type_variable;

	char *key = get_string(position);

	Tvalue value;
	temp_item->type_strct.function->arg_count = 0;

	set_args_function(temp_item->type_strct.function, key, type_str, value);

	//temp_item->type_strct.function->sym_table = symtab_init(11);

	Tsymtab_item *param1 = symtab_insert(temp_item->type_strct.function->sym_table, token, type);
	
	param1->type_strct.variable->declared = true;
	param1->type_strct.variable->type = type_str;

	// druhy parametr 
	position = add_string_to_storage("i");
	token->atribute.int_value = position;
	
	type = type_variable;

	char *key2 = get_string(position);
	
	set_args_function(temp_item->type_strct.function, key2, type_int, value);
	
	Tsymtab_item *param2 = symtab_insert(temp_item->type_strct.function->sym_table, token, type);
	
	param2->type_strct.variable->declared = true;
	param2->type_strct.variable->type = type_int;

	// treti parametr

	position = add_string_to_storage("n");
	token->atribute.int_value = position;
	type = type_variable;
	char *key3 = get_string(position);

	set_args_function(temp_item->type_strct.function, key3, type_int, value);

	Tsymtab_item *param3 = symtab_insert(temp_item->type_strct.function->sym_table, token, type);
	
	param3->type_strct.variable->declared = true;
	param3->type_strct.variable->type = type_int;

	free(token);
}	

void insert_asc()
{
	int position = add_string_to_storage("Asc");

	Tsymtab_item *temp_item;

	Token *token = (Token *) malloc(sizeof(Token));
	if (token == NULL)
	{
		fprintf(stderr, "%s\n", COMPILER_MESSAGE);
        dispose_global();
	}

	token->atribute.int_value = position;
	Telement_type type = type_function;
	
	temp_item = symtab_insert(symtab, token, type);

	temp_item->type_strct.function->declared = true;
	temp_item->type_strct.function->defined = true;
	temp_item->type_strct.function->return_type = type_int;

	// prvni parametr
	position = add_string_to_storage("s");
	token->atribute.int_value = position;
	type = type_variable;
	char *key = get_string(position);
	Tvalue value;
	temp_item->type_strct.function->arg_count = 0;

	set_args_function(temp_item->type_strct.function, key, type_str, value);

	//temp_item->type_strct.function->sym_table = symtab_init(11);

	Tsymtab_item *param1 = symtab_insert(temp_item->type_strct.function->sym_table, token, type);
	param1->type_strct.variable->declared = true;
	param1->type_strct.variable->type = type_str;

	// druhy parametr
	position = add_string_to_storage("i");
	token->atribute.int_value = position;
	type = type_variable;
	char *key2 = get_string(position);

	set_args_function(temp_item->type_strct.function, key2, type_int, value);

	Tsymtab_item *param2 = symtab_insert(temp_item->type_strct.function->sym_table, token, type);
	param2->type_strct.variable->declared = true;
	param2->type_strct.variable->type = type_int;

	free(token);
}

void insert_chr()
{
	int position = add_string_to_storage("Chr");

	Tsymtab_item *temp_item;

	Token *token = (Token *) malloc(sizeof(Token));
	if (token == NULL)
	{
		fprintf(stderr, "%s\n", COMPILER_MESSAGE);
        dispose_global();
	}

	token->atribute.int_value = position;
	Telement_type type = type_function;
	
	temp_item = symtab_insert(symtab, token, type);

	temp_item->type_strct.function->declared = true;
	temp_item->type_strct.function->defined = true;
	temp_item->type_strct.function->return_type = type_str;

	// parametr 

	position = add_string_to_storage("i");
	token->atribute.int_value = position;
	type = type_variable;
	char *key = get_string(position);
	Tvalue value;

	temp_item->type_strct.function->arg_count = 0;
	//temp_item->type_strct.function->sym_table = symtab_init(11);

	set_args_function(temp_item->type_strct.function, key, type_int, value);

	Tsymtab_item *param = symtab_insert(temp_item->type_strct.function->sym_table, token, type);
	param->type_strct.variable->declared = true;
	param->type_strct.variable->type = type_int;

	free(token);
}

void insert_built_in()
{
	insert_length();
	insert_substr();
	insert_asc();
	insert_chr();
}



/*

int main()
{
	symtab = symtab_init(17);
	insert_built_in();
	for (unsigned int i = 0; i < symtab->size; i++)
	{
		Tsymtab_item *temp_item = symtab->symtab_list[i];
		if (temp_item == NULL)
		{
				printf("NULL");
		}
		while(temp_item != NULL)
		{
			
			printf(" %s ", temp_item->key);
			if (temp_item->type == type_function)
			{	
				printf("->");
				for (unsigned int i = 0; i < temp_item->type_strct.function->sym_table->size; i++)
				{
				Tsymtab_item *temp = temp_item->type_strct.function->sym_table->symtab_list[i];
					if (temp == NULL)
					{
						printf("NULL->");
					}
					while(temp != NULL)
					{
				
						printf("%s ->", temp->key);
						temp = temp->next;
					}
				}
			}
			temp_item = temp_item->next;
		}
		printf("\n");
	}
	symtab_free(symtab);
	destruct_storage();
	
	return 0;
}

*/
