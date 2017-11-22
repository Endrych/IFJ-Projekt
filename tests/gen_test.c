#include "../src/precedence_sa.h"
#include "../src/lexical.h"
#include "../src/precedence_sa.h"
#include <stdlib.h>
#include <stdio.h>
#include "../src/error.h"
#include "../src/string_storage.h"
#include "../src/generator.h"
#include "../src/at_que.h"

extern Tsymtab * symtab;
#define N 1

int main(){
    symtab = symtab_init(17);
   
}