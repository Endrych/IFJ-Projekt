/* Nazev - Implementace prekladace imperativniho jazyka IFJ17
 * Petr Zubalik - xzubal04
 * Marek Kukucka - xkukuc04
 * Jan Koci - xkocij01
 * David Endrych - xendry02
 */


#ifndef ERROR_H
#define ERROR_H

#define OK 0

#define LEXICAL_ERROR 1
#define LEXICAL_MESSAGE "Lexical analysis error: wrong structure of actual lexem."

#define SYNTAX_ERROR 2
#define SYNTAX_MESSAGE 	"Syntax analysis error: wrong syntax of the source program."

#define SEMANTIC_ERROR 3
#define SEMANTIC_MESSAGE "Semantic error: undefined function or variable / trying to redefined function or variable."

#define SEMANTIC_TYPE_ERROR 4
#define SEMANTIC_TYPE_MESSAGE "Semantic error: wrong compatibility of types in an expression / wrong number or types of parameters at function call."

#define OTHER_ERROR 6
#define OTHER_MESSAGE "Semantic error."

#define COMPILER_ERROR 99
#define COMPILER_MESSAGE "Compiler error: inner error of the compiler (probably fail of memory allocation)."

void print_error(int error_code);

#endif // ERROR_H