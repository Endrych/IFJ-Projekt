#include <stdio.h>
#include "../src/name_generator.h"
#include <stdlib.h>

int main(){
    for(int i = 0;i<10000;i++){
        printf("%s\n",generate_name(gt_label));
    }
   /* for(int i = 0;i<250;i++){
        printf("%s\n",generate_name(gt_variable));
    }*/
}