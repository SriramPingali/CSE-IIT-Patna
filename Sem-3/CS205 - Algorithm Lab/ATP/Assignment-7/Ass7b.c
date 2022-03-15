#include <stdio.h>
#define MAX 100
#define NIL -1

struct vertex {
    char col;
    int sno, prev, dt, ft, root;
};

void DFSVisit(int u, struct vertex V[], int adj[][MAX], int n, int* time, int discover[], int* index) {
    V[u].col = 'g';
    *time = *time + 1;
    V[u].dt = *time;
    for(int i = 0; i < n; i++) {
        if(adj[u][i] == 1) {
            if(V[i].col == 'w') {
                V[i].prev = u;
                V[i].root = V[u].root;
                DFSVisit(i,V,adj,n,time,discover,index);
            }
        }
    }
    V[u].col = 'b';
    *time = *time + 1;
    V[u].ft = *time;
    discover[*index] = u;
    *index = *index + 1;
}

void DFS(struct vertex V[], int adj[][MAX], int order[], int n, int discover[]) {
    for(int i = 0; i < n; i++) {
        V[i].col = 'w';
        V[i].prev = NIL;
        V[i].dt = V[i].ft = 0;
        V[i].root = V[i].sno;
    }
    int time = 0, index = 0;
    for(int i = 0; i < n; i++) {
        if(V[order[i]].col == 'w') DFSVisit(order[i],V,adj,n,&time,discover,&index);
    }
}

int main() {
    int n,e;
    scanf("%d %d",&n,&e);
    struct vertex V[n];
    int discover[n], adj[n][MAX], order[n];
    for(int i = 0; i < n; i++) {
        order[i] = V[i].sno = i;
        for(int j = 0; j < n; j++)
            adj[i][j] = 0;
    }
    for(int i = 0; i < e; i++) {
        int a,b;
        scanf("%d %d",&a,&b);
        adj[a-1][b-1] = 1;
    }
    DFS(V,adj,order,n,discover);
    for(int i = 0; i < n/2; i++) {
        int temp = discover[i];
        discover[i] = discover[n-1-i];
        discover[n-1-i] = temp;
    }
    int discover2[n], adj2[n][MAX];
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            adj2[i][j] = adj[j][i];
        }
    }
    DFS(V,adj2,discover,n,discover2);
    int sccCount = 0;
    for(int i = 0; i < n; i++) {
        if(V[i].sno == V[i].root) {
            sccCount++;
        }
    }
    printf("\n%d\n",sccCount);
    int scc = 1;
    for(int i = 0; i < n; i++) {
        if(V[discover2[i]].sno == V[discover2[i]].root) {
            printf("SCC%d: ", scc++);
            for(int j = 0; j < n; j++) {
                if(V[discover2[j]].root == V[discover2[i]].root) printf("%d ",V[discover2[j]].sno+1);
            }
            printf("\n");
        }
    }
}