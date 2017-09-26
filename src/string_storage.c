#include<stdlib.h>

#define ALLOC_SPACE 100

static char* _session_storage = NULL;

char* add_string(char* array,int length)
{
    static int pos;
    static int space;
    if(_session_storage == NULL)
    {
        _session_storage = (char*)malloc(ALLOC_SPACE * sizeof(char));
        pos = 0;
        space = ALLOC_SPACE;
    }
    else if(pos + length > space)
    {
        _session_storage = realloc(_session_storage,pos + length + ALLOC_SPACE);
    }
    
}

void destruct_storage()
{
    free(_session_storage);
    _session_storage = NULL;
}