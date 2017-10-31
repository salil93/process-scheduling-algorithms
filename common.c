#ifndef COMMON_FILE
#define COMMON_FILE

#include <stdio.h>
#include <stdlib.h>

//Structure to store task parameters
typedef struct _task
{
	int id; //ID of the task
	int a; //Arrival time of the task
	int P; //Period of the task
	int e; //Worst case execution time
	int d; //Relative deadline
} task;

typedef struct _ready_node
{ 
	int task_id; // Task/job number
	int priority; // Priority of the task
	int time_left; // Execution time left
	int arrival; // Arrival time (in the Queue)
} ready_node;

//Read the task set from STDIN
int read_taskset(task **taskset)
{
	int N; //Total number of tasks in the system
	//Read the total number of tasks
	scanf("%d",&N);
	// allocate memory
	*taskset = (task*)malloc(sizeof(task) * N);
	task *temp_set = *taskset;
	//read the individual task parameters
	int i = 0;
	int value; // Variable used to read parameter values
	for(i = 0 ; i < N ; i++)
	{

		//ID
		(*taskset)[i].id = i + 1;
		
		//Arrival time
		scanf("%d", &value);
		(*taskset)[i].a = value == -1 ? 0 : value;

		//Period of the task
		scanf("%d", &value);
		(*taskset)[i].P = value;

		//Worst case execution time
		scanf("%d", &value);
		(*taskset)[i].e = value;

		//Relative deadline
		scanf("%d", &value);
		(*taskset)[i].d = value == -1 ? (*taskset)[i].P : value;		

	}

	return N;
}

//Calculate the hyperperiod of a given task set
int hyperperiod(task *taskset, int N)
{
	int lcm = taskset[0].P;
	int i = 0;
	int a, b;
	for(i = 1 ; i < N ; i++)
	{
		a = lcm;
		b = taskset[i].P;
		while(a != b)
		{
			if (a > b)
				a = a - b;
			else
				b = b - a;
		}
		
		lcm = (lcm * taskset[i].P) / a;
	}
	return lcm;
} 

#endif