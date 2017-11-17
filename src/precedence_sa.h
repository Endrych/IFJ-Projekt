#include "token.h"
#include "stack.h"
#include "symtable.h"

#ifndef PRECEDENCE_H
#define PRECEDENCE_H

typedef struct{
    Tvariable_type Type;
    Token* ReturnToken;
    ATLeaf * Tree;
}PrecendentOutput;

PrecendentOutput* precedence_analysis(Token* token);
int precedence_operation(Token* stack_token,Token* lexical_token);
int findRule(tStack * s);
#endif