#include "token.h"

typedef enum{
    dt_Integer,
    dt_String,
    dt_Double
}DataType;

typedef struct{
    DataType Type;
    Token* ReturnToken;
    int Pntr;
}PrecendentOutput;

PrecendentOutput* precedence_analysis(Token* token, int * ptr);
int precedence_operation(Token* stack_token,Token* lexical_token);
