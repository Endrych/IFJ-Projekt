#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "token.h"

char* str = NULL;    
char* storage = NULL;
char* pointer = NULL;
char temp;
Token* token;
bool comment = false;
bool is_string = false;
size_t size = 0;
size_t storage_size = 0;
size_t len = 0;
size_t newSize = 0;

FILE* handle_file(char* arg){
    FILE *source_code; 
    char* adresa;
    char c;
    source_code = fopen(arg, "r");
    if(source_code == NULL){
        printf("Error");
        fclose(source_code);
    }
    printf("The contents of %s file are :\n", source_code);

    return source_code;
}

void* handle_characters(char* arg){
    FILE* source;
    source = handle_file(arg);
    if(str == NULL){
        size = 10;
        str=(char*)malloc(size*sizeof(char));
    }
    len = strlen(str);   
    // char c;
    // while(c != EOF){
    //     c = getc(source);
    //     printf("%c", c);
    // switch(c){
    //     case '\'':
    //         if(temp == '/'){
    //             comment = true;
    //         }
    //         break;
    //     case '0':
    //     case '1':
    //     case '2':
    //     case '3':
    //     case '4':
    //     case '5':
    //     case '6':
    //     case '7':
    //     case '8':
    //     case '9':
    //         if(comment){
    //             break;
    //         }
    //         if(len == size){
    //             size += 10;
    //             str = (char *)realloc(str, size*sizeof(char));
    //         }
    //         str[len] = c;
    //         break;
    //     case 'a' ... 'z':
    //     case 'A' ... 'Z':
    //         if(comment){
    //             break;
    //         }
    //         if(len == size){
    //             size += 10;
    //             str = (char *)realloc(str, size*sizeof(char));
    //         }
    //         str[len] = c;
    //         break;
    //     case '+':
    //     case '-':
    //     case '*':
    //     case '/':
    //         if(temp == '\''){
    //             comment = false;
    //         }
    //     case '<':
    //     case '>':
    //     case '=':
    //         break;

    //     case '(':
    //         break;
    //     case ')':
    //         break;
    //     case '!':
    //         if(len == 0){
    //             break;
    //         }
    //     case '"':
    //         if(temp == '!'){
    //             is_string = true;
    //             break;
    //         }
    //         else if (is_string){
    //             is_string = false;
    //         }
    //     case ' ':
    //         if(is_string){
    //             if(len == size){
    //                 size += 10;
    //                 str = (char *)realloc(str, size*sizeof(char));
    //             }
    //             str[len] = c;
    //             break;
    //         }
    //     case '\n':
    //         if(len > 0){
    //             if(storage == NULL){
    //                 newSize = len + 1;
    //                 storage=(char *)calloc(newSize,sizeof(char));
    //             }else{
    //                 newSize = storage_size + len + 1;
    //                 storage = (char *)realloc(storage, newSize*sizeof(char));
    //             }
                
    //             for (size_t i = storage_size, j = 0; i < newSize;i++, j++){
    //                 if(i == newSize - 1){
    //                 storage[i] = '#';     
    //                 printf("%c",storage[i]);            
                    
    //                 }else{
    //                 storage[i] = str[j];  
    //                 printf("%c",storage[i]);            
    //                 }
    //             }
    //             pointer = &storage[storage_size];
    //             // printf("\nPointer: %c\n", *pointer);
    //             storage_size += len + 1;
    //             free(str);
    //             size = 10;
    //             str=(char*)calloc(size,sizeof(char));

    //             token = create_token();
    //             token->type = type_string;
    //             token->atribute.chain_pointer = &storage[storage_size];
    //             handle_token(token);
    //         }

    //     break;
    // }
    // temp = c;
    // }
    
    // switch (c){
    //     // case '/':
    //     //     printf("Comment");
    //     //     break;
    //     case ' ':
    //     case '\n':
    //         // printf("%d\n", storage_size);
    //         // printf("%d\n", len);
            
    //         if(storage == NULL){
    //             newSize = len + 1;
    //             storage=(char *)calloc(newSize,sizeof(char));
    //         }else{
    //             newSize = storage_size + len + 1;
    //             storage = (char *)realloc(storage, newSize*sizeof(char));
    //         }
            
    //         for (size_t i = storage_size, j = 0; i < newSize;i++, j++){
    //             if(i == newSize - 1){
    //               storage[i] = '#';                                    
    //             }else{
    //               storage[i] = str[j];  
    //               printf("%c",storage[i]);              
    //             }
    //         }
    //         pointer = &storage[storage_size];
    //         printf("\nPointer: %c\n", *pointer);
    //         storage_size += len + 1;
    //         free(str);
    //         size = 10;
    //         str=(char*)calloc(size,sizeof(char));
    //         break;
    //     case EOF:
    //         free(str);
    //         break;
    //     default:
    //         if(len == size){
    //             size += 10;
    //             str = (char *)realloc(str, size*sizeof(char));
    //         }
    //         str[len] = c;
    // }
    // return storage;
    fclose(source);
    return 0;
}

void handle_token(Token* token){
    printf("ID: %d Value: %p\n", token->type, token->atribute);

}