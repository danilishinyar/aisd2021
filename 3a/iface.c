#include"lab.h"


const char *msgs_del[] = {"0.Quit", "1.Delete from ks1", "2.Delete from ks2", "3.Delete by release from ks2", "4.Erase ks2(delete all releases without last)", "5.Delete with two keys", "6.Delete in range from ks1"};
const int NMsgs_del = sizeof(msgs_del) / sizeof(msgs_del[0]);
const char *msgs_find[] = {"0.Quit", "1.Find from ks1", "2.Find from ks2", "3.Find by release", "4.Find with two keys"};
const int NMsgs_find = sizeof(msgs_find) / sizeof(msgs_find[0]);
const char *errmsgs[] = {"Ok", "Duplicate key", "Table overflow", "This key doesn't exist"};


int (*fptr_find[])(Table* ptb) = {NULL, D_Find_ks1, D_Find_ks2, D_Find_ks2_release, D_Find_ks1ks2};
int (*fptr_del[])(Table* ptb) = {NULL, D_Del_ks1, D_Del_ks2, D_Del_Release, D_Del_Erase, D_Del_ks1ks2, D_Del_Range};


char* getStr(){
    char *p = (char*)malloc(1);
    *p = '\0';
    char buf[81];
    int n, len = 0;
    do{
        n = scanf("%80[^\n]", buf);
        if(n<0){
            free(p);
            p = NULL;
            continue;
            }
        if(n==0)
            scanf("%*c");
        else{
            len += strlen(buf);
            p = (char*)realloc(p, len+1);
            strcat(p, buf);
        }
    }while(n>0);
    return p;
}


int getInt(int* a){
    int n;
    do {
        n = scanf("%d", a);
        if (n == 0) {
            puts("ERROR");
            scanf("%*c");
        }
    } while (n == 0);
    return n < 0 ? 0 : 1;
}


int dialog(const char *msgs[], int N){
  char *errmsg = "";
  int rc;
  int i, n;
  do{
    puts(errmsg);
    errmsg = "You are wrong. Repeat, please!";
    for(i = 0; i < N; ++i)
    puts(msgs[i]);
    printf("Make your choice: --> ");
    n = getInt(&rc);
    if(n == 0)
    rc = 0;
  }while(rc < 0 || rc >= N);
  return rc;
}


int D_Add(Table *ptb){
  int key1, rc, n;
  char* info = NULL;
  char* key2 = NULL;
  printf("Enter key1: ");
  n = getInt(&key1);
  if(n == 0)
    return 0;
  scanf("%*c");
  printf("Enter key2: ");
  key2 = getStr();
  if(key2 == NULL)
    return 0;
  printf("Enter info: ");
  info = getStr();
  if (info == NULL)
    return 0;
  rc = insert(ptb, key1, key2, info);
  free(info);
  free(key2);
  printf("%s\n", errmsgs[rc]);
  return 1;
}

//FIND

int Find_dialog(const char *msgs_find[], int N){
  char *errmsg = "";
  int rc;
  int i, n;
  do{
    puts(errmsg);
    errmsg = "You are wrong. Repeat, please!";
    for(i = 0; i < N; ++i)
    puts(msgs_find[i]);
    printf("Make your choice: --> ");
    n = getInt(&rc);
    if(n == 0)
    rc = 0;
  }while(rc < 0 || rc >= N);
  return rc;
}


int D_Find(Table* ptb){
  int rc;
  while(rc = Find_dialog(msgs_find, NMsgs_find))
    if(!fptr_find[rc](ptb))
     break;
  return 1;
}


int D_Find_ks1(Table* ptb){
  int key1, n;
  printf("Enter key of the element you want to find -->");
  n = getInt(&key1);
  if(n == 0)
    return 0;
  KeySpace1* ptr = find_ks1(ptb, key1);
  if(ptr == NULL) {
      printf("This key doesn't exist");
      return 0;
  }
  printf("(%d, %s): %s\n", key1, ptr->item->key2, ptr->item->info);
  return 1;
}


int D_Find_ks2(Table* ptb){
  char* key2;
  scanf("%*c");
  printf("Enter key of the element you want to find -->");
  key2 = getStr();
  if(key2 == 0)
    return 0;
  KeySpace2* ptr = find_ks2(ptb, key2);
  if(ptr == NULL){
    printf("This key doesn't exist");
    free(key2);
    return 0;
  }
  while(ptr){
    if(!strcmp(key2, ptr->key))
        printf("%s(%d): %s\n", key2, ptr->release, ptr->item->info);
    ptr = ptr->next;
  }
  free(key2);
  return 1;
}


int D_Find_ks2_release(Table* ptb){
  char* key2;
  int release, n;
  scanf("%*c");
  printf("Enter key of the element you want to find -->");
  key2 = getStr();
  if(key2 == 0)
    return 0;
  printf("Enter release -->");
  n = getInt(&release);
  if(n == 0)
    return 0;
  KeySpace2 * ptr = find_ks2_release(ptb, key2, release);
  if(ptr == NULL){
    printf("This key doesn't exist");
    free(key2);
    return 0;
    }
  else{
    printf("(%d, %s(%d)): %s", ptr->item->key1, ptr->key, release, ptr->item->info);
    free(key2);
    return 1;
    }
}


int D_Find_ks1ks2(Table* ptb){
  int key1, n;
  char* key2;
  printf("Enter key1 of the element you want to find -->");
  n = getInt(&key1);
  if(n == 0)
    return 0;
  scanf("%*c");
  printf("Enter key2 of the element you want to find -->");
  key2 = getStr();
  if(key2 == 0)
      return 0;
  KeySpace2* ptr = find_ks1ks2(ptb, key1, key2);
  if(ptr == NULL){
      printf("This key doesn't exist");
      free(key2);
      return 0;
  }
  else{
    Item* item = ptr->item;
    printf("(%d, %s): %s", key1, key2, item->info);
    free(key2);
        return 1;
  }
}

//DELETE

int Del_dialog(const char *msgs_del[], int N){
  char *errmsg = "";
  int rc;
  int i, n;
  do{
    puts(errmsg);
    errmsg = "You are wrong. Repeat, please!";
    for(i = 0; i < N; ++i)
    puts(msgs_del[i]);
    printf("Make your choice: --> ");
    n = getInt(&rc);
    if(n == 0)
    rc = 0;
  }while(rc < 0 || rc >= N);
  return rc;
}


int D_Del(Table *ptb){
  int rc;
  while(rc = Del_dialog(msgs_del, NMsgs_del))
    if(!fptr_del[rc](ptb))
     break;
  return 1;
}


void D_Del_Table(Table* ptb){
  int rc;
  rc = delete_tb(ptb);
  printf("%s\n", errmsgs[rc]);
}


int D_Del_Range(Table* ptb){
  int min, max, rc, n1, n2;
  printf("Enter range of keys you want to delete -->");
  n1 = getInt(&min);
  n2 = getInt(&max);
  if(n1 == 0 || n2 == 0)
    return 0;
  rc = delete_ks1_range(ptb, min, max);
  printf("%s\n", errmsgs[rc]);
  return 1;
}


int D_Del_Erase(Table *ptb){
  int rc;
  char* key2;
  rc = delete_ks2_erase(ptb);
  printf("%s\n", errmsgs[rc]);
  return 1;
}


int D_Del_ks1(Table* ptb){
  int n, rc, key1;
  printf("Enter key1 of the element you want to delete -->");
  n = getInt(&key1);
  if(n == 0)
    return 0;
  rc = delete_ks1(ptb, key1);
  printf("%s\n", errmsgs[rc]);
  return 1;
}


int D_Del_ks2(Table* ptb){
  int rc;
  char* key2;
    scanf("%*c");
  printf("Enter key2 of the element you want to delete -->");
  key2 = getStr();
  if(key2 == 0)
    return 0;
  rc = delete_ks2(ptb, key2);
  printf("%s\n", errmsgs[rc]);
  free(key2);
  return 1;
}


int D_Del_ks1ks2(Table* ptb){
  int n, rc, key1;
  char* key2;

  printf("Enter key1 of the element you want to delete -->");
  n = getInt(&key1);
  if(n == 0)
    return 0;
    scanf("%*c");
  printf("Enter key2 of the element you want to delete -->");
  key2 = getStr();
  if(key2 == 0)
    return 0;
  rc = delete_ks1ks2(ptb, key1, key2);
  printf("%s\n", errmsgs[rc]);
  free(key2);
  return 1;
}


int D_Del_Release(Table* ptb){
  int n, rc, release;
  char* key2;
  scanf("%*c");
  printf("Enter key2 of the element you want to delete -->");
  key2 = getStr();
  if(key2 == 0)
    return 0;
  printf("Enter release of the element you want to delete -->");
  n = getInt(&release);
  if(n == 0)
    return 0;
  rc = delete_ks2_r(ptb, key2, release);
  printf("%s\n", errmsgs[rc]);
  free(key2);
  return 1;
}

//SHOW

int D_Show(Table* ptb){
  for(int i = 0; i < ptb->csize1; ++i){
    int key1 = ptb->ks1[i].key;
    char* key2 =  ptb->ks1[i].item->key2;
    char* info = ptb->ks1[i].item->info;
    int release = find_release(ptb, key1, key2);
    printf( "(%d, %s(%d)): %s\n", key1, key2, release, info);
  }
  return 1;
}
