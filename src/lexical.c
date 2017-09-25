#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "constants.h"

Token toSend;
void handle_file(char* arg){

}

char* newName = NULL;    
int size = 0;

void handle_characters(char c){
    if(newName == NULL){
        newName=(char*)malloc(10*sizeof(char));
        size = 10;

    }
    long int len = strlen(newName);    
    switch (c){
        case '/':
            printf("Comment");
            break;
        case ' ':
            printf("%s\n", newName);
            free(newName);
            newName=(char*)malloc(10*sizeof(char));
            for(int i =0; i < len; i++){
                newName[i] = 0;
            }
            break;
        case EOF:
            free(newName);
            break;
        default:
            if(len == size){
                size += 10;
                newName = (char *)realloc(newName, size*sizeof(char));
            }
            // printf("len : %ld \n", len);
            newName[len] = c;
    }
}