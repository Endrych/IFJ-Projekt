#include "abstract_tree.h"
#include "symtable.h"

#ifndef AT_QUE_H
#define AT_QUE_H
 
typedef struct eq_item{
    ATLeaf * Expr;
    struct eq_item * Next;
}eQItem;

typedef struct{
    eQItem * Front;
}eQueue;

typedef enum{
    gt_var_declar,
    gt_func_declar,
    gt_assign,
    gt_input,
    gt_if,
    gt_while,
    gt_call_func,
    gt_return
}GenType;

typedef struct{
    Tsymtab_item * id;
    Tvariable_type type;
    ATLeaf * expr;
}VarDeclarInput;

typedef struct{
    Tsymtab_item * sym_item;
    ATQueue * queue;
}FuncDeclarInput;

typedef struct{
    ATLeaf * expr;
    Tsymtab_item * id;
}AssignInput;

typedef struct{
    ATLeaf * cond_expr;
    ATQueue * true_queue;
    ATQueue * false_queue;
}IfInput;

typedef struct{
    ATLeaf * cond_expr;
    ATQueue * queue;
}WhileInput;

typedef struct{
    Tsymtab_item * id;
    Tsymtab_item * sym_item;
    eQueue * param;
}CallFuncInput;

typedef union{
    VarDeclarInput * var_declar_input;
    FuncDeclarInput * func_declar_input;
    AssignInput * assign_input;
    Tsymtab_item * id;
    eQueue * exprs;
    IfInput * if_input;
    WhileInput * while_input;
    CallFuncInput * call_func_input;
    ATLeaf * expr;
}GenValue;

typedef struct atq_item{
    GenType GenType;
    GenValue GenValue;
    struct atq_item * Next;
}ATQItem;

typedef struct{
    ATQItem * Front;
}ATQueue;

void queInit ( ATQueue* s );
void queDestruct ( ATQueue* s );
int queEmpty ( const ATQueue* s );
ATQItem* queFront ( const ATQueue* s);
void queRemove ( ATQueue* s );
void queUp ( ATQueue* s, ATQItem* Tree);

void equeInit ( eQueue* s );
void equeDestruct ( eQueue* s );
int equeEmpty ( const eQueue* s );
ATLeaf* equeFront ( const eQueue* s);
void equeRemove ( eQueue* s );
void equeUp ( eQueue* s, ATLeaf* Tree);

#endif