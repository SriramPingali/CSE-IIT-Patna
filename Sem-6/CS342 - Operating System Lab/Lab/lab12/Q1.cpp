/*
    Name: M Maheeth Reddy
    Roll: 1801CS31

    Operating Systems Lab - Assignment 12
                Question 1
    
    Simulation of FCFS,SCAN,C-SCAN,SSTF disk scheduling
    and plotting head movements as a 2D graph
*/

#include <bits/stdc++.h>
using namespace std;

// function to plot graph using gnuplot library
void plot(vector<double> &x_cords, vector<double> &y_cords, string name){
    string datafile = name+".data";
    
    vector<string> commands;
    commands.push_back("set title \"" + name + "\"");
    commands.push_back("set xlabel \"Seek time (ms)\"");
    commands.push_back("set ylabel \"Cylinder No.\"");
    
    int pts = x_cords.size();
    for (int i = 0; i < pts; i++) {
        int x = (int)x_cords[i], y = (int)y_cords[i];
        
        string plt = "\"" + to_string(x) + " " + to_string(y) + "\"" ;
        plt = "set label " + plt + " at " +  to_string(x_cords[i]) + "," + to_string(y_cords[i]);
        commands.push_back(plt);
    }
    commands.push_back("plot '"+datafile+"' with linespoints");
    
    FILE* temp = fopen(datafile.c_str(), "w");
    for(int i = 0; i < pts; i++) {
        fprintf(temp, "%d %d\n", (int)x_cords[i], (int)y_cords[i]);
    }

    FILE* gnu_plot_pipe = popen("gnuplot -persistent", "w");
    int cmds = commands.size();
    for(int i = 0; i < cmds; i++) {
        fprintf(gnu_plot_pipe, "%s\n", commands[i].c_str());
    }

    fclose(temp);
    fclose(gnu_plot_pipe);
    
    cout << "See the plot for " << name << " Scheduling...\n";
}

// FCFS scheduling
int FCFS(int cylinders, int head, vector<int> workQueue, int seekRate) {
    cout << "\nFCFS Algorithm\n";
    
    vector<double> x_cords, y_cords;
    x_cords.push_back(0);
    y_cords.push_back(head);

    int thm = 0, n = workQueue.size();
    // service requests in given order
    for(int i = 0; i < n; i++) {
        thm += abs(workQueue[i]-head);
        head = workQueue[i];

        x_cords.push_back(seekRate*thm);
        y_cords.push_back(head);

        cout << head;
        if(i < n-1) cout << " -> ";
        else cout << "\n";
    }

    // calculate seek time
    int seekTime = thm*seekRate;
    cout << "Total Head Movement: " << thm << "\n";
    cout << "Seek Time: " << seekTime << " ms\n";

    plot(x_cords,y_cords,"FCFS");
    return seekTime;
}

// SCAN scheduling
int SCAN(int cylinders, int head, vector<int> workQueue, int seekRate) {
    cout << "\nSCAN Algorithm\n";

    int zeroThere = ( find(workQueue.begin(),workQueue.end(),0) != workQueue.end() );
    if(!zeroThere) workQueue.push_back(0);

    vector<double> x_cords, y_cords;
    x_cords.push_back(0);
    y_cords.push_back(head);

    // calculating work queue order
    int thm = 0, n = workQueue.size();
    // sort the queue in increasing order of cylinder number
    sort(workQueue.begin(), workQueue.end());

    auto left = workQueue.begin();
    // partition the requests on left and right side of current head
    for(auto it = workQueue.begin(); it != workQueue.end(); it++) {
        if(*it < head) left++;
        else break;
    }

    // service requests on the left of head first
    sort(workQueue.begin(), left, greater<int>());

    for(int i = 0; i < n; i++) {
        thm += abs(workQueue[i]-head);
        head = workQueue[i];

        if(head == 0 && zeroThere || head != 0) {
            x_cords.push_back(seekRate*thm);
            y_cords.push_back(head);
        }

        cout << head;
        if(i < n-1) cout << " -> ";
        else cout << "\n";
    }

    // calculate seek time
    int seekTime = thm*seekRate;
    cout << "Total Head Movement: " << thm << "\n";
    cout << "Seek Time: " << seekTime << " ms\n";

    plot(x_cords,y_cords,"SCAN");
    return seekTime;
}

// C-SCAN scheduling
int C_SCAN(int cylinders, int head, vector<int> workQueue, int seekRate) {
    cout << "\nC-SCAN Algorithm\n";

    int isLeftThere = ( find(workQueue.begin(),workQueue.end(),0) != workQueue.end() );
    if(!isLeftThere) workQueue.push_back(0);

    int isRightThere = ( find(workQueue.begin(),workQueue.end(),(const int)(cylinders-1)) != workQueue.end() );
    if(!isRightThere) workQueue.push_back(cylinders-1);

    vector<double> x_cords, y_cords;
    x_cords.push_back(0);
    y_cords.push_back(head);

    // calculating work queue order
    int thm = 0, n = workQueue.size();
    // sort the queue in increasing order of cylinder number
    sort(workQueue.begin(), workQueue.end());

    auto left = workQueue.begin();
    // partition the requests on left and right side of current head
    for(auto it = workQueue.begin(); it != workQueue.end(); it++) {
        if(*it < head) left++;
        else break;
    }

    // service requests on the left of head, go to left end, then go to right end, service requests in the reverse direction
    sort(workQueue.begin(), left, greater<int>());
    sort(left, workQueue.end(), greater<int>());

    for(int i = 0; i < n; i++) {
        thm += abs(workQueue[i]-head);
        head = workQueue[i];

        if(head == 0 && isLeftThere || head == cylinders-1 && isRightThere || (head > 0 && head < (cylinders-1))) {
            x_cords.push_back(seekRate*thm);
            y_cords.push_back(head);
        }

        cout << head;
        if(i < n-1) cout << " -> ";
        else cout << "\n";
    }

    // calculate seek time
    int seekTime = thm*seekRate;
    cout << "Total Head Movement: " << thm << "\n";
    cout << "Seek Time: " << seekTime << " ms\n";

    plot(x_cords,y_cords,"C-SCAN");
    return seekTime;
}

// SSTF scheduling
int SSTF(int cylinders, int head, vector<int> workQueue, int seekRate) {
    cout << "\nSSTF Algorithm\n";

    vector<double> x_cords, y_cords;
    x_cords.push_back(0);
    y_cords.push_back(head);

    // calculate work queue order
    int thm = 0, n = workQueue.size();
    // sort the queue in increasing order of cylinder number
    sort(workQueue.begin(), workQueue.end());

    // partition the requests on left and right side of current head
    int left = 0;
    for(int i = 0; i < n; i++) {
        if(workQueue[i] < head) left++;
        else break;
    }
    left--;
    int right = left+1;

    // between immediate left and immediate right of head, service the request that is nearest to head.
    for(int i = 0; i < n; i++) {
        int leftDiff = abs(workQueue[left]-head);
        int rightDiff = abs(workQueue[right]-head);

        if(leftDiff <= rightDiff) {
            thm += abs(workQueue[left]-head);
            head = workQueue[left];

            x_cords.push_back(seekRate*thm);
            y_cords.push_back(head);
            left--;
        } else {
            thm += abs(workQueue[right]-head);
            head = workQueue[right];

            x_cords.push_back(seekRate*thm);
            y_cords.push_back(head);
            right++;
        }

        cout << head;
        if(i < n-1) cout << " -> ";
        else cout << "\n";
    }

    // calculate seek time
    int seekTime = thm*seekRate;
    cout << "Total Head Movement: " << thm << "\n";
    cout << "Seek Time: " << seekTime << " ms\n";

    plot(x_cords,y_cords,"SSTF");
    return seekTime;
}

// function for sorting algorithms
bool compare(pair<string,int>& a, pair<string,int>& b) {
    return a.second > b.second;
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
    
    // vector to store seek time of each algorithm
    vector< pair<string,int> > seekTimes = {
        {"FCFS",0},
        {"SCAN",0},
        {"C-SCAN",0},
        {"SSTF",0}
    };
    
    seekTimes[0].second = FCFS(cylinders,head,workQueue,seekRate);
    seekTimes[1].second = SCAN(cylinders,head,workQueue,seekRate);
    seekTimes[2].second = C_SCAN(cylinders,head,workQueue,seekRate);
    seekTimes[3].second = SSTF(cylinders,head,workQueue,seekRate);

    // sort the algorithms by seek time: worst to best
    sort(seekTimes.begin(),seekTimes.end(),compare);

    cout << "\nSorted order of algorithms: ";
    for(int i = 0; i < 4; i++) {
        cout << seekTimes[i].first;
        if(i < 3) cout << ", ";
        else cout << "\n";
    }
}