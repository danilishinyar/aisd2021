#include "lab.h"

const char *msgs[] = { "0. Quit", "1. Add vertex", "2. Add edge", "3. Delete vertex", "4. Delete edge", "5. Show", "6. Find vertex (bfs)", "7. Find shortest way", "8. Check acyclicity", "9. Timing", "10. Generate graph (will make random graph instead of yours"};
const int NMsgs = sizeof(msgs) / sizeof(msgs[0]);
int (*fptr[])(Graph** trp) = {NULL, D_Add_vertex, D_Add_edge, D_Delete_vertex, D_Delete_edge, D_Show, D_Find_vertex_bfs, D_Find_shortest_way, D_Check, D_Timing, D_Generate};


int main(){
    int rc, n, answer;
    Graph* graph = NULL;

    printf("Do you want to load tree from file?\n1.Yes\n2.No\n");
    n = getInt(&answer);
    if(n == 0)
        return 0;
    if(answer == 1)
        load(&graph);
    else{
        printf("Enter size of graph: ");
        n = getInt(&N);
        if(n == 0)
            return 0;
        graph = initialize(N);
    }

    while(rc = dialog(msgs, NMsgs))
        if(!fptr[rc](&graph))
            break;
    printf("That's all. Bye!\n");
    save(&graph);
    if(graph != NULL)
        delete_graph(&graph);
    return 0;
}


