#include"lab.h"


const char *msgs[] = {"0.Quit", "1.Add", "2.Find", "3.Delete", "4.Show"};
const int NMsgs = sizeof(msgs) / sizeof(msgs[0]);

int (*fptr[])(Table* ptb) = {NULL, D_Add, D_Find,  D_Del, D_Show};


int main(){
    int rc, msize1, msize2, n, answer = 0;
    Table *tb;
    printf("Do you want to load table from file?\n1.Yes\n2.No\n");
    n = getInt(&answer);
    if(n == 0)
        return 0;
    if(answer == 1){
        tb = load();
    }
    else {
        if(answer == 2) {
            printf("Enter msize1: ");
            n = getInt(&msize1);
            if (n == 0)
                return 0;
            printf("Enter msize1: ");
            n = getInt(&msize2);
            if (n == 0)
                return 0;
        }
        else
            return 0;
        tb = initialize(msize1, msize2);
    }
    while(rc = dialog(msgs, NMsgs))
        if(!fptr[rc](tb))
            break;
    save(tb);
    D_Del_Table(tb);
    printf("That's all. Bye!\n");
    return 0;
}

