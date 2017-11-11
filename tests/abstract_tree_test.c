#include <stdio.h>
#include "../src/abstract_tree.h"
#include "../src/token.h"


int main(){
    printf("Wow@)");
    ATLeaf *leaf;
    Operators operator;
    ATData data;
    data.int_value = 1;
    leaf = make_leaf(data);
    printf("HOTOVO: %d", leaf->data.int_value);

    return 0;
}