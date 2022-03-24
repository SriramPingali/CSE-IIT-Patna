/*
    Name: M Maheeth Reddy
    Roll: 1801CS31

    Operating Systems Lab - Endsem
                Question 2
    
    Implement custom paging scheme and compare with FIFO, LRU, LIFO
*/

#include <bits/stdc++.h>
using namespace std;

// check if an element is present in an array
int isPresent(int arr[], int size, int elt) {
    for(int i = 0; i < size; i++) {
        if(arr[i] == elt) return 1;
    }
    return 0;
}

// print the current contents of frame
void printFrame(int frame[], int cap, int time) {
    for(int i = 0; i < cap; i++) {
        if(frame[i] == 0) cout << "X";
        else cout << frame[i];

        cout << "\t";
    }
    cout << "at t = " << time << "\n";
}

void pop_front(vector<int> &v) {
    if (v.size() > 0) {
        int n = v.size();
        for(int i = 1; i < n; i++) {
            v[i-1] = v[i];
        }
        v.pop_back();
    }
}

// helper function for LRU algorithm to find index for next replacement by
// going through previous page requests
int findIndex(int frame[], int cap, vector<int> pgnums, int cur) {
    int nextIndex = 0, nearest[cap];

    for(int i = 0; i < cap; i++) {
        int index = 0;

        for(int j = cur-1; j >= 0; j--) {
            if(pgnums[j] == frame[i]) break;
            index++;
        }

        nearest[i] = index;
    }

    for(int i = 0; i < cap; i++) {
        if(nearest[nextIndex] < nearest[i]) nextIndex = i;
    }

    return nextIndex;
}

// FIFO algorithm
void FIFO(int cap, vector<int> pgnums, int *pf) {
    // frame
    int frame[cap];

    // index at which replacement needs to be done
    int firstIndex = 0;
    
    // count pagefaults
    int pageFaults = 0;
    
    // count time
    int time = 0;
    
    // initially frame is empty
    bzero(frame,sizeof(frame));

    for(int i = 0; i < pgnums.size(); i++) {
        // replace first-in page with next page
        int nextPage = pgnums[i];
        if(!isPresent(frame,cap,nextPage)) {
            frame[firstIndex] = nextPage;
            
            firstIndex++;
            firstIndex %= cap;

            pageFaults++;
        }

        time++;
    }

    *pf = pageFaults;
}

// LRU algorithm
void LRU(int cap, vector<int> pgnums, int *pf) {
    // frame, count of pagefaults, count of time
    int frame[cap], pageFaults = 0, time = 0;
    
    // initially frame is empty
    bzero(frame,sizeof(frame));

    // if frame size < capacity, fill it with page numbers
    int initialIndex = 0;
    for(int i = 0; i < pgnums.size(); i++) {
        int nextPage = pgnums[i];
        
        if(!isPresent(frame,cap,nextPage)) {
            frame[initialIndex++] = nextPage;
            pageFaults++;
        }
        
        time++;

        if(initialIndex == cap) break;
    }
    
    // for further page nos, replacement should be done if necessary
    for(int i = initialIndex; i < pgnums.size(); i++) {
        // replace least recently used page with next page
        int nextPage = pgnums[i];
        int nextIndex = findIndex(frame,cap,pgnums,i);

        if(!isPresent(frame,cap,nextPage)) {
            frame[nextIndex] = nextPage;
            pageFaults++;
        }

        time++;
    }
    *pf = pageFaults;
}

// my own algorithm (described in README)
void ownAlgo(int cap, vector<int> pgnums, int *pf) {
    // frame, count of pagefaults, count of time
    int frame[cap], pageFaults = 0, time = 0;
    
    // initially frame is empty
    bzero(frame,sizeof(frame));

    // Algorithm goes here...
    for(int i = 0; i < pgnums.size(); i++) {
        // if page fault occurs, retrieve the adjacent distinct pages
        int nextPage = pgnums[i];
        if(!isPresent(frame,cap,nextPage)) {
            int j = 0, k = i;

            int tempFrame[cap];
            for(int p = 0; p < cap; p++) tempFrame[p] = 0;

            while(j < cap && k < pgnums.size()) {
                if(!isPresent(tempFrame,cap,pgnums[k])) {
                    tempFrame[j] = pgnums[k];
                    j++;
                }
                k++;
            }

            for(int p = 0; p <= j; p++) {
                frame[p] = tempFrame[p];
            }

            pageFaults++;
        }

        time++;
    }
    *pf = pageFaults;
}

int main() {
    // store page faults and times in seperate vectors for each algorithm
    vector<int> fifoPF, lruPF, ownalgoPF;
    vector<double> fifoTimes, lruTimes, ownalgoTimes;

    // no. of trials
    int n;
    cout << "No. of Trials: ";
    cin >> n;
    
    for(int i = 0; i < n; i++) {
        cout << "\nInputs for Trial " << (i+1) << ":\n";
        cout << "--------------------------\n";
        // get frame capacity and length of page sequence of each trial
        int k,p;
        cout << "No. of Frames: ";
        cin >> k;
        cout << "Length of page sequence: ";
        cin >> p;
        
        // get page numbers
        cout << "Enter Page Sequence: ";
        vector<int> pgnums;
        for(int i = 0; i < p; i++) {
            int x;
            cin >> x;

            pgnums.push_back(x);
        }
        
        int pf,timetaken;

        clock_t start,end;
        double time_taken;

        start = clock();
        FIFO(k,pgnums,&pf);
        end = clock();
        fifoPF.push_back(pf);
        time_taken = double(end-start);
        fifoTimes.push_back(time_taken);

        start = clock();
        LRU(k,pgnums,&pf);
        end = clock();
        lruPF.push_back(pf);
        time_taken = double(end-start);
        lruTimes.push_back(time_taken);

        start = clock();
        ownAlgo(k,pgnums,&pf);
        end = clock();
        ownalgoPF.push_back(pf);
        time_taken = double(end-start);
        ownalgoTimes.push_back(time_taken);
    }

    cout << "\nOutput:\n-------\n";
    cout << "\nPage faults for various trials:\n";
    cout << "A:\t";
    for(int i = 0; i < ownalgoPF.size(); i++) {
        cout << "Trial " << (i+1) << ": " << ownalgoPF[i] << "\t\t";
    }
    cout << "\n";

    cout << "LRU:\t";
    for(int i = 0; i < lruPF.size(); i++) {
        cout << "Trial " << (i+1) << ": " << lruPF[i] << "\t\t";
    }
    cout << "\n";

    cout << "FCFS:\t";
    for(int i = 0; i < fifoPF.size(); i++) {
        cout << "Trial " << (i+1) << ": " << fifoPF[i] << "\t\t";
    }
    cout << "\n";

    double totalOwn = 0, totalLRU = 0, totalFifo = 0;
    cout << "\nTime taken for various trials:\n";
    for(int i = 0; i < n; i++) {
        cout << "Trial " << (i+1) << ": ";
        cout << "own algorithm: " << ownalgoTimes[i] << " us\t\t";
        cout << "LRU: " << lruTimes[i] << " us\t\t";
        cout << "FIFO: " << fifoTimes[i] << " us\n";

        totalOwn += ownalgoTimes[i];
        totalLRU += lruTimes[i];
        totalFifo += fifoTimes[i];
    }
    
    cout << "\nTotal time for all trials by my own algorithm = " << totalOwn << " us\n";
    cout << "Total time for all trials by LRU algorithm = " << totalLRU << " us\n";
    cout << "Total time for all trials by FIFO algorithm = " << totalFifo << " us\n";
}