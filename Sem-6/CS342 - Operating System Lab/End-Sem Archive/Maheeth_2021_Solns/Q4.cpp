/*
    Name: M Maheeth Reddy
    Roll: 1801CS31

    Operating Systems Lab - Endsem
            Question 4
    
    S-LOOK disk scheduling
*/

#include <bits/stdc++.h>
using namespace std;

// function for S-LOOK scheduling
int S_LOOK(int cylinders, int head, vector<int> workQueue, int seekRate) {
    cout << "\nS-LOOK Algorithm\n";

    // calculate the work queue order
    int thm = 0, n = workQueue.size();
    // sort the work queue in increasing order of cylinder number
    sort(workQueue.begin(), workQueue.end());

    // partition the requests on left and right side of current head
    vector<int> left, right;
    for(int i = 0; i < n; i++) {
        if(workQueue[i] <= head) left.push_back(workQueue[i]);
        if(workQueue[i] > head) right.push_back(workQueue[i]);
    }

    // requests on the closer far-end are serviced first
    // head reverses direction and services requests on farther far-end
    vector<int> order;
    if( abs(left.front()-head) <= abs(right.back()-head) ) {
        // if left far-end is closer than right far-end
        // left side requests are serviced first
        for(int i = left.size()-1; i >= 0; i--) {
            order.push_back(left[i]);
        }

        for(int i = 0; i < right.size(); i++) {
            order.push_back(right[i]);
        }
    } else {
        // if right far-end is closer than left far-end
        // right side requests are serviced first
        for(int i = 0; i < right.size(); i++) {
            order.push_back(right[i]);
        }

        for(int i = left.size()-1; i >= 0; i--) {
            order.push_back(left[i]);
        }
    }

    cout << "Disk head movement:\t";
    n = order.size();
    for(int i = 0; i < n; i++) {
        thm += abs(order[i]-head);
        head = order[i];

        cout << head;
        if(i < n-1) cout << " -> ";
        else cout << "\n";
    }

    // calculate seek time
    int seekTime = thm*seekRate;
    cout << "Total Head movement (THM): " << thm << "\n";
    cout << "Seek Time is " << seekTime << " ms for seek rate of " << seekRate << " ms\n";

    return seekTime;
}

int main() {
    int cylinders;
    cout << "Number of Cylinders: ";
    cin >> cylinders;

    int head;
    cout << "Head position: ";
    cin >> head;

    // scan the work queue as a string
    string request;
    cout << "Disk request: ";
    getline(cin,request);
    getline(cin,request);

    // convert the work queue into vector<int>
    stringstream strin(request);
    vector<int> workQueue;
    int queueSize = 0, temp;
    while(strin >> temp) {
        workQueue.push_back(temp);
        queueSize++;
    }

    int seekRate = 5;   // 5 milliseconds
    int seekTime = S_LOOK(cylinders,head,workQueue,seekRate);
}