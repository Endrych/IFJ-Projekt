#include <stdio.h>
#include "../src/string_storage.h"
#include <stdlib.h>

int main()
{
    char* ahoj_string = add_string_to_storage("ahoj");
    int pos=0;
    while(ahoj_string[pos]!='#')
    {
        putchar(ahoj_string[pos]);
        pos++;
    }
    putchar('\n');
    char * lengt_str = NULL;
    lengt_str = (char*) malloc(180 * sizeof(char));
    char c = 'a';
    for(int i=0;i<178;i++)
    {
        lengt_str[i] = c;
        if(c != 'z')
            c++;
        else
            c = 'a';
    }
    lengt_str[179] = '\n';
    char* length_string = add_string_to_storage(lengt_str);
    pos=0;
    while(length_string[pos]!='#')
    {
        putchar(length_string[pos]);
        pos++;
    }
    putchar('\n');

    pos=0;
    while(ahoj_string[pos]!='#')
    {
        putchar(ahoj_string[pos]);
        pos++;
    }
    putchar('\n');

    pos=0;
    while(length_string[pos]!='#')
    {
        putchar(length_string[pos]);
        pos++;
    }
    putchar('\n');

    free(lengt_str);
    destruct_storage();
    return 0;
}