/*
    Name: M Maheeth Reddy
    Roll: 1801CS31

    Operating Systems Lab - Assignment 10
                Question 2
    
    Least Recently Used (LRU) paging scheme
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

// function to find index for next replacement by
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

int main() {
    // open input file "pages.txt"
    ifstream finput;
    finput.open("pages.txt");
    if(!finput) {
        cout << "Can\'t open file \"pages.txt\"\n";
        return 0;
    }
    
    cout << "LRU:\n\n";

    // maintain count of testcases
    int testcase = 1;
    while(finput) {
        if(finput.eof()) break;

        // print testcase
        cout << "Testcase " << testcase << ":\n";

        // get frame capacity
        int cap;
        finput >> cap;
        cout << "Capacity: " << cap << "\n";
        
        // get page numbers
        vector<int> pgnums;
        while(1) {
            int x;
            finput >> x;

            if(x == -1) break;
            cout << x << " ";

            pgnums.push_back(x);
        }
        cout << "\n\n";

        // frame, count of pagefaults, count of time
        int frame[cap], pageFaults = 0, time = 0;
        
        // initially frame is empty
        bzero(frame,sizeof(frame));

        cout << "Frame content at each time step t\n";
        for(int i = 1; i <= cap; i++) cout << "F" << i << "\t";
        cout << "\n";

        // if frame size < capacity, fill it with page numbers
        int initialIndex = 0;
        for(int i = 0; i < pgnums.size(); i++) {
            printFrame(frame,cap,time);
            
            int nextPage = pgnums[i];
            
            frame[initialIndex++] = nextPage;
            pageFaults++;
            
            time++;

            if(initialIndex == cap) break;
        }
        
        // for further page nos, replacement should be done if necessary
        for(int i = initialIndex; i < pgnums.size(); i++) {
            // print the current contents of frame
            printFrame(frame,cap,time);
            
            // replace least recently used page with next page
            int nextPage = pgnums[i];
            int nextIndex = findIndex(frame,cap,pgnums,i);

            if(!isPresent(frame,cap,nextPage)) {
                frame[nextIndex] = nextPage;
                pageFaults++;
            }

            time++;
        }
        printFrame(frame,cap,time);

        cout << "\n#Page Faults: " << pageFaults << "\n";
        cout << "---------------\n\n";

        testcase++;
    }
}