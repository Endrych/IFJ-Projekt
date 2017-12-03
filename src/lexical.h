/* Nazev - Implementace prekladace imperativniho jazyka IFJ17
 * Petr Zubalik - xzubal04
 * Marek Kukucka - xkukuc04
 * Jan Koci - xkocij01
 * David Endrych - xendry02
 */

#include "token.h"

#ifndef LEXICAL_H
#define LEXICAL_H

/**
 * Slouzi ke zpracovani jednotlivych lexemu ze vstupu programu. 
 * Funkce rozpozna o jaky typ lexemu se jedna a vrati token s danou informaci.
 * @return funkce vraci token s informacemi o zpracovanem lexemu
 */
Token* get_token();

#endif
