#include "abstract_tree.h"

#ifndef AT_STACK_H
#define AT_STACK_H
 
typedef struct ats_item{
    ATLeaf * Tree;
    struct ats_item * Next;
}ATSItem;

typedef struct{
    ATSItem * Head;
}ATStack;

void stackInit ( ATStack* s );
void stackDestruct ( ATStack* s );
int stackEmpty ( const ATStack* s );
int stackFull ( const ATStack* s );
ATLeaf* stackTop ( const ATStack* s);
void stackPop ( ATStack* s );
void stackPush ( ATStack* s, ATLeaf* Tree);

#endif