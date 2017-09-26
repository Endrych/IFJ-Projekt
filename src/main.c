#include <stdio.h>
#include <stdlib.h>
#include "token.h"
#include "lexical2.h"

int main(int argc, char **argv)
{
  load_file(argv[1]);
  get_token();
  
  return 0;
}
