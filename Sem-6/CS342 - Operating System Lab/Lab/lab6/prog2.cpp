/*
    Assignment 6

    Name: M. Maheeth Reddy
    Roll: 1801CS31
    
    Shortest Job First (SJF) scheduling
*/
#include <bits/stdc++.h>
using namespace std;

// structure to store ID, times for processes: Arrival, Burst, Start, Completion, Turn Around, Waiting
typedef struct ptime {
    int id;
    int at, bt;
    int st = 0, ct = 0;
    int tat = 0, wt = 0;

    ptime() {}

    ptime(int id, int at, int bt) {
        this->id = id;
        this->at = at;
        this->bt = bt;
    }
} ptime;

// function to pop element from front of vector
void pop_front(vector<ptime> &v) {
    if (v.size() > 0) {
        int n = v.size();
        for(int i = 1; i < n; i++) {
            v[i-1] = v[i];
        }
        v.pop_back();
    }
}

// comparision function to sort by arrival times
bool cmpInitial(ptime p1, ptime p2) {
    return (p1.at < p2.at) || (p1.at == p2.at && p1.bt < p2.bt) || (p1.at == p2.at && p1.bt == p2.bt && p1.id < p2.id);
}

// comparision function to sort by burst times
bool cmp(ptime p1, ptime p2) {
    return (p1.bt < p2.bt) || (p1.bt == p2.bt && p1.id < p2.id);
}

// function for shortest job first scheduling
void sjf(vector<ptime> &times) {
    int n = times.size();

    // sort processes by arrival times
    sort(times.begin(), times.end(), cmpInitial);

    // begin shortest job first scheduling
    int timer = times[0].at;
    times[0].st = times[0].at;
    times[0].ct = times[0].st + times[0].bt;
    timer = times[0].ct;

    // vector to store order of completion
    vector<ptime> order;
    while(1) {
        ptime temp = times[0];
        pop_front(times);
        order.push_back(temp);
        if(times.empty()) break;

        int i;
        for(i = 0; (times[i].at <= timer) && i < times.size(); i++);
        sort( times.begin(), next(times.begin(),i), cmp );

        times[0].st = timer;
        times[0].ct = times[0].st + times[0].bt;
        timer = times[0].ct;
    }

    for(int i = 0; i < n; i++) {
        times.push_back(order[i]);
    }

    // calculate waiting time and turn around time
    for(int i = 0; i < n; i++) {
        times[i].tat = times[i].ct - times[i].at;
        times[i].wt = times[i].tat - times[i].bt;
    }

    // calculate average waiting time and turn around time
    double avg_tat = 0, avg_wt = 0;
    for(int i = 0; i < n; i++) {
        avg_tat += times[i].tat;
        avg_wt += times[i].wt;
    }
    avg_tat /= n;
    avg_wt /= n;

    // print average waiting time and turn around time
    cout << fixed << setprecision(2) << avg_wt << " " << avg_tat << "\n";
    for(int i = 0; i < n; i++) {
        cout << "P"+to_string(times[i].id) << " ";
    }
    cout << "\n";
}

int main() {
    int n;
    cin >> n;

    // vector to store input (process times)
    vector<ptime> times;
    for(int i = 0; i < n; i++) {
        int at, bt;
        cin >> at >> bt;
        ptime temp(i+1,at,bt);
        times.push_back(temp);
    }

    sjf(times);
}