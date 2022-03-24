/*
    Name: M Maheeth Reddy
    Roll: 1801CS31

    Operating Systems Lab - Endsem
            Question 3
*/
#include <iostream>
#include <queue>
#include <string>
#include <algorithm>
using namespace std;

// structure to store ID, times for processes: Arrival, Burst, Start, Completion and remaining burst time
typedef struct ptime {
    string job_id;
    int sno;
    int at, bt, rbt;
    int st = -1, ct = -1;
    char status;

    ptime() {}

    ptime(string job_id, int sno, int at, int bt) {
        this->job_id = job_id;
        this->sno = sno;
        this->at = at;
        this->bt = bt;
        this->rbt = bt;
    }
} ptime;

// structure for processor
typedef struct processor {
    int timeSlice;     // time slice
    int exec = 0;      // processor is executing or not
    int curSlice = 0;  // time elapsed in current slice
    int curPID = -1;   // 
} processor;
processor procs[2];

// comparator function to sort processes by arrival time
int sortbyArrival(ptime p1, ptime p2) {
    return (p1.at < p2.at);
}

// function to schedule the processes
int scheduling(ptime _jobs[], int numJobs, int instant) {
    ptime jobs[numJobs];
    for(int i = 0; i < numJobs; i++) {
        jobs[i] = _jobs[i];
    }

    // sort processes by arrival time
    sort(jobs,jobs+numJobs,sortbyArrival);
    
    queue<int> readyQueue;
    int timer;      // global timer for both processors

    for(timer = 0; timer < instant; timer++) {
        // push arriving processes into ready queue
        for(int i = 0; i < numJobs; i++) {
            if(jobs[i].at == timer) {
                readyQueue.push(i);
            }
        }

        // for loop ensures processor 1 is preferred over processor 2
        for(int i = 0; i < 2; i++) {
            // if processor is free, assign a process
            if(procs[i].exec == 0) {
                if(readyQueue.size() >= 1) {
                    procs[i].exec = 1;

                    procs[i].curPID = readyQueue.front();
                    readyQueue.pop();
                    
                    int curPID = procs[i].curPID;
                    if(jobs[curPID].st == -1) jobs[curPID].st = timer;
                    jobs[curPID].status = 'E';
                }
            }
            // if processor is not free, and slice is finishing...
            if(procs[i].curSlice % procs[i].timeSlice != procs[i].timeSlice-1) {
                int curPID = procs[i].curPID;
                procs[i].curSlice++;

                jobs[curPID].rbt--;
                // and current process has been executed, then mark the process as complete
                if(jobs[curPID].rbt == 0) {
                    jobs[curPID].ct = timer+1;
                    procs[i].exec = 0;
                    jobs[curPID].status = 'C';
                    procs[i].curSlice = 0;
                }
            } else {
                // if current process is still running, then append it to ready queue
                int curPID = procs[i].curPID;
                jobs[curPID].rbt--;
                if(jobs[curPID].rbt == 0) {
                    jobs[curPID].ct = timer+1;
                    jobs[curPID].status = 'C';
                } else if(jobs[curPID].rbt > 0) {
                    readyQueue.push(curPID);
                    jobs[curPID].status = 'W';
                }

                procs[i].exec = 0;
                procs[i].curSlice = 0;
            }
        }
    }

    // print output
    cout << "Output:\n";
    cout << "job_id\tarrival_time\tExecution time\tstart_time\tend_time\tcurrent_status\n";
    for(int i = 0; i < numJobs; i++) {
        cout << jobs[i].job_id << "\t" << jobs[i].at << "\t\t" << jobs[i].bt << "\t\t" << jobs[i].st << "\t\t" << jobs[i].ct << "\t\t" << jobs[i].status << "\n";
    }
    cout << "Note: if end_time = -1, then process is not yet completed\n";
}

int main() {
    // get inputs
    int d1,d2,numJobs,instant;
    cout << "Time slice for processor 1 (d1): ";
    cin >> d1;
    cout << "Time slice for processor 2 (d2): ";
    cin >> d2;
    cout << "Number of Jobs: ";
    cin >> numJobs;
    
    procs[0].timeSlice = d1;
    procs[1].timeSlice = d2;

    ptime jobs[numJobs];
    cout << "Enter job id, arrival time and execution time for each job:\n";
    for(int i = 0; i < numJobs; i++) {
        string job_id;
        int at,bt;
        cin >> job_id;
        cin >> at;
        cin >> bt;
        ptime temp(job_id,i+1,at,bt);
        jobs[i] = temp;
    }
    
    cout << "Time instant: ";
    cin >> instant;
    scheduling(jobs,numJobs,instant);

}