/*
    Name: M Maheeth Reddy
    Roll: 1801CS31

    Operating Systems Lab - Assignment 10
                Question 1
    
    First-in-First-out (FIFO) paging scheme
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

int main() {
    // open input file "pages.txt"
    ifstream finput;
    finput.open("pages.txt");
    if(!finput) {
        cout << "Can\'t open file \"pages.txt\"\n";
        return 0;
    }
    
    cout << "FIFO:\n\n";

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

        cout << "Frame content at each time step t\n";
        for(int i = 1; i <= cap; i++) cout << "F" << i << "\t";
        cout << "\n";

        for(int i = 0; i < pgnums.size(); i++) {
            // print the current contents of frame
            printFrame(frame,cap,time);
            
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
        printFrame(frame,cap,time);

        cout << "\n#Page Faults: " << pageFaults << "\n";
        cout << "---------------\n\n";

        testcase++;
    }
}