#include <stdio.h>
#include "../src/string_storage.h"

int main()
{
    add_string_to_storage("ahoj",4);
    destruct_storage();
    return 0;
}