#include "abstract_tree.h"

#ifndef AT_STACK_H
#define AT_STACK_H
 
typedef struct atq_item{
    ATLeaf * Tree;
    struct atq_item * Next;
}ATQItem;

typedef struct{
    ATQItem * Front;
}ATQueue;

void queInit ( ATQueue* s );
void queDestruct ( ATQueue* s );
int queEmpty ( const ATQueue* s );
ATLeaf* queFront ( const ATQueue* s);
void queRemove ( ATQueue* s );
void queUp ( ATQueue* s, ATLeaf* Tree);

#endif