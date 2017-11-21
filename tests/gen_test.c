#include "../src/precedence_sa.h"
#include "../src/lexical.h"
#include "../src/precedence_sa.h"
#include <stdlib.h>
#include <stdio.h>
#include "../src/error.h"
#include "../src/string_storage.h"
#include "../src/generator.h"

extern Tsymtab * symtab;

int main(){
    symtab = symtab_init(17);
    while(1){
        printf("Zadejte vyraz\n");
        PrecendentOutput * out = precedence_analysis(NULL);
        generate_expression(out->Tree);
    }
}