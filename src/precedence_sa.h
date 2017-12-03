/* Nazev - Implementace prekladace imperativniho jazyka IFJ17
 * Petr Zubalik - xzubal04
 * Marek Kukucka - xkukuc04
 * Jan Koci - xkocij01
 * David Endrych - xendry02
 */


#include "token.h"
#include "stack.h"
#include "symtable.h"

#ifndef PRECEDENCE_H
#define PRECEDENCE_H

// Struktura pro vystup precedencni analyzy
// StatusCode Informace o uspechu analyzy
// Type Typ vysledku vyrazu
// ReturnToken Zbyvajici nezpracovany Token
// Tree Ukazatel na strom vyrazu
typedef struct{
    int StatusCode;
    Tvariable_type Type;
    Token* ReturnToken;
    ATLeaf * Tree;
}PrecendentOutput;

/**
 * Funkce zpracovava vyraz
 * @param  Pokud je potreba je mozne poslat posledni nacteny token,
 *         vyraz pote zacina timto tokenem
 * @return  Funkce vraci strukturu PrecendentOutput
 */
PrecendentOutput* precedence_analysis(Token* last_token);

/**
 * Funkce vyhodnocuje operaci se zasobnikem podle precedencni tabulky
 * @param  stack_token   Token z vrcholu zasbobniku
 * @param  lexical_token Aktualne nacteny token
 * @return Vraci cislo, 0-2, ktere urcuje jaka operace se ma provest
 */
int precedence_operation(Token* stack_token,Token* lexical_token);

/**
 * Funkce vyhodnocuje pravidla precedencni analyzy
 * @param  s Ukazatel na zasobnik precedencni analyzy
 * @return  Vraci cislo pravidla
 */
int findRule(tStack * s);
#endif
