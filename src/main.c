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
    data.type = type_int;
    data2.type = type_int;    
    leaf = make_leaf(data);
    leaf2 = make_leaf(data2);
    data3.type = type_operator;
    data3.Atr.op_value = 3;
    tree = make_tree(leaf, leaf2, data3);
    to_print(tree, 1);
}
