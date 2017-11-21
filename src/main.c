#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include "symtable.h"
#include "token.h"
#include "abstract_tree.h"
#include "string_storage.h"
#include "lexical.h"
#include "generator.h"
#include "gen_stacks.h"

int main(int argc, char **argv)
{
  // load_file(argv[1]);
  // Token* token; 
  // token = get_token();
  // printf("\n%d\n", token->type);

  // while(token->type != 9){
  //   token = get_token();
  //   printf("%d", token->type);
  //   if(token->type == 7){
  //     printf("\n");
  //   }
  // }
  // printf("\n");
  // close_file();
  // destruct_storage();
  // destruct_token_storage();
  // return 0;

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

    token->atribute.int_value = 7;
    token2->atribute.int_value = 2;    
    data.Atr.token = token;
    data2.Atr.token = token2;
    data.type = at_token;
    data2.type = at_token;    
    leaf = make_leaf(data);
    leaf2 = make_leaf(data2);
    data3.type = at_operators;
    data3.Atr.op_value = 3;
    tree = make_tree(leaf, leaf2, data3);
    data3.type = at_operators;
    data3.Atr.op_value = 0;
    Token * x = create_token();
    x->type = type_id;
    char * x_str = "x";
    int x_index = add_string_to_storage(x_str);
    x->atribute.int_value = x_index;
    Tsymtab * symtab = symtab_init(17);
    Tsymtab_item *item;
    item = symtab_insert(symtab,x,type_variable);
    data.Atr.tsItem = item;
    data.type = at_tsitem;
    leaf = make_leaf(data);
    tree2 = make_tree(leaf, tree, data3);
    to_print(tree2, 1);

    GVStack *s = (GVStack*) malloc(sizeof(struct GVStack));
    gsval_init(s);
    GSVData new_data;
    new_data.type = gvs_type_int;
    new_data.value.int_value = 45;
    gsval_stackPush(s, &new_data);
    gsval_stackPush(s, &new_data);
    gsval_stackPush(s, &new_data);
    gsval_stackPush(s, &new_data);
    gsval_stackPush(s, &new_data);    
    int one;
    one = gsval_stackCount(s);
    printf("\nw%d\n", one);
    gsval_stackPop(s);
    one = gsval_stackCount(s);
    printf("\nw%d\n", one);
    gsval_stackPop(s);
    GSVData *lol;
    lol = gsval_stackTop(s);
    printf("\n%d\n",lol->value.int_value);

    gsval_stackDestruct(s);

    GPStack *p = (GPStack*) malloc(sizeof(struct GPStack));
    gsptr_stackInit(p);
    gsptr_stackPush(p, leaf);
    gsptr_stackPush(p, leaf);
    ATLeaf * todo;
    todo = gsptr_stackTop(p);
    gsptr_stackDestruct(p);
    to_print(todo, 1);
    // one = gsval_stackCount(s);
    // printf("\nw%d\n", one);
    // free(s);
}
