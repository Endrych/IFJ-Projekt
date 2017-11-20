#include <stdio.h>
#include "../src/generator.h"
#include <stdlib.h>
#include "../src/gen_stacks.h"

int main(){
    open_output();
     ATLeaf *leaf;
    ATLeaf *leaf2;
    ATLeaf *tree;
    ATLeaf *tree2;
    ATData data;
    ATData data2;    
    ATData data3; 
   	Token* token = create_token();
   	Token* token2 = create_token();
    printf("@@\n");

    token->atribute.double_value = 7.5;
    token->type = type_double;
    token2->atribute.int_value = 2; 
    token2->type = type_integer;
    data.Atr.token = token;
    data2.Atr.token = token2;
    data.type = at_token;
    data2.type = at_token;    
    leaf = make_leaf(data);
    leaf2 = make_leaf(data2);
    data3.type = at_operators;
    data3.Atr.op_value = 3;
    tree = make_tree(leaf, leaf2, data3);
    to_print(tree, 1);
    generate_expression(tree);

    return 0;
}