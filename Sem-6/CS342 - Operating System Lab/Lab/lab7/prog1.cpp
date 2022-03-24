/*
    Name: M Maheeth Reddy
    Roll: 1801CS31

    Operating Systems Lab - Assignment 7
                Question 1
    
    Round Robin (RR) scheduling
*/
#include <bits/stdc++.h>
using namespace std;

// structure to store ID, times for processes: Arrival, Burst, Start, Completion, Turn Around, Waiting
typedef struct ptime {
    int id;
    int at, bt;
    int st = -1, ct = -1;
    int tat = -1, wt = -1;

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
    return (p1.at < p2.at) || (p1.at == p2.at && p1.id < p2.id);
}

int queueFind(queue<int> Q, int s) {
    while( !Q.empty() ) {
		if(Q.front() == s) return 1;

		Q.pop();
	}

    return 0;
}

// check whether all processes are executed or not
int timesEmpty(vector<ptime> &times) {
    for(auto it = times.begin(); it != times.end(); it++) {
        if(it->bt > 0) return 0;
    }
    return 1;
}

// calculate average waiting time and turn around time
void output(vector<ptime> &order) {
    int n = order.size();

    // calculate waiting time and turn around time
    for(int i = 0; i < n; i++) {
        order[i].tat = order[i].ct - order[i].at;
        order[i].wt = order[i].tat - order[i].bt;
    }

    // calculate average waiting time and turn around time
    double avg_tat = 0, avg_wt = 0;
    for(int i = 0; i < n; i++) {
        avg_tat += order[i].tat;
        avg_wt += order[i].wt;
    }
    avg_tat /= n;
    avg_wt /= n;

    // print average waiting time and turn around time
    cout << fixed << setprecision(2) << "\nAvg_WT = " << avg_wt << "\t";
    cout << fixed << setprecision(2) << "Avg_TAT = " << avg_tat << "\n";
    
    for(int i = 0; i < n; i++) {
        cout << "P"+to_string(order[i].id) << " ";
    }
    cout << "\n";
}

// function for round robin scheduling
void RR(vector<ptime> &times, int tq) {
    int n = times.size();
    queue<int> readyQueue;

    vector<int> btimes;
    for(auto it = times.begin(); it != times.end(); it++) {
        btimes.push_back(it->bt);
    }

    // sort processes by arrival times
    sort(times.begin(), times.end(), cmpInitial);

    // associative array to store process ids in order of arrival times
    int pids[n];
    for(int i = 0; i < n; i++) {
        pids[i] = times[i].id;
    }

    for(int i = 0; i < n; i++) {
        times[i].id = i+1;
    }

    // begin round robin scheduling
    readyQueue.push(times[0].id);
    int timer = times[0].at;

    // vector to store order of completion
    vector<ptime> order;

    while(1) {
        if(!readyQueue.empty()) {
            // if ready queue is not empty

            // pop first process from ready queue
            int pid = readyQueue.front();
            
            readyQueue.pop();
            
            // set start time
            if(times[pid-1].st == -1) times[pid-1].st = timer;

            // execute process, and change timer value accordingly
            if(times[pid-1].bt >= tq) {
                times[pid-1].bt -= tq;
                timer += tq;

                // push more processes into ready queue as per timer value
                for(int i = 0; i < times.size(); i++) {
                    if(times[i].at <= timer && times[i].id != pid && !queueFind(readyQueue,times[i].id) && times[i].bt > 0) {
                        readyQueue.push(times[i].id);
                    }
                }

                // push current process into ready queue as per burst time remaining
                if(times[pid-1].bt > 0) {
                    readyQueue.push(pid);
                } else {
                    times[pid-1].ct = timer;

                    ptime temp = times[pid-1];
                    
                    order.push_back(temp);
                    if(timesEmpty(times)) break;
                }
                
            } else {
                timer += times[pid-1].bt;
                
                times[pid-1].bt = 0;
                times[pid-1].ct = timer;
                
                ptime temp = times[pid-1];
                
                order.push_back(temp);
                if(timesEmpty(times)) break;
            }
        } else {
            // if ready queue is empty
            // go to the immediate next arriving process in the input

            int i;
            for(i = 0; i < n; i++) {
                if(times[i].at > timer) break;
            }
            
            readyQueue.push(times[i].id);
            timer = times[i].at;
        }
    }

    // restore burst times
    for(int i = 0; i < order.size(); i++) {
        order[i].bt = btimes[order[i].id-1];
    }

    // restore process ids
    for(int i = 0; i < n; i++) {
        times[i].id = pids[times[i].id-1];
    }

    // print avg turnaround time and avg waiting time
    output(order);
}

int main() {
    int n, tq;
    cin >> n >> tq;

    // vector to store input (process times)
    vector<ptime> times, order;
    for(int i = 0; i < n; i++) {
        int at, bt;
        cin >> at >> bt;
        ptime temp(i+1,at,bt);
        times.push_back(temp);
    }

    // perform round robin scheduling
    RR(times,tq);
}