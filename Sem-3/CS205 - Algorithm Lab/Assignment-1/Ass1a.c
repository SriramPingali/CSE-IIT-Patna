#include <stdio.h>

int choose(int noEV, int noCh) {
	int poss = 0;
	if(noEV <= 0 || noCh == 0) return 0;	//basecase1

	if(noCh == 1) {												//basecase2
		poss += noEV;
		printf("%d\n",noEV);
	} else {
		for(int vno = 1; vno <= noEV; vno++) {
			for(int vne = vno + 2; vne <= noEV; vne++) {
				printf("%d %d\n", vno, vne);
			}
			poss += choose(noEV-1-vno,noCh-1);
		}
	}
	printf("-----\n");
	return poss;
}

int main() {
	int noEV;
	printf("Enter a number\n");	//no of EV's
	scanf("%d",&noEV);
	int totPoss = 0;	//total no of possible groups
	for(int i = 0; i <= noEV; i++) {
		totPoss += choose(noEV,i);
		//printf("choose(%d,%d) = %d\n",noEV,i,choose(noEV,i));
	}
	printf("Number of ways a compatible project group can be chosen is: %d\n",totPoss);
}
