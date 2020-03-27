//Name: schedule_fcfs.c
//Project: Assignment 6 CSCI 485
//Authors: Brayden Faulkner and Hayden Nanny
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "schedulers.h"
#include "list.h"
#include "task.h"
#include "cpu.h"

//Keeps track of how many items are in the list
int listIndex = 0;
//Stores the head of the list
struct node *head = NULL;
/*
 *Name: add
 *Purpose: Adds task to the list
 *Parameters:
 *	char *name: Pointer that stores the name of the task
 *	int priority: Stores the priority of the task
 *	int burst: Stores how many burst long the task is
*/
void add(char *name, int priority, int burst){
	//Temp variable that creates and stores the task being added to the list
	struct task *temp = malloc(sizeof(struct task));
	temp->name = name;
	temp->tid = listIndex;
	temp->priority = priority;
	temp->burst = burst;
	__sync_fetch_and_add(&listIndex, 1);
	insert(&head, temp);
}

/*
 *Name: schedule:
 *Purpose: Runs the task in the shortest job first order
 *Parameters: NA
*/
void schedule(){
	//Used to pass through the list
	struct node *temp = head;
	//Stores the current node being acted upon
	struct node *currLow = head;
	//Loops throughh the list, finds the shortest job each iteration and runs it
	for(int i = 0; i < listIndex; i++){
		temp = head;
		currLow = head;	
		//Finds the shortest task and runs it
		while(temp != NULL){
			if(temp->task->burst < currLow->task->burst){
				currLow = temp;
			}
			temp = temp->next;
		}
		run(currLow->task, currLow->task->burst);
		//Removes the shortest job so it is not run again 
		delete(&head, currLow->task);
	}
}
