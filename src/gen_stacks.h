
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

// typedef struct Stack{
//     TSItem* Top;
// } tStack;

// xxxxxxxx PTR STACK xxxxxxxxx

// typedef enum{
//     type_token,
//     type_nonterm,
//     type_handler
// }Type;

// typedef struct{
//     Tvariable_type DataType;
//     SType Type;
//     SAtribute Atr;
// }SData;

void gsval_init(GVStack *s);
void gsval_stackPush ( GVStack* s, GSVData* data);
int gsval_stackEmpty(const GVStack *s);
GSVData* gsval_stackTop(const GVStack *s);
void gsval_stackPop(GVStack *s);
int gsval_stackCount(GVStack *s);
void gsval_stackDestruct(GVStack *s);

#endif