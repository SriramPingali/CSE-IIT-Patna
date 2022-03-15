#include <bits/stdc++.h>
#define INF 99999
using namespace std;

struct Edge {
    int u,v,w;
};

vector<int> h;

bool bellmanFord(vector<Edge> edge, int n, int s) {
    vector<int> prev(n,-1), dist(n+1,INF);
    dist[s] = 0;
    for(int i = 0; i < n-1; i++) {
        for (Edge e : edge) {
			if(dist[e.u] + e.w < dist[e.v]) {
            	dist[e.v] = dist[e.u] + e.w;
              	prev[e.v] = e.u;
          	}
      	}
    }
    for(Edge e : edge) {
		if(dist[e.u] + e.w < dist[e.v]) {
          	return false;
       	}
    }
    for(int d : dist) {
        h.push_back(d);
    }
    return true;
}

int dijkstra(vector<Edge> edge, int n, int source) {
    int adj[n][n], cost[n][n], dist[n], pred[n], visited[n], mindist;
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            adj[i][j] = 0;
        }
    }
    for(Edge e : edge) {
        adj[e.u][e.v] = e.w;
    }
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            if(adj[i][j] == 0) cost[i][j] = INF;
            else cost[i][j] = adj[i][j];
        }
    }
    for(int i = 0; i < n; i++) {
        dist[i] = cost[source][i];
        pred[i] = source;
        visited[i] = 0;
    }
    dist[source] = 0;
    visited[source] = 1;
    for(int count = 0; count < n-1; count++) {
        mindist = INF;
        int u;
        for(int i = 0; i < n; i++){
            if(dist[i]<mindist && !visited[i]) {
                mindist = dist[i];
                u = i;
            }
        }
        visited[u] = 1;
        for(int i = 0; i < n; i++) {
            if(!visited[i]) {
                if(mindist+cost[u][i] < dist[i]) {
                    dist[i] = mindist + cost[u][i];
                    pred[i] = u;
                }
            }
        }
    }
    int sum = 0;
    for(int i = 0; i < n; i++) {
        if(i!=source) sum += dist[i];
    }
    return sum;
}

int johnson(vector<Edge> edge, int n) {
    vector<Edge> edge1(edge);
    for(int i = 0; i < n; i++) {
        edge1.push_back((Edge){n,i,0});
    }
    if(bellmanFord(edge1,n+1,n) == false) {
        cout << "The input graph contains a negative-weight cycle\n";
        return -1;
    } else {
        for(Edge e : edge1) {
            e.w = e.w + h[e.u] - h[e.v];
        }
    }
    int sum[n], minIndex = 0;
    for(int i = 0; i < n; i++) {
        sum[i] = dijkstra(edge,n,i);
        minIndex = (sum[i] < sum[minIndex]) ? i : minIndex;
    }
    return minIndex;
}

int main() {
    cout << "Start Indices from \'0\'\n";
    int n,e;
    cout << "No. of Spaceports: ";
    cin >> n;
    cout << "No. of Flights: ";
    cin >> e;
    cout << "Give Flights' Details:\n";
    vector<Edge> edge;
    for(int i = 0; i < e; i++) {
        int u,v,w;
        cin >> u >> v >> w;
        edge.push_back((Edge){u,v,w});
    }
    int result = johnson(edge,n);
    if(result != -1) {
        cout << "Headquarters should be at Spaceport " << result << endl;
    }
}

/*
5 9
0 1 3
0 2 8
0 4 -4
1 3 1
1 4 7
2 1 4
3 2 -5
3 0 2
4 3 6
*/