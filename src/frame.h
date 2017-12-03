/* Nazev - Implementace prekladace imperativniho jazyka IFJ17
 * Petr Zubalik - xzubal04
 * Marek Kukucka - xkukuc04
 * Jan Koci - xkocij01
 * David Endrych - xendry02
 */

#include "symtable.h"
#include <stdbool.h>
#include <stdlib.h>


#ifndef FRAME_H
#define FRAME_H


// nova struktura pro promennou ulozenou ve frame
typedef struct variable {
	Tvalue value;
	bool defined;
	char *id;
	Tvariable_type type;
} Tvariable;

//
typedef struct frame {
	unsigned next_instr;
	Tvariable *return_value;
	Tvariable *vars;
	int var_count;
	struct frame *next;
} Tframe;

//
typedef struct frame_stack {
	Tframe *top;
} TFstack;		// frame stack

/**
 * [FS_init description]
 * @param stack [description]
 */
void FS_init(TFstack *stack);					// inicializuje zasobnik

/**
 * [FS_destruct description]
 * @param stack [description]
 */
void FS_destruct(TFstack *stack);				// uvolni pamet zasobniku

/**
 * [FS_empty description]
 * @param  stack [description]
 * @return       [description]
 */
bool FS_empty(TFstack *stack);					// je zasobnik volny?

/**
 * [FS_push description]
 * @param stack [description]
 * @param frame [description]
 */
void FS_push(TFstack *stack, Tframe *frame);	// pushne frame na vrchol

/**
 * [FS_pop description]
 * @param stack [description]
 */
void FS_pop(TFstack *stack);					// popne frame z vrcholu

/**
 * [FS_top description]
 * @param  stack [description]
 * @return       [description]
 */
Tframe *FS_top(TFstack *stack);					// vrati frame z vrcholu


/**
 * Vytvori novy docasny ramec
 * pokud je vytvoren, tak zahodi jeho obsah
 */
void create_frame();

/**
 * prida promennou nebo parametr funkce do zadaneho ramce
 * alokuje pro tuto promennou pamet
 */
/**
 * [add_var_to_frame description]
 * @param frame [description]
 * @param var   [description]
 */
void add_var_to_frame( Tframe *frame, Tvariable *var);

/*
 * definuje promennou pro navratovou hodnotu z funkce
 * definuje ji podle promenne zadane jako parametr var
 */
/**
 * [def_return_value description]
 * @param frame [description]
 * @param var   [description]
 */
void def_return_value(Tframe *frame, Tvariable *var);

/*
 *  zatim neimplementovano, mozna nebude potreba
 *
 */
/**
 * [destruct_frame description]
 * @param stack [description]
 * @param frame [description]
 */
void destruct_frame(TFstack *stack, Tframe *frame);

/*
 * presune temp_frame na vrchol zasobniku skrz promennou local_frame
 * pro local_frame alokuje pamet, vola def_return_value(),
 * po ukonceni funkce je temp_frame nedefinovan a je potreba jej znovu
 * vytvorit pomoci create_frame
 * pokud temp_frame == NULL nastava chyba 55
 */

/**
 * [push_frame description]
 * @param stack      [description]
 * @param var        [description]
 * @param next_instr [description]
 */
void push_frame(TFstack *stack, Tvariable *var, unsigned next_instr);

/*
 * presune vrchol zasobniku do temp_frame
 * pokud FS_empty() == true, nastava chyba 55
 */

/**
 * [pop_frame description]
 * @param stack [description]
 */
void pop_frame(TFstack *stack);

/*
 * vraci hodnotu z temp_frame, ktery byl popnut ze zasobniku
 * do ramce, ktery je aktualne na vrcholu zasobniku
 * vola funkci add_var_to_frame(), nevim jestli je to dobre
 * mozna bude potreba predelat
 * je volana funkci pop_frame()
 */

/**
 * [get_return_value description]
 * @param end_frame [description]
 * @param frame     [description]
 */
void get_return_value(Tframe *end_frame, Tframe *frame);


#endif /* FRAME_H */
