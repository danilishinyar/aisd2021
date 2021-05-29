#include <stdio.h>
#include <stdlib.h>
#include<string.h>


typedef struct Item{
    char c;// значение элемента
    struct Item* next;// указатель на следующий элемент списка
}Item;



Item* deleteList(Item* p){
    Item *tmp = NULL;
    while(p != NULL){
        tmp = p;//запоминаем указатель на элемент списка
        p = p->next;//сдвигаемся к следующему элементу
        free(tmp);//освобождаем память от элемента, указатель на который запомнили
    }
    return p;
}



int getList(Item **pp){
    char buf[81], *str;
    Item head = {'*', NULL};
    Item *last = &head;
    int n, r = 1;
    do{
        n = scanf("%80[^\n]", buf);
        if(n<0){//если конец ввода освобождаем память
            deleteList(head.next);
            head.next = NULL;
            r = 0;
            continue;
        }
        if(n>0){//посимвольно вносим элементы в список
            for(str = buf; *str != '\0'; ++str){
                last->next = (Item*)malloc(sizeof(Item));
                last = last->next;
                last->c = *str;
            }
            last->next = NULL;//устанавливаем нулевой указатель в конец списка
        }
        else
            scanf("%*c");
    }while(n>0);
    *pp = head.next;
    return r;
}



void printList(Item* p){
    for(;p != NULL; p = p->next)
        printf("%c", p->c);
    printf("\n");
}



void deleteFirstChar(Item** head) {
  if(*head != NULL) {//если список не нулевой
    char fc = (*head)->c;//символ первого элемента
    Item* next_Item = (*head)->next;
    Item* prev_Item = *head;
    while(next_Item != NULL) {
      if (next_Item->c == fc){
        prev_Item->next = next_Item->next;//перескакиваем через удаляемый елемент
        free(next_Item);
        next_Item = prev_Item->next;//указатель на предыдущий оставляем на месте, а следующий переводим на элемент вперед
      }
      else{//иначе переводим оба указателя на элемент веперд
      prev_Item = prev_Item->next;
      next_Item = prev_Item->next;
      }
    }
  }
}



void deleteSpace(Item** head){
  if(*head != NULL){
    //удаляем начальные пробелы и/или табуляции
    Item* elm;
    Item* elm_next;
    while((*head)->c == ' ' || (*head)->c == '\t'){
      elm = *head;
      *head = (*head)->next;
      free(elm);
    }
    //удаляем лишние пробелы между словами
    elm = *head;
    elm_next = (*head)->next;

    while(elm_next != NULL){
        if((elm->c == ' ' || elm->c == '\t') && (elm_next->c == ' ' || elm_next->c == '\t')){
            elm->next = elm_next->next;
            free(elm_next);
            elm_next = elm->next;
          }
        else{
          elm = elm->next;
          elm_next = elm->next;
        }
      }
    //удаляем хвостовой пробел
    elm = *head;
    while(elm->next->next != 0)
      elm = elm->next;
    if(elm->next->c == ' ' || elm->next->c == '\t'){
      free(elm->next);
      elm->next = NULL;
    }
    //меняем оставшиеся табуляции на пробелы
    elm = *head;
    while(elm != NULL){
      if(elm->c == '\t')
        elm->c = ' ';
      elm = elm->next;
      }
  }
}



int main() {
    Item *str;
    while(puts("\nEnter string:"), getList(&str)){
        deleteSpace(&str);
        deleteFirstChar(&str);
        printList(str);
        str = deleteList(str);
    }
    return 0;
}
