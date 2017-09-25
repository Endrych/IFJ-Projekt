typedef enum{
    type_id,
    type_keyword,
    type_string,
    type_integer,
    type_double
}Type;

typedef enum{
    kw_and,
    kw_as,
    kw_asc,
    kw_boolean,
    kw_chr,
    kw_continue,
    kw_declare,
    kw_dim,
    kw_do,
    kw_double,
    kw_else,
    kw_elseif,
    kw_end,
    kw_exit,
    kw_false,
    kw_for,
    kw_function,
    kw_if,
    kw_input,
    kw_integer,
    kw_length,
    kw_loop,
    kw_next,
    kw_not,
    kw_or,
    kw_print,
    kw_return,
    kw_scope,
    kw_shared,
    kw_static,
    kw_string,
    kw_subStr,
    kw_then,
    kw_true,
    kw_while
}Keywords;

typedef union{
    Keywords keyword;
    int int_value;
    double double_value;
    char* chain_pointer;
}Atribute;

typedef struct{
    Type type;
    Atribute atribute;
}Token;


Token* create_token();
void destruct_token(Token* token);