
#ifndef LAB4A_LAB_H
#define LAB4A_LAB_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <time.h>



#  define INT_MAX    2147483647
#  define INT_MIN   (-INT_MAX - 1)

int N;


typedef struct Item{
    int key[2];
    int release;
    char* info;
    struct Item* next;
}Item;




typedef struct Tree{
    int limit;
    int cd;
    Item* head;
    struct Tree* left;
    struct Tree* right;
}Tree;

Tree* tree_root;

int getInt(int* a);
char* getStr();
Tree* NewNode(Tree** trp, int* point, char* info, int cd);
int insert(Tree** trp, int* point, char* info, int depth);
Item* find_in_list(Item* head, int* point, Item*** list, int* i);
Item* find_in_list_r(Item* head, int* point);
Item* find_in_list_release(Item* head, int* point, int release);
Item** find(Tree** trp, int* point, Item*** list, int depth, int* i);
int find_max_release(Tree** trp, int* point, int r, int depth);
void find_nearest(Tree** trp, int* point, int depth, Item** tmp, int m);
int del_node(Tree** ptr, Tree* ptr_parent, int* point, int release, int depth);
void del_elm(Item** head, int* point, int release);
Tree* minimum(Tree *trp);
void del_list(Item* p);
void delTree(Tree** trp);
int create(Tree** trp);
int load(Tree** trp);
int save(Tree** trp);
void save_tree(Tree* ptr, FILE* fd);
int dialog(const char *msgs[], int N);
int D_Add(Tree** trp);
int D_Find(Tree** trp);
int D_Find_nearest(Tree** trp);
int D_Delete(Tree** trp);
void print_list(Item* head, int d);
void show(Tree* trp, int d);
void show_in_range(Tree* trp, int x, int y);
int D_Show_in_range(Tree** trp);
int D_Show(Tree** trp);
int D_Timing(Tree **);



#endif //LAB4A_LAB_H
