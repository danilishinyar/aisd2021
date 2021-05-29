#ifndef LAB_H
#define LAB_H


#include<stdio.h>
#include<stdlib.h>
#include<string.h>


typedef struct KeySpace1{
    int key;
    int offset_item;
}KeySpace1;


typedef struct KeySpace2{
    char* key;
    int offset_item;
    int release;
    struct KeySpace2* next;
}KeySpace2;


typedef struct Table{
    KeySpace1* ks1;
    KeySpace2** ks2;
    char* fname;
    FILE* fd;
    FILE* ks1_fd;
    FILE* ks2_fd;
    FILE* item_fd;
    char* ks1_fname;
    char* ks2_fname;
    char* item_fname;
    int* ks2_offsets;
    int msize1;
    int msize2;
    int csize1;
    int csize2;
}Table;


char* getStr();
int getInt(int* a);
int dialog(const char *msgs[], int N);
int D_Add(Table *ptb);
int Find_dialog(const char *msgs_find[], int N);
int D_Find(Table* ptb);
int D_Find_ks1(Table* ptb);
int D_Find_ks2(Table* ptb);
int D_Find_ks2_release(Table* ptb);
int D_Find_ks1ks2(Table* ptb);
int Del_dialog(const char *msgs_del[], int N);
int D_Del(Table *ptb);
void D_Del_Table(Table* ptb);
int D_Del_Range(Table* ptb);
int D_Del_Erase(Table *ptb);
int D_Del_ks1(Table* ptb);
int D_Del_ks2(Table* ptb);
int D_Del_ks1ks2(Table* ptb);
int D_Del_Release(Table* ptb);
int D_Show(Table* ptb);
Table* initialize(int msize1, int msize2);
int insert(Table* ptb, int key1, char* key2, char* info);
KeySpace2 * find_ks2_release(Table* ptb, char* key, int release);
KeySpace1* find_ks1(Table* ptb, int key);
KeySpace2* find_ks2(Table* tb, char* key);
KeySpace2* find_ks1ks2(Table* ptb, int key1, char* key2);
int delete_tb(Table* ptb);
int delete_ks1(Table* ptb, int key);
int delete_ks1_range(Table* ptb, int min, int max);
int delete_ks2(Table *ptb, char* key2);
int delete_ks2_erase(Table* ptb);
int delete_ks2_r(Table* ptb, char* key2, int realese);
int delete_ks1ks2(Table* ptb, int key1, char* key2);
int hash_function(char *key2, int size);
int find_release(Table* ptb, int key1, char* key2);
int save(Table* ptb);
Table* load();
int del_items(Table* ptb);
KeySpace2* find_ks2_by_ks1(Table* ptb, int key1);

#endif