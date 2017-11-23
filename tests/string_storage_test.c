#include <stdio.h>
#include "../src/string_storage.h"
#include <stdlib.h>
#include "../src/destructor.h"
#include "../src/stack.h"
#include "../src/at_que.h"
#include "../src/frame.h"
#include "../src/gen_stacks.h"
#include "../src/error.h"
#include "../src/abstract_tree.h"
#include "../src/at_que.h"
//#include "../src/"


int main()
{
    int ahoj_pos = add_string_to_storage("ahoj");
    char* ahoj_string = get_string(ahoj_pos);
    int pos=0;
    while(ahoj_string[pos]!='\0')
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
    int length_pos =  add_string_to_storage(lengt_str);
    char* length_string = get_string(length_pos);
    pos=0;
    while(length_string[pos]!='\0')
    {
        putchar(length_string[pos]);
        pos++;
    }
    putchar('\n');
    

    ahoj_string = get_string(ahoj_pos);
    pos=0;
    while(ahoj_string[pos]!='\0')
    {
        putchar(ahoj_string[pos]);
        pos++;
    }
    putchar('\n');
    
    pos=0;
    while(length_string[pos]!='\0')
    {
        putchar(length_string[pos]);
        pos++;
    }
    putchar('\n');

    free(lengt_str);
    destruct_storage();
    return 0;
}