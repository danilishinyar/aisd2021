//
// Created by yaroslav on 22.04.2021.
//

#ifndef LAB4A_LAB_H
#define LAB4A_LAB_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <time.h>

typedef struct Info{
    int release;
    char* info;
}Info;




typedef struct Tree{
    char* key;
    Info* info;
    struct Tree* left;
    struct Tree* right;
}Tree;




int getInt(int* a);
char* getStr();
Tree* NewElement(char* key, char* info);
int insert_duplicate(Tree* ptr, Tree* prev, char* info);
int insert(Tree** trp, char* key, char* info);
Tree* find(Tree** trp, char* key, int release);
void find_nearest(Tree** trp, char* key, int min, Tree** tmp);
void del_element(Tree** ptr, Tree* ptr_parent, char* key, int release);
Tree* minimum(Tree *trp);
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
void show(Tree* trp, int d);
void show_in_range(Tree* trp, char* min, char* max);
int D_Show_in_range(Tree** trp);
int D_Show(Tree** trp);
int D_Timing(Tree **);



#endif //LAB4A_LAB_H
