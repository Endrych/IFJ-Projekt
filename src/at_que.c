#include "at_que.h"
#include <stdlib.h>

void equeInit ( eQueue* s ){
    s->Front = NULL;
}
void equeDestruct ( eQueue* s ){
    if(s != NULL){
        eQItem* current = s->Front;
        eQItem* rm = NULL;
        while(current != NULL){
            rm = current;
            current = current->Next;
            dispose_at(rm->Expr);
            free(rm);
        }
        free(s);
        s = NULL;
    }
}
int equeEmpty ( const eQueue* s ){
    return (s->Front == NULL);
}
ATLeaf* equeFront ( const eQueue* s){
    if(!equeEmpty(s))
        return s->Front->Expr;
    else
        return NULL;
}
void equeRemove ( eQueue* s ){
    eQItem * rm = s->Front;
    s->Front = rm->Next;
    dispose_at(rm->Expr);
    free(rm);
}
void equeUp ( eQueue* s, ATLeaf* Tree){
    eQItem* new_elem = (eQItem*) malloc(sizeof(eQItem));
    if(new_elem == NULL){
        return;
    }
    new_elem->Expr = Tree;
    new_elem->Next = NULL;
    eQItem * current = s->Front;
    if(equeEmpty(s)){
        s->Front = new_elem;
        return;
    }
    while(current->Next != NULL){
        current = current->Next;
    }
    current->Next = new_elem;
}

void queInit ( ATQueue* s ){
    s->Front = NULL;
}

void queDestruct ( ATQueue* s ){
    if(s != NULL){
        ATQItem* current = s->Front;
        ATQItem* rm = NULL;
        while(current != NULL){
            rm = current;
            current = current->Next;
            free(rm);
        }
        free(s);
        s = NULL;
    }
}

int queEmpty ( const ATQueue* s ){
    return (s->Front == NULL);
}

ATQItem* queFront ( const ATQueue* s){
    if(!queEmpty(s))
        return s->Front;
    else
        return NULL;
}

void queRemove ( ATQueue* s ){
    ATQItem * rm = s->Front;
    s->Front = rm->Next;
    free(rm);
}

void queUp ( ATQueue* s, ATQItem * item){
    ATQItem* new_elem = (ATQItem*) malloc(sizeof(ATQItem));
    if(new_elem == NULL){
        return;
    }
    new_elem = item;
    new_elem->Next = NULL;
    ATQItem * current = s->Front;
    if(queEmpty(s)){
        s->Front = new_elem;
        return;
    }
    while(current->Next != NULL){
        current = current->Next;
    }
    current->Next = new_elem;
}

void qstackInit ( QStack* s ){
    s->Top = NULL;
}
void qstackDestruct ( QStack* s ){
    if(s != NULL){
        QSItem* current = s->Top;
        QSItem* rm = NULL;
        while(current != NULL){
            rm = current;
            current = current->Next;
            free(rm);
        }
        free(s);
        s = NULL;
    }
}
int qstackEmpty ( const QStack* s ){
    return (s->Top == NULL);
}
ATQueue* qstackTop ( const QStack* s){
    if(!qstackEmpty(s))
        return s->Top->Queue;
    else
        return NULL;
}
void qstackPop ( QStack* s ){
    QSItem* rm = s->Top;
    s->Top = rm->Next;
    free(rm);
}
void qstackPush ( QStack* s, ATQueue* queue){
    QSItem* new_elem = (QSItem*) malloc(sizeof(struct qSItem));
    if(new_elem == NULL){
        return;
    }
    new_elem->Queue = queue;
    new_elem->Next = s->Top;
    s->Top = new_elem;
}