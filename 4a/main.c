#include"lab.h"



const char *msgs[] = { "0. Quit", "1. Add", "2. Find", "3. Find nearest", "4. Delete element", "5. Show", "6. Show in range", "7. Timing"};
const int NMsgs = sizeof(msgs) / sizeof(msgs[0]);
int (*fptr[])(Tree** trp) = {NULL, D_Add, D_Find, D_Find_nearest, D_Delete, D_Show, D_Show_in_range, D_Timing};


int main(){
    int rc, n, answer;
    Tree* trp = NULL;
    printf("Do you want to load tree from file?\n1.Yes\n2.No\n");
    n = getInt(&answer);
    if(n == 0)
        return 0;
    if(answer == 1){
        load(&trp);
    }
    else {
        if(answer == 2) {
            create(&trp);
        }
        else
            return 0;
    }
    while(rc = dialog(msgs, NMsgs))
        if(!fptr[rc](&trp))
            break;
    printf("That's all. Bye!\n");
    save(&trp);
    if(trp != NULL)
        delTree(&trp);
    return 0;
}

