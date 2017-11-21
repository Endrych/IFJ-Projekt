#include "../src/precedence_sa.h"
#include "../src/lexical.h"
#include "../src/precedence_sa.h"
#include <stdlib.h>
#include <stdio.h>
#include "../src/error.h"
#include "../src/string_storage.h"
#include "../src/generator.h"

extern Tsymtab * symtab;
#define N 0

int main(){
    printf("prom je definovana na 5\n");
    symtab = symtab_init(17);
    Token x;
    x.type = type_id;
    char * x_id = "prom";
    x.atribute.int_value = add_string_to_storage(x_id);
    Tsymtab_item * item =  symtab_insert(symtab,&x,type_variable);
    item->type = type_int;
    item->type_strct.variable->declared = true;
    item->type_strct.variable->type = type_int;
    item->type_strct.variable->value.value_int = 5;
    switch(N){
        case 0:
            printf("Zadejte vyraz\n");
            PrecendentOutput * out = precedence_analysis(NULL);
            generate_expression(out->Tree);
            break;
    }
   
}