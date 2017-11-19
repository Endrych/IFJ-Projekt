#include "name_generator.h"
#include <stdio.h>
#include <stdlib.h>

/*
Identifikátor pro- ˇ
menné se skládá ze dvou ˇ cástí odd ˇ elených zaviná ˇ cem (znak ˇ @; bez bílých znak ˚u), oznacení ˇ
rámce LF, TF nebo GF a samotného jména promenné (sekvence libovolných alfanume- ˇ
rický a speciálních znak ˚u bez bílých znak ˚u zacínající písmenem nebo speciálním znakem, ˇ
kde speciální znaky jsou: _, -, $, &, %, *). Napˇr. GF@_x znací prom ˇ ennou ˇ _x uloženou
v globálním rámci.
*/

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
        return NULL;
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
        else if(curr >= 26 && curr <= 61){
            name[i] = 'A' + (curr - 26);
        }   
        else{
            name[i] = '0' + (curr - 62);
        }  
        index = (int) index / 62;
        i++;
    }while(index > 0);
    name[i] = '\0';

    return name;
}