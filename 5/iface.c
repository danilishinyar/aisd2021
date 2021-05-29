#include "lab.h"


const char *errmsgs[] = {"Ok", "Graph overflow", "Edge duplicate", "Vertex duplicate", "This edge doesn't exist", "This vertex doesn't exist", "Same vertex"};

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


int D_Add_vertex(Graph **ptr){
    int rc;
    char* name = NULL;
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
    printf("Enter name: ");
    name = getStr();
    if (name == NULL)
        return 0;
    rc = insert_vertex(ptr, point, name);
    free(name);
    printf("%s\n", errmsgs[rc]);
    return 1;
}



int D_Add_edge(Graph **ptr){
    int rc;
    char* name1 = NULL;
    char* name2 = NULL;
    scanf("%*c");
    printf("Enter name(from): ");
    name1 = getStr();
    if (name1 == NULL)
        return 0;
    printf("Enter name(to): ");
    name2 = getStr();
    if (name2 == NULL)
        return 0;
    rc = insert_edge(ptr, name1, name2);
    printf("%s\n", errmsgs[rc]);
    free(name1);
    free(name2);
    return 1;
}







int D_Find_vertex_bfs(Graph** trp){
    char* name1 = NULL;
    char* name2 = NULL;
    scanf("%*c");
    printf("Enter name(from): ");
    name1 = getStr();
    if (name1 == NULL)
        return 0;
    printf("Enter name(to): ");
    name2 = getStr();
    if (name2 == NULL)
        return 0;
    Graph* ptr = NULL;
    ptr = bfs_by_name(trp, name1, name2);
    if(ptr != NULL)
        printf("There is way from %s to %s: (%d, %d)\n", name1, ptr->info->name, ptr->x, ptr->y);
    else
        printf("There is no way between this vertexes");
    free(name1);
    free(name2);
    return 1;
}


int D_Find_shortest_way(Graph** tmp){
    char* name1 = NULL;
    char* name2 = NULL;
    int res;
    scanf("%*c");
    printf("Enter name(from): ");
    name1 = getStr();
    if (name1 == NULL)
        return 0;
    printf("Enter name(to): ");
    name2 = getStr();
    if (name2 == NULL)
        return 0;
    if(find_by_name(tmp, name1) == NULL || find_by_name(tmp, name2) == NULL){
        printf("%s\n", errmsgs[5]);
        return 1;
    }
    res = shortest_path(tmp, name1, name2);
    free(name1);
    free(name2);
    if (res == INT_MAX - 1 || res == INT_MAX - 2){
        printf("There is no way between vertexes");
        return 1;
    }
    printf("Shortest path length: %d\n", res);
    return 1;
}


int D_Delete_vertex(Graph** ptr){
    int rc;
    char* name = NULL;
    scanf("%*c");
    printf("Enter name: ");
    name = getStr();
    if (name == NULL)
        return 0;
    rc = delete_vertex(ptr, name);
    free(name);
    printf("%s\n", errmsgs[rc]);
    return 1;
}


int D_Delete_edge(Graph** ptr){
    int rc;
    char* name1 = NULL;
    char* name2 = NULL;
    scanf("%*c");
    printf("Enter name(from): ");
    name1 = getStr();
    if (name1 == NULL)
        return 0;
    printf("Enter name(to): ");
    name2 = getStr();
    if (name2 == NULL)
        return 0;
    rc = delete_edge(ptr, name1, name2);
    free(name1);
    free(name2);
    printf("%s\n", errmsgs[rc]);
    return 1;
}



void print_list(Graph* head){
    while(head){
        printf(" --> %s: (%d, %d)",head->info->name, head->x, head->y);
        head = head->next;
    }
    printf("\n");
}


int D_Show(Graph** trp){
    Graph* tmp = *trp;
    int i = 0;
    while(i < N){
        if(tmp->info != NULL) {
            printf("%s: (%d, %d)", tmp->info->name, tmp->x, tmp->y);
            Graph* head = tmp->next;
            print_list(head);
        }
        tmp = tmp + 1;
        i++;
    }
    return 1;
}


int D_Check(Graph** ptr){
    int res;
    res = dfs(ptr);
    if(res == 1)
        printf("\nCyclic graph\n");
    if(res == 0)
        printf("\nAcyclic graph\n");
    return 1;
}



int D_Timing(Graph** tmp){
    Graph* trp = NULL;
    int n = 10, cnt = 100000, i, m, j;
    char names[100000][6];
    char* name;
    trp = initialize(n*cnt);
    int k[2];
    clock_t first, last;
    srand(time(NULL));
    while (n-- > 0) {
        printf("\nTEST #%d", 10 - n);
        for (i = 0; i < 10000; ++i) {
            for (j = 0; j < 5; j++) {
                names[i][j] = '0' + rand() % 8;
            }
            names[i][j] = '\0';
        }
        first = clock();
        for (i = 0; i < cnt; ++i) {
            for (j = 0; j < 2; j++)
                k[j] = rand() % 1000;
            name = (char *) calloc(1, 6);
            for (int o = 0; o < 5; o++)
                name[o] = '0' + rand() % 8;
            name[5] = '\0';
            insert_vertex(&trp, k, name);
            free(name);
        }
        last = clock();
        printf("\nnumber of inserted vertexes = %d, time = %ld\n\n", (10 - n) * cnt, last - first);
        first = clock();
        for (i = 0; i < 10000; ++i)
            insert_edge(&trp, names[i], 0);
        last = clock();
        printf("number of inserted edges = %d, time = %ld\n\n", (10 - n) * cnt, last - first);
        /*first = clock();
        dfs(&trp);
        last = clock();
        printf("acyclicity check time = %ld\n",  last - first);
        */
    }
    delete_graph(&trp);
    return 1;
}



int D_Generate(Graph** ptr){
    delete_graph(ptr);
    *ptr = NULL;
    N = 20 + rand() % 10;
    *ptr = initialize(N);
    int k[2];
    char names[N][6];
    for (int i = 0; i < N; ++i) {
        int j = 0;
        for (j = 0; j < 5; j++) {
            names[i][j] = '0' + rand() % 10;
        }
        names[i][j] = '\0';
    }
    for(int i = 0; i < N; ++i){
        for (int l = 0; l < 2; l++)
            k[l] = rand() % 100;
        insert_vertex(ptr, k, names[i]);
    }
    int p = 0;
    int num = 10 + rand() % (N*N - 10);//number of edges
    while(p < num){
        int i = rand() % (N);
        int j = rand() % N;
        insert_edge(ptr, names[i], names[j]);
        p++;
    }
    return 1;
}


