#include "lab.h"


const char *errmsgs[] = {"Ok", "Duplicate key", "Table overflow", "This key doesn't exist"};

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


int D_Add(Tree **trp){
    int rc;
    char* info = NULL;
    char* key = NULL;
    scanf("%*c");
    printf("Enter key: ");
    key = getStr();
    if(key == NULL)
        return 0;
    printf("Enter info: ");
    info = getStr();
    if (info == NULL)
        return 0;
    rc = insert(trp, key, info);
    free(info);
    free(key);
    printf("%s\n", errmsgs[rc]);
    return 1;
}


int D_Find(Tree** trp){
    char* key = NULL;
    int n, release;
    scanf("%*c");
    printf("Enter key of the element you want to find -->");
    key = getStr();
    if(key == NULL)
        return 1;
    printf("Enter release -->");
    n = getInt(&release);
    if(n == 0)
        return 1;
    Tree* ptr = NULL;
    ptr = find(trp, key, release);
    if(ptr == NULL) {
        printf("This key doesn't exist");
        free(key);
        return 1;
    }
    printf("%s: %s(%d)\n", key, ptr->info->info, ptr->info->release);
    free(key);
    return 1;
}


int D_Find_nearest(Tree** trp){
    char* key = NULL;
    int n, release;
    scanf("%*c");
    printf("Enter key of the element you want to find -->");
    key = getStr();
    if(key == NULL)
        return 1;
    Tree* ptr = NULL;
    find_nearest(trp, key, 2147483, &ptr);
    if(ptr == NULL) {
        printf("This key doesn't exist");
        return 1;
    }
    printf("%s: %s(%d)\n", ptr->key, ptr->info->info, ptr->info->release);
    free(key);
    return 1;
}


int D_Delete(Tree** trp){
    int rc, n, release;
    char* key;
    scanf("%*c");
    printf("Enter key of the element you want to delete -->");
    key = getStr();
    if(key == 0)
        return 0;
    printf("Enter release of the element you want to delete -->");
    n = getInt(&release);
    if(n == 0)
        return 0;
    del_element(trp, *trp, key, release);
    //printf("%s\n", errmsgs[rc]);
    free(key);
    return 1;
}

void show(Tree* trp, int d){
    if(trp) {
        d += 10;
        show(trp->right, d);
        printf("\n");
        for (int i = 10; i < d; i++)
            printf(" ");
        printf("%s: %s(%d)\n", trp->key, trp->info->info, trp->info->release);
        show(trp->left, d);
    }
}

int D_Show(Tree** trp){
    show(*trp, 0);
    return 1;

}


void show_in_range(Tree* trp, char* min, char* max){//right/root/left
    if(trp){
        if(trp->right)
            show_in_range(trp->right, min, max);
        if(strcmp(trp->key, min) >= 0 && strcmp(trp->key, max)<=0)
            printf("%s: %s(%d)\n", trp->key, trp->info->info, trp->info->release);
        if(trp->left)
            show_in_range(trp->left, min, max);

    }
}


int D_Show_in_range(Tree** trp){
    int rc, n, release;
    char* key_max;
    char*key_min;
    scanf("%*c");
    printf("Enter key min -->");
    key_min = getStr();
    if(key_min == 0)
        return 0;
    printf("Enter key max-->");
    key_max = getStr();
    if(key_max == 0)
        return 0;
    show_in_range((*trp), key_min, key_max);
    free(key_min);
    free(key_max);
    return 1;
}



int D_Timing(Tree ** tmp){
    Tree* trp = NULL;
    int n = 10, cnt = 100000, i, m, j;
    char key[10000][6];
    char* k;
    char* info;
    clock_t first, last;
    srand(time(NULL));
    while (n-- > 0){
        for (i = 0; i < 10000; ++i) {
            for (j = 0; j < 6 - 1; j++) {
                key[i][j] = '0' + rand() % 8;
            }
            key[i][j] = '\0';
        }
        first = clock();
        for (i = 0; i < cnt; ++i) {
            k = (char *) calloc(1, 6);
            for (j = 0; j < 6 - 1; j++)
                k[j] = '0' + rand() % 8;
            k[j] = '\0';
            info = (char *) calloc(1, 6);
            for (j = 0; j < 6 - 1; j++)
                info[j] = '0' + rand() % 8;
            info[j] = '\0';
            insert(&trp, k, info);
            free(k);
            free(info);
        }
        last = clock();
        printf("\ntest #%d, number of inserted nodes = %d, time = %ld\n\n", 10 - n, (10 - n) * cnt, last - first);
        m = 0;
        first = clock();
        for (i = 0; i < 10000; ++i)
            if (find(&trp, key[i], 0))
                ++m;
        last = clock();
        printf("%d items was found\n", m);
        printf("test #%d, number of nodes = %d, time = %ld\n", 10 - n, (10 - n) * cnt, last - first);
    }
    delTree(&trp);
    return 1;
}