#include<stdlib.h>

#define ALLOC_SPACE 100

static char* _string_storage = NULL;

char* add_string_to_storage(char* array,int length)
{
    static int pos;
    static int space;
    if(_string_storage == NULL)
    {
        _string_storage = (char*)malloc(ALLOC_SPACE * sizeof(char));
        pos = 0;
        space = ALLOC_SPACE;
    }
    else if(pos + length > space)
    {
        _string_storage = realloc(_string_storage,pos + length + ALLOC_SPACE);
    }

}

void destruct_storage()
{
    free(_string_storage);
    _string_storage = NULL;
}