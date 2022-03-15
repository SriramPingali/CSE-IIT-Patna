#include <stdio.h>
#define MAX 1000
#define INF 10000
#define NIL -1

struct vertex {
    char col;
    int d, prev;
};

struct queue {
    int mem[MAX], front, end;
};

void enqueue(struct queue *Q, int new) {
    Q->mem[Q->end] = new;
    Q->end++;
}

int dequeue(struct queue *Q) {
    int top = Q->mem[Q->front];
    Q->mem[Q->front] = NIL;
    Q->front++;
    return top;
}

int peek(struct queue *Q) {
    return Q->mem[Q->front];
}

int isEmpty(struct queue *Q) {
    if(Q->front >= Q->end) return 1;
    return 0;
}

void display(struct queue *Q) {
    if(Q->front < Q->end) {
        for(int i = Q->front; i < Q->end; i++) {
            printf("%d",Q->mem[i]);
            if(i != Q->end-1) printf(", ");
            else printf("\n");
        }
    }
}

void BFS(struct vertex V[], int adjMatrix[][MAX], int source, int n, int discover[]) {
    //printf("BFS begins\n");
    int index = 0;
    for(int i = 0; i < n; i++) {
        if(i != source) {
            V[i].col = 'w';
            V[i].d = INF;
            V[i].prev = NIL;
        }
    }
    V[source].col = 'g';
    V[source].d = 0;
    V[source].prev = NIL;
    struct queue Q;
    Q.front = 0;
    Q.end = 0;
    enqueue(&Q,source);
    while(!isEmpty(&Q)) {
        int u = dequeue(&Q);
        for(int i = 0; i < n; i++) {
            //printf("adj(%d,%d) = %d ",u,i,adjMatrix[u][i]);
            if(adjMatrix[u][i] == 1) {
                //printf("%d: col:%c\n",i,V[i].col);
                if(V[i].col == 'w') {
                    V[i].col = 'g';
                    V[i].d = V[u].d + 1;
                    V[i].prev = u;
                    enqueue(&Q,i);
                }
            } else {
                //printf("\n");
            }
        }
        V[u].col = 'b';
        discover[index++] = u;
        //display(&Q);
    }
}

int winner(int adjMatrix[][MAX], int I, int J, int n) {
    struct vertex V[n];
    int discover[n];

    BFS(V,adjMatrix,I,n,discover);
    if(V[J].d < INF) return I;
    
    BFS(V,adjMatrix,J,n,discover);
    if(V[I].d < INF) return J;

    return (I<J)?I:J;
}

int main() {
    int n,e,q;
    printf("No of players: ");
    scanf("%d",&n);
    printf("No of known results: ");
    scanf("%d",&e);
    int query[q];
    int adjMatrix[n][MAX];
    for(int i = 0; i < n; i++) for(int j = 0; j < n; j++) adjMatrix[i][j] = 0;
    printf("Give known results:\n");
    for(int i = 0; i < e; i++) {
        int a,b;
        scanf("%d %d",&a,&b);
        adjMatrix[b][a] = 1;
    }
    printf("No of queries: ");
    scanf("%d",&q);
    printf("Give Queries:\n");
    for(int i = 0; i < q; i++) {
        int I,J;
        scanf("%d %d",&I,&J);
        query[i] = winner(adjMatrix,I,J,n);
    }
    for(int i = 0; i < q; i++) {
        printf("%d",query[i]);
        if(i != q-1) printf(" ");
        else printf("\n");
    }
}