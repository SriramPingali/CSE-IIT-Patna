#include <bits/stdc++.h>
using namespace std;

int min(int a, int b) {
    if(a < b) return a;
    return b;
}

int main() {
    string s1,s2;
    cout << "Enter gene1= ";
    cin >> s1;
    cout << "Enter gene2= ";
    cin >> s2;
    int len1 = s1.length(), len2 = s2.length();
    int D[len1+1][len2+1];
    for(int i = 0; i <= len1; i++) {
        D[i][0] = i;
    }
    for(int j = 0; j <= len2; j++) {
        D[0][j] = j;
    }
    for(int i = 1; i <= len1; i++) {
        for(int j = 1; j <= len2; j++) {
            if(s1[i-1] == s2[j-1]) D[i][j] = D[i-1][j-1];
            else D[i][j] = 1+min(D[i-1][j],min(D[i][j-1],D[i-1][j-1]));
        }
    }
    cout << "Similarity:" << len1+len2-D[len1][len2] << endl;
    cout << "Total number of operations required =" << D[len1][len2] << endl;
}