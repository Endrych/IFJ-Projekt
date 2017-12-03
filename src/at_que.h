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

// Typ polozky fronty
typedef enum{
    eq_token,
    eq_id,
    eq_tree
}eType;

// Hodnota polozky fronty
// id_value Ukazatel do TS
// tree_value Ukazatel na ASS
// token_value Ukazatel na token
typedef union{
    Tsymtab_item * id_value;
    ATLeaf * tree_value;
    Token * token_value;
}eValue;

// Struktura polozky fronty
// etype Typ polozky
// eValue Hodnota polozky
// Next Ukazatel na nasledujici polozku
typedef struct eq_item{
    eType etype;
    eValue eValue;
    struct eq_item * Next;
}eQItem;

// Struktura fronty
// Front Ukazatel na prvni polozku
typedef struct{
    eQItem * Front;
}eQueue;

// Struktura fronty pro ukladani bloku
struct ATQueue;

// Typ polozky fronty
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

// Struktura pro ulozeni deklerace promenne
// id Ukazatel do TS
// expr Ukazatel na ASS vyrazu
// expr_type Datovy typ vyrazu
typedef struct{
    Tsymtab_item * id;
    ATLeaf * expr;
    Tvariable_type expr_type;
}VarDeclarInput;

// Struktura pro ulozeni deklerace funkce
// sym_item Ukazatel do TS
// queue Fronta bloku funkce
typedef struct{
    Tsymtab_item * sym_item;
    struct ATQueue * queue;
}FuncDeclarInput;

// Struktura pro ulozeni prirazeni do promenne
// expr Ukazatel na ASS vyrazu
// id Ukazatel do TS na promennou
// expr_type Datovy typ vyrazu
typedef struct{
    ATLeaf * expr;
    Tsymtab_item * id;
    Tvariable_type expr_type;
}AssignInput;

// Struktura pro ulozeni podminky
// cond_expr Ukazatel na ASS podminky
// true_queue Fronta bloku pokud je podminka pravdiva
// false_queue Fronta bloku pokud neni podminka pravdiva
typedef struct{
    ATLeaf * cond_expr;
    struct ATQueue * true_queue;
    struct ATQueue * false_queue;
}IfInput;

// Struktura pro ulozeni cyklu
// cond_expr Ukazatel na ASS podminky
// queue Fronta bloku cyklu
typedef struct{
    ATLeaf * cond_expr;
    struct ATQueue * queue;
}WhileInput;

// Struktura pro ulozeni volani funkce
// id Ukazatel do TS na promennou do ktere se uklada vysledek volani funkce
// sym_item Ukazatel do TS na funkci
// param Fronta parametru
typedef struct{
    Tsymtab_item * id;
    Tsymtab_item * sym_item;
    eQueue * param;
}CallFuncInput;

// Struktura pro ulozeni prikazu navratu z funkce
// sym_item Ukazatel do TS na funkci
// expr Ukazatel na vystup precedencni analyzy
typedef struct{
    Tsymtab_item * sym_item;
    PrecendentOutput * expr;
}ReturnInput;

// Hodnota polozky fronty
// var_declar_input Ukazatel na strukturu pro ulozeni deklerace promenne
// func_declar_input Ukazatel na strukturu pro ulozeni deklerace funkce
// assign_input Ukazatel na strukturu pro ulozeni prirazeni do promenne
// id Ukazatel do TS
// exprs Ukazatel na frontu
// if_input Ukazatel na strukturu pro ulozeni podminky
// while_input Ukazatel na strukturu pro ulozeni cyklu
// call_func_input Ukazatel na strukturu pro ulozeni volani funkce
// return_input Ukazatel na strukturu pro ulozeni prikazu navratu z funkce
// at_queue Ukazatel na frontu pro ukladani bloku
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

// Polozka fronty pro ukladani bloku
// GenType Typ polozky fronty
// GenValue Hodnota polozky fronty
// Next Ukazatel na nasledujici polozku
typedef struct atq_item{
    GenType GenType;
    GenValue GenValue;
    struct atq_item * Next;
}ATQItem;

// Struktura fronty pro ukladani bloku
// Front Ukazatel na prvni polozku
typedef struct atQueue{
    ATQItem * Front;
}ATQueue;

// Polozka zasobniku
// Queue Ukazatel na frontu
// Next Ukazatel na nasledujici polozku
typedef struct qSItem{
    ATQueue * Queue;
    struct qSItem * Next;
} QSItem;

// Zasobnik front
// Top Ukazatel na vrchol zasobniku
typedef struct{
    QSItem * Top;
} QStack;

/**
 * Fuknce inicializuje frontu
 * @param s Ukazatel na frontu
 */
void queInit ( ATQueue* s );

/**
 * Funkce zrusi frontu
 * @param s Ukazatel na frontu
 */
void queDestruct ( ATQueue* s );

/**
 * Funkce pro zjisteni jesli je fronta prazdna
 * @param s Ukazatel na frontu
 * @return  Vraci 1 pokud je fronta prazdny jinak vraci 0
 */
int queEmpty ( const ATQueue* s );

/**
 * Funkce pro ziskani prvni polozky fronty
 * @param s Ukazatel na frontu
 * @return Vraci prvni polozku fronty
 */
ATQItem* queFront ( const ATQueue* s);

/**
 * Funkce smaze prvni polozku fronty
 * @param s Ukazatel na frontu
 */
void queRemove ( ATQueue* s );

/**
 * Funkce prida polozku na konec fronty
 * @param s Ukazatel na frontu
 * @param Tree Polozka fronty
 */
void queUp ( ATQueue* s, ATQItem* Tree);

/**
 * Funkce inicializuje frontu
 * @param s Ukazatel na frontu
 */
void equeInit ( eQueue* s );

/**
 * Funkce zrusi frontu
 * @param s Ukazatel na frontu
 */
void equeDestruct ( eQueue* s );

/**
 * Funkce pro zjisteni jesli je fronta prazdna
 * @param s Ukazatel na frontu
 * @return  Vraci 1 pokud je fronta prazdny jinak vraci 0
 */
int equeEmpty ( const eQueue* s );

/**
 * Funkce pro ziskani prvni polozky fronty
 * @param s Ukazatel na frontu
 * @return Vraci prvni polozku fronty
 */
eQItem* equeFront ( const eQueue* s);

/**
 * Funkce smaze prvni polozku fronty
 * @param s Ukazatel na frontu
 */
void equeRemove ( eQueue* s );

/**
 * Funkce prida polozku na konec fronty
 * @param s Ukazatel na frontu
 * @param item Polozka fronty
 */
void equeUp ( eQueue* s, eQItem* item);

/**
 * Funkce inicializuje zasobnik front
 * @param s Ukazatel na zasobnik
 */
void qstackInit ( QStack* s );

/**
 * Funkce zrusi zasobnik
 * @param s Ukazatel na zasobnik
 */
void qstackDestruct ( QStack* s );

/**
 * Funkce pro zjisteni jesli je zasobnik prazdny
 * @param s Ukazatel na zasobnik
 * @return Vraci 1 pokud je zasobnik prazdny jinak vraci 0
 */
int qstackEmpty ( const QStack* s );

/**
 * Funkce pro ziskani vrcholu zasobniku
 * @param s Ukazatel na zasobnik
 * @return Vraci vrchol zasobniku
 */
ATQueue* qstackTop ( const QStack* s);

/**
 * Funkce odebere polozku z vrcholu zasobniku
 * @param s Ukazatel na zasobnik
 */
void qstackPop ( QStack* s );

/**
 * Funkce vlozi polozku do zasobniku
 * @param s Ukazatel na zasobnik
 * @param queue Fronta, ktera se ma pridat do zasobniku
 */
void qstackPush ( QStack* s, ATQueue* queue);

#endif
