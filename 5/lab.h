//
// Created by yaroslav on 10.05.2021.
//

#ifndef LAB5_LAB_H
#define LAB5_LAB_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <time.h>

#  define INT_MAX    2147483647
#  define INT_MIN   (-INT_MAX - 1)


typedef struct InfoType{
    char* name;
}InfoType;


typedef struct Graph{
    int x;
    int y;
    InfoType* info;
    struct Graph* next;
    int place_in_arr;
}Graph;


typedef struct list{
    Graph* ptr;
    struct list* next;
}list;

typedef  struct queue{
    list* head;
    list* tail;
}queue;

int N;

void init(queue* q);
list* new_node(Graph* ptr);
int is_empty(queue* q);
int remove_node(queue* q);
void insert(queue* q, Graph* ptr);
Graph* initialize();
Graph* find_by_name(Graph** ptr, char* name);
Graph* find_by_xy(Graph** ptr, int* point);
int find_edge(Graph** ptr, char* name1, char* name2);
int insert_vertex(Graph** ptr, int* point, char* name);
int insert_edge(Graph** ptr, char* name1, char* name2);
int delete_vertex(Graph** ptr, char* name);
int delete_edge(Graph** ptr, char* name1, char* name2);
Graph* bfs_by_name(Graph** ptr, char* name2, char* name1);
int dfs(Graph** ptr);
void dfs_visit(Graph** ptr, Graph* u, int* color, int* b);
int shortest_path(Graph** ptr, char* name1, char* name2);
int delete_graph(Graph** ptr);
void load(Graph** ptr);
void save(Graph** ptr);
char* getStr();
int getInt(int* a);
int dialog(const char *msgs[], int N);
int D_Add_vertex(Graph **ptr);
int D_Add_edge(Graph **ptr);
int D_Delete_vertex(Graph** ptr);
int D_Delete_edge(Graph** ptr);
int D_Timing(Graph** tmp);
int D_Show(Graph** tmp);
int D_Find_vertex_bfs(Graph** tmp);
int D_Find_shortest_way(Graph** tmp);
int D_Check(Graph** tmp);
int D_Generate(Graph** tmp);


#endif //LAB5_LAB_H
