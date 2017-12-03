/* Nazev - Implementace prekladace imperativniho jazyka IFJ17
 * Petr Zubalik - xzubal04
 * Marek Kukucka - xkukuc04
 * Jan Koci - xkocij01
 * David Endrych - xendry02
 */

#ifndef STORAGE_H
#define STORAGE_H

/**
 * Funkce prida retezec do uloziste retezcu
 * @param  array Ukazatel na retezec, ktery se ma ulozit
 * @return Vraci pozici ulozeneho retezce
 */
int add_string_to_storage(char* array);

/**
 * Funkce pro ziskani retezce z uloziste
 * @param  pos Pozice retezce v ulozisti
 * @return  Ukazatel na retezec
 */
char * get_string(int pos);

/**
 * Funkce uvolni uloziste retezcu 
 */
void destruct_storage();
#endif
