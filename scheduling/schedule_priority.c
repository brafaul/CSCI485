/*****************************************************************************
 * Authors: Brayden Faulkner and Hayden Nanney
 * Date: 3/26/2020
 * File: schedule_priority.c
 * Descriptiom: this program implements the priority scheduling algorithm
 *****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "schedulers.h"
#include "cpu.h"
#include "task.h"
 
struct node *head = NULL;
int listIndex = 0;

void add(char *name, int priority, int burst){
	Task*  newTask 		= (malloc(sizeof(Task)));
	newTask->name 		= name;
	newTask->tid 		= listIndex;
	newTask->priority 	= priority;
	newTask->burst 		= burst;
	__sync_fetch_and_add(&listIndex,1);
	insert(&head, newTask);			//add to beginning of list
}
void schedule(){
	struct node* curr = head;		//store current position in list
	struct node* highest = head;		//stores the current highest priority

	for(int i = 0; i < listIndex; i++){
		curr = head;			//start at beginning
		highest = head;
						//find highest priority in list
		while(curr != NULL){
			if(curr->task->priority > highest->task->priority){
				highest = curr;	//replace highest if curr is higher
			}
			curr = curr->next;	//go to next node
		}
		run(highest->task, highest->task->burst);//run highest priority task
		delete(&head,highest->task);		//remove highest priority task after running
	}
}
