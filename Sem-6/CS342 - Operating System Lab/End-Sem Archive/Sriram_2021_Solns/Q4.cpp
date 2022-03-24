#include <bits/stdc++.h>

using namespace std;

// Book keeping vectors 
// for all algorithms
int count_ = 0;
int slookup_thm = 0;
vector<double> slookup;
vector<double> slookup_time;

// Utility function to simulate SLOOKUP
void SLOOKUP(int current, vector<int> requests, int flag)
{
    // If single request left, execute it
    if(requests.size() == 1)
    {
        slookup.push_back(requests[0]);
        return;
    }

    // Iterator to find index of element
    vector<int>::iterator it = std::find(requests.begin(), requests.end(), current);
    int index = it - requests.begin();

    // If an end reached, reverse
    if(index == 0 || index == requests.size() - 1)
    {
        if(count_ == 0)
            flag = (flag + 1) % 2;
        count_++;
    }
    
    // Move left or right according to flag
    if(flag == 0)
        current = requests[index - 1];
    else if(flag == 1)
        current = requests[index + 1];

    //Update log books
    cout << "Next Position: " << current << endl;
    slookup_thm += abs(requests[index] - current);
    slookup.push_back(requests[index]);
    slookup_time.push_back(slookup_thm * 5);
    requests.erase(it);

    SLOOKUP(current, requests, flag);
}

// Driver code
int main()
{
    int req;
    int temp;
    int size = 200;
    int current;
    vector<int> requests;

    // No. of requests
    cout << "Enter number of requests" << endl;
    cin >> req;

    // Current Head
    cout << "Enter current head location" << endl;
    cin >> current;

    // Work queue
    cout << "Enter the work queue" << endl;
    for (int i = 0; i < req; i++)
    {
        cin >> temp;
        requests.push_back(temp);
    }
    if (std::find(requests.begin(), requests.end(), current) == requests.end())
    	requests.insert(requests.begin(), current);

    // Determine the direction
    int flag;
    sort(requests.begin(), requests.end());
    if(abs(current - requests[0]) <= abs(current - requests[requests.size() - 1]))
        flag = 0;
    else
        flag = 1;

    // Perform SLOOKUP
    cout << "S-LOOKUP Algorithm" << endl;
    slookup_time.push_back(0);
    cout << "Execution Order:" << endl << "Starting From: " << current << endl;
    SLOOKUP(current, requests, flag);
    requests.pop_back();
    cout << "Head Movement: " << slookup_thm << " Seek Time: " << slookup_thm * 5 << "ms" << endl;
}