#include<stdlib.h>
#include<string.h>
#include<stdio.h>

#define ALLOC_SPACE 100

static char* _string_storage = NULL;

int add_string_to_storage(char* array)
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
                return -1;
            }
            pos = 0;
            space = ALLOC_SPACE + length;
        }
        else if(pos + length > space)
        {
            _string_storage =  (char*) realloc(_string_storage,(space + strlen(array) + ALLOC_SPACE) * sizeof(char));
            if(_string_storage == NULL)
            {
                fprintf(stderr,"Problem with memory\n");
                return -1;
            }
            space = space + strlen(array) + ALLOC_SPACE;
        }
        
        char* result = NULL;

        result = strcpy(&_string_storage[pos],array);
        result[length] = '#';

        int old_pos = pos;
        pos += length + 1;

        return old_pos;
    }
    return -1;
}

char * get_string(int pos)
{
	int length = 0;
	int temp = pos;
	while(_string_storage[temp] != '#')
	{	
		temp++;
		length++;
	}

	char *key = (char *) malloc(sizeof(char) * length + 1);
	if (key == NULL)
	{
		fprintf(stderr, "Memory allocation failed!\n");
		return NULL;
	}

	int i = 0;
	while(i < length)
	{
		key[i] = _string_storage[pos];
		i++;
		pos++;
	}

	key[i] = '\0';
    
    return key;
}

void destruct_storage()
{
    free(_string_storage);
    _string_storage = NULL;
}
