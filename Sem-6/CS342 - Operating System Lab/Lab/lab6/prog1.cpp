/*
    Assignment 6

    Name: M. Maheeth Reddy
    Roll: 1801CS31

    First Come First Serve (FCFS) scheduling
*/
#include <bits/stdc++.h>
using namespace std;

// structure to store ID, times for processes: Arrival, Burst, Start, Completion, Turn Around, Waiting
typedef struct ptime {
    int id;
    int at,bt;
    int st = 0, ct = 0;
    int tat = 0, wt = 0;

    ptime() {}

    ptime(int id, int at, int bt) {
        this->id = id;
        this->at = at;
        this->bt = bt;
    }
} ptime;

// comparision function
bool cmp(ptime p1, ptime p2) {
    return (p1.at < p2.at) || (p1.at == p2.at && p1.id < p2.id);
}

// First Come First Serve scheduling function
void fcfs(vector<ptime> &times) {
    int n = times.size();
    
    // sort processes in increasing order of arrival time
    sort(times.begin(), times.end(), cmp);

    // perform fcfs
    times[0].st = times[0].at;
    times[0].ct = times[0].st + times[0].bt;
    
    for(int i = 1; i < n; i++) {
        times[i].st = times[i-1].ct;
        times[i].ct = times[i].st + times[i].bt;
    }

    // calculate Turn Around and Waiting Times
    for(int i = 0; i < n; i++) {
        times[i].tat = times[i].ct - times[i].at;
        times[i].wt = times[i].tat - times[i].bt;
    }

    // calculate Average Turn Around and Average Waiting Times
    double avg_tat = 0, avg_wt = 0;
    for(int i = 0; i < n; i++) {
        avg_tat += times[i].tat;
        avg_wt += times[i].wt;
    }
    avg_tat /= n;
    avg_wt /= n;

    // Print Avg. Waiting Time, Avg. Turn Around Time, process-ids in the order of completion
    cout << fixed << setprecision(2) << avg_wt << " " << avg_tat << "\n";
    for(int i = 0; i < n; i++) {
        cout << "P"+to_string(times[i].id) << " ";
    }
    cout << "\n";
}

int main() {
    int n;
    cin >> n;

    // vector to store all times
    vector<ptime> times;
    for(int i = 0; i < n; i++) {
        int at, bt;
        cin >> at >> bt;
        ptime temp(i+1,at,bt);
        times.push_back(temp);
    }
    
    fcfs(times);
}