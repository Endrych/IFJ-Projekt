#include "token.h"

typedef enum{
    dt_Integer,
    dt_String,
    dt_Double
}DataType;

typedef struct{
    DataType Type;
    int * pntr;
}PrecendentOutput;

PrecendentOutput precedence_analysis(Token* token, int * ptr);
