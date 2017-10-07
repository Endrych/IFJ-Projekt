typedef enum{
    type_id,
    type_keyword,
    type_string,
    type_integer,
    type_double,
    type_operator,
    type_wrong
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

typedef enum{
    op_assign,
    op_add,
    op_sub,
    op_mul,
    op_division_int,
    op_slash,
    op_bracket,
    op_bracket_end,
    op_not_equal,
    op_lesser,
    op_lesser_equal,
    op_greater,
    op_greater_equal
}Operators;

typedef union{
    Keywords keyword_value;
    int int_value;
    double double_value;
    Operators operator_value;
}Atribute;

typedef struct{
    Type type;
    Atribute atribute;
}Token;


Token* create_token();
int is_keyword(char* text);
void destruct_token(Token* token);
