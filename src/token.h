/* Nazev - Implementace prekladace imperativniho jazyka IFJ17
 * Petr Zubalik - xzubal04
 * Marek Kukucka - xkukuc04
 * Jan Koci - xkocij01
 * David Endrych - xendry02
 */

#ifndef TOKEN_H
#define TOKEN_H

// Typ tokenu
typedef enum{
    type_id,
    type_keyword,
    type_string,
    type_integer,
    type_double,
    type_operator,
    type_wrong,
    type_eol,
    type_semicolon,
    type_eof,
    type_comma,
}Type;

// Pro ulozeni hodnoty klicovych slov
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

// Pro ulozeni hodnoty operatoru
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

// Ukladaji se hodnoty tokenu
// Keywords Ulozena hodnota tokenu, ktery reprezentuje klicove slovo
// int_value Ulozena hodnota tokenu, ktery reprezentuje integer, string nebo identifikator
// double_value Ulozena hodnota tokenu, ktery reprezentuje double
// Operators Ulozena hodnota tokenu, ktery reprezentuje nejaky operator
typedef union{
    Keywords keyword_value;
    int int_value;
    double double_value;
    Operators operator_value;
}Atribute;

// Struktura tokenu
// type Typ tokenu
// Atribute Hodnota tokenu
typedef struct{
    Type type;
    Atribute atribute;
}Token;

/**
 * Funkce vytvori token
 * @return Vraci ukazatel na vytvoreny token
 */
Token* create_token();

/**
 * Funkce urcuje jestli se jedna o klicove slove nebo identifikator
 * @param  text Ukazatel na retezec, ktery se ma vyhodnotit
 * @return  Vrati -1 pokud se jedna o klicove slovo, jinak
 *          vrati hodnotu z enumu Keywords
 */
int is_keyword(char* text);

/**
 * Funkce zrusi token
 * @param token Ukazatel na token, ktery chceme zrusit
 */
void destruct_token(Token* token);

/**
 * Funkce znici vsechny tokeny
 */
void destruct_token_storage();
#endif // TOKEN_H
