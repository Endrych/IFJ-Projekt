#include "symtable.h"
#include <stdbool.h>
#include <stdlib.h>


#ifndef FRAME_H
#define FRAME_H



typedef struct variable {
	Tvalue value;
	bool defined;
	char *id;
	Tvariable_type type;
} Tvariable;			// nova struktura pro promennou ulozenou ve frame


typedef struct frame {
	unsigned next_instr;
	Tvariable *return_value;
	Tvariable *vars;
	int var_count;
	struct frame *next;
} Tframe;	


typedef struct frame_stack {
	Tframe *top;
} TFstack;		// frame stack



//Tframe *temp_frame = NULL;		// docasny ramec


void FS_init(TFstack *stack);					// inicializuje zasobnik

void FS_destruct(TFstack *stack);				// uvolni pamet zasobniku

bool FS_empty(TFstack *stack);					// je zasobnik volny?

void FS_push(TFstack *stack, Tframe *frame);	// pushne frame na vrchol

void FS_pop(TFstack *stack);					// popne frame z vrcholu

Tframe *FS_top(TFstack *stack);					// vrati frame z vrcholu


/*
 * vytvori novy docasny ramec
 * pokud je vytvoren, tak zahodi jeho obsah 
 */
void create_frame();	

/*
 * prida promennou nebo parametr funkce do zadaneho ramce
 * alokuje pro tuto promennou pamet
 */
void add_var_to_frame( Tframe *frame, Tvariable *var);	

/*
 * definuje promennou pro navratovou hodnotu z funkce 
 * definuje ji podle promenne zadane jako parametr var
 */
void def_return_value(Tframe *frame, Tvariable *var);	

/*
 *  zatim neimplementovano, mozna nebude potreba
 *
 */
void destruct_frame(TFstack *stack, Tframe *frame);		

/*
 * presune temp_frame na vrchol zasobniku skrz promennou local_frame
 * pro local_frame alokuje pamet, vola def_return_value(),
 * po ukonceni funkce je temp_frame nedefinovan a je potreba jej znovu
 * vytvorit pomoci create_frame
 * pokud temp_frame == NULL nastava chyba 55
 */
void push_frame(TFstack *stack, Tvariable *var, unsigned next_instr);	

/*
 * presune vrchol zasobniku do temp_frame
 * pokud FS_empty() == true, nastava chyba 55
 */
void pop_frame(TFstack *stack);

/*
 * vraci hodnotu z temp_frame, ktery byl popnut ze zasobniku  
 * do ramce, ktery je aktualne na vrcholu zasobniku
 * vola funkci add_var_to_frame(), nevim jestli je to dobre
 * mozna bude potreba predelat
 * je volana funkci pop_frame()
 */
void get_return_value(Tframe *end_frame, Tframe *frame);


#endif /* FRAME_H */ 