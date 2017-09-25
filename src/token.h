typedef enum{
    And,
    As,
    Asc,
    Boolean,
    Continue,
    Declare,
    Dim,
    Do,
    Double,
    Else,
    Elseif,
    End,
    Exit,
    False,
    For,
    Chr,
    Function,
    If,
    Input,
    Integer,
    Length,
    Loop,
    Next,
    Not,
    Or,
    Print,
    Return,
    Scope,
    Shared,
    Static,
    String,
    SubStr,
    Then,
    True,
    While
}Keyword;

typedef union{
    Keywords keyword;
    int int_value;
    double double_value;
    int* chain_pointer;
}Atribute;

typedef struct{
    Type type;
    Atribute atribute;
}Token;
