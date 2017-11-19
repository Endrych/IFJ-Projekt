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
  printf("\n%d\n", token->type);

  while(token->type != 9){
    token = get_token();
    printf("%d", token->type);
    if(token->type == 7){
      printf("\n");
    }
  }
  printf("\n");
  close_file();
  destruct_storage();
  destruct_token_storage();
  return 0;
}
