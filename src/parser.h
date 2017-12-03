/* Nazev - Implementace prekladace imperativniho jazyka IFJ17
 * Petr Zubalik - xzubal04
 * Marek Kukucka - xkukuc04
 * Jan Koci - xkocij01
 * David Endrych - xendry02
 */

#include <stdbool.h>
#include "precedence_sa.h"
#include "symtable.h"
#include "at_que.h"

#ifndef PARSER_H
#define PARSER_H

/**
 * Funkce volajici se na zacatku analyzi (prvni volana funkce)
 * @return vraci navratovy kod indikujici uspech/neuspech analyzi
 	pripadne navratovy kod chyby
 */
int parse();

/**
 * Funkce pro neterminal <Prog> = zacatek programu
 * @return navratovy kod chyby / 0 kdyz uspech
 */
int Prog();

/**
 * Funkce popisujici neterminal <Scope> = hlavni telo programu
 * @return navratovy kod chyby / 0 kdyz uspech
 */
int Scope();

/**
 * Funkce pro neterminal Statement list = seznam vyrazu
 * @return navratovy kod chyby / 0 kdyz uspech
 */
int St_list();

/**
 * Funkce pro neterminal popisujici jednotlive vyrazy
 * @return navratovy kod chyby / 0 kdyz uspech
 */
int Stat();


/**
 * Funkce pro neterminal prirazeni do promenne
 * @return navratovy kod chyby / 0 kdyz uspech
 */
int Assign();

/**
 * Popisuje neterminal predstavujici definici nebo deklaraci funkce
 * @return navratovy kod chyby / 0 kdyz uspech
 */
int Func();

/**
 * Zpracovani parametru pri deklaraci nebo definici funkce
 * @param  symtab_item identifikator funkce v tabulce symbolu
 * @param  params_iter hodnota kontrolujici pocet parametru
 * @param  declaration znaci jestli se jedna o deklaraci nebo definici funkce
 * @return             navratovy kod chyby / 0 kdyz uspech
 */
int Param_list(Tsymtab_item *symtab_item, int* params_iter, bool declaration);

/**
 * Zpracovava jednotlive parametry u definic nebo deklaraci funkci
 * @param  symtab_item identifikator funkce v tabulce symbolu
 * @param  params_iter hodnota kontrolujici pocet parametru
 * @param  declaration znaci jestli se jedna o deklaraci nebo definici funkce
 * @return             navratovy kod chyby / 0 kdyz uspech
 */
int Param(Tsymtab_item *symtab_item, int* params_iter, bool declaration);

/**
 * Funkce rozhoduje zda za aktualnim parametrem nasleduje dalsi
 * @param  symtab_item identifikator funkce v tabulce symbolu
 * @param  params_iter hodnota kontrolujici pocet parametru
 * @param  declaration znaci jestli se jedna o deklaraci nebo definici funkce
 * @return             navratovy kod chyby / 0 kdyz uspech
 */
int Next_par(Tsymtab_item* symtab_item, int* params_iter, bool declaration);

/**
 * Zpracovani jednotlivych vyrazu u funkce print
 * @param  exprs fronta vyrazu u daneho volani funkce print
 * @return       navratovy kod chyby / 0 kdyz uspech
 */
int ExprPrint(eQueue* exprs);

/**
 * Rozpozna jestli je dany token klicove slovo pro datovy typ String,Integer nebo Double
 * @return navratovy kod chyby / 0 kdyz uspech
 */
int Tyype();

/**
 * Nastavi navratovou hodnotu funkce v tabulce symbolu
 * @param  function identifikator dane funkce v tabulce symbolu
 * @return navratovy kod chyby / 0 kdyz uspech
 */
int set_return(Tfunction_item *function);

/**
 * Nastavi typ a inicializuje promennou v tabulce symbolu
 * @param  symtab_item odkaz do tabulky symbolu na danou promennou
 * @return             navratovy kod chyby / 0 kdyz uspech
 */
int set_type_variable(Tsymtab_item* symtab_item);

/**
 * Zkontroluje jestli jsou dve promenne stejneho typu nebo je mozne 
 	provest implicitni konverzi
 * @param  type1 typ prvni promenne
 * @param  type2 typ druhe promenne
 * @return       navratovy kod chyby / 0 kdyz uspech
 */
int check_type(Tvariable_type type1, Tvariable_type type2);

/**
 * Kontroluje jestli je typ promenne, predany jako jediny parametr,
 	stejneho typu jako soucasny token
 * @param  type typ promenne
 * @return      navratovy kod chyby / 0 kdyz uspech
 */
int check_param_type(Tvariable_type type);

/**
 * Kontrola seznamu argumentu pri u volani funkce
 * @param  symtab_item odkaz do tabulky symbolu na danou funkci
 * @param  eque        fronta do ktere ulozime argumenty a kterou predame generatoru
 * @return             navratovy kod chyby / 0 kdyz uspech
 */
int Args_list(Tsymtab_item* symtab_item, eQueue* eque);

/**
 * Zpracuje jednotlivy argument u volani funkce
 * @param  function  ukazatel na funkci v tabulce symbolu
 * @param  args_iter promenna pro pocitani poctu argumentu
 * @param  eque      fronta do ktere ulozime argumenty a kterou predame generatoru
 * @return           navratovy kod chyby / 0 kdyz uspech
 */
int Arg(Tfunction_item* function, int* args_iter, eQueue* eque);

/**
 * Urcuje jestli za argumentem nasleduje dalsi argument
 * @param  function  ukazatel na funkci v tabulce symbolu
 * @param  args_iter promenna pro pocitani poctu argumentu
 * @param  eque      fronta do ktere ulozime argumenty a kterou predame generatoru
 * @return           navratovy kod chyby / 0 kdyz uspech
 */
int Next_arg(Tfunction_item* function, int* args_iter, eQueue* eque);

#endif
