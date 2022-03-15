#include <bits/stdc++.h>
#define INF 99999
using namespace std;

struct Edge {
	int u,v,w;
};

struct Region {
	string name;
	char col;
};

int comm(string s1, string s2) {
    int freq1[26] = { 0 }; 
    int freq2[26] = { 0 }; 
    int i, count = 0; 
    for (i = 0; i < s1.length(); i++) freq1[s1[i]-'a']++;
  	for (i = 0; i < s2.length(); i++) freq2[s2[i]-'a']++;
    for (i = 0; i < 26; i++)
        count += (min(freq1[i],freq2[i]));
    return count;
}

int main() {
	int n;
	cout << "Number of regions: ";
	cin >> n;
	vector<Edge> edge;
	Region region[n];
	cout << "Enter r->RED b->BLUE g->GREEN\n";
	for(int i = 0; i < n; i++) {
		cin >> region[i].name >> region[i].col;
	}
	for(int i = 0; i < n; i++) {
		for(int j = i+1; j < n; j++) {
			string s1 = region[i].name;
			string s2 = region[j].name;
			int com = comm(s1,s2);
			int cost = s1.length() + s2.length();
			if(com >= 2) {
				if(s1[com-1] < s2[com-1]) {
					if(region[i].col=='g' && region[j].col=='b') cost *= 2;
					else if(region[i].col == region[j].col) cost *= -1;
					edge.push_back((Edge){i,j,cost});
				} else if(s1[com-1] > s2[com-1]) {
					if(region[i].col=='b' && region[j].col=='g') cost *= 2;
					else if(region[i].col == region[j].col) cost *= -1;
					edge.push_back((Edge){j,i,cost});
				} else {
					if(s1.length() > s2.length()) {
						if(region[i].col=='g' && region[j].col=='b') cost *= 2;
						else if(region[i].col == region[j].col) cost *= -1;
						edge.push_back((Edge){i,j,cost});
					} else /*(if(s1.length() < s2.length())*/ {
						if(region[i].col=='b' && region[j].col=='g') cost *= 2;
						else if(region[i].col == region[j].col) cost *= -1;
						edge.push_back((Edge){j,i,cost});
					}
				}
			}
		}
	}
	for(Edge e : edge) {
		cout << region[e.u].name << " " << region[e.v].name << " " << e.w << endl;
	}
	vector<int> dist(n,INF), prev(n, -1);
	dist[0] = 0;
  	int lastV;
  	for(int i = 0; i < n; i++) {
		lastV = -1;
      	for (Edge e : edge) {
			if(dist[e.u] + e.w < dist[e.v]) {
            	dist[e.v] = dist[e.u] + e.w;
              	prev[e.v] = e.u;
              	lastV = e.v;
          	}
      	}
  	}
  	if(lastV == -1) {
      	cout << "There is no negative cycle\n";
  	} else {
      	for(int i = 0; i < n; i++) lastV = prev[lastV];
      	vector<int> cycle;
      	for (int v = lastV; ; v = prev[v]) {
			cycle.push_back(v);
			if(v == lastV && cycle.size() > 1) break;
      	}
		cout << endl;
		for(int i = cycle.size()-1; i >= 0; i--) {
			cout << region[cycle[i]].name;
			if(i==0) cout << endl;
			else cout << "->";
		}
    }
}

/*
DABE g
FDABCZ g
CBA g
ANABE b
AT r

DABE g
CBA g
ANABE g
FDABCZ b
AT r

ALPHA b
BETAAM g
GRAMAD r
DELTA b
EPSILON g
*/