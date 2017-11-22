
#include "abstract_tree.h"
#include "at_que.h"

#ifndef GENERATOR_H
#define GENERATOR_H

void generate_start(ATQueue *queue);
void generate_program(ATQueue *queue);
void generate_variable_declaration(Tsymtab_item * id, ATLeaf * expr);
void generate_assign(Tsymtab_item* id, ATLeaf * expr);
void generate_input(Tsymtab_item * id);
void generate_print(eQueue * exprs);
void generate_call_function(Tsymtab_item * id, Tsymtab_item * sym_item, eQueue * param);
void generate_return(ATLeaf * expr);
void to_print(ATLeaf *leaf, int value);
void generate_expression(ATLeaf *leaf);
void generate_if(ATLeaf * condition, ATQueue * state_true, ATQueue * state_false);
void generate_condition(ATLeaf *leaf, char* label);
void generate_while(ATLeaf * condition, ATQueue * state);
void generate_function(Tsymtab_item * item, ATQueue * state);
#endif