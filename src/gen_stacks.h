#include "abstract_tree.h"

#ifndef STACK_H
#define STACK_H

// xxxxxxx VAR STACK xxxxxxxxx
typedef enum{
    gvs_type_int,
    gvs_type_double
}GVSType;

typedef union{
    int int_value;
    double float_value;
}GVSValue;

typedef struct{
    GVSType type;
    GVSValue value;
}GSVData;

typedef struct GVSVal{
    GSVData* data;
    struct GVSVal* next;
}GVSVal;

typedef struct GVStack{
    GVSVal* Top;
} GVStack;

// xxxxxxxx PTR STACK xxxxxxxxx

typedef struct GPSVal{
    ATLeaf *leaf;
    struct GPSVal *next;
}GPSVal;

typedef struct GPStack{
    GPSVal* Top;
} GPStack;


void gsval_init(GVStack *s);
void gsval_stackPush ( GVStack* s, GSVData* data);
int gsval_stackEmpty(const GVStack *s);
GSVData* gsval_stackTop(const GVStack *s);
void gsval_stackPop(GVStack *s);
int gsval_stackCount(GVStack *s);
void gsval_stackDestruct(GVStack *s);
void gsptr_stackInit(GPStack *s);
void gsptr_stackPush(GPStack* s, ATLeaf* leaf);
int gsptr_stackEmpty(const GPStack *s);
ATLeaf* gsptr_stackTop(const GPStack *s);
void gsptr_stackPop(GPStack *s);
void gsptr_stackDestruct(GPStack *s);

#endif