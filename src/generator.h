
#include "abstract_tree.h"

#ifndef GENERATOR_H
#define GENERATOR_H

void open_output();
void to_print(ATLeaf *leaf, int value);
void generate_expression(ATLeaf *leaf);

#endif
