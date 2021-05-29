#include "lab.h"


const char *errmsgs[] = {"Ok", "Table overflow", "This key doesn't exist"};

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
        printf("Make your choice: ");
        n = getInt(&rc);
        if(n == 0)
            rc = 0;
    }while(rc < 0 || rc >= N);
    return rc;
}


int D_Add(Tree **trp){
    int rc;
    char* info = NULL;
    int point[2];
    int n;
    scanf("%*c");
    printf("Enter key1(x): ");
    n = getInt(point);
    if(n == 0)
        return 0;
    printf("Enter key1(y): ");
    n = getInt(&point[1]);
    if(n == 0)
        return 0;
    scanf("%*c");
    printf("Enter info: ");
    info = getStr();
    if (info == NULL)
        return 0;
    rc = insert(trp, point, info, 0);
    free(info);
    printf("%s\n", errmsgs[rc]);
    return 1;
}


int D_Find(Tree** trp){
    int point[2];
    int n, release;
    scanf("%*c");
    printf("Enter key1(x): ");
    n = getInt(point);
    if(n == 0)
        return 0;
    printf("Enter key1(y): ");
    n = getInt(&point[1]);
    if(n == 0)
        return 0;
    Item** ptr = NULL;
    int i = 0;
    find(trp, point, &ptr, 0, &i);
    if(ptr == NULL) {
        printf("This key doesn't exist\n");
        return 1;
    }
    for(int j = 0; j<i; j++){
        printf("(%d, %d)(%d): %s\n",ptr[j]->key[0], ptr[j]->key[1], ptr[j]->release, ptr[j]->info);
    }
    free(ptr);
    return 1;
}




int D_Find_nearest(Tree** trp){
    char* key = NULL;int n;
    int point[2];
    scanf("%*c");
    printf("Enter key1(x): ");
    n = getInt(point);
    if(n == 0)
        return 0;
    printf("Enter key1(y): ");
    n = getInt(&point[1]);
    if(n == 0)
        return 0;
    Item* head = NULL;
    find_nearest(trp, point, 0, &head, INT_MAX);
    if(head != NULL)
        printf("(%d, %d)(%d): %s\n",head->key[0], head->key[1], head->release, head->info);
    return 1;
}


int D_Delete(Tree** trp){
    int rc;
    int point[2];
    int n, release;
    scanf("%*c");
    printf("Enter key1(x): ");
    n = getInt(point);
    if(n == 0)
        return 0;
    printf("Enter key1(y): ");
    n = getInt(&point[1]);
    if(n == 0)
        return 0;
    scanf("%*c");
    printf("Enter release: ");
    n = getInt(&release);
    if(n == 0)
        return 1;
    rc = del_node(trp, *trp, point, release, 0);
    printf("%s\n", errmsgs[rc]);
    return 1;
}

void print_list(Item* head, int d){
    while(head){
        for (int i = 10; i < d; i++)
            printf(" ");
        printf("(%d, %d)(%d): %s\n",head->key[0], head->key[1], head->release, head->info);

        head = head->next;
    }
}



void show(Tree* trp, int d){
    if(trp) {
        d += 10;
        show(trp->right, d);
        printf("%d", trp->limit);
        printf("\n");
        print_list(trp->head, d);
        show(trp->left, d);
    }
}

int D_Show(Tree** trp){
    show(*trp, 0);
    return 1;

}


void show_in_range(Tree* trp, int x, int y){//right/root/left
    if(trp){
        if(trp->right)
            show_in_range(trp->right, x, y);
        Item* head = trp->head;
        while(head){
            /*if(x == INT_MAX && y == INT_MAX)
                printf("(%d, %d)(%d): %s\n", head->key[0], head->key[1], head->release, head->info);
            if(x == INT_MAX && head->key[1] <= y)
                printf("(%d, %d)(%d): %s\n", head->key[0], head->key[1], head->release, head->info);
            if(y == INT_MAX && head->key[0] <= x)
                printf("(%d, %d)(%d): %s\n", head->key[0], head->key[1], head->release, head->info);
           */ if(head->key[1] <= y && head->key[0] <= x)
                printf("(%d, %d)(%d): %s\n", head->key[0], head->key[1], head->release, head->info);
            head = head->next;
        }
        if(trp->left)
            show_in_range(trp->left, x, y);

    }
}


int D_Show_in_range(Tree** trp){
    int n, x, y;
    scanf("%*c");
    printf("Enter key1(x) (if empty enter '-'): ");
    n = scanf("%d",&x);
    if(n == 0)
        x = INT_MAX;
    printf("Enter key1(y) (if empty enter '-'): ");
    n = scanf("%d", &y);
    if(n == 0)
        y = INT_MAX;

    scanf("%*c");
    show_in_range((*trp), x, y);
    return 1;
}


int D_Timing(Tree ** tmp){
    Tree* trp = NULL;
    int n = 10, cnt = 100000, i, m, j;
    int keys[10000][2];
    int k[2];
    char* info;
    clock_t first, last;
    srand(time(NULL));
    while (n-- > 0){
        for (i = 0; i < 10000; ++i) {
            for (j = 0; j < 2; j++) {
                keys[i][j] = rand() % 1000;
            }
        }
        first = clock();
        for (i = 0; i < cnt; ++i) {
            for (j = 0; j < 2; j++)
                k[j] = rand() % 1000;
            info = (char *) calloc(1, 6);
            for (j = 0; j < 6 - 1; j++)
                info[j] = '0' + rand() % 8;
            info[j] = '\0';
            insert(&trp, k, info, 0);
            free(info);
        }
        last = clock();
        printf("\ntest #%d, number of inserted nodes = %d, time = %ld\n\n", 10 - n, (10 - n) * cnt, last - first);
        m = 0;
        first = clock();
        for (i = 0; i < 10000; ++i) {
            Item **ptr = NULL;
            int k = 0;
            if (find(&trp, keys[i], &ptr, 0, &k));
            m = m + k;
        }
        last = clock();
        printf("%d items was found\n", m);
        printf("test #%d, number of nodes = %d, time = %ld\n", 10 - n, (10 - n) * cnt, last - first);
    }
    delTree(&trp);
    return 1;
}
