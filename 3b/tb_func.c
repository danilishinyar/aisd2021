#include"lab.h"

Table* load(){
    Table* ptb = (Table*) calloc(1, sizeof(Table));
    ptb->fname = "./Table.txt";
    ptb->ks2_fname = "./Ks2.txt";
    ptb->ks1_fname = "./Ks1.txt";
    ptb->item_fname = "./Item.txt";
    ptb->item_fd = fopen(ptb->item_fname, "a+b");
    ptb->fd = fopen(ptb->fname, "r+b");
    if (ptb->fd == NULL) {
        puts("Error");
        return NULL;
    }
    //ks1
    fread(&ptb->msize1, sizeof(int), 1, ptb->fd);//msize1
    fread(&ptb->csize1, sizeof(int), 1, ptb->fd);//csize1
    fread(&ptb->msize2, sizeof(int), 1, ptb->fd);//msize2
    fread(&ptb->csize2, sizeof(int), 1, ptb->fd);//csize2
    ptb->ks1_fd = fopen(ptb->ks1_fname, "r+b");
    ptb->ks1 = (KeySpace1*)calloc(ptb->msize1, sizeof(KeySpace1));
    fread(ptb->ks1, sizeof(KeySpace1), ptb->csize1, ptb->ks1_fd);
    //ks2

    ptb->ks2_fd= fopen(ptb->ks2_fname, "r+b");
    ptb->ks2 = (KeySpace2 **) calloc(ptb->msize2, sizeof(KeySpace2 *));
    KeySpace2* cur2;
    KeySpace2* last;
    for(int i = 0; i < ptb->msize2; ++i) {
        int offset_ks2;
        fread(&offset_ks2, sizeof(int), 1, ptb->fd);
        if (offset_ks2 != -1) {
            last = (KeySpace2 *) calloc(1, sizeof(KeySpace2));
            int key2_len;
            fread(&key2_len, sizeof(int), 1, ptb->ks2_fd);//key2 len
            fread(&last->offset_item, sizeof(int), 1, ptb->ks2_fd);//item offset
            int offset_next_ks2;
            fread(&last->release, sizeof(int), 1, ptb->ks2_fd);//release
            fread(&offset_next_ks2, sizeof(int), 1, ptb->ks2_fd);//next ks2 offset
            last->key = (char*)calloc(key2_len, 1);
            fread(last->key, sizeof(char), key2_len, ptb->ks2_fd);//key2
            ptb->ks2[i] = last;
            while (offset_next_ks2 != -1) {
                fseek(ptb->ks2_fd, offset_next_ks2, SEEK_SET);
                cur2 = (KeySpace2 *) calloc(1, sizeof(KeySpace2));
                fread(&key2_len, sizeof(int), 1, ptb->ks2_fd);// key2 len
                fread(&cur2->offset_item, sizeof(int), 1, ptb->ks2_fd);//item offset next
                fread(&cur2->release, sizeof(int), 1, ptb->ks2_fd);//release
                fread(&offset_next_ks2, sizeof(int), 1, ptb->ks2_fd);//next ks2 offset
                cur2->key = (char*)calloc(key2_len, 1);
                fread(cur2->key, sizeof(char), key2_len, ptb->ks2_fd);//key2
                last->next = cur2;
                last = cur2;
            }
            last->next = NULL;
        } else
            ptb->ks2[i] = NULL;
    }
    fclose(ptb->ks2_fd);
    fclose(ptb->ks1_fd);
    fclose(ptb->fd);
    return ptb;
}


int save(Table* ptb){
    del_items(ptb);
    ptb->ks2_fd = fopen(ptb->ks2_fname, "w+b");
    ptb->ks1_fd = fopen(ptb->ks1_fname, "w+b");
    ptb->fd = fopen(ptb->fname, "w+b");
    fwrite(&ptb->msize1, sizeof(int), 1, ptb->fd);//msize1
    fwrite(&ptb->csize1, sizeof(int), 1, ptb->fd);//csize1
    fwrite(&ptb->msize2, sizeof(int), 1, ptb->fd);//msize2
    fwrite(&ptb->csize2, sizeof(int), 1, ptb->fd);//csize2
    //save ks1;
    fwrite(ptb->ks1, sizeof(KeySpace1), ptb->csize1, ptb->ks1_fd);
    //save ks2;
    long int offset_ks2;
    long int offset_next;
    KeySpace2* last;
    for(int i = 0; i < ptb->msize2; ++i){
        last = ptb->ks2[i];
        if(ptb->ks2[i] == NULL){
            offset_ks2 = -1;
            fwrite(&offset_ks2, sizeof(int), 1, ptb->fd);
        }
        else {
            offset_ks2 = ftell(ptb->ks2_fd);
            fwrite(&offset_ks2, sizeof(int), 1, ptb->fd);
            do {
                unsigned int key2_len = strlen(last->key) + 1;
                fwrite(&key2_len, sizeof(int), 1, ptb->ks2_fd);//key2 len
                fwrite(&last->offset_item, sizeof(int), 1, ptb->ks2_fd);//item_offset
                fwrite(&last->release, sizeof(int), 1, ptb->ks2_fd);//release
                if (last->next != NULL)
                    offset_next = ftell(ptb->ks2_fd) + key2_len + sizeof(int);
                else
                    offset_next = -1;
                fwrite(&offset_next, sizeof(int), 1, ptb->ks2_fd);//next offset
                fwrite(last->key, 1, key2_len, ptb->ks2_fd);
                last = last->next;
            }while(last != NULL);
        }
    }
    fclose(ptb->ks2_fd);
    fclose(ptb->ks1_fd);
    fclose(ptb->fd);
    return 0;
}


int del_items(Table* ptb){
    FILE* items_buf = fopen("./Item_buf.txt", "w+b");
    //key1/key2_len/info_len/key2/info
    int key1, key2_len = 0, info_len = 0;
    char* key2;
    char* info;
    for(int i = 0; i < ptb->csize1; ++i){
        fseek(ptb->item_fd, ptb->ks1[i].offset_item, SEEK_SET);
        fread(&key1, sizeof(int), 1, ptb->item_fd);
        fread(&key2_len, sizeof(int), 1, ptb->item_fd);
        fread(&info_len, sizeof(int), 1, ptb->item_fd);
        key2 = (char*)calloc(key2_len, 1);
        info = (char*)calloc(info_len, 1);
        fread(key2, sizeof(char), key2_len, ptb->item_fd);
        fread(info, sizeof(char), info_len, ptb->item_fd);
        KeySpace2* ptr2 = find_ks2_by_ks1(ptb, ptb->ks1[i].key);
        ptb->ks1[i].offset_item = ftell(items_buf);
        ptr2->offset_item = ftell(items_buf);
        fwrite(&key1, sizeof(int), 1, items_buf);
        fwrite(&key2_len, sizeof(int), 1, items_buf);
        fwrite(&info_len, sizeof(int), 1, items_buf);
        fwrite(key2, sizeof(char), key2_len, items_buf);
        fwrite(info, sizeof(char), info_len, items_buf);
        free(key2);
        free(info);
    }
    fclose(ptb->item_fd);
    fclose(items_buf);
    remove(ptb->item_fname);
    ptb->item_fd = items_buf;
    rename("./Item_buf.txt", "./Item.txt");
    return 0;
}



KeySpace2* find_ks2_by_ks1(Table* ptb, int key1){
    KeySpace1* ptr1 = find_ks1(ptb, key1);
    int key2_len, key1_tmp, info_len;
    //key1/key2_len/info_len/key2/info
    fseek(ptb->item_fd, ptr1->offset_item, SEEK_SET);
    fread(&key1_tmp, sizeof(int), 1, ptb->item_fd);
    fread(&key2_len, sizeof(int), 1, ptb->item_fd);
    fread(&info_len, sizeof(int), 1, ptb->item_fd);
    char* key2 = (char*)calloc(key2_len, 1);
    fread(key2, sizeof(char), key2_len, ptb->item_fd);
    KeySpace2* ptr2 = find_ks1ks2(ptb, key1, key2);
    free(key2);
    return ptr2;
}


Table* initialize(int msize1, int msize2){
  if(msize1 <= 0 || msize2<= 0){
    printf("\nWrong table size!\n");
    return NULL;
  }
  Table* ptb = (Table*) calloc(1, sizeof(Table));
  ptb->fname = "./Table.txt";
  ptb->fd = fopen(ptb->fname, "w+b");
  ptb->ks2_fname = "./Ks2.txt";
  ptb->ks2_fd = fopen(ptb->ks2_fname, "w+b");
  ptb->ks1_fname = "./Ks1.txt";
  ptb->ks1_fd = fopen(ptb->ks1_fname, "w+b");
  ptb->item_fname = "./Item.txt";
  ptb->item_fd = fopen(ptb->item_fname, "w+b");
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
  int l = 0;
  int r = ptb->csize1 - 1;
  while(l <= r) {
      int mid = (l + r) / 2;
      if (ptb->ks1[mid].key == key)
          return ptb->ks1 + mid;
      if (key < ptb->ks1[mid].key)
          r = mid - 1;
      else
          l = mid + 1;
  }
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
    int item = find_ks1(ptb, key1)->offset_item;
    KeySpace2* ptr = find_ks2(ptb, key2);
    while(ptr){
        if(ptr->offset_item == item) {
            r = ptr->release;
            return r;
        }
        ptr = ptr->next;
    }
    return r;
}


KeySpace2* find_ks1ks2(Table* ptb, int key1, char* key2){
    KeySpace2* ptr2 = find_ks2(ptb, key2);
    int key2_len;
    char* key2_tmp;
    if(ptr2 == NULL){
        return NULL;
    }
    KeySpace1* ptr1 = find_ks1(ptb, key1);
    if(ptr1 == NULL){
        return NULL;
    }
    while(ptr2){
        fseek(ptb->item_fd, ptr2->offset_item, SEEK_SET);
        int key1_tmp;
        fread(&key1_tmp, sizeof(int), 1, ptb->item_fd);
        fread(&key2_len, sizeof(int), 1, ptb->item_fd);
        int info_len;
        fread(&info_len, sizeof(int), 1, ptb->item_fd);
        key2_tmp = (char*)calloc(key2_len, 1);
        fread(key2_tmp, sizeof(char), key2_len, ptb->item_fd);
        if(!strcmp(key2_tmp, ptr2->key)) {
            free(key2_tmp);
            return ptr2;
        }
        ptr2 = ptr2->next;

    }
    fseek(ptb->item_fd, 0, SEEK_END);
    free(key2_tmp);
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
    int key2_len = strlen(key2) + 1;
    int info_len = strlen(info) + 1;
    //в первое пространство ключей
    int i = ptb->csize1 - 1;
        while (i >= 0 && ptb->ks1[i].key > key1) {
            ptb->ks1[i + 1] = ptb->ks1[i];
            i = i - 1;

    }
    i++;
    ptb->ks1[i].key = key1;
    fseek(ptb->item_fd, 0, SEEK_END);
    int k = ftell(ptb->item_fd);
    ptb->ks1[i].offset_item = k;
    fwrite(&key1, sizeof(int), 1, ptb->item_fd);
    fwrite(&key2_len, sizeof(int), 1, ptb->item_fd);
    fwrite(&info_len, sizeof(int), 1, ptb->item_fd);
    fwrite(key2, sizeof(char), key2_len, ptb->item_fd);
    fwrite(info, sizeof(char), info_len, ptb->item_fd);
    //key1/key2_len/info_len/key2/info
    ptb->csize1++;
    //во второе
    int r = find_ks2_mrelease(ptb, key2)+1;//релиз нового элемента
    int hash = hash_function(key2, ptb->msize2);//место в массиве для ks2
    if(ptb->ks2[hash] == NULL){//если ячейка пустая
        ptb->ks2[hash] = (KeySpace2*)malloc(1*sizeof(KeySpace2));
        ptb->ks2[hash]->key = (char*)malloc(strlen(key2)+1);
        memcpy(ptb->ks2[hash]->key, key2, strlen(key2)+1);
        ptb->ks2[hash]->offset_item = ptb->ks1[i].offset_item;
        ptb->ks2[hash]->release = 0;
        ptb->ks2[hash]->next = NULL;
        ptb->csize2++;
    }
    else{//если ячейка занята
    KeySpace2* ptr = ptb->ks2[hash];
    KeySpace2* tptr = (KeySpace2*)malloc(sizeof(KeySpace2));
    tptr->key = (char*)malloc(strlen(key2)+1);
    memcpy(tptr->key, key2, strlen(key2)+1);
    tptr->offset_item = ptb->ks1[i].offset_item;
    tptr->release = r;
    ptb->ks2[hash] = tptr;
    tptr->next = ptr;
    }
    return 0;
}

//DELETE

int delete_tb(Table* ptb){


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
    //key1/key2_len/info_len/key2/info
    int key1_tmp = 0;
    int info_len = 0;
    fseek(ptb->item_fd, ptr1->offset_item, SEEK_SET);
    fread(&key1_tmp, sizeof(int), 1, ptb->item_fd);
    int key2_len = 0;
    fread(&key2_len, sizeof(int), 1, ptb->item_fd);
    fread(&info_len, sizeof(int), 1, ptb->item_fd);
    char* key2 = NULL;
    key2 = (char*)calloc(key2_len, sizeof(char));
    fread(key2, sizeof(char), key2_len, ptb->item_fd);
    ptr2 = find_ks1ks2(ptb, key1, key2);
    //удаление информации из айтема


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
    fseek(ptb->item_fd, 0, SEEK_END);
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
            int key1 = 0;
            fseek(ptb->item_fd, ptr2->offset_item, SEEK_SET);
            fread(&key1, sizeof(int), 1, ptb->item_fd);
            ptb->ks2[hash] = ptr2->next;
            free(ptr2->key);

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
                int key1 = 0;
                fseek(ptb->item_fd, next->offset_item, SEEK_SET);
                fread(&key1, sizeof(int), 1, ptb->item_fd);
                free(next->key);

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
    fseek(ptb->item_fd, 0, SEEK_END);
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
            KeySpace2* prev = ptr1;
            KeySpace2* next = ptr1->next;
                while(next){
                    if(!strcmp(key2, next->key)){
                        prev->next = next->next;
                        int key1 = 0;
                        fseek(ptb->item_fd, next->offset_item, SEEK_SET);
                        fread(&key1, sizeof(int), 1, ptb->item_fd);
                        free(next->key);

                        free(next);
                        next = prev->next;
                        int k = 0;
                        for (int j = 0; j < ptb->csize1; ++j) {
                            if (ptb->ks1[j].key == key1) {
                                k = 1;
                            }
                            if (k == 1 && j < ptb->csize1 - 1) {
                                ptb->ks1[j] = ptb->ks1[j + 1];
                            }
                        }
                        ptb->csize1--;

                    }
                    else {
                        prev = prev->next;
                        next = next->next;
                    }
                }
            ptr1 = ptr1->next;
            free(key2);
        }
    }



    fseek(ptb->item_fd, 0, SEEK_END);
    return 0;
}


int delete_ks2_r(Table* ptb, char* key2, int release){//удаление по конкретному релизу
    KeySpace2* ptr = find_ks2_release(ptb, key2, release);
    if(ptr == NULL){
        return 3;
    }
    int key1 = 0;
    fseek(ptb->item_fd, ptr->offset_item, SEEK_SET);
    fread(&key1, sizeof(int), 1, ptb->item_fd);
    int n = delete_ks1(ptb, key1);
    fseek(ptb->item_fd, 0, SEEK_END);
    return n;
}


int delete_ks1ks2(Table* ptb, int key1, char* key2){
    KeySpace2* ptr = find_ks1ks2(ptb, key1, key2);
  if(ptr != NULL){
    int r = delete_ks1(ptb, key1);
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