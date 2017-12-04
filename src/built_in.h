/* Nazev - Implementace prekladace imperativniho jazyka IFJ17
 * Petr Zubalik - xzubal04
 * Marek Kukucka - xkukuc04
 * Jan Koci - xkocij01
 * David Endrych - xendry02
 */


#ifndef BUILT_IN_H
#define BUILT_IN_H

/**
 * Vlozi built-in funkci Length() do globalni tabulky symbolu
 */
void insert_length();

/**
 * Vlozi built-in funkci SubStr() do globalni tabulky symbolu
 */
void insert_substr();

/**
 * Vlozi built-in funkci Asc() do globalni tabulky symbolu
 */
void insert_asc();

/**
 * Vlozi built-in funkci Chr() do globalni tabulky symbolu
 */
void insert_chr();

/**
 * Vola funkce pro vkladani built-in funkci do globalni tabulky symbolu
 */
void insert_built_in();
#endif /* BUILT_IN_H */
