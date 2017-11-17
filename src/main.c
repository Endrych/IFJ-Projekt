#include <stdio.h>
#include <stdlib.h>
#include "token.h"
#include "string_storage.h"
#include "lexical.h"

int main(int argc, char **argv)
{
  load_file(argv[1]);
  Token* token; 
  token = get_token();
  printf("\n%d", token->type);
  // destruct_token(token);
    token = get_token();
  printf("\n%d", token->type);
  // destruct_token(token);
    token = get_token();
  printf("\n%d", token->type);
  // destruct_token(token);
  // token = get_token();
  // printf("\n%d %f", token->type, token->atribute.operator_value);
  // destruct_token(token);
  // token = get_token();
  // printf("\n%d %f", token->type, token->atribute.double_value);
  // destruct_token(token);
  // token = get_token();
  // printf("\n%d %d", token->type, token->atribute);  
  // destruct_token(token);  
  // token = get_token();  
  // printf("\n%d %d", token->type, token->atribute);  
  // destruct_token(token);  
  close_file();
   destruct_storage();
  destruct_token_storage();
  return 0;
}
