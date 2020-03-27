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

//Tracks how many items are in the index
int listIndex = 0;
//Stores the head of the list
struct node *head = NULL;
/*
 * Name: add
 * Purpose: Adds task to the list
 * Parameters:
 *	char *name: stores the name of the task
 *	int priority: stores the priority of the task
 *	int burst: stores how many burst long the task is
*/
void add(char *name, int priority, int burst){
	//Temp task that stores all the info about the task 
	struct task *temp = malloc(sizeof(struct task));
	temp->name = name;
	temp->tid = listIndex;
	temp->priority = priority;
	temp->burst = burst;
	//Increments number of items in the list
	__sync_fetch_and_add(&listIndex, 1);
	//Inserts item into the list
	insert(&head, temp);
}

/*
 * Name: schedule
 * Purpose:
 * Parameters: NA
*/
void schedule(){
	//A reverse of the list used to execute the task in the correct order
	struct node *reverse[listIndex];
	//Used to keep track of current element
	int tempIndex = listIndex;
	struct node *temp = head;
	//Loop through the whole list to reverse it
	while(temp != NULL){
		reverse[--tempIndex] = temp;
		temp = temp->next;
	}
	//Run all the task in the list
	for(int i = 0; i < listIndex; i++)
		run(reverse[i]->task, reverse[i]->task->burst);
}
