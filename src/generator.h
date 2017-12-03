/* Nazev - Implementace prekladace imperativniho jazyka IFJ17
 * Petr Zubalik - xzubal04
 * Marek Kukucka - xkukuc04
 * Jan Koci - xkocij01
 * David Endrych - xendry02
 */

#include "abstract_tree.h"
#include "at_que.h"

#ifndef GENERATOR_H
#define GENERATOR_H

// Typ pro generovani nazvu
typedef enum{
    gt_variable,
    gt_label
}GType;

/**
 * Funkce vygeneruje nazev pro promenou nebo label
 * @param  type Parametr urcuje jestli vygenerovane jmeno ma vypadat jako promena nebo label
 * @return funkce vraci ukazatel na pole ve kterem je ulozen vygenerovany nazev
 */
char * generate_name(GType type);

/**
 * Funkce generuje vestavenou funkci Asc
 */
void generate_Asc();

/**
 * Funkce generuje vestavenou funkci Length
 */
void generate_Length();

/**
 * Funkce generuje vestavenou funkci SubStr
 */
void generate_SubStr();

/**
 * Funkce generuje vestavenou funkci Chr
 */
void generate_Chr();

/**
 * Funkce postupne zavola generatory vestavenych funkci
 */
void generate_built_in();

/**
 * Funkce vygeneruje main funkci
 * @param queue Fronta obsahujici blok scopu
 */
void generate_main(ATQueue * queue);

/**
 * Vstupni funkce pro generovani
 * @param queue Hlavni fronta
 */
void generate_start(ATQueue *queue);

/**
 * Funkce generuje blok kodu
 * @param queue Front obsahujici blok kodu
 */
void generate_program(ATQueue *queue);

/**
 * Funkce generuje deklaraci promenne a muze i vygenerovat prirazeni hodnoty promenne
 * @param id  Ukazatel do TS na promennou
 * @param expr Ukazatel na ASS vyrazu
 * @param expr_type Datovy typ vyrazu
 */
void generate_variable_declaration(Tsymtab_item * id, ATLeaf * expr, Tvariable_type expr_type);

/**
 * Funkce generuje prirazeni vyrazu do promenne
 * @param id Ukazatel do TS na promennou
 * @param expr Ukazatel na ASS vyrazu
 * @param expr_type Datovy typ vyrazu
 */
void generate_assign(Tsymtab_item* id, ATLeaf * expr, Tvariable_type expr_type);

/**
 * Funkce generuje nacteni hodnoty do promenne
 * @param id Ukazatel do TS na promennou
 */
void generate_input(Tsymtab_item * id);

/**
 * Funkce generuje vypis vyrazu
 * @param exprs Fronta vyrazu
 */
void generate_print(eQueue * exprs);

/**
 * Funkce generuje volani funkce
 * @param id Ukazatel do TS na promennou do ktere se uklada vysledek volani funkce
 * @param sym_item Ukazatel do TS na funkci
 * @param param Fronta parametru
 */
void generate_call_function(Tsymtab_item * id, Tsymtab_item * sym_item, eQueue * param);

/**
 * Funkce generuje prikaz navratu z funkce
 * @param sym_item Ukazatel do TS na funkci
 * @param expr Ukazatel na vystup precedencni analyzy
 */
void generate_return(Tsymtab_item * sym_item, PrecendentOutput * expr);

/**
 * Funkce generuje vyrazy
 * @param  leaf Ukazatel na ASS vyrazu
 * @return  Vraci Ukazatel na nazev promenne ve ktere je ulozen vysledek vyrazu
 */
char * generate_expression(ATLeaf *leaf);

/**
 * Funkce generuje podminku
 * @param condition Ukazatel na ASS podminky
 * @param state_true  Fronta bloku pokud je podminka pravdiva
 * @param state_false Fronta bloku pokud neni podminka pravdiva
 */
void generate_if(ATLeaf * condition, ATQueue * state_true, ATQueue * state_false);

/**
 * Funkce generuje cyklus
 * @param condition Ukazatel na ASS podminky
 * @param state Fronta bloku cyklu
 */
void generate_while(ATLeaf * condition, ATQueue * state);

/**
 * Funkce generuje funkci
 * @param item Ukazatel do TS na funkci
 * @param state Fronta bloku funkce
 */
void generate_function(Tsymtab_item * item, ATQueue * state);
#endif
