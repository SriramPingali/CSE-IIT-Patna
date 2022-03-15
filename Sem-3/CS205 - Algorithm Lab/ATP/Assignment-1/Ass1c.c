#include <stdio.h>

struct point {
	int x;
	int y;
};

const struct point left = {-1,0};
const struct point down = {0,-1};
const struct point end = {1,1};

void ordinal(int x) {
	switch(x) {
		case 1:
			printf("%dst",x);
			break;
		case 2:
			printf("%dnd",x);
			break;
		case 3:
			printf("%drd",x);
			break;
		default:
			printf("%dth",x);
	}
}

void display(struct point p) {
	printf("Intersection of "); ordinal(p.y); printf(" Street and "); ordinal(p.x); printf(" Avenue");
}

struct point move(struct point pos, char dir) {
	if(dir == 'l') return (struct point){(pos.x + left.x),(pos.y + left.y)};
	if(dir == 'd') return (struct point){(pos.x + down.x),(pos.y + down.y)};
}

int equality(struct point p1, struct point p2) {
	if(p1.x == p2.x && p1.y == p2.y) return 1;
	/*else*/ return 0;
}

void printMove(char move) {
	if(move == 'l') printf("left");
	if(move == 'd') printf("down");
}

int numPaths(struct point start, char moves[], int moveIndex, struct point current) {
	if(equality(start,end)) return 0;
	if(current.x < end.x || current.y < end.y) return 0;

	if(equality(current,end)) {
		for(int i = 0; i < moveIndex; i++) {
			if(i == 0) printf("Move ");
			else printf("then ");
			printMove(moves[i]);
			if(i != moveIndex-1) printf(", ");
			else printf("\n");
		}
		return 1;
	}

	int noOfPaths = 0;

	moves[moveIndex] = 'l';
	noOfPaths += numPaths(start,moves,moveIndex+1,move(current,'l'));

	moves[moveIndex] = 'd';
	noOfPaths += numPaths(start,moves,moveIndex+1,move(current,'d'));

	return noOfPaths;
}

int main() {
	struct point start;
	printf("Enter co-ordinates of starting point: ");
	scanf("%d %d", &start.x, &start.y);

	int movesInAPath = (start.x-end.x)+(start.y-end.y);
	char moves[movesInAPath];
	for(int i = 0; i < movesInAPath; i++) moves[i] = 0;
	int moveIndex = 0;

	printf("\nStart: "); display(start); printf("\nEnd: "); display(end); printf("\n\n");
	int totalPaths = numPaths(start,moves,moveIndex,start);
	printf("\nTotal number of paths = %d\n", totalPaths);
}