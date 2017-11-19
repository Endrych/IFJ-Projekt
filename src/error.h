/* error.h

*/
#ifndef ERROR_H
#define ERROR_H

#define OK 0

#define LEXICAL_ERROR 1

#define SYNTAX_ERROR 2

#define SEMANTIC_ERROR 3

#define SEMANTIC_TYPE_ERROR 4

#define OTHER_ERROR 6

#define COMPILER_ERROR 99

typedef int make_iso_compilers_happy;

#endif // ERROR_H