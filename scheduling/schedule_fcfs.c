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
	//traverse(head);
}

void schedule(){
	struct node *reverse[listIndex];
	int tempIndex = listIndex;
	struct node *temp = head;
	while(temp != NULL){
		reverse[--tempIndex] = temp;
		temp = temp->next;
	}
	for(int i = 0; i < listIndex; i++)
		run(reverse[i]->task, reverse[i]->task->burst);
}
