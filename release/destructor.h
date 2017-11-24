/* Nazev - Implementace prekladace imperativniho jazyka IFJ17
 * Petr Zubalik - xzubal04
 * Marek Kukucka - xkukuc04
 * Jan Koci - xkocij01
 * David Endrych - xendry02
 */

#include "string_storage.h"
#include "stack.h"
#include "at_que.h"
#include "frame.h"
#include "gen_stacks.h"
#include "error.h"

#ifndef DESTRUCTOR_H
#define DESTRUCTOR_H

void dispose_all(tStack *stack, ATLeaf *leaf, ATQueue* s, eQueue* s2, TFstack *frame_stack, GPStack *s3);

/* 
 * Pouziti pri chybe prekladace
 * napr. pri spatne alokaci pameti
 * uvolni pamet globalnich promennych a da exit(99)
 */
void dispose_global();


#endif /* DESTRUCTOR_H */