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


// struktura pro ramec
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
 *  Funkce inicializuje zasobnik
 *
 * @param stack ukazatel na zasobnik, ktery ma byt inicializovan
 */
void FS_init(TFstack *stack);					

/**
 * Funkce uvolni pamet zasobniku 
 *
 * @param stack ukazatel na uvolnovany zasobnik
 */
void FS_destruct(TFstack *stack);

/**
 *	Funkce zjisti, zda je zasobnik prazny 
 *
 * @param  stack ukazatel na zasobnik
 * @return 	vraci hodnotu true, pokud je prazdny
 *			pokud neni, tak false
 */
bool FS_empty(TFstack *stack);	

/**
 * Funkce vlozi ramec na vrchol zasobniku
 *
 * @param stack ukazatel na zasobnik ramcu
 * @param frame ukazatel na vkladany ramec
 */
void FS_push(TFstack *stack, Tframe *frame);

/**
 * Funkce uvolni ramec na vrcholu
 * na vrchol ulozi nasledujici ramec
 *
 * @param stack ukazatel na zasobnik ramcu 
 */
void FS_pop(TFstack *stack);	

/**
 * Funkce vraci ukazatel na ramec, 
 * ktery je na vrcholu zasobniku ramcu
 *
 * @param  stack ukazatel na zasobnik ramcu
 * @return       ukazatel na ramec na vrcholu
 */
Tframe *FS_top(TFstack *stack);	


/**
 * Vytvori novy docasny ramec
 * pokud je vytvoren, tak zahodi jeho obsah
 */
void create_frame();


/**
 * prida promennou nebo parametr funkce do zadaneho ramce
 * alokuje pro tuto promennou pamet
 * @param frame ukazatel na ramec
 * @param var   ukazatel na promennou, ktera bude ulozena
 */
void add_var_to_frame( Tframe *frame, Tvariable *var);

/**
 * definuje promennou pro navratovou hodnotu z funkce
 *
 * @param frame ukazatel na ramec
 * @param var   ukazatel na promennou,
 *				podle ktere se definuje navratova hodnota funkce
 */
void def_return_value(Tframe *frame, Tvariable *var);

/**
 * presune temp_frame na vrchol zasobniku skrz promennou local_frame
 * pro local_frame alokuje pamet, vola def_return_value(),
 * po ukonceni funkce je temp_frame nedefinovan a je potreba jej znovu
 * vytvorit pomoci create_frame
 * pokud temp_frame == NULL nastava chyba 55

 * @param stack      ukazatel na zasobnik ramcu
 * @param var        ukazatel na promennou, je predan funkci def_return_value
 * @param next_instr ukazatel na nasledujici radek --> nakonec nepouzit 
 */
void push_frame(TFstack *stack, Tvariable *var, unsigned next_instr);

/**
 * presune vrchol zasobniku do temp_frame
 * pokud FS_empty() == true, nastava chyba 55

 * @param stack ukazatel na zasobnik ramcu
 */
void pop_frame(TFstack *stack);

/**
 * vraci hodnotu z temp_frame, ktery byl popnut ze zasobniku
 * do ramce, ktery je aktualne na vrcholu zasobniku
 * vola funkci add_var_to_frame()

 * @param end_frame ukazatel na uonceny ramec
 * @param frame     ukazatel na lokalni ramec
 */
void get_return_value(Tframe *end_frame, Tframe *frame);


#endif /* FRAME_H */
