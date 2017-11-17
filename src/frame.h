#include "symtable.h"
#include <stdbool.h>

#ifndef FRAME_H
#define FRAME_H

typedef struct variable {
	Tvalue value;
	bool defined;
	char *id;
	Tvariable_type type;
} Tvariable;			// nova struktura pro promennou ulozenou ve frame

typedef struct frame {
	Tvariable *return_value;
	Tvariable *vars;
	int var_count;
	struct frame *next;
} Tframe;	

typedef struct frame_stack {
	Tframe *top;
} TFstack;		// frame stack

void FS_init(TFstack *stack);					// inicializuje zasobnik
void FS_destruct(TFstack *stack);				// uvolni pamet zasobniku
bool FS_empty(TFstack *stack);					// je zasobnik volny?
void FS_push(TFstack *stack, Tframe *frame);	// pushne frame na vrchol
void FS_pop(TFstack *stack);					// popne frame z vrcholu
Tframe *FS_top(TFstack *stack);					// vrati frame z vrcholu

Tframe *create_frame();							// vytvori novy frame
void add_param_to_frame( Tframe *frame, Tvariable *param);	// prida argument funkce do ramce
void def_return_value(Tframe *frame, Tvariable *var);	// definuje promenou pro navratovou hodnotu
void destruct_frame(TFstack *stack, Tframe *frame);		// zrusi ramec
void push_frame(TFstack *stack, Tframe *frame, Tvariable *var);	//FS_push() + def_return_value()

#endif /* FRAME_H */ 