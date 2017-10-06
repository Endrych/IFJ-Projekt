/*
Projekt IFJ
Datum: 3.10.2017
Autori: Petr Zubalik, Marek Kukucka, Jan Koci, David Endrych
file: lexical_test.c
Popis: Testy lexikalniho analyzatoru v souboru lexical.c
*/

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "../src/lexical.h"
#include "../src/token.h"
#include "../src/string_storage.h"

char *test_type[] = {
	"type_id",
	"type_keyword",
	"type_string",
	"type_integer",
	"type_double",
	"type_operator"
};

char* test_kw[] = {
	"And",
	"As",
 	"Asc",
 	"Boolean",
 	"Chr",
 	"Continue",
 	"Declare",
 	"Dim",
 	"Do",
 	"Double",
 	"Else",
 	"Elseif",
 	"End",
 	"Exit",
 	"False",
 	"For",
 	"Function",
 	"If",
 	"Input",
 	"Integer",
 	"Length",
 	"Loop",
 	"Next",
 	"Not",
 	"Or",
 	"Print",
 	"Return",
 	"Scope",
 	"Shared",
 	"Static",
 	"String",
 	"SubStr",
 	"Then",
 	"True",
 	"While"
};

char* test_operator[] = {
	"op_assign",
	"op_add",
	"op_sub",
	"op_mul",
	"op_division_int",
	"op_slash",
	"op_bracket",
	"op_bracket_end",
	"op_equal",
	"op_lesser",
	"op_lesser_equal",
	"op_greater",
	"op_greater_equal"
};


/*
Funkce pro porovnavani retezce ve string_storage a retezce name
Parametr attr_string slouzi k ulozeni retezce precteneho ze string_storage
*/
int name_cmp(char* name, char* storage, char* attr_string)
{
	int i = 0;
	while(storage[i] != '#')
	{
		attr_string[i] = storage[i];
		i++;
	}
	attr_string[i] = '\0';
	return strcmp(attr_string, name);
}


int main(int argc, char *argv[])
{
	Token* test_token;
	char* test_file; 
	bool is_ok = true;
	char* storage;
	char attr_string[42] = {0};	

	// Nacteme testovaci soubor
	if (argc != 2)
	{
		printf("CHYBA: Nebyl zadan testovaci soubor.\n\n");
		printf("Usage:\n\t ./lexical_test test_file \n");
		return 1;
	}
	test_file = argv[1];

	// funkce otevre testovaci soubor
	load_file(test_file);
	
	printf("\n====================== LEXICAL ANALYZER TESTS ======================\n\n");
	

	/***************************** Testy identifikatoru ******************
	Test input:  name1 _anotherIdentifier ValidId 1non_valid @nonvalid? >nonvalid
	*/
	printf("1) Id: ");
	char* test_input[] = {"name1", "_anotherIdentifier", "ValidId"};
	int input_count = 3;
	
	for (int i = 0; i < input_count; i++)
	{
		test_token = get_token();

		if (test_token->type != type_id)
		{
			printf("\n[TYPE_ERROR]\n");
			printf("Expected: type_id\n");
			// test_type definovano v lexical_test.h
			printf("In file: %s\n\n", test_type[test_token->type]);
			is_ok = false;
		} 

		else
		{
			storage = get_string(test_token->atribute.int_value);

			if (name_cmp(test_input[i], storage, attr_string) != 0)
			{
				printf("\n[ATTR_ERROR]\n");
				printf("Expected: %s\n", test_input[i]);
				printf("In file: %s\n", attr_string);
				is_ok = false;
			}
		
		}
		destruct_token(test_token);
	}

	if (is_ok)
	{
		printf("[SUCCESS]\n\n");
	}

	/************************ TESTY KW  **********************/
	
	printf("2) Keywords: ");
	is_ok = true;
	
  for (unsigned int i = 0; i <= kw_while; i++)
  {
  	test_token = get_token();

   	if (test_token->type != type_keyword)
   	{
   		printf("\n[TYPE_ERROR]\n");
			printf("Expected: type_keyword\n");
			printf("In file: %s\n\n", test_type[test_token->type]);
			is_ok = false;
   	} 

   	else if (test_token->atribute.keyword_value != i)
   	{
   		printf("\n[ATTR_ERROR]\n");
			printf("Expected: %s\n", test_kw[i]);
			printf("In file: %s\n", test_kw[test_token->atribute.keyword_value]);
			is_ok = false;
   	}
   	destruct_token(test_token);
    }


  if (is_ok)
	{
		printf("[SUCCESS]\n\n");
	}

	/***************** TESTY STRINGU ******************************/
	
	printf("3) String: ");
	is_ok = true;

	char* string_input[] = {"Retezec v IFJ17"};
	input_count = 1;
	for (int i = 0; i < input_count; i++)
	{
		test_token = get_token();

		if (test_token->type != type_string)
		{
			is_ok = false;
			printf("\n[TYPE_ERROR]\n");
			printf("Expected: type_string\n");
			printf("In file: %s\n\n", test_type[test_token->type]);
		}

		else
		{
			char* storage1 = get_string(test_token->atribute.int_value);
			
			if (name_cmp(string_input[i], storage1, attr_string) != 0)
			{
				printf("\n[ATTR_ERROR]\n");
				printf("Expected: %s\n", string_input[i]);
				printf("In file: %s\n\n", attr_string);
				is_ok = false;

			}
		}

		destruct_token(test_token);
	}
	if (is_ok)
	{
		printf("[SUCCESS]\n\n");
	}

	/************************ TESTY INTEGERU *********************/
	// ZAPORNA CISLA ??
	printf("4) Integer: ");
	is_ok = true;
	input_count = 3;
	int input_int[] = {42, 196, 0};

	for (int i = 0; i < input_count; i++)
	{
		test_token = get_token();

		if (test_token->type != type_integer)
		{
			is_ok = false;
			printf("\n[TYPE_ERROR]\n");
			printf("Expected: type_integer\n");
			printf("In file: %s\n\n", test_type[test_token->type]);
		}

		else if (test_token->atribute.int_value != input_int[i])
		{
			is_ok = false;
			printf("\n[ATTR_ERROR]\n");
			printf("Expected: %d\n", input_int[i]);
			printf("In file: %d\n\n", test_token->atribute.int_value);
		}
		destruct_token(test_token);
	}
	if (is_ok)
	{
		printf("[SUCCESS]\n\n");
	}

	/********************** TESTY DOUBLE ***********************/
	printf("5) Double: ");
	is_ok = true;
	double input_double[] = {0.0, 1344.54, 12.042, 56.};
	input_count = 4;

	for (int i = 0; i < input_count; i++)
	{
		test_token = get_token();

		if (test_token->type != type_double)
		{
			is_ok = false;
			printf("\n[TYPE_ERROR]\n");
			printf("Expected: type_double\n");
			printf("In file: %s\n\n", test_type[test_token->type]);
		}
		else if (test_token->atribute.double_value != input_double[i])
		{
			is_ok = false;
			printf("\n[ATTR_ERROR]\n");
			printf("Expected: %f\n", input_double[i]);
			printf("In file: %f\n\n", test_token->atribute.double_value);
		}
		destruct_token(test_token);
	}
	if (is_ok)
	{
		printf("[SUCCESS]\n\n");
	}

	/******************** TESTY OPERATORU ***************/
	printf("5) Operators: ");
	is_ok = true;

	for (unsigned int i = 0; i <= op_greater_equal; i++)
	{
		test_token = get_token();

		if (test_token->type != type_operator)
		{
			is_ok = false;
			printf("\n[TYPE_ERROR]\n");
			printf("Expected: type_operator\n");
			printf("In file: %s\n\n", test_type[test_token->type]);
		}
		else if (test_token->atribute.operator_value != i)
		{
			is_ok = false;
			printf("\n[ATTR_ERROR]\n");
			printf("Expected: %s\n", test_operator[i]);
			printf("In file: %s\n\n", test_operator[test_token->atribute.operator_value]);
		}
		destruct_token(test_token);
	}
	if (is_ok)
	{
		printf("[SUCCESS]\n\n");
	}

	destruct_storage();
	close_file();
	return 0;
}