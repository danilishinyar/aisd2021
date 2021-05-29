#include "lab.h"



Graph* initialize(int k){
    Graph* ptr = (Graph*)calloc(k, sizeof(Graph));//массив вершин
    return ptr;
}



int hash_function(char *name, int size){
    int hash = 0, i = 0;
    while(name[i] != '\0'){
        hash = hash*37 + name[i];
        i++;
    }
    return abs(hash) % size;
}




int insert_vertex(Graph** ptr, int* point, char* name){

    int k = 0;
    for(int i = 0; i < N; ++i)
        if((*ptr)[i].info != NULL)
            k++;
    if(k == N)
        return 1;//overflow
    //duplicate check
    if(find_by_name(ptr, name) != NULL || find_by_xy(ptr, point)){
        return 3;
    }
    int hash = hash_function(name, N);
    if((*ptr)[hash].info == NULL) {
        (*ptr)[hash].x = point[0];
        (*ptr)[hash].y = point[1];
        (*ptr)[hash].info = (InfoType *) calloc(1, sizeof(InfoType));
        (*ptr)[hash].info->name = (char *) calloc(strlen(name) + 1, 1);
        memcpy((*ptr)[hash].info->name, name, strlen(name) + 1);
        (*ptr)[hash].next = NULL;
        (*ptr)[hash].place_in_arr = hash;
    }
    else{
        while((*ptr)[hash].info != NULL)
            hash = (hash + 1) % N;
        (*ptr)[hash].x = point[0];
        (*ptr)[hash].y = point[1];
        (*ptr)[hash].info = (InfoType *) calloc(1, sizeof(InfoType));
        (*ptr)[hash].info->name = (char *) calloc(strlen(name) + 1, 1);
        memcpy((*ptr)[hash].info->name, name, strlen(name) + 1);
        (*ptr)[hash].next = NULL;
        (*ptr)[hash].place_in_arr = hash;
    }
    return 0;
}


int insert_edge(Graph** ptr, char* name1, char* name2){//name1 -------> name2
    Graph* ptr1 = find_by_name(ptr, name1);
    Graph* ptr2 = find_by_name(ptr, name2);
    Graph* tmp = ptr1;
    if(ptr1 == NULL || ptr2 == NULL)
        return 5;
    if(find_edge(ptr, name1, name2))
        return 2;
    //duplicate check
    while(ptr1->next)
        ptr1 = ptr1->next;
    ptr1->next = (Graph*)calloc(1, sizeof(Graph));
    ptr1->next->x = ptr2->x;
    ptr1->next->y = ptr2->y;
    ptr1->next->info = ptr2->info;
    ptr1->next->place_in_arr = ptr2->place_in_arr;
    ptr1->next->next = NULL;
    return 0;
}


Graph* find_by_name(Graph** ptr, char* name){
    if(name == NULL)
        return *ptr;
    for(int i = 0; i < N; ++i)
        if((*ptr)[i].info != NULL)
            if(!strcmp((*ptr)[i].info->name, name))
                return (*ptr)+i;
    return NULL;
}


Graph* find_by_xy(Graph** ptr, int* point){
    for(int i = 0; i < N; ++i)
        if((*ptr)[i].info != NULL)
            if((*ptr)[i].x == point[0] && (*ptr)[i].y == point[1])
                return (*ptr)+i;
    return NULL;
}

int find_edge(Graph** ptr, char* name1, char* name2){
    if(name1 == NULL || name2 == NULL)
        return 0;
    Graph* ptr1 = find_by_name(ptr, name1)->next;
    while(ptr1){
        if(!strcmp(ptr1->info->name, name2))
            return 1;
        ptr1 = ptr1->next;
    }
    return 0;
}



int delete_vertex(Graph** ptr, char* name){
    Graph* ptr1 = find_by_name(ptr, name);
    if(ptr1 == NULL)
        return 5;
    Graph* tmp = ptr1;
    for(int i = 0; i < N; ++i){
        if((*ptr)[i].info)
            delete_edge(ptr, (*ptr)[i].info->name, name);
    }
    while(ptr1->next){
        tmp = ptr1->next;
        ptr1->next = ptr1->next->next;
        free(tmp);
    }
    free(ptr1->info->name);
    free(ptr1->info);
    ptr1->info = NULL;
    return 0;
}


int delete_edge(Graph** ptr, char* name1, char* name2){
    Graph* ptr1 = find_by_name(ptr, name1);
    if(ptr1 == NULL)
        return 5;
    if(!find_edge(ptr, name1, name2))
        return 4;
    Graph* tmp = ptr1;
    while(tmp->next != NULL){
        if(strcmp(tmp->next->info->name, name2) == 0)
            break;
        tmp = tmp->next;
    }
    Graph* tmp1 = tmp->next;
    tmp->next = tmp->next->next;
    free(tmp1);
    return 0;
}

int delete_graph(Graph** ptr){
    for(int i = 0; i < N; ++i){
        Graph* ptr1 = *ptr + i;
        Graph* tmp = NULL;
        while(ptr1->next){
            tmp = ptr1->next;
            ptr1->next = ptr1->next->next;
            free(tmp);
        }
        if(ptr1->info != NULL) {
            free(ptr1->info->name);
            free(ptr1->info);
        }
    }
    free(*ptr);
}


Graph* bfs_by_name(Graph** ptr, char* name1, char* name2){
    Graph* tmp = find_by_name(ptr, name1);
    queue* q = (queue*)calloc(1, sizeof(queue));
    init(q);
    insert(q, tmp);
    int* color = (int*)calloc(N, sizeof(int));//0-white 1-grey 2-black
        while (is_empty(q)) {
            tmp = q->head->ptr;
            while (tmp) {
                if (color[tmp->place_in_arr] == 0) {
                    color[tmp->place_in_arr] = 1;
                    if (!strcmp(tmp->info->name, name2)) {
                        while(q->head){
                            list* tmp = q->head;
                            q->head = q->head->next;
                            free(tmp);
                        }
                        free(q);
                        free(color);
                        return tmp;
                    }
                    insert(q, (*ptr) + tmp->place_in_arr);
                }
                tmp = tmp->next;
            }
            color[q->head->ptr->place_in_arr] = 2;
            remove_node(q);
        }
    free(color);
    free(q);
    return NULL;
}

int shortest_path(Graph** ptr, char* name1, char* name2){
    Graph* ptr1 = find_by_name(ptr, name1);
    Graph* ptr2 = find_by_name(ptr, name2);
    int res = INT_MAX - 2;
    int* dist = (int*)calloc(N, sizeof(int));
    for(int i = 0; i < N; ++i){
        dist[i] = res;
    }
    dist[ptr1->place_in_arr] = 0;
    Graph* tmp;
    int i = 0;
    int k = 1;
    while(i != N){
        tmp = (*ptr) + i;
            while(tmp){
                if(dist[tmp->place_in_arr] > dist[(*ptr)[i].place_in_arr] + 1)
                    dist[tmp->place_in_arr] = dist[(*ptr)[i].place_in_arr] + 1;

                if(dist[tmp->place_in_arr] < dist[(*ptr)[i].place_in_arr] + 1){
                    tmp = tmp->next;
                    continue;
                }
                tmp = tmp->next;
            }
        i++;
    }
    res = dist[ptr2->place_in_arr];
    free(dist);
    return res;
}



void dfs_visit(Graph** ptr, Graph* u, int* color, int* b){
    color[u->place_in_arr] = 1;
    Graph* v = u->next;
    while(v){
        if (color[v->place_in_arr] == 1)
            *b = 1;
        if (color[v->place_in_arr] == 0)
            dfs_visit(ptr, find_by_name(ptr, v->info->name), color, b);
        v = v->next;
    }
    color[u->place_in_arr] = 2;
}

int dfs(Graph** ptr){
    int b = 0;
    int* color = (int*)calloc(N, sizeof(int));//0-white 1-grey 2-black
    for(int i = 0; i < N; ++i)
        if(color[i] == 0)
            dfs_visit(ptr, *ptr + i, color, &b);
    free(color);
    if(b == 1)
        return 1;//цикличный граф
    else
        return 0;
}




void load(Graph** ptr){
    FILE* fd_vertex = fopen("Graph_vertexes.txt", "rb");
    FILE* fd_edge = fopen("Graph_edges.txt", "rb");
    char* name1 = NULL;
    char* name2 = NULL;
    int name_len;
    int point[2];
    fseek(fd_vertex, 0, SEEK_END);
    int end = ftell(fd_vertex);
    fseek(fd_vertex, 0, SEEK_SET);
    fread(&N, sizeof(int), 1, fd_vertex);
    *ptr = initialize(N);
    while(ftell(fd_vertex) != end) {
        fread(&name_len,  sizeof(int), 1, fd_vertex);//read vertex
        fread(point, sizeof(int), 2, fd_vertex);
        name1 = (char*)calloc(name_len, 1);
        fread(name1, sizeof(char), name_len, fd_vertex);
        insert_vertex(ptr, point, name1);
        free(name1);
    }
    fseek(fd_edge, 0, SEEK_END);
    end = ftell(fd_edge);
    fseek(fd_edge, 0, SEEK_SET);
    while(ftell(fd_edge) != end) {
        fread(&name_len, sizeof(int), 1, fd_edge);//read edge
        name1 = (char *) calloc(name_len, 1);
        fread(name1, sizeof(char), name_len, fd_edge);
        fread(&name_len, sizeof(int), 1, fd_edge);
        name2 = (char *) calloc(name_len, 1);
        fread(name2, sizeof(char), name_len, fd_edge);
        insert_edge(ptr, name1, name2);
        free(name1);
        free(name2);
    }
    fclose(fd_vertex);
    fclose(fd_edge);
}

void save(Graph** ptr){
    FILE* fd_vertex = fopen("Graph_vertexes.txt", "wb");
    FILE* fd_edge = fopen("Graph_edges.txt", "wb");
    int name_len;
    fwrite(&N, sizeof(int), 1, fd_vertex);
    for(int i = 0; i < N; ++i){
        if((*ptr)[i].info != NULL){
            name_len = strlen((*ptr)[i].info->name) + 1;//save vertex
            fwrite(&name_len, sizeof(int), 1,fd_vertex);
            int x = ((*ptr)[i].x);
            int y = ((*ptr)[i].y);
            fwrite(&x, sizeof(int), 1,fd_vertex);
            fwrite(&y, sizeof(int), 1,fd_vertex);
            fwrite((*ptr)[i].info->name, sizeof(char), name_len, fd_vertex);
            Graph* tmp = ((*ptr) + i)->next;
            while(tmp){
                int name_len1 = strlen(tmp->info->name) + 1;
                fwrite(&name_len, sizeof(int), 1,fd_edge);
                fwrite((*ptr)[i].info->name, sizeof(char), name_len, fd_edge);
                fwrite(&name_len1, sizeof(int), 1,fd_edge);
                fwrite(tmp->info->name, sizeof(char), name_len1, fd_edge);
                tmp = tmp->next;
            }
        }
    }
    fclose(fd_vertex);
    fclose(fd_edge);
}