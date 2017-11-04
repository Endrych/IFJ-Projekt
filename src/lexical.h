#include "token.h"

#ifndef LEXICAL_H
#define LEXICAL_H

Token* get_token();
void load_file(char* arg);
void close_file();

#endif