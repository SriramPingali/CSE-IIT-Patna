#include <bits/stdc++.h>
// #include <cstdlib>
// #include <vector>
using namespace std;

int main() {
	int n,M,T = 0;
	cout << "Enter number of houses (n) : ";
	cin >> n;
	cout << "Enter profit bound (M) : ";
	cin >> M;
	int P[n];
	cout << "Profits are ";
	for(int i = 0; i < n; i++) {
		P[i] = rand()%M + 1;
		T += P[i];
		cout << P[i] << " ";
	}
	cout << '\n';

	int A[n][T+1];
	for(int i = 0; i < n; i++) {
		for(int j = 0; j < T+1; j++) {
			A[i][j] = 0;
		}
	}

	for(int j = 0; j < T+1; j++) {
		if(P[0] == j) A[0][j] = 1;
	}
	
	for(int i = 1; i < n; i++) {
		for(int j = 0; j < T+1; j++) {
			if(A[i-1][j] != 0) A[i][j] = 1;
			else if(j-P[i]>=0 && A[i-1][j-P[i]]!= 0) A[i][j] = 1;
		}
	}

	// cout << "DP Array\n";
	// for(int i = 0; i < n; i++) {
	// 	for(int j = 0; j < T+1; j++) {
	// 		cout << A[i][j] << " ";
	// 	}
	// 	cout << '\n';
	// }

	int temp = 2*T/3;
	int nearest;
	for(int j = 0; j < T+1; j++) {
		if(A[n-1][j] == 1) {
			nearest = j;
			break;
		}
	}
	for(int j = 0; j < T+1; j++) {
		if(A[n-1][j] == 1 && abs(temp-j) < abs(temp-nearest)) {
			nearest = j;
			//break;
		}
	}
	//cout << nearest << " " << A[n-1][nearest] << " " << temp << " "<< A[n-1][temp] << endl;

	int s1 = nearest;
	int j = s1,i = n-1;
	vector<int> s1h;
	while(j!=0) {
		while(A[i-1][j] == 1) i--;
		s1h.push_back(i);
		j -= P[i];
	}

	int profit1 = 0;
	for(int k = 0; k < s1h.size(); k++) profit1 += P[s1h[k]];
	cout << "Profit for first son : " << profit1 << endl;

	int flag[n] = { 0 };
	for(int k = 0; k < n; k++) {
		for (int p = 0; p < s1h.size(); p++) {
			if(k == s1h[p]) flag[k] = 1;
		}
	}

	int profit2 = 0;
	for(int t = 0; t < n; t++) {
		if(flag[t] == 0) profit2 += P[t];
	}
	cout << "Profit for second son : " << profit2 << endl;

	cout << "Imbalance (S1-2*S2): " << profit1-2*profit2 << endl;

	cout << "First son gets houses ";
	for(int k = 0; k < s1h.size(); k++) cout << s1h[k] << " ";
	cout << '\n';

	cout << "Second son gets houses ";
	for(int t = 0; t < n; t++) {
		if(flag[t] == 0) cout << t << " ";
	}
	cout << '\n';
}