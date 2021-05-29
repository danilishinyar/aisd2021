#include "lab.h"


int create(Tree** trp){
    FILE* fd = fopen("Tree.txt", "w+b");
    fclose(fd);
    return 0;
}



int load(Tree** trp){
    FILE* fd = fopen("Tree.txt", "r+b");
    char* key;
    char* info;
    int key_len, info_len;
    fseek(fd, 0, SEEK_END);
    int end = ftell(fd);
    fseek(fd, 0, SEEK_SET);
    while(ftell(fd) != end) {
        fread(&key_len, sizeof(int), 1, fd);
        key = (char *) calloc(1, key_len);
        fread(key, sizeof(char), key_len, fd);
        fread(&info_len, sizeof(int), 1, fd);
        info = (char *) calloc(1, info_len);
        fread(info, sizeof(char), info_len, fd);
        insert(trp, key, info);
        free(key);
        free(info);
    }
    fclose(fd);
    return 0;
}


void save_tree(Tree* ptr, FILE* fd){
    if(ptr) {
        int key_len = strlen(ptr->key) + 1;
        int info_len = strlen(ptr->info->info) + 1;
        fwrite(&key_len, sizeof(int), 1, fd);
        fwrite(ptr->key, 1, key_len, fd);
        fwrite(&info_len, sizeof(int), 1, fd);
        fwrite(ptr->info->info, 1, info_len, fd);
        save_tree(ptr->left, fd);
        save_tree(ptr->right, fd);
    }
}


int save(Tree** trp){
    FILE* fd = fopen("Tree.txt", "w+b");
    Tree* ptr = *trp;
    save_tree(ptr, fd);
    fclose(fd);
    return 0;
}




Tree* NewElement(char* key, char* info){
    Tree* ptr = (Tree*)calloc(1,sizeof(Tree));
    ptr->key = (char*)calloc(strlen(key) + 1, 1);
    memcpy(ptr->key, key, strlen(key)+1);
    ptr->info = (Info*)calloc(1, sizeof(Info));
    ptr->info->info = (char*)calloc(strlen(info) + 1, 1);
    memcpy(ptr->info->info, info, strlen(info)+1);
    ptr->info->release = 0;
    ptr->left = ptr->right = NULL;
    return ptr;
}


int insert_duplicate(Tree* ptr, Tree* prev, char* info){
    Tree* cur = (Tree*)calloc(1, sizeof(Tree));
    cur->key = (char*)calloc(strlen(ptr->key) + 1, 1);
    memcpy(cur->key, ptr->key, strlen(ptr->key)+1);
    cur->info = (Info*)calloc(1, sizeof(Info));
    cur->info->info = (char*)calloc(strlen(info) + 1, 1);
    memcpy(cur->info->info, info, strlen(info)+1);
    cur->info->release = ptr->info->release + 1;
    if (prev->right == ptr){
        cur->right = ptr;
        prev->right = cur;
    }
    else{
        cur->right = ptr;
        prev->left = cur;
    }
    return 0;
}


int insert(Tree** trp, char* key, char* info){
    if(*trp == NULL){
        *trp = NewElement(key, info);
        return 0;
    }
    Tree* prev = *trp;
    Tree* ptr;
    if(!strcmp((*trp)->key, key)){
        ptr = (Tree*)calloc(1,sizeof(Tree));
        ptr->key = (char*)calloc(strlen(key) + 1, 1);
        memcpy(ptr->key, key, strlen(key)+1);
        ptr->info = (Info*)calloc(1, sizeof(Info));
        ptr->info->info = (char*)calloc(strlen(info) + 1, 1);
        memcpy(ptr->info->info, info, strlen(info)+1);
        ptr->info->release = (*trp)->info->release+1;
        ptr->right = *trp;
        ptr->left = NULL;
        *trp = ptr;
        return 0;
    }
    while(prev) {
        if (strcmp((prev)->key, key) < 0) {//if ptr->key < key
            ptr = prev;
            prev = prev->right;
        } else {
            ptr = prev;
            prev = prev->left;
        }
        if(prev!=NULL && !strcmp(key, (prev)->key)){//key duplicate
            insert_duplicate(prev, ptr, info);
            return 0;
        }
    }
    if(strcmp((ptr)->key, key) < 0)
        insert(&(ptr->right), key, info);
    else
        insert(&(ptr->left), key, info);
    return 0;
}


void delTree(Tree** trp){
    if ((*trp)->left)
        delTree((&(*trp)->left));
    if ((*trp)->right)
        delTree((&(*trp)->right));
    free((*trp)->key);
    free((*trp)->info->info);
    free((*trp)->info);
    free(*trp);
}


Tree* find(Tree** trp, char* key, int release){
    Tree* tmp = *trp;
    if(tmp == NULL)
        return NULL;

    if(!strcmp(tmp->key, key) && release == tmp->info->release)
        return tmp;

    if(strcmp(tmp->key, key) > 0)//if ptr->key < key
        find(&(tmp->left), key, release);
    else
        find(&(tmp->right), key, release);

}


void find_nearest(Tree** trp, char* key, int m, Tree** tmp){
    if(*trp == NULL)
        return;

    if(abs(strcmp((*trp)->key, key)) < m && abs(strcmp((*trp)->key, key)) != 0){
        *tmp = *trp;
        m = abs(strcmp((*trp)->key, key));
    }

    if(strcmp((*trp)->key, key) > 0)//if ptr->key < key
        find_nearest(&((*trp)->left), key, m, tmp);
    else
        find_nearest(&((*trp)->right), key, m, tmp);

}



Tree* minimum(Tree *trp){
    if (!trp->left->left) return trp;
    return minimum(trp->left);
}


void del_element(Tree** ptr, Tree* ptr_parent, char* key, int release) {
    if (*ptr == NULL)//если пустой указатель на удаляемый елемент
        return;

    if (strcmp((*ptr)->key, key) > 0)//продвигаемся по дереву в поисках нужного элемента
        del_element(&((*ptr)->left), *ptr, key, release);
    else if (strcmp((*ptr)->key, key) < 0)
        del_element(&((*ptr)->right), *ptr, key, release);

    else {
        if(!strcmp((*ptr)->key, key) && release != (*ptr)->info->release)
            del_element(&((*ptr)->right), *ptr, key, release);
        //если найден элемент, рассматриваем 3 возможных случая
        if (!(*ptr)->right && !(*ptr)->left) {//у элемента нет правого и левого потомка
            if (ptr_parent) {//если есть родительский элемент
                Tree* tmp = (*ptr);
                if (ptr_parent->left) //если это левый элемент
                    if (ptr_parent->left == tmp)
                        ptr_parent->left = NULL;

                if (ptr_parent->right)//если это правый элемент
                    if (ptr_parent->right == tmp)
                        ptr_parent->right = NULL;
                free((tmp)->key);
                free((tmp)->info->info);
                free((tmp)->info);
                free((tmp));
            }

            return;

        }
        //если у удаляемого элемента есть хотя бы один потомок
        if (!(*ptr)->right || !(*ptr)->left) {
            Tree *tmp;
            if ((*ptr)->left)
                tmp = (*ptr)->left;
            else
                tmp = (*ptr)->right;
            (*ptr)->left = tmp->left;
            (*ptr)->right = tmp->right;
            free((*ptr)->key);
            free((*ptr)->info->info);
            free((*ptr)->info);
            (*ptr)->key = tmp->key;
            (*ptr)->info = tmp->info;
            free(tmp);
            return;

        } else {//если есть и левый и правыей потомок
            //нужно найти наименьший элемент в правом поддереве
            if (!(*ptr)->right->left) {//если у потомка только правые потомки
                free((*ptr)->key);
                free((*ptr)->info->info);
                free((*ptr)->info);
                (*ptr)->info = (*ptr)->right->info;
                (*ptr)->key = (*ptr)->right->key;
                Tree *tmp = (*ptr)->right->right;
                free((*ptr)->right);
                (*ptr)->right = tmp;
            } else {
                Tree *tmp = minimum((*ptr)->right);//родитель наименьшего элемента в правом поддереве
                free((*ptr)->key);
                free((*ptr)->info->info);
                free((*ptr)->info);
                (*ptr)->info = tmp->left->info;
                (*ptr)->key = tmp->left->key;
                free(tmp->left);
                tmp->left = NULL;
            }
            return;
        }
    }
}

        