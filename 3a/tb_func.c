#include"lab.h"


Table* initialize(int msize1, int msize2){
  if(msize1 <= 0 || msize2<= 0){
    printf("\nWrong table size!\n");
    return NULL;
  }
  Table* ptb = (Table*) calloc(1, sizeof(Table));
  ptb->ks1 = (KeySpace1*) calloc(msize1, sizeof(KeySpace1));
  ptb->ks2 = (KeySpace2**) calloc(msize2, sizeof(KeySpace2*));
  ptb->msize1 = msize1;
  ptb->msize2 = msize2;
  ptb->csize1 = 0;
  ptb->csize2 = 0;
  return ptb;
}

//FIND

KeySpace1* find_ks1(Table* ptb, int key){
  int i;
  for(i = 0; i < ptb->csize1; ++i)
    if(ptb->ks1[i].key == key)
      return ptb->ks1+i;
  return NULL;
}


KeySpace2* find_ks2(Table *ptb, char* key) {
    int i;
    KeySpace2* ptr;
    for(i = 0; i < ptb->msize2; ++i) {
        ptr = ptb->ks2[i];
        if (ptr != NULL) {
            while (ptr) {
                if (!strcmp(key, ptr->key))
                    return ptr;
                ptr = ptr->next;
            }
        }
    }
    return NULL;
}


KeySpace2* find_ks2_release(Table* ptb, char* key, int release){
    int i;
    KeySpace2* ptr;
    for(i = 0; i < ptb->msize2; ++i) {
        ptr = ptb->ks2[i];
        if (ptr != NULL) {
            while (ptr) {
                if (!strcmp(key, ptr->key) && ptr->release == release)
                    return ptr;
                ptr = ptr->next;
            }
        }
    }
    return NULL;
}


int find_ks2_mrelease(Table* ptb, char* key){
  if(!find_ks2(ptb, key))
    return -1;
  int r = find_ks2(ptb, key)->release;
  return r;
}


int find_release(Table* ptb, int key1, char* key2){
    int r=0;
    Item* item = find_ks1(ptb, key1)->item;
    KeySpace2* ptr = find_ks2(ptb, item->key2);
    while(ptr){
        if(ptr->item == item) {
            r = ptr->release;
            return r;
        }
        ptr = ptr->next;
    }
    return r;
}


KeySpace2* find_ks1ks2(Table* ptb, int key1, char* key2){
    KeySpace2* ptr2 = find_ks2(ptb, key2);
    if(ptr2 == NULL){
        return NULL;
    }
    KeySpace1* ptr1 = find_ks1(ptb, key1);
    if(ptr1 == NULL){
        return NULL;
    }
    while(ptr2){
        if(!strcmp(ptr1->item->key2, ptr2->key))
            return ptr2;
        ptr2 = ptr2->next;
    }
  return NULL;
}

//ADD

int insert(Table* ptb, int key1, char* key2, char* info){
    if(ptb->csize1 == ptb->msize1){
        printf("Error! Table overflow!\n");
        return 2;
    }
    if(find_ks1(ptb, key1) != NULL){
        printf("Error! Key duplicate!\n");
        return 1;
    }
    //в первое пространство ключей
    int i = ptb->csize1 - 1;
        while (i >= 0 && ptb->ks1[i].key > key1) {
            ptb->ks1[i + 1] = ptb->ks1[i];
            i = i - 1;

    }
    i++;
    ptb->ks1[i].key = key1;
    ptb->ks1[i].item = (Item*)malloc(sizeof(Item));
    ptb->ks1[i].item->info = (char*)calloc(strlen(info)+1, sizeof(char));
    memcpy(ptb->ks1[i].item->info, info, strlen(info)+1);
    ptb->ks1[i].item->key1 = key1;
    ptb->ks1[i].item->key2 = (char*)calloc(strlen(key2)+1, sizeof(char));
    memcpy(ptb->ks1[i].item->key2, key2, strlen(key2)+1);
    ptb->csize1++;
    //во второе
    int r = find_ks2_mrelease(ptb, key2)+1;//релиз нового элемента
    int hash = hash_function(key2, ptb->msize2);//место в массиве для ks2
    if(ptb->ks2[hash] == NULL){//если ячейка пустая
        ptb->ks2[hash] = (KeySpace2*)malloc(1*sizeof(KeySpace2));
        ptb->ks2[hash]->key = (char*)malloc(strlen(key2)+1);
        memcpy(ptb->ks2[hash]->key, key2, strlen(key2)+1);
        ptb->ks2[hash]->item = ptb->ks1[i].item;
        ptb->ks2[hash]->release = 0;
        ptb->ks2[hash]->next = NULL;
        ptb->csize2++;
    }
    else{//если ячейка занята
    KeySpace2* ptr = ptb->ks2[hash];
    KeySpace2* tptr = (KeySpace2*)malloc(sizeof(KeySpace2));
    tptr->key = (char*)malloc(strlen(key2)+1);
    memcpy(tptr->key, key2, strlen(key2)+1);
    tptr->item = ptb->ks1[i].item;
    tptr->release = r;
    ptb->ks2[hash] = tptr;
    tptr->next = ptr;
    }
    return 0;
}

//DELETE

int delete_tb(Table* ptb){
  for (int i = 0; i < ptb->csize1; i++){//удаляем элементы
    free(ptb->ks1[i].item->info);//удаляем строку
    free(ptb->ks1[i].item->key2);//удаляем строку
    free(ptb->ks1[i].item);
  }

  for (int i = 0; i < ptb->msize2; i++){
      KeySpace2* ptr = ptb->ks2[i];
      while(ptr != NULL) {
          KeySpace2* tptr = ptr;
          ptr = ptr->next;
          free(tptr->key);// удаляем значения ключей
          free(tptr);
      }

  }
  free(ptb->ks1);//удаляем 1 пр-во
  free(ptb->ks2);//удаляем 2 пр-во
  free(ptb);//удаляем саму таблицу
  return 0;
}


int delete_ks1(Table* ptb, int key1){
    KeySpace1* ptr1 = find_ks1(ptb, key1);
    if(ptr1 == NULL)
        return 3;
    KeySpace2 *ptr2;
    char* key2 = (char*)calloc(strlen(ptr1->item->key2)+1, sizeof(char));
    memcpy(key2, ptr1->item->key2, strlen(ptr1->item->key2)+1);
    ptr2 = find_ks1ks2(ptb, key1, key2);
    //удаление информации из айтема
    free(ptr1->item->info);
    free(ptr1->item->key2);
    free(ptr1->item);

    //удаления ключа из 1 пространства
    int k = 0;
    for(int i = 0; i < ptb->csize1; ++i){
        if (ptb->ks1[i].key == key1){
            k = 1;
        }
        if(k == 1 && i < ptb->csize1 - 1){
            ptb->ks1[i] = ptb->ks1[i+1];
        }
    }
    ptb->csize1--;
    //удаление из 2 пространства
    KeySpace2 *ptr3;
    ptr3 = find_ks2(ptb, key2);//первый элемент с таким ключом
    if(ptb->ks2[hash_function(key2, ptb->msize2)] == ptr3){
        ptb->ks2[hash_function(key2, ptb->msize2)] = ptr3->next;
        free(ptr3->key);
        free(ptr3);
    }
    else{
        ptr3 = ptb->ks2[hash_function(key2, ptb->msize2)];
        while(ptr3 != NULL){
            if(ptr3->next == ptr2){
                ptr3->next = ptr2->next;
                free(ptr2->key);
                free(ptr2);
            }
            ptr3 = ptr3->next;
        }
    }
  free(key2);
  return 0;
}


int delete_ks1_range(Table* ptb, int min, int max){
  int k = 0;
  while(min != max+1){
      if(!delete_ks1(ptb, min))
          k++;

      min++;
  }
  if(k==0)
    return 3;//нет таких ключей
  else
    return 0;
}


int delete_ks2(Table *ptb, char* key2){ //удаляет все релизы
    int hash = hash_function(key2, ptb->msize2);
    KeySpace2* ptr2 = ptb->ks2[hash];
    //удаляем из начала списка
    while(ptb->ks2[hash] != NULL){
        if(!strcmp(key2, ptr2->key)) {
            int key1 = ptr2->item->key1;
            ptb->ks2[hash] = ptr2->next;
            free(ptr2->key);
            free(ptr2->item->info);
            free(ptr2->item->key2);
            free(ptr2->item);
            free(ptr2);
            ptr2 = ptb->ks2[hash];
            //из 1 пространства
            int k = 0;
            for (int i = 0; i < ptb->csize1; ++i) {
                if (ptb->ks1[i].key == key1) {
                    k = 1;
                }
                if (k == 1 && i < ptb->csize1 - 1) {
                    ptb->ks1[i] = ptb->ks1[i + 1];
                }
            }
            ptb->csize1--;

        }
        else{
            break;
        }
    }
    if(ptb->ks2[hash] != NULL) {
        KeySpace2* prev = ptb->ks2[hash];
        KeySpace2* next = ptb->ks2[hash]->next;
        while(next){
            if(!strcmp(key2, next->key)){
                prev->next = next->next;
                int key1 = next->item->key1;
                free(next->key);
                free(next->item->info);
                free(next->item->key2);
                free(next->item);
                free(next);
                next = prev->next;
                int k = 0;
                for (int i = 0; i < ptb->csize1; ++i) {
                    if (ptb->ks1[i].key == key1) {
                        k = 1;
                    }
                    if (k == 1 && i < ptb->csize1 - 1) {
                        ptb->ks1[i] = ptb->ks1[i + 1];
                    }
                }
                ptb->csize1--;

            }
            else {
                prev = prev->next;
                next = next->next;
            }
        }
    }
    if(ptb->ks2[hash] == NULL){
        ptb->csize2--;
    }
    return 0;
}


int delete_ks2_erase(Table* ptb){ //удаляет все до последнего в списке
    for(int i = 0; i < ptb->msize2; ++i) {
        char* key2;
        KeySpace2* ptr1 = ptb->ks2[i];
        KeySpace2* ptr2;
        while(ptr1) {
            ptr2 = ptr1;
            key2 = (char*)calloc(strlen(ptr2->key)+1, sizeof(char));
            memcpy(key2, ptr2->key, strlen(ptr2->key)+1);
                while (ptr2) {
                    if (ptr2->next) {
                        if (!strcmp(key2, ptr2->next->key)) {
                            KeySpace2 *tmp = ptr2->next;
                            ptr2->next = ptr2->next->next;
                            int key1 = tmp->item->key1;
                            free(tmp->item->info);
                            free(tmp->item->key2);
                            free(tmp->item);
                            free(tmp->key);
                            free(tmp);

                            int k = 0;
                            for (int i = 0; i < ptb->csize1; ++i) {
                                if (ptb->ks1[i].key == key1) {
                                    k = 1;
                                }
                                if (k == 1 && i < ptb->csize1 - 1) {
                                    ptb->ks1[i] = ptb->ks1[i + 1];
                                }
                            }
                            ptb->csize1--;

                        }
                    }
                    ptr2 = ptr2->next;
            }
            ptr1 = ptr1->next;
            free(key2);
        }
    }


    for(int i = 0; i < ptb->msize2; ++i){
        char* key2;
        KeySpace2* ptr1 = ptb->ks2[i];
        KeySpace2* ptr2;
        while(ptr1){
            ptr2 = ptr1;
            key2 = (char*)calloc(strlen(ptr2->key)+1, sizeof(char));
            memcpy(key2, ptr2->key, strlen(ptr2->key)+1);
        }
    }

    return 0;
}


int delete_ks2_r(Table* ptb, char* key2, int release){//удаление по конкретному релизу
    KeySpace2* ptr = find_ks2_release(ptb, key2, release);
    if(ptr == NULL){
        return 3;
    }
    int n = delete_ks1(ptb, ptr->item->key1);
    return n;
}


int delete_ks1ks2(Table* ptb, int key1, char* key2){
    KeySpace2* ptr = find_ks1ks2(ptb, key1, key2);
  if(ptr != NULL){
    int r = delete_ks1(ptb, ptr->item->key1);
    return r;
  }
  else
    return 3;
}


int hash_function(char *key2, int size){
    int hash = 0, i = 0;
    while(key2[i] != '\0'){
        hash = hash*37 + key2[i];
        i++;
    }
    return abs(hash) % size;
}