#include <stdio.h>
#include <stdlib.h>
#include "lexical.h"
#include "constants.h"

int main(int argc, char **argv)
{
  FILE *source_code; 
  char c;
  char *filename= argv[1];
  printf("Filename: %s\n", filename);
  source_code = fopen(filename, "r");
  if(source_code == NULL){
    printf("Error");
  }
  printf("The contents of %s file are :\n", filename);

  while(( c = fgetc(source_code)) != EOF )
    handle_characters(c);

    

  fclose(source_code);

  return 0;
}
