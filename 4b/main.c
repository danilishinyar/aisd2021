#include"lab.h"



const char *msgs[] = { "0. Quit", "1. Add", "2. Find", "3. Delete element", "4. Show", "5. Show in range", "6. Timing", "7. Find nearest"};
const int NMsgs = sizeof(msgs) / sizeof(msgs[0]);
int (*fptr[])(Tree** trp) = {NULL, D_Add, D_Find, D_Delete, D_Show, D_Show_in_range,  D_Timing, D_Find_nearest};


int main(){
    int rc, n, answer;
    tree_root = NULL;
    printf("Do you want to load tree from file?\n1.Yes\n2.No\n");
    n = getInt(&answer);
    if(n == 0)
        return 0;
    if(answer == 1){
        load(&tree_root);
    }
    else {
        if(answer == 2) {
            create(&tree_root);
            printf("Enter N: ");
            n = getInt(&N);
            if(n == 0)
                return 0;
        }
        else
            return 0;
    }

    while(rc = dialog(msgs, NMsgs))
        if(!fptr[rc](&tree_root))
            break;
    printf("That's all. Bye!\n");
    save(&tree_root);
    if(tree_root != NULL)
        delTree(&tree_root);
    return 0;
}

