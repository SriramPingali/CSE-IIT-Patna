#include <stdio.h>
#include <string.h>
#include <ctype.h>

struct Patient {
	char name[20];
	float time;
};

int strcomp(struct Patient s1, char s2[]) {
    int i,j,count=0;
    for(i=0;i<strlen(s1.name);i++)
        for(j=0;j<strlen(s2);j++)
            if(tolower(s1.name[i])==tolower(s2[j]))
                count++;
    return count;
}

void merge(struct Patient arr[], int l, int m, int r, char doctor[]) { 
    int i, j, k; 
    int n1 = m - l + 1; 
    int n2 =  r - m; 
    struct Patient L[n1], R[n2]; 
    for(i = 0; i < n1; i++) L[i] = arr[l+i]; 
    for(j = 0; j < n2; j++) R[j] = arr[m+1+j]; 
    i = 0;j = 0; k = l;
    while (i < n1 && j < n2) { 
        if (strcomp(L[i], doctor) > strcomp(R[j], doctor)) { 
            arr[k] = L[i]; 
            i++; 
        } else if(strcomp(L[i], doctor) == strcomp(R[j], doctor)) {
            if(L[i].time <= R[j].time) {
                arr[k] = L[i]; 
                i++;
            } else {
                arr[k] = R[j];
                j++;
            }
        } else { 
            arr[k] = R[j]; 
            j++; 
        } 
        k++; 
    } 
    while (i < n1) { 
        arr[k] = L[i]; 
        i++; 
        k++; 
    } 
    while (j < n2) { 
        arr[k] = R[j]; 
        j++; 
        k++; 
    } 
} 
  
void mergeSort(struct Patient arr[], int l, int r, char doctor[]) { 
    if (l < r) { 
        int m = l+(r-l)/2;
        mergeSort(arr,l,m,doctor); 
        mergeSort(arr,m+1,r,doctor);
        merge(arr,l,m,r,doctor); 
    } 
} 

struct Patient nextPatient(struct Patient list[], int curPatients, int start, char doctor[]) {
	mergeSort(list, start, curPatients, doctor);
	return list[start];
} 

int main() {
	int total, num, isNewPatient, numNewPatients, start = 0, numPatientsNow;
	char doctor[20];
	printf("Doctor's name: ");
	scanf("%s", doctor);
	printf("Enter total number of patients: ");
	scanf("%d", &total);
	printf("Enter number of patients who came prior to 8 am:");
	scanf("%d", &num);
	numPatientsNow = num;
	struct Patient list[total];
	for (int i = 0; i < num; ++i) {
		scanf("%s", list[i].name);
		scanf("%f", &list[i].time);
	}
	nextPatient(list, numPatientsNow, start, doctor);
	start++;
	while(total - numPatientsNow != 0) {
		printf("Any new patient?\n");
		scanf("%d", &isNewPatient);
		if(isNewPatient) {
			printf("How many new patients?\n");
			scanf("%d", &numNewPatients);
			for (int i = 0; i < numNewPatients; ++i) {
				scanf("%s", list[numPatientsNow + i].name);
				scanf("%f", &list[numPatientsNow + i].time);
			}
			numPatientsNow += numNewPatients;
			nextPatient(list, numPatientsNow, start, doctor);
			start++;
		}
		else {
			nextPatient(list, numPatientsNow, start, doctor);
			start++;
		}
	}
	printf("\nOrder is:\n");
	for (int i = 0; i < total; ++i) {
		printf("%s, %.2f\n", list[i].name,list[i].time);
	}
}