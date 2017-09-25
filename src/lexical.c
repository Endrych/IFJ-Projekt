#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "constants.h"

char* str = NULL;    
char* storage = NULL;
char* pointer = NULL;
size_t size = 0;
size_t storage_size = 0;
size_t len = 0;
size_t newSize = 0;

Token toSend;
void handle_file(char* arg){

}

int* handle_characters(char c){
    if(str == NULL){
        size = 10;
        str=(char*)malloc(size*sizeof(char));
    }
    len = strlen(str);    
    switch (c){
        // case '/':
        //     printf("Comment");
        //     break;
        case ' ':
        case '\n':
            // printf("%d\n", storage_size);
            // printf("%d\n", len);
            
            if(storage == NULL){
                newSize = len + 1;
                storage=(char *)calloc(newSize,sizeof(char));
            }else{
                newSize = storage_size + len + 1;
                storage = (char *)realloc(storage, newSize*sizeof(char));
            }
            
            for (size_t i = storage_size, j = 0; i < newSize;i++, j++){
                if(i == newSize - 1){
                  storage[i] = '#';                                    
                }else{
                  storage[i] = str[j];  
                  printf("%c",storage[i]);              
                }
            }
            pointer = &storage[storage_size];
            printf("\nPointer: %c\n", *pointer);
            storage_size += len + 1;
            free(str);
            size = 10;
            str=(char*)calloc(size,sizeof(char));
            break;
        case EOF:
            free(str);
            break;
        default:
            if(len == size){
                size += 10;
                str = (char *)realloc(str, size*sizeof(char));
            }
            str[len] = c;
    }
    return storage;
}