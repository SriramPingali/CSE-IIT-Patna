#include <bits/stdc++.h>
#define MAX 1000

using namespace std; 

// Declare Frame Size and Page Sequence size
int k, p, temp;

// Declare Page Sequence Vector
vector<int> reference;

// Declare page faults book reading vectors
vector<int> lama_pf;
vector<double> lama_time;
vector<int> fifo_pf;
vector<double> fifo_time;
vector<int> lru_pf;
vector<double> lru_time;

// Utility function to search
// For an element within an array
int search(int* array, int length, int element)
{
	int flag = -1;
	for (int i = 0; i < length; i++)
	{
		if(element == array[i])
		{
			flag = i;
		}
	}
	return flag;
}

// Utility function to simulate 
// fifo memory management
void fifo(int frame_size)
{
	// Dynamic memory for frames
	int* frames = (int*) malloc (frame_size * sizeof(int));
	// Pointer to indicate first in first out element
	int pointer = 0;
	// Book keeping for progress
	int miss = 0, hit = 0;

	printf("FIFO:\n");
	for (int i = 0; i < reference.size(); i++)
	{
		printf("%d ", reference[i]);
	}
	printf("\n");

	// Loop through reference vector 
	// Apply FIFO
	for (int i = 0; i < reference.size(); i++)
	{
		// If element not found in frame => miss
		if(search(frames, frame_size, reference[i]) == -1)
		{
			miss++;
			frames[pointer] = reference[i];
			pointer = (pointer + 1) % frame_size;
		}
		// If element found => Hit
		else
		{
			hit++;
		}
	}
	// Book Keeping
	fifo_pf.push_back(miss);
	printf("#Page Faults: %d\n", miss);
	printf("#Successful Hits: %d\n", hit);
	printf("#Success Ratio: %0.3lf\n", (float)hit / (miss + hit));
	printf("*****************************************\n");
	return;
}

// Utility function to find
// Location of the least used page
void pointer_update_lru(int* pointer, int* frames ,int frame_size, int limit)
{
	// Temp for pointing to farthest page
	int temp = MAX;

	// Loop through frame 
	for(int i = 0; i < frame_size; i++)
	{
		// Search for page in reference string
		int position = search(&reference[0], limit, frames[i]);
		
		// If found, check if farthest using temp
		if(position != -1)
		{
			// If farthest, update pointer
			if(position < temp)
			{
				*pointer = i;
				temp = position;
			}
			// printf("New pointer is %d\n", position);
		}
		else if(position == -1)
		{
			*pointer = i;
			return;
		}
	}
}

// Utility function to simulate 
// LRU memory management
void lru(int frame_size)
{
	int* frames = (int*) malloc (frame_size * sizeof(int));
	
	// Pointer to indicate first in first out element
	int pointer = 0;
	
	// Book keeping for progress
	int miss = 0, hit = 0;

	printf("LRU:\n");
	for (int i = 0; i < reference.size(); ++i)
	{
		printf("%d ", reference[i]);
	}
	cout << endl;

	// Loop through reference vector 
	// Apply Least Recently Used Algo
	for (int i = 0; i < reference.size(); i++)
	{
		// If element not found in frame => miss
		if(search(frames, frame_size, reference[i]) == -1)
		{
			miss++;
			// Calculate index of element in frame to be replaced
			pointer_update_lru(&pointer, frames, frame_size, i);
			frames[pointer] = reference[i];
		}
		// If element found => Hit
		else
		{
			hit++;
		}
	}
	// Book Keeping
	lru_pf.push_back(miss);
	printf("#Page Faults: %d\n", miss);
	printf("#Successful Hits: %d\n", hit);
	printf("#Success Ratio: %0.3lf\n", (float)hit / (miss + hit));
	printf("*****************************************\n");
	return;
}

// Utility function to find
// Location of the least frequent page
void pointer_update_lama(int* pointer, int* frames ,int frame_size, int limit)
{
	// Temp for pointing to least frequent page
	int temp = MAX;

	// Array for Frequency of page
	int freq[frame_size] = {0};

	// Loop through frame 
	for(int i = 0; i < frame_size; i++)
	{
		// Count frequency of occurence
		for (int j = limit; j < reference.size(); j++)
		{
			if(frames[i] == reference[j])
				freq[i]++;
		}
	}

	// Check for least frequent page in frame
	for (int i = 0; i < frame_size; i++)
	{
		// Update pointer for newly found 
		// frequency less than temp
		if(temp > freq[i])
		{
			*pointer = i;
			temp = freq[i];
		}
	}
	return;
}

// Utility function to simulate 
// LAMA memory management
void lama(int frame_size)
{
	int* frames = (int*) malloc (frame_size * sizeof(int));
	// Pointer to indicate first in first out element
	int pointer = 0;
	// Book keeping for progress
	int miss = 0, hit = 0;

	printf("LAMA:\n");
	for (int i = 0; i < reference.size(); ++i)
	{
		printf("%d ", reference[i]);
	}
	cout << endl;
	
	// Loop through reference vector 
	// Apply LAMA
	for (int i = 0; i < reference.size(); i++)
	{
		// If element not found in frame => miss
		if(search(frames, frame_size, reference[i]) == -1)
		{
			miss++;
			// Calculate index of element in frame to be replaced
			pointer_update_lama(&pointer, frames, frame_size, i);
			frames[pointer] = reference[i];
		}
		// If element found => Hit
		else
		{
			hit++;
		}
	}
	// Book Keeping
	lama_pf.push_back(miss);
	printf("#Page Faults: %d\n", miss);
	printf("#Successful Hits: %d\n", hit);
	printf("#Success Ratio: %0.3lf\n", (float)hit / (miss + hit));
	printf("*****************************************\n");
	return;
}

// Driver Code
int main()
{
	// Number of trials
	int n;

	// Variables for time performance
	clock_t start, end;

	cout << "Enter Number of Trials: ";
	cin >> n;

	for (int i = 0; i < n; i++)
	{
		cout << "Enter Number of Frames: ";
		cin >> k;
		cout << "Length of Page Sequence: ";
		cin >> p;

		for(int i = 0; i < p; i++)
		{
			cin >> temp;
			reference.push_back(temp);
		}

		// Execute LAMA Algorithm
		start = clock();
		lama(k);
		end = clock();
		double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
		lama_time.push_back(time_taken);
	   
	    // Execute FIFO Algorithm
		start = clock();
		fifo(k);
		end = clock();
		time_taken = double(end - start)  / double(CLOCKS_PER_SEC);
		fifo_time.push_back(time_taken);

	    // Execute LRU Algorithm
	    start = clock();
		lru(k);
		end = clock();
		time_taken = double(end - start) / double(CLOCKS_PER_SEC);
		lru_time.push_back(time_taken);

	}
	    cout << "Page Faults List for LAMA Algorithm: ";
	    for (int i = 0; i < lama_pf.size(); i++)
	    	cout << lama_pf[i] << " ";
	    cout << endl;

	    cout << "Page Faults List for FIFO Algorithm: ";
	    for (int i = 0; i < fifo_pf.size(); i++)
	    	cout << fifo_pf[i] << " ";
	    cout << endl;

	    cout << "Page Faults List for LRU Algorithm: ";
	    for (int i = 0; i < lru_pf.size(); i++)
	    	cout << lru_pf[i] << " ";
	    cout << endl;

	    cout << "Execution times List for LAMA Algorithm: ";
	    for (int i = 0; i < lama_time.size(); i++)
	    	cout << lama_time[i] << " ";
	    cout << endl;

	    cout << "Execution times List for FIFO Algorithm: ";
	    for (int i = 0; i < fifo_time.size(); i++)
	    	cout << fifo_time[i] << " ";
	    cout << endl;

	    cout << "Execution times List for LRU Algorithm: ";
	    for (int i = 0; i < lru_time.size(); i++)
	    	cout << lru_time[i] << " ";
	    cout << endl;
}