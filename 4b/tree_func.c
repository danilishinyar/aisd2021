#include "lab.h"


int create(Tree** trp){
    FILE* fd = fopen("Tree.txt", "w+b");
    fclose(fd);
    return 0;
}



int load(Tree** trp){
    FILE* fd = fopen("Tree.txt", "r+b");
    int point[2];
    int info_len;
    char* info = NULL;
    //read item list
    // key0/key1/info_len/info
    fseek(fd, 0, SEEK_END);
    int end = ftell(fd);
    fseek(fd, 0, SEEK_SET);
    while(ftell(fd) != end) {
        fread(point,  sizeof(int),2,  fd);//keys
        fread(&info_len, sizeof(int), 1, fd);
        info = (char *) calloc(1, info_len);
        fread(info, 1, info_len, fd);
        insert(trp, point, info, 0);
        free(info);
    }
    fclose(fd);
    return 0;
}


void save_tree(Tree* ptr, FILE* fd){
    if(ptr) {
        Item* tmp = ptr->head;
        while(tmp) {
            int info_len = strlen(tmp->info) + 1;
            fwrite(tmp->key, sizeof(int), 2, fd);
            fwrite(&info_len, sizeof(int), 1, fd);
            fwrite(tmp->info, 1, info_len, fd);
            tmp = tmp->next;
        }
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


Tree* NewNode(Tree** trp, int* point, char* info, int cd){
    Tree* ptr = (Tree*)calloc(1,sizeof(Tree));
    ptr->limit = point[cd];
    ptr->right = NULL;
    ptr->left = NULL;
    ptr->head = (Item*)calloc(1, sizeof(Item));
    for (int i=0; i<2; i++)
        ptr->head->key[i] = point[i];
    int r = 25;
    r = find_max_release(&tree_root, point, -1, 0);
    ptr->head->release = r + 1;
    ptr->head->next = NULL;
    ptr->head->info = (char*)calloc(sizeof(info)+1, 1);
    memcpy(ptr->head->info, info, strlen(info)+1);
    return ptr;
}


int num_elms(Item* head){
    int i = 0;
    while(head){
        i++;
        head = head->next;
    }
    return i;
}

int limit_node(Item* head, int cd){
    int i = 0;
    while(head){
        i+=head->key[cd];
        head = head->next;
    }
    return i/N;
}

void insert_in_list(Tree** ptr, int* point, char* info, int cd){
    Item* next = (*ptr)->head;
    Item* prev = (*ptr)->head;
    int k = 0;
    while(next && next->key[cd]<=point[cd]){
        prev = next;
        next = next->next;
        k++;
    }
    Item* tmp = (Item*)calloc(1, sizeof(Item));
    for (int i=0; i<2; i++)
        tmp->key[i] = point[i];
    tmp->info = (char*)calloc(sizeof(info)+1, 1);
    memcpy(tmp->info, info, strlen(info)+1);
    tmp->release = find_max_release(&tree_root, point, -1, 0) + 1;
    if(prev == (*ptr)->head && k == 0) {
        (*ptr)->head = tmp;
        tmp->next = prev;
    }
    else {
        prev->next = tmp;
        tmp->next = next;
    }
}


int insert(Tree** trp, int* point, char* info, int depth){
    int cd = depth % 2;
    if(*trp == NULL){//if root is empty
        *trp = NewNode(trp, point, info, cd);
        return 0;
    }
    int k = num_elms((*trp)->head);
    //if list isn't full
    if(k < N){
        insert_in_list(trp, point, info, cd);
        if(num_elms((*trp)->head) == N){
            (*trp)->limit = limit_node((*trp)->head, cd);
        }
    }
    if(k == N){
        if(point[cd] <= (*trp)->limit)
            insert(&((*trp)->left), point, info, depth + 1);
        else
            insert(&((*trp)->right), point, info, depth + 1);
    }
    return 0;
}



Tree* minimum(Tree *trp){
    if (!trp->left->left) return trp;
    return minimum(trp->left);
}


void del_list(Item* p){
    Item *tmp = NULL;
    while(p != NULL){
        tmp = p;
        p = p->next;
        free(tmp->info);
        free(tmp);
    }
}


void delTree(Tree** trp){
    if ((*trp)->left)
        delTree((&(*trp)->left));
    if ((*trp)->right)
        delTree((&(*trp)->right));
    del_list((*trp)->head);
    free(*trp);
}

void del_elm(Item** head, int* point, int release){// delete item from list
    Item* cur = (*head)->next;
    Item* prev = *head;
    if((cur == NULL) || (prev->key[0] == point[0] && prev->key[1] == point[1] && prev->release == release)) {
        *head = cur;
        free(prev->info);
        free(prev);
        return;
    }
    while (cur != NULL && prev != NULL) {
        if (cur->key[0] == point[0] && cur->key[1] == point[1] && cur->release == release) {
            Item* tmp = cur;
            prev->next = cur->next;
            free(tmp->info);
            free(tmp);
            return;
        }
        prev = cur;
        cur = cur->next;
    }
    return;
}





int del_node(Tree** ptr, Tree* ptr_parent, int* point, int release, int depth) {
    if (*ptr == NULL)//если пустой указатель на удаляемый елемент
        return 2;
    int cd = depth % 2;

    if (find_in_list_release((*ptr)->head, point, release) != NULL) {
        del_elm(&((*ptr)->head), point, release);
        if((*ptr)->right != NULL){

        }
        if ((*ptr)->head == NULL) {//delete last elm and then delete node
            //если найден элемент, рассматриваем 3 возможных случая
            if (!(*ptr)->right && !(*ptr)->left) {//у элемента нет правого и левого потомка
                if (ptr_parent) {//если есть родительский элемент
                    Tree *tmp = (*ptr);
                    if (ptr_parent->left) //если это левый элемент
                        if (ptr_parent->left == tmp)
                            ptr_parent->left = NULL;

                    if (ptr_parent->right)//если это правый элемент
                        if (ptr_parent->right == tmp)
                            ptr_parent->right = NULL;
                    free(tmp);
                }
                return 0;
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
                (*ptr)->head = tmp->head;
                (*ptr)->limit = tmp->limit;
                free(tmp);
                return 0;

            } else {//если есть и левый и правыей потомок
                //нужно найти наименьший элемент в правом поддереве
                if (!(*ptr)->right->left) {//если у потомка только правые потомки
                    Tree *tmp = (*ptr)->right->right;
                    (*ptr)->head = (*ptr)->right->head;
                    (*ptr)->limit = (*ptr)->right->limit;
                    free((*ptr)->right);
                    (*ptr)->right = tmp;
                } else {
                    Tree *tmp = minimum((*ptr)->right);//родитель наименьшего элемента в правом поддереве
                    (*ptr)->head = (*ptr)->left->head;
                    (*ptr)->limit = (*ptr)->left->limit;
                    free(tmp->left);
                    tmp->left = NULL;
                }
                return 0;
            }
        }
        return 0;
    } else {//продвигаемся по дереву в поисках нужного элемента
        if (point[cd] <= (*ptr)->head->key[cd])
            del_node(&((*ptr)->left), *ptr, point, release, depth + 1);
        else if (point[cd] > (*ptr)->head->key[cd])
            del_node(&((*ptr)->right), *ptr, point, release, depth + 1);
    }
}


void include_in_list(Item*** list, Item* item, int* i){
    *i = *i + 1;
    *list = (Item**)realloc(*list, *i * sizeof(Item*));
    int r = *i - 1;
    (*list)[r] = item;
}


Item** find(Tree** trp, int* point, Item*** list, int depth, int* i){
    Tree* tmp = *trp;
    int cd = depth % 2;
    if(tmp == NULL)
        return *list;
    find_in_list(tmp->head, point, list, i);
    if (point[cd] <= tmp->head->key[cd])//if ptr->key < key
        find(&(tmp->left), point, list, depth + 1, i);
    else
        find(&(tmp->right), point, list, depth + 1, i);
}



Item* find_in_list(Item* head, int* point, Item*** list, int* i){
    Item* ptr = NULL;
    while(head){
        if (head->key[0] == point[0] && head->key[1] == point[1]) {
            ptr = head;
            include_in_list(list, ptr, i);
        }
        head = head->next;
    }
    return ptr;
}


Item* find_in_list_r(Item* head, int* point){
    Item* ptr = NULL;
    while(head){
        Item* tmp = head;
        if (tmp->key[0] == point[0] && tmp->key[1] == point[1]) {
            ptr = tmp;
        }
        head = head->next;
    }
    return ptr;
}


Item* find_in_list_release(Item* head, int* point, int release){
    Item* ptr = NULL;
    while(head){
        if (head->key[0] == point[0] && head->key[1] == point[1] && head->release == release) {
            ptr = head;
        }
        head = head->next;
    }
    return ptr;
}


int find_max_release(Tree** trp, int* point, int r, int depth){
    Tree* tmp = *trp;
    Item* ptr = NULL;
    int cd = depth % 2;
    if(tmp == NULL)
        return r;
    ptr = find_in_list_r(tmp->head, point);
    if(ptr != NULL && ptr->release > r){
        r = ptr->release;
    }
    if (point[cd] <= tmp->head->key[cd])//if ptr->key < key
        find_max_release(&(tmp->left), point, r, depth + 1);
    else
        find_max_release(&(tmp->right), point, r, depth + 1);
}


void find_nearest(Tree** trp, int* point, int depth, Item** tmp, int m){
    int cd = depth % 2;
    if(*trp == NULL)
        return;
    Item* head = (*trp)->head;
    while(head){
        if(head->key[0] * head->key[0] + head->key[1] * head->key[1] < m) {
            m = head->key[0] * head->key[0] + head->key[1] * head->key[1];
            *tmp = head;
        }
        head = head->next;
    }

    if(abs(point[cd]<(*trp)->limit))
        find_nearest(&((*trp)->left), point, depth + 1, tmp, m);
    else
        find_nearest(&((*trp)->right), point, depth + 1, tmp, m);

}




