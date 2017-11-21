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
#define N 3

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

    Token f;
    f.type = type_id;
    char * f_id = "func";
    f.atribute.int_value = add_string_to_storage(f_id);
    Tsymtab_item * item1 =  symtab_insert(symtab,&f,type_function);
    item1->type = type_int;
    item1->type_strct.function->declared = true;
    item1->type_strct.function->return_type = type_int;

    ATLeaf* cond;
    ATQueue state_true;
    ATQueue state_false;
    queInit(&state_true);
    queInit(&state_false);
    char *input = malloc(sizeof(250));
    PrecendentOutput * out;

    switch(N){
        case 0:
            while(1){
                printf("Zadejte vyraz\n");
                out = precedence_analysis(NULL);
                generate_expression(out->Tree);
            }
            break;
        case 1:
            while(1){
                printf("Zadejte podminku\n");
                out = precedence_analysis(NULL);
                cond = out->Tree;
                printf("Zadejte true vyraz\n");
                out = precedence_analysis(NULL);
                queUp(&state_true, out->Tree);
                 printf("Zadejte druhy true vyraz\n");
                out = precedence_analysis(NULL);
                queUp(&state_true, out->Tree);
                printf("Zadejte false vyraz\n");
                out = precedence_analysis(NULL);
                queUp(&state_false, out->Tree);
                printf("Zadejte druhy false vyraz\n");
                out = precedence_analysis(NULL);
                queUp(&state_false, out->Tree);
                generate_if(cond,&state_true,&state_false);
                printf("\n______________________________\n");
            }
            break;
        case 2:
            while(1){
                printf("Zadejte podminku\n");
                out = precedence_analysis(NULL);
                cond = out->Tree;
                printf("Zadejte vyraz\n");
                out = precedence_analysis(NULL);
                queUp(&state_true, out->Tree);
                 printf("Zadejte druhy vyraz\n");
                out = precedence_analysis(NULL);
                queUp(&state_true, out->Tree);
                
                generate_while(cond,&state_true);
                printf("\n______________________________\n");
            }
            break;
        case 3:
            while(1){
                printf("Generovani funkci\n");
                printf("Zadejte vyraz\n");
                out = precedence_analysis(NULL);
                queUp(&state_true, out->Tree);
                 printf("Zadejte druhy vyraz\n");
                out = precedence_analysis(NULL);
                queUp(&state_true, out->Tree);
                
                generate_function(item1,&state_true);
                printf("\n______________________________\n");
            }
            break;
    }
    
   
}