#include <stdio.h>
#include <stdlib.h>
#include "token.h"
#include "lexical2.h"

int main(int argc, char **argv)
{
  load_file(argv[1]);
  Token* token; 
  token = get_token();
  printf("\n%d %p", token->type, token->atribute.chain_pointer);
  destruct_token(token);
  token = get_token();
  printf("\n%d %d", token->type, token->atribute);  
  destruct_token(token);  
  close_file();
  return 0;
}
