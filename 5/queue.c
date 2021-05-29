#include"lab.h"


void init(queue* q){
    q->head = NULL;
    q->tail = NULL;
}


list* new_node(Graph* ptr){
    list* tmp = (struct list*)malloc(sizeof(list));
    tmp->ptr = ptr;
    tmp->next = NULL;
    return tmp;
}


void insert(queue* q, Graph* ptr){
    list* tmp = new_node(ptr);
    if (q->tail == NULL) {
        q->head = q->tail = tmp;
        return;
    }
    q->tail->next = tmp;
    q->tail = tmp;
}


int remove_node(queue* q){
    if (q->head == NULL)
        return 0;
    list* tmp = q->head;
    q->head = q->head->next;
    free(tmp);
    if (q->head == NULL)
        q->tail = NULL;
    return 1;

}


int is_empty(queue* q){
    if(q->head == NULL)
        return 0;
    return 1;
}