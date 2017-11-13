#include "token.h"
#include "stack.h"

#ifndef PRECEDENCE_H
#define PRECEDENCE_H

typedef enum{
    dt_Integer,
    dt_String,
    dt_Double
}DataType;

typedef struct{
    DataType Type;
    Token* ReturnToken;
    ATLeaf * Tree;
}PrecendentOutput;

PrecendentOutput* precedence_analysis(Token* token);
int precedence_operation(Token* stack_token,Token* lexical_token);
int findRule(tStack * s);
#endif