#include <stdio.h>
//#define MAX 100

struct job {
	int sno,l,d;
};

void swap(struct job* j1, struct job* j2) {
	struct job temp = *j1;
	*j1 = *j2;
	*j2 = temp;
}

float dlratio(struct job j) {
    return (j.d)*1.0/j.l;
}

void mergeSortedSubarrays(struct job arr[], int start, int mid, int end) {
    struct job temp[end]; 
    int index = start;
    for(int i = start, j = mid; ( i<mid || j<end ); index++) {
        if(i<mid && j<end) {
            if(dlratio(arr[i]) > dlratio(arr[j])) {
                temp[index] = arr[j];
                j++;
            } else {
                temp[index] = arr[i];
                i++;
            }
        } else {
            if(i < mid) { temp[index] = arr[i]; i++; }
            else { temp[index] = arr[j]; j++; }
        }
        //index++;
    }
    for(int i = start; i < end; i++) {
        arr[i] = temp[i];
    }
}

void mergesort(struct job arr[], int start, int end) {
    if(end == start + 1) return;
    int mid = (start+end)/2;
    mergesort(arr, start, mid);
    mergesort(arr, mid, end);
    mergeSortedSubarrays(arr, start, mid, end);
}

int main() {
	int n;
	printf("Give no. of jobs: ");
	scanf("%d",&n);
	struct job j[n];
	printf("Give each job's loss/hr\n");
	for(int i = 0; i < n; i++) {
		j[i].sno = i+1;
		scanf("%d",&j[i].l);
	}
	printf("Give each job's duration\n");
	for(int i = 0; i < n; i++) scanf("%d",&j[i].d);
	int index = 0;
	mergesort(j,0,n);
	printf("\n");
	for(int i = 0; i < n; i++) {
		printf("%d",j[i].sno);
		if(i == n-1) printf("\n");
		else printf(", ");
	}
}