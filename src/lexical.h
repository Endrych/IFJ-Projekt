#include "token.h"

#ifndef LEXICAL_H
#define LEXICAL_H

Token* get_token();
int load_file(char* arg);
int close_file();

#endif