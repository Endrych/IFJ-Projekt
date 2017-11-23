#include "name_generator.h"
#include "destructor.h"
#include <stdio.h>
#include <stdlib.h>

char * generate_name(GType type){
    int length = 1;
    int index = 0;
    int i = 0;
    if(type == gt_variable){
        static int variable_index;
        index = variable_index;
        variable_index++;
    }
    else{
        static int label_index;
        index = label_index;
        label_index++;
    }

    length += index / 62;
    length = length + 1;
    char * name = malloc(sizeof(char) * length);
    if(name == NULL){
        fprintf(stderr, "%s\n", COMPILER_MESSAGE);
        dispose_global();
    }
    if(type == gt_label)
        name[0] = '$';
    else
        name[0] = '&';
    i++;
    do{
        int curr = index % 62;
        if(curr >= 0 && curr <= 25){
            name[i] = 'a' + curr;
        }   
        else if(curr >= 26 && curr <= 51){
            name[i] = 'A' + (curr - 26);
        }   
        else{
            name[i] = '0' + (curr - 52);
        }  
        index = (int) index / 62;
        i++;
    }while(index > 0);
    name[i] = '\0';

    return name;
}