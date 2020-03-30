/*****************************************************************************
 * Authors: Brayden Faulkner and Hayden Nanney
 * Date: 3/26/2020
 * File: schedule_rr.c
 * Descriptiom: this program implements a round robin scheduling algorithm
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
	insert(&head, newTask);//add to beginning of list
}
void schedule(){
	//reverse list order
	struct node* prev = NULL;
	struct node* currNode = head;
	struct node* next = NULL;
	while (currNode != NULL){
		next = currNode->next;
		currNode->next = prev;
		prev = currNode;
		currNode = next;
	}
	head = prev;
	
	currNode = head;
	Task* nextTask;
	nextTask = head->task;				//initialize task to first on list
	while(nextTask != NULL){
		if(nextTask->burst / (int)QUANTUM >= 1){//run an entire QUANTUM
			run(nextTask,(int)QUANTUM);
		}else{					//run what is left of the process
			run(nextTask,nextTask->burst);
		}
		nextTask->burst -= (int) QUANTUM;	//modify burst

		if(nextTask->burst <= 0){ 
			delete(&head, nextTask); 	//remove it from the queue when finished
		}
		//get next task
		if(head == NULL){			//finished
			nextTask = NULL;
		}else if(currNode->next == NULL){	//start back at the beginning
			currNode = head;
			nextTask = currNode->task;
		}else{					//go to next
			currNode = currNode->next;
			nextTask = currNode->task;
		}
	}	
}

