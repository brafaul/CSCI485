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

int listIndex = 0;
struct node *head = NULL;

void add(char *name, int priority, int burst){
	struct task *temp = malloc(sizeof(struct task));
	temp->name = name;
	temp->tid = listIndex;
	temp->priority = priority;
	temp->burst = burst;
	__sync_fetch_and_add(&listIndex, 1);
	insert(&head, temp);
}

void schedule(){
	struct node *temp = head;
	struct node *currLow = head;
	for(int i = 0; i < listIndex; i++){
		temp = head;
		currLow = head;	
		while(temp != NULL){
			if(temp->task->burst < currLow->task->burst){
				currLow = temp;
			}
			temp = temp->next;
		}
		run(currLow->task, currLow->task->burst);
		delete(&head, currLow->task);
	}
}
