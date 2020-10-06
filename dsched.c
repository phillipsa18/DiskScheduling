#include <stdio.h>
#include <stdlib.h>
#include <string.h> //I added this to use memset

/* Assume no more than 1000 requests in the input file */
#define BUFSIZE 1000

/* Assume 5000 cylinders */
#define CYL 5000

/* Assume no requests will be longer than 80 characterse */
#define LINELEN 80

//Calculates the total head movement using the First Come First Served algorithm
int fcfs(int initpos, int requests[], int nmemb)
{
	int i;

	int temp = initpos;
	int sum = 0;

	for(i = 0; i < nmemb; ++i)
	{
		temp = temp - requests[i];
		if(temp < 0)
			sum += (temp * -1);
		else
			sum += temp;
		temp = requests[i];
	}
	return sum;
}

//Calculates the total head movement using the Shortest Seek Time First algorithm
int sstf(int initpos, int requests[], int nmemb)
{
	int i, j;
	
	int temp;
	int min;
	int min_val;
	
	int seek_order[nmemb];
	int seek_ind = -1;
	
	int visited[nmemb];
	memset(visited, 0, nmemb * sizeof(int)); //Had to research this, equivalent
						//to {0}, but does not cause warnings.
	/* Similar to a sort, except the minimum seek time is found each iteration
	Seek Time = Head Position - Position in Queue */
	for(i = 0; i < nmemb; ++i)
	{
		min = 0;
		min_val = -1;
		for(j = 0; j < nmemb; ++j)
		{
			//To avoid redunancy among values currently in the seek list
			if(visited[j] == 0)
			{
				temp = initpos - requests[j];
				//Absolute value if request is greater than head position.
				if(temp < 0)
					temp *= -1;
				/*Checks if value is first compared or smaller than 
				the current minimum.*/
				if(temp < min_val || min_val < 0)
				{
					min = j;
					min_val = temp; 
				}
			}
		}
		seek_order[++seek_ind] = requests[min];
		visited[min] = 1;
	}
	//Once it is sorted, logic for finding movement is same as in FCFS
	return fcfs(initpos, seek_order, nmemb);
}

//Calculates the total head movement using the scan algorithm
int scan(int initpos, int requests[], int nmemb)
{
	sort(requests, nmemb);
	//Sum of moving from head to 0 and the highest request in the queue.
	return initpos + requests[nmemb - 1];
}

//Calculates the total head movement using the circular scan algorithm
int cscan(int initpos, int requests[], int nmemb)
{
	/*Finding the values less than the initial head position will allow me
	to easily access the largest value smaller than the current head position.*/
	int l_count = get_less_count(initpos, requests, nmemb);	
	int less[l_count];

	fill_less(less, initpos, requests, nmemb);
	sort(less, l_count);

	//Sum of distance from head to far end of disk, entire disk (move to 0) and the highest value smaller than the disk.
	return (CYL - initpos) + CYL + less[l_count - 1];
}

//Calculates the total head movement using the look algorithm
int look(int initpos, int requests[], int nmemb)
{
	sort(requests, nmemb);
	//Sum of distance from head to lowest position in queue and lowest position in queue to highest position in queue.
	return (initpos - requests[0]) + (requests[nmemb - 1] - requests[0]);
}

//Calculates the total head movement using the circular look algorithm.
int clook(int initpos, int requests[], int nmemb)
{
	int sum = 0;
	int l_count = get_less_count(initpos, requests, nmemb);
	int less[l_count];

	fill_less(less, initpos, requests, nmemb);
	sort(less, l_count);
	sort(requests, nmemb); //Allows me to find the highest request

	/*Sum of the distance between the initial position of the head to the highest position
	position on the queue, the distance between the highest and lowest positions on the
	queue, and the distance between the lowest and highest positions smaller than the head.*/
	sum += requests[nmemb - 1] - initpos; 
	if(sum < 0) //Needed to catch scenarios where the head is greater than the highest position in queue.
		sum *= -1;
	return sum + (requests[nmemb - 1] - less[0]) + (less[l_count - 1] - less[0]);
}

//Calculates the necessary size for the array of positions less than the head
int get_less_count(int initpos, int requests[], int nmemb)
{
	int i;
	int count = 0;
	for(i = 0; i < nmemb; ++i)
	{
		if(requests[i] <= initpos)
			count++;
	}
	return count;
}

//Fills array with positions less than the head
fill_less(int less[], int initpos, int requests[], int nmemb)
{
	int i;
	int l_pos = -1;
	for(i = 0; i < nmemb; ++i)
	{
		if(requests[i] <= initpos)
			less[++l_pos] = requests[i];
	}
}

//Performs a selection sort on the array passed in
sort(int a[], int n)
{
	int i, j, min, temp;
	for(i = 0; i < n - 1; ++i)
	{
		min = i;
		for(j = i; j < n; ++j)
		{
			if(a[min] > a[j])
			{
				min = i;
				temp = a[min];
				a[min] = a[j];
				a[j] = temp;
			}
		}
	}	
}

main(int argc, char* argv[])
{
	char s[LINELEN];
	int requests[BUFSIZE];
	int count;
	int initpos;

	if (argc < 2)
	{
		printf("Usage: ./a.out initpos\n");
		exit(1);
	}

	initpos = atoi(argv[1]);

	count=0;
	while (fgets(s, LINELEN, stdin))
		requests[count++]=atoi(s);

	printf("FCFS: %d\n", fcfs(initpos, requests, count));
	printf("SSTF: %d\n", sstf(initpos, requests, count));
	printf("SCAN: %d\n", scan(initpos, requests, count));
	printf("C-SCAN: %d\n", cscan(initpos, requests, count));
	printf("LOOK: %d\n", look(initpos, requests, count));
	printf("C-LOOK: %d\n", clook(initpos, requests, count));
}
		
		
