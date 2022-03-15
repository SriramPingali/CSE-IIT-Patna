#include <stdio.h> 
#define MAX 100

void transform(int *x, int *y, int n) {
    int temp = *x;
    *x = n-1-*y;
    *y = temp;
}

void print(int move) {
    switch(move) {
        case 0:
            printf("U ");
            break;
        case 1:
            printf("NE ");
            break;
        case 2:
            printf("R ");
            break;
        case 3:
            printf("SE ");
            break;
        case 4:
            printf("D ");
            break;
        case 5:
            printf("SW ");
            break;
        case 6:
            printf("L ");
            break;
        case 7:
            printf("NW ");
            break;
    }
}

int inMaze(int maze[][MAX], int x, int y, int n) { 
    if (x >= 0 && x < n && y >= 0 && y < n && maze[x][y] == 1) 
        return 1;
    return 0;
} 

int validMove(int maze[][MAX], int x, int y, int sol[][MAX], int x1, int y1, int n, int moves[], int index) { 
    if(x == x1 && y == y1) { 
        sol[x][y] = 1;
        for(int i = 0; i < index; i++)
            print(moves[i]);
        return 1;
    }
  
    if(inMaze(maze,x,y,n) == 1) { 
        sol[x][y] = 1;
        if(inMaze(maze,x-1,y,n) && sol[x-1][y] != 1) {
            moves[index] = 0;
            if(validMove(maze,x-1,y,sol,x1,y1,n,moves,1+index) == 1) {
                return 1; 
            }
        }
        if(inMaze(maze,x-1,y+1,n) && sol[x-1][y+1] != 1) {
            moves[index] = 1;
            if(validMove(maze,x-1,y+1,sol,x1,y1,n,moves,1+index) == 1) {
                return 1; 
            }
        }
        if(inMaze(maze,x,y+1,n) && sol[x][y+1] != 1) {
            moves[index] = 2;
            if(validMove(maze,x,y+1,sol,x1,y1,n,moves,1+index) == 1) {
                return 1; 
            }
        }
        if(inMaze(maze,x+1,y+1,n) && sol[x+1][y+1] != 1) {
             moves[index] = 3;
            if(validMove(maze,x+1,y+1,sol,x1,y1,n,moves,1+index) == 1) {
                return 1; 
            }
        }
        if(inMaze(maze,x+1,y,n) && sol[x+1][y] != 1) {
            moves[index] = 4;
            if(validMove(maze,x+1,y,sol,x1,y1,n,moves,1+index) == 1) {
                return 1; 
            }
        }
        if(inMaze(maze,x+1,y-1,n) && sol[x+1][y-1] != 1) {
            moves[index] = 5;
            if(validMove(maze,x+1,y-1,sol,x1,y1,n,moves,1+index) == 1) {
                return 1; 
            }
        }
        if(inMaze(maze,x,y-1,n) && sol[x][y-1] != 1) {
            moves[index] = 6;
            if(validMove(maze,x,y-1,sol,x1,y1,n,moves,1+index) == 1) {
                return 1; 
            }
        }
        if(inMaze(maze,x-1,y-1,n) && sol[x-1][y-1] != 1) {
            moves[index] = 7;
            if(validMove(maze,x-1,y-1,sol,x1,y1,n,moves,1+index) == 1) { 
                return 1;
            }
        }
        sol[x][y] = 0; 
        return 0; 
    }
    return 0; 
} 
  
int findPath(int maze[][MAX], int sx, int sy, int kx, int ky, int dx, int dy, int n, int moves[], int index) {
    int sol[n][MAX];
    for(int i = 0; i < n; i++) for(int j = 0; j < n; j++) sol[i][j] = 0;
    if(validMove(maze,sx,sy,sol,kx,ky,n,moves,index) == 0) {
        printf("Solution doesn't exist\n");
        return 0;
    }
    if(validMove(maze,kx,ky,sol,dx,dy,n,moves,index) == 0) {
        printf("Solution doesn't exist\n");
        return 0;
    }
    printf("\n");
    return 1;
}

int main() { 
    int n,sx,sy,kx,ky,dx,dy;
    printf("Enter the number of rows = ");
    scanf("%d",&n);
    printf("Enter Source = ");
    scanf("%d %d",&sx,&sy);
    printf("Enter destination = ");
    scanf("%d %d",&dx,&dy);
    printf("Enter Key cell = ");
    scanf("%d %d",&kx,&ky);
    printf("Rows: %d\n",n);
    printf("Source: (%d,%d)\n",sx,sy);
    printf("Destination: (%d,%d)\n",dx,dy);
    printf("Key: (%d,%d)\n",kx,ky);
    transform(&sx,&sy,n);
    transform(&kx,&ky,n);
    transform(&dx,&dy,n);
    int maze[n][MAX],moves[MAX],index = 0;
    printf("Input Maze:\n");
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            scanf("%d",&maze[i][j]);
        }
    }
    maze[kx][ky] = 1;
    printf("Directions are: ");
    findPath(maze,sx,sy,kx,ky,dx,dy,n,moves,index);
    return 0;
}