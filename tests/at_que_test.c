#include <stdio.h>
#include "../src/at_que.h"
#include <stdlib.h>

int main(){
    ATQueue * a = malloc(sizeof(ATQueue));
    eQueue *  e = malloc(sizeof(eQueue));
    queInit(a);
    equeInit(e);
    ATQItem * aItem = malloc(sizeof(ATQItem));
    aItem->GenType = gt_return;
    aItem->GenValue.id = NULL;
    queUp(a,aItem);
    equeUp(e,NULL);
    if(queFront(a)->GenType == gt_return){
        printf("Correct\n");
    }
    queRemove(a);
    equeRemove(e);
    //equeRemove(e);
    queDestruct(a);
    equeDestruct(e);
    return 0;
}