#include<stdlib.h>
#include<string.h>
#include<stdio.h>

#define ALLOC_SPACE 100

static char* _string_storage = NULL;

char* add_string_to_storage(char* array)
{
    if(array != NULL)
    {
        int length = strlen(array);
        static int pos;
        static int space;
        if(_string_storage == NULL)
        {
            _string_storage = (char*)malloc((ALLOC_SPACE + length) * sizeof(char));
            if(_string_storage == NULL)
            {
                fprintf(stderr,"Problem with memory\n");
                return NULL;
            }
            pos = 0;
            space = ALLOC_SPACE + length;
        }
        else if(pos + length > space)
        {
            _string_storage = realloc(_string_storage,pos + strlen(array) + ALLOC_SPACE);
            if(_string_storage == NULL)
            {
                fprintf(stderr,"Problem with memory\n");
                return NULL;
            }
        }
        char* result = NULL;
        result = strcpy(&_string_storage[pos],array);
        result[length] = '#';
        pos = length + 1;

        return result;
    }
    return NULL;
}

void destruct_storage()
{
    free(_string_storage);
    _string_storage = NULL;
}