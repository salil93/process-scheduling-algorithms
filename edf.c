#include <stdio.h>
#include <stdlib.h>
#include "common.c"

#define STACK_PER_TASK 10
#define MIN_STACK_PER_TASK 3

//Initialize the ready queue.
void initialize_queue(ready_node **ready_queue, int H)
{
	*ready_queue = (ready_node*)malloc(sizeof(ready_node) * H);
	int i = 0;
	for( i = 0 ; i < H; i++)
	{
		(*ready_queue)[i].task_id = -1; //Indicating there is no job arriving at this time
	}
}

//update the ready queue at after every unit of time
int update_queue(task **taskset, ready_node **ready_queue, int ready_length, int t, int N)
{
	int i = 0;
	for( i = 0 ; i < N ; i++)
	{
		//Check if a process has to enter the ready queue
		if ( ( (t - (*taskset)[i].a) % (*taskset)[i].P == 0 ))
		{
			//Add the task to the ready queue
			ready_length++;
			(*ready_queue)[ready_length].task_id = (*taskset)[i].id;
			(*ready_queue)[ready_length].priority = -1 * (t + (*taskset)[i].d);
			(*ready_queue)[ready_length].time_left = (*taskset)[i].e;
			(*ready_queue)[ready_length].arrival = t;
		}
	}

	return ready_length;
}

void print_ready_queue(ready_node *queue, int ready_length)
{
	int i = 0;
	for (i = 0 ; i <= ready_length ; i++)
	{
		printf("ID: %d | Priority: %d | Time Left: %d | Arrival: %d\n", queue[i].task_id, queue[i].priority, queue[i].time_left, queue[i].arrival);
	}
}

void edf_sched(task **taskset, ready_node **ready_queue, int N, int H)
{
	int worst_stack = 0;
	int ready_length = -1;
	int t = 0;
	int i = 0;
	for( t = 0 ; t < H; t++)
	{
		//Update the ready queue
		
		ready_length = update_queue(taskset, ready_queue, ready_length, t, N);

		//printf("---------------------READY QUEUE------------------------\n");
		//print_ready_queue(*ready_queue, ready_length);

		int index = -1; //Index of the task that has to run
		int task_id = -1; //ID of the task that has to run
		int max_priority = -100; //Store the priority of the selected task
		int current_stack = 0;

		//Select a suitable process from the ready queue
		for( i = 0 ; i <= ready_length ; i++)
		{
			if ( (*ready_queue)[i].task_id != -1 )
			{
				if ((*ready_queue)[i].priority > max_priority)
				{	
					index = i;
					task_id = (*ready_queue)[i].task_id;
					max_priority = (*ready_queue)[i].priority;
				}
			}
		}

		//Decrease the time left for the selected task
	    (*ready_queue)[index].time_left--;

		//Calculating the current stack space required
		for( i = 0 ; i <= ready_length ; i++)
		{
			if ( (*ready_queue)[i].task_id != -1 )
			{
				if ((*ready_queue)[i].time_left < (*taskset)[(*ready_queue)[i].task_id - 1].e)
					current_stack += STACK_PER_TASK;
				else
					current_stack += MIN_STACK_PER_TASK;
			}
		}			

		//Update the task in the ready queue (i.e update the time left)
		if (current_stack > worst_stack)
			worst_stack = current_stack;
		
		//Remove the task from the ready queue if it is completed
		if ( (*ready_queue)[index].time_left == 0 )
			(*ready_queue)[index].task_id = -1;

		//Print the schedule
		if (task_id == -1)
			printf("Time: %d ----> IDLE | Stack Space: %d\n", t, task_id, current_stack);
		else
			printf("Time: %d ----> Task: %d | Stack Space: %d\n", t, task_id, current_stack);
	}
	printf("Worst Case Stack Space needed: %d frames\n", worst_stack);
}

void main()
{
	//Read the edf task set
	task *taskset; 
	int N = read_taskset(&taskset);
	printf("Total tasks in the system: %d\n", N);

	//Calculate the hyperperiod of the task set
	int H = hyperperiod(taskset, N);
	printf("Hyperperiod: %d\n", H);
	
	//Initialize the ready queue
	ready_node *ready_queue;
	initialize_queue(&ready_queue, H);

	//Schedule the tasks
	edf_sched(&taskset, &ready_queue, N, H);	
}