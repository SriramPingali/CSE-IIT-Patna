#include<bits/stdc++.h>
using namespace std;

// Structure for a process
struct process
{
	int sno;
   int arr_time;
	int burst_time;
   int remaining_burst;
   int start_time;
	int compl_time;
	char pid;
   char status;
};

struct processor
{
	bool occupied;
	char hold_sno;
};

// Declare number of 
// processes, time quantum
int n, tq1, tq2, time_limit;

// Utility function for sorting 
// process according to arrival time
bool compare2process(process p1,process p2)
{
	return p1.arr_time < p2.arr_time;
}

// Function to emulate Round Robin scheduling algorithm
int simulation(process* p, int n)
{
   	// Indicator variable
    int index = 0, t1 = 0, t2 = 0, t = 0;
   	int count = 0;

   	queue <process> ready_queue;
   	processor proc[2];

   	for(int i = 0; i < 2; i++)
   	{
    	proc[i].occupied = 0; 
   	}
    
   	while(t < time_limit)
   	{
   		while(p[index].arr_time <= t && index <= n)
         {
   			// cout << index << " ";
            ready_queue.push(p[index++]);
         }
         // cout << endl;

   		if(proc[0].occupied == 0)
   		{
            if(ready_queue.size() >= 1)
            {
                proc[0].occupied = 1;
      			proc[0].hold_sno = ready_queue.front().sno;
      			ready_queue.pop();
                if(p[proc[0].hold_sno].start_time == -1)
                    p[proc[0].hold_sno].start_time = t;
                p[proc[0].hold_sno].status = 'E';
      			// cout << "Proc 1 " << p[proc[0].hold_sno].pid <<  " at t = " << t << " Remaining: " << p[proc[0].hold_sno].remaining_burst << endl;
            }
   		}
   		if(proc[1].occupied == 0)
   		{
            if(ready_queue.size() >= 1)
            {
      			proc[1].occupied = 1;
      			proc[1].hold_sno = ready_queue.front().sno;
      			ready_queue.pop();
                if(p[proc[1].hold_sno].start_time == -1)
                    p[proc[1].hold_sno].start_time = t;
                p[proc[1].hold_sno].status = 'E';
      			// cout << "Proc 2 " << p[proc[1].hold_sno].pid <<  " at t = " << t << " Remaining: " << p[proc[1].hold_sno].remaining_burst << endl;
            }
   		}

   		if(t1 % tq1 == tq1 - 1)
   		{
   			p[proc[0].hold_sno].remaining_burst--;
   			if(p[proc[0].hold_sno].remaining_burst == 0)
            {
   				p[proc[0].hold_sno].compl_time = t + 1;
               p[proc[0].hold_sno].status = 'C';
            }
   			else if(p[proc[0].hold_sno].remaining_burst > 0)
            {
   				ready_queue.push(p[proc[0].hold_sno]);
               p[proc[0].hold_sno].status = 'W';
            }

   			proc[0].occupied = 0;
   			t1 = 0;
   		}
   		else if(t1 % tq1 != tq1 - 1)
   		{
   			t1++;
   			p[proc[0].hold_sno].remaining_burst--;
   			if(p[proc[0].hold_sno].remaining_burst == 0)
   			{
   				p[proc[0].hold_sno].compl_time = t + 1;
               p[proc[0].hold_sno].status = 'C';
   				proc[0].occupied = 0;
   				t1 = 0;
   			}
   		}

   		if(t2 % tq2 == tq2 - 1)
   		{
   			p[proc[1].hold_sno].remaining_burst--;
   			if(p[proc[1].hold_sno].remaining_burst == 0)
            {
   				p[proc[1].hold_sno].compl_time = t + 1;
               p[proc[1].hold_sno].status = 'C';
            }
   			else if(p[proc[1].hold_sno].remaining_burst > 0)
            {
   				ready_queue.push(p[proc[1].hold_sno]);
               p[proc[1].hold_sno].status = 'W';
            }

   			proc[1].occupied = 0;
   			t2 = 0;
   		}
   		else if(t2 % tq2 != tq2 - 1)
   		{
   			t2++;
   			p[proc[1].hold_sno].remaining_burst--;
   			if(p[proc[1].hold_sno].remaining_burst == 0)
   			{
   				p[proc[1].hold_sno].compl_time = t + 1;
               p[proc[1].hold_sno].status = 'C';
   				proc[1].occupied = 0;
   				t2 = 0;
   			}
   		}

   		t++;
   	}
   
   	return(0);
}

// Driver code
int main(){

   	cout << "Enter Number of Processes: ";
   	cin >> n;

   	cout << "Enter Time Quantum 1: ";
   	cin >> tq1;

   	cout << "Enter Time Quantum 2: ";
   	cin >> tq2;

   	// Array of processes
   	process p[n];

   	// Boolean array for 
   	// process completed or not
   	bool is_compl[n];

   	// Array for remaining burst time
   	// int remaining_burst[n];
   	memset(is_compl, false, sizeof(is_compl));
   	cout << setprecision(2) << fixed;
   
   	// Read data regarding 
   	// the processes
   	cout << "Enter List of Processes (Arrival Time, Burst Time):" << endl;
   	for(int i = 0; i < n; i++)
   	{
       	p[i].compl_time = -1; 
         p[i].start_time = -1; 
      	cin >> p[i].pid >> p[i].arr_time >> p[i].burst_time;
      	p[i].remaining_burst = p[i].burst_time;
   	}

      // Sort acccording to arrival time first
      sort(p, p + n, compare2process);

      for(int i = 0; i < n; i++)
      {
         p[i].sno = i; 
      }

   	cout << "Enter Time Limit: " << endl;
   	cin >> time_limit;

   	// Execute scheduling process
   	simulation(p, n);

      cout << "Job ID\tArrival Time\tExecution Time\tStart Time\tEnd Time\tCurrent  Status" << endl;
   	for(int i = 0; i < n; i++)
   	{
   		cout << p[i].pid << "\t" << p[i].arr_time << "\t\t" << 
                  p[i].burst_time << "\t\t" << p[i].start_time << "\t\t" <<
                  p[i].compl_time << "\t\t" << p[i].status << endl;
   	}
   	cout << endl;
}

