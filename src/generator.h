
#include "abstract_tree.h"
#include "at_que.h"

#ifndef GENERATOR_H
#define GENERATOR_H

void open_output();
void to_print(ATLeaf *leaf, int value);
void generate_expression(ATLeaf *leaf);
void generate_if(ATLeaf * condition, ATQueue * state_true, ATQueue * state_false);
void generate_condition(ATLeaf *leaf, char* label);
void generate_while(ATLeaf * condition, ATQueue * state);
#endif
