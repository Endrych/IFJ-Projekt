#include <stdio.h>
#include <stdlib.h>
#include "lexical.h"
#include "constants.h"

int main(int argc, char **argv)
{
  FILE *source_code; 
  char* adresa;
  char c;
  char *filename= argv[1];
  printf("Filename: %s\n", filename);
  source_code = fopen(filename, "r");
  if(source_code == NULL){
    printf("Error");
  }
  printf("The contents of %s file are :\n", filename);

  while(( c = fgetc(source_code)) != EOF ){
    adresa = handle_characters(c);
  }
  printf("\nAdresa: %p\n", adresa);
  for (int i = 0; i < 300; i++){
    printf("%c",adresa[i]);
  }
  free(adresa);
  fclose(source_code);

  return 0;
}
