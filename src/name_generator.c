#include "name_generator.h"


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
    if(type == gt_variable){
        static int variable_index;
        variable_index++;
    }
    else if(type == gt_label){
        static int label_index;
        label_index++;
    }
}