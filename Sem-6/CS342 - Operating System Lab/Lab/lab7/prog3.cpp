/*
    Name: M Maheeth Reddy
    Roll: 1801CS31

    Operating Systems Lab - Assignment 7
                Question 3
    
    Multilevel Queue (MLQ) scheduling
*/
#include <bits/stdc++.h>
using namespace std;

// structure to store ID, Queue No., times for processes: Arrival, Burst, Start, Completion, Turn Around, Waiting
typedef struct ptime {
    int id;
    int at, bt;
    int st = -1, ct = -1;
    int tat = -1, wt = -1;
    int pq;

    ptime() {}

    ptime(int id, int at, int bt, int pq) {
        this->id = id;
        this->at = at;
        this->bt = bt;
        this->pq = pq;
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

// check whether a process is in corresponding ready queue
int queueFind(queue<int> Q, queue<int> Q2, ptime s) {
    if(s.pq == 1) {
        while( !Q.empty() ) {
            if(Q.front() == s.id) return 1;

            Q.pop();
        }
        return 0;
    }

    if(s.pq == 2) {
        while( !Q2.empty() ) {
            if(Q2.front() == s.id) return 1;

            Q2.pop();
        }
        return 0;
    }
}

// check whether are all processes executed or not
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

// push process to corresponding ready queue
void pushQueue(ptime var, queue<int> &readyQueue, queue<int> &readyQueue2) {
    if(var.pq == 1) readyQueue.push(var.id);
    if(var.pq == 2) readyQueue2.push(var.id);
}

// function for multilevel queue scheduling
void MLQ(vector<ptime> &times) {
    int n = times.size();
    int tq = 4, tq2 = 3;
    queue<int> readyQueue, readyQueue2;

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

    // begin multilevel queue scheduling
    pushQueue(times[0], readyQueue, readyQueue2);
    int timer = times[0].at;

    // vector to store order of completion
    vector<ptime> order;

    while(1) {
        if(!readyQueue.empty()) {
            // ready queue 1 is not empty
            // pop process from ready queue 1

            int pid = readyQueue.front();
            
            readyQueue.pop();
            
            // update start time
            if(times[pid-1].st == -1) times[pid-1].st = timer;

            // execute process and change timer value accordingly
            if(times[pid-1].bt >= tq) {
                times[pid-1].bt -= tq;
                timer += tq;

                // push more processes into ready queue as per timer value
                for(int i = 0; i < times.size(); i++) {
                    if(times[i].at <= timer && times[i].id != pid && !queueFind(readyQueue,readyQueue2,times[i]) && times[i].bt > 0) {
                        pushQueue(times[i],readyQueue,readyQueue2);
                    }
                }

                // push current process into ready queue as per burst time remaining
                if(times[pid-1].bt > 0) {
                    pushQueue(times[pid-1],readyQueue,readyQueue2);
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

                for(int i = 0; i < times.size(); i++) {
                    if(times[i].at <= timer && times[i].id != pid && !queueFind(readyQueue,readyQueue2,times[i]) && times[i].bt > 0) {
                        pushQueue(times[i],readyQueue,readyQueue2);
                    }
                }
                
                order.push_back(temp);
                if(timesEmpty(times)) break;
            }
        }
        else if(!readyQueue2.empty()) {
            // if ready queue 1 is empty and ready queue 2 is not empty

            // pop first process from ready queue 2
            int pid = readyQueue2.front();
            
            readyQueue2.pop();
            
            // set start time
            if(times[pid-1].st == -1) times[pid-1].st = timer;

            // execute process, and change timer value accordingly
            if(times[pid-1].bt >= tq2) {
                times[pid-1].bt -= tq2;
                timer += tq2;

                // push more processes into ready queue as per timer value
                for(int i = 0; i < times.size(); i++) {
                    if(times[i].at <= timer && times[i].id != pid && !queueFind(readyQueue,readyQueue2,times[i]) && times[i].bt > 0) {
                        pushQueue(times[i],readyQueue,readyQueue2);
                    }
                }

                // push current process into ready queue as per burst time remaining
                if(times[pid-1].bt > 0) {
                    pushQueue(times[pid-1],readyQueue,readyQueue2);
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

                for(int i = 0; i < times.size(); i++) {
                    if(times[i].at <= timer && times[i].id != pid && !queueFind(readyQueue,readyQueue2,times[i]) && times[i].bt > 0) {
                        pushQueue(times[i],readyQueue,readyQueue2);
                    }
                }
                
                order.push_back(temp);
                if(timesEmpty(times)) break;
            }
        }
        else {
            // if both ready queues are empty
            // go to the immediate next arriving process in the input

            int i;
            for(i = 0; i < n; i++) {
                if(times[i].at > timer) break;
            }
            
            pushQueue(times[i],readyQueue,readyQueue2);
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
    int n, nosq;
    cin >> n >> nosq;

    // vector to store input (process times)
    vector<ptime> times, order;
    for(int i = 0; i < n; i++) {
        int at, bt, pq;
        cin >> at >> bt >> pq;
        ptime temp(i+1,at,bt,pq);
        times.push_back(temp);
    }

    // perform multilevel queue scheduling
    MLQ(times);
}