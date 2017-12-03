/* Nazev - Implementace prekladace imperativniho jazyka IFJ17
 * Petr Zubalik - xzubal04
 * Marek Kukucka - xkukuc04
 * Jan Koci - xkocij01
 * David Endrych - xendry02
 */

#include "abstract_tree.h"
#include "symtable.h"
#include "precedence_sa.h"

#ifndef AT_QUE_H
#define AT_QUE_H

//
struct ATQueue;

//
typedef enum{
    eq_token,
    eq_id,
    eq_tree
}eType;

//
typedef union{
    Tsymtab_item * id_value;
    ATLeaf * tree_value;
    Token * token_value;
}eValue;

//
typedef struct eq_item{
    eType etype;
    eValue eValue;
    struct eq_item * Next;
}eQItem;

//
typedef struct{
    eQItem * Front;
}eQueue;

//
typedef enum{
    gt_var_declar,
    gt_func_declar,
    gt_assign,
    gt_input,
    gt_print,
    gt_if,
    gt_while,
    gt_call_func,
    gt_return,
    gt_main
}GenType;

//
typedef struct{
    Tsymtab_item * id;
    ATLeaf * expr;
    Tvariable_type expr_type;
}VarDeclarInput;

//
typedef struct{
    Tsymtab_item * sym_item;
    struct ATQueue * queue;
}FuncDeclarInput;

//
typedef struct{
    ATLeaf * expr;
    Tsymtab_item * id;
    Tvariable_type expr_type;
}AssignInput;

//
typedef struct{
    ATLeaf * cond_expr;
    struct ATQueue * true_queue;
    struct ATQueue * false_queue;
}IfInput;

//
typedef struct{
    ATLeaf * cond_expr;
    struct ATQueue * queue;
}WhileInput;

//
typedef struct{
    Tsymtab_item * id;
    Tsymtab_item * sym_item;
    eQueue * param;
}CallFuncInput;

//
typedef struct{
    Tsymtab_item * sym_item;
    PrecendentOutput * expr;
}ReturnInput;

//
typedef union{
    VarDeclarInput * var_declar_input;
    FuncDeclarInput * func_declar_input;
    AssignInput * assign_input;
    Tsymtab_item * id;
    eQueue * exprs;
    IfInput * if_input;
    WhileInput * while_input;
    CallFuncInput * call_func_input;
    ReturnInput * return_input;
    struct ATQueue * at_queue;
}GenValue;

//
typedef struct atq_item{
    GenType GenType;
    GenValue GenValue;
    struct atq_item * Next;
}ATQItem;

//
typedef struct atQueue{
    ATQItem * Front;
}ATQueue;

//
typedef struct qSItem{
    ATQueue * Queue;
    struct qSItem * Next;
} QSItem;

//
typedef struct{
    QSItem * Top;
} QStack;

/**
 * [queInit  description]
 * @param s [description]
 */
void queInit ( ATQueue* s );

/**
 * [queDestruct  description]
 * @param s [description]
 */
void queDestruct ( ATQueue* s );

/**
 * [queEmpty  description]
 * @param  s [description]
 * @return   [description]
 */
int queEmpty ( const ATQueue* s );

/**
 * [queFront  description]
 * @param  s [description]
 * @return   [description]
 */
ATQItem* queFront ( const ATQueue* s);

/**
 * [queRemove  description]
 * @param s [description]
 */
void queRemove ( ATQueue* s );

/**
 * [queUp  description]
 * @param s    [description]
 * @param Tree [description]
 */
void queUp ( ATQueue* s, ATQItem* Tree);

/**
 * [equeInit  description]
 * @param s [description]
 */
void equeInit ( eQueue* s );

/**
 * [equeDestruct  description]
 * @param s [description]
 */
void equeDestruct ( eQueue* s );

/**
 * [equeEmpty  description]
 * @param  s [description]
 * @return   [description]
 */
int equeEmpty ( const eQueue* s );

/**
 * [equeFront  description]
 * @param  s [description]
 * @return   [description]
 */
eQItem* equeFront ( const eQueue* s);

/**
 * [equeRemove  description]
 * @param s [description]
 */
void equeRemove ( eQueue* s );

/**
 * [equeUp  description]
 * @param s    [description]
 * @param item [description]
 */
void equeUp ( eQueue* s, eQItem* item);

void qstackInit ( QStack* s );

/**
 * [qstackDestruct  description]
 * @param s [description]
 */
void qstackDestruct ( QStack* s );

/**
 * [qstackEmpty  description]
 * @param  s [description]
 * @return   [description]
 */
int qstackEmpty ( const QStack* s );

/**
 * [qstackTop  description]
 * @param  s [description]
 * @return   [description]
 */
ATQueue* qstackTop ( const QStack* s);

/**
 * [qstackPop  description]
 * @param s [description]
 */
void qstackPop ( QStack* s );

/**
 * [qstackPush  description]
 * @param s     [description]
 * @param queue [description]
 */
void qstackPush ( QStack* s, ATQueue* queue);

#endif
