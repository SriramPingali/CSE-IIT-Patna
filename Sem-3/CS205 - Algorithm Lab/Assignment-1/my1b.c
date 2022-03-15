#include <stdio.h>

struct freq {
	int xfreq;
	int yfreq;
	int zfreq;
};

int storeComb(int tot, struct freq comb[]) {
	int a,b,c,noOfComb = 0;
	for(a = 0; a <= tot; a++) {
		for(b = 0; b <= tot; b++) {
			for(c = 0; c <= tot; c++) {
				if(expr(a,b,c,tot)) {
					printf("a = %d, b = %d, c = %d\n",a,b,c);
					comb[noOfComb] = (struct freq){a,b,c};
					noOfComb++;
					printCombo(a,b,c,tot);
				}
			}
		}
	}
	return noOfComb;
}

int expr(int a, int b, int c, int tot) {
	if(1*a + 2*b + 3*c == tot) return 1;
	return 0;
}

int printCombo(int a, int b, int c, int tot) {
	for(int i = 0; i < a; i++) {
		printf("1 ");
	}
	for(int i = 0; i < b; i++) {
		printf("2 ");
	}
	for(int i = 0; i < c; i++) {
		printf("3 ");
	}
	printf("\n");}

void printAllPermutation(int noOfComb, struct freq comb[]) {
	for(int i = 0; i < noOfComb; i++) {
		printPermutation(comb);
	}
}

void storePermutation(int array[], int length, int noOfElements) {
	if(length == noOfElements) {
		//print
	}


}

void printPermutation(struct freq comb) {
	int noOfElements = 0;
	int length = comb.x + comb.y + comb.z;
	int array[length];
	storePermutation(array,length,noOfElements);
}

int main() {
	int a,b,c,tot;
	struct freq comb[1000]; int noOfComb = 0;
	printf("Total no of steps = ");
	scanf("%d",&tot);
	int noOfComb = storeComb(tot,comb);
}